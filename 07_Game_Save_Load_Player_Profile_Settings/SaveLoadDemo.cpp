#include "07_Game_Save_Load_Player_Profile_Settings/SaveLoadDemo.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

namespace game_ref
{
    namespace
    {
        struct PlayerProfile
        {
            std::string playerName = "Player";
            std::string checkpointName = "Start";
            int level = 1;
            int experience = 0;
            float musicVolume = 0.8f;
            bool invertCameraY = false;
        };

        std::string SerializeProfile(const PlayerProfile& profile)
        {
            // A simple key-value format is easy to debug before moving to JSON, binary files, or platform cloud saves.
            std::ostringstream output;
            output << "playerName=" << profile.playerName << '\n';
            output << "checkpointName=" << profile.checkpointName << '\n';
            output << "level=" << profile.level << '\n';
            output << "experience=" << profile.experience << '\n';
            output << "musicVolume=" << profile.musicVolume << '\n';
            output << "invertCameraY=" << (profile.invertCameraY ? 1 : 0) << '\n';
            return output.str();
        }

        void ApplyProfileValue(PlayerProfile& profile, const std::string& key, const std::string& value)
        {
            if (key == "playerName")
            {
                profile.playerName = value;
            }
            else if (key == "checkpointName")
            {
                profile.checkpointName = value;
            }
            else if (key == "level")
            {
                profile.level = std::max(1, std::stoi(value));
            }
            else if (key == "experience")
            {
                profile.experience = std::max(0, std::stoi(value));
            }
            else if (key == "musicVolume")
            {
                profile.musicVolume = std::max(0.0f, std::min(1.0f, std::stof(value)));
            }
            else if (key == "invertCameraY")
            {
                profile.invertCameraY = value == "1";
            }
        }

        PlayerProfile DeserializeProfile(const std::string& saveText)
        {
            PlayerProfile profile;
            std::istringstream input(saveText);
            std::string line;

            while (std::getline(input, line))
            {
                const std::size_t separator = line.find('=');
                if (separator == std::string::npos)
                {
                    continue;
                }

                ApplyProfileValue(profile, line.substr(0, separator), line.substr(separator + 1));
            }

            return profile;
        }
    }

    void RunSaveLoadDemo()
    {
        std::cout << "[07] Game Save, Load, Player Profile, Settings\n";

        PlayerProfile savedProfile;
        savedProfile.playerName = "AshenLearner";
        savedProfile.checkpointName = "Ruined Gate";
        savedProfile.level = 8;
        savedProfile.experience = 240;
        savedProfile.musicVolume = 0.55f;
        savedProfile.invertCameraY = true;

        const std::string saveText = SerializeProfile(savedProfile);
        const PlayerProfile loadedProfile = DeserializeProfile(saveText);

        std::cout << "  Loaded player=" << loadedProfile.playerName
                  << " checkpoint=" << loadedProfile.checkpointName
                  << " level=" << loadedProfile.level
                  << " volume=" << loadedProfile.musicVolume
                  << " invertY=" << (loadedProfile.invertCameraY ? "true" : "false") << '\n';
        std::cout << '\n';
    }
}
