#include "15_Master_Rollback_Networking_Input_Prediction_System/RollbackNetworkingDemo.h"

// Master networking lesson: rollback prediction.
//
// The client predicts movement immediately so controls feel responsive. Later,
// when the server sends an authoritative past state, the client rolls back to
// that state and replays saved local inputs. This demo keeps the simulation tiny
// so the correction process is visible.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace game_ref
{
    namespace
    {
        struct NetworkInput
        {
            // Network inputs are stored per frame so they can be replayed after rollback.
            int frame = 0;
            int moveX = 0;
            bool dashPressed = false;
        };

        struct PlayerSnapshot
        {
            // Snapshots capture enough state to rewind and rebuild the future.
            int frame = 0;
            int positionX = 0;
            int stamina = 100;
        };

        struct PredictionBuffer
        {
            // Inputs and snapshots are both needed: inputs rebuild future frames,
            // snapshots let us restore an older point in time.
            std::vector<NetworkInput> inputs;
            std::vector<PlayerSnapshot> snapshots;
        };

        const NetworkInput* FindInputForFrame(const std::vector<NetworkInput>& inputs, int frame)
        {
            const auto inputIt = std::find_if(inputs.begin(), inputs.end(), [frame](const NetworkInput& input)
            {
                return input.frame == frame;
            });

            return inputIt == inputs.end() ? nullptr : &(*inputIt);
        }

        PlayerSnapshot SimulateFrame(const PlayerSnapshot& previous, const NetworkInput* input, bool serverCollision)
        {
            PlayerSnapshot next = previous;
            ++next.frame;

            if (input == nullptr)
            {
                next.stamina = std::min(100, next.stamina + 5);
                return next;
            }

            next.positionX += input->moveX;

            if (input->dashPressed && next.stamina >= 30)
            {
                const int dashDistance = serverCollision ? 1 : 2;
                next.positionX += input->moveX * dashDistance;
                next.stamina -= 30;
            }

            next.stamina = std::min(100, next.stamina + 5);
            return next;
        }

        void PredictLocalFrame(PredictionBuffer& buffer, const NetworkInput& input)
        {
            buffer.inputs.push_back(input);

            const PlayerSnapshot previous = buffer.snapshots.empty()
                ? PlayerSnapshot{}
                : buffer.snapshots.back();

            buffer.snapshots.push_back(SimulateFrame(previous, &input, false));
        }

        PlayerSnapshot ServerSimulateAuthoritativeFrame(
            const std::vector<NetworkInput>& inputs,
            int targetFrame)
        {
            PlayerSnapshot state;
            for (int frame = 0; frame < targetFrame; ++frame)
            {
                const NetworkInput* input = FindInputForFrame(inputs, frame);
                const bool collisionBlocksDash = frame == 2;
                state = SimulateFrame(state, input, collisionBlocksDash);
            }

            return state;
        }

        void ReconcileWithServer(PredictionBuffer& buffer, const PlayerSnapshot& authoritativeSnapshot)
        {
            if (buffer.snapshots.empty() || authoritativeSnapshot.frame > buffer.snapshots.back().frame)
            {
                return;
            }

            const int finalPredictedFrame = buffer.snapshots.back().frame;

            // Keep only states before the corrected frame, then insert the authoritative state.
            buffer.snapshots.erase(
                std::remove_if(buffer.snapshots.begin(), buffer.snapshots.end(), [&authoritativeSnapshot](const PlayerSnapshot& snapshot)
                {
                    return snapshot.frame >= authoritativeSnapshot.frame;
                }),
                buffer.snapshots.end());

            buffer.snapshots.push_back(authoritativeSnapshot);

            // Replay saved local inputs after the corrected frame to rebuild current prediction.
            for (int frame = authoritativeSnapshot.frame; frame < finalPredictedFrame; ++frame)
            {
                const NetworkInput* input = FindInputForFrame(buffer.inputs, frame);
                const PlayerSnapshot previous = buffer.snapshots.back();
                buffer.snapshots.push_back(SimulateFrame(previous, input, false));
            }
        }

        void PrintSnapshot(const std::string& label, const PlayerSnapshot& snapshot)
        {
            std::cout << "  " << label
                      << " frame=" << snapshot.frame
                      << " x=" << snapshot.positionX
                      << " stamina=" << snapshot.stamina << '\n';
        }
    }

    void RunRollbackNetworkingDemo()
    {
        std::cout << "[15] Master Tutorial: Rollback Networking, Input Prediction System\n";

        PredictionBuffer clientBuffer;
        const std::vector<NetworkInput> localInputs = {
            { 0, 1, false },
            { 1, 1, false },
            { 2, 1, true },
            { 3, 1, false },
            { 4, 1, false }
        };

        for (const NetworkInput& input : localInputs)
        {
            PredictLocalFrame(clientBuffer, input);
        }

        const PlayerSnapshot predictedBeforeCorrection = clientBuffer.snapshots.back();
        const PlayerSnapshot authoritativeFrameThree = ServerSimulateAuthoritativeFrame(clientBuffer.inputs, 3);

        PrintSnapshot("Predicted before correction", predictedBeforeCorrection);
        PrintSnapshot("Server authoritative", authoritativeFrameThree);

        ReconcileWithServer(clientBuffer, authoritativeFrameThree);

        const PlayerSnapshot predictedAfterCorrection = clientBuffer.snapshots.back();
        PrintSnapshot("Predicted after rollback", predictedAfterCorrection);
        std::cout << '\n';
    }
}
