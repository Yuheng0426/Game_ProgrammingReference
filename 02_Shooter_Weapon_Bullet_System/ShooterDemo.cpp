#include "02_Shooter_Weapon_Bullet_System/ShooterDemo.h"

// Shooter lesson: weapon rules, magazine ammo, reload timing, bullets, and hits.
//
// The important teaching idea is that "fire a weapon" is not just spawning a
// bullet. A robust weapon usually checks cooldown, reload state, ammo count,
// muzzle position, aim direction, projectile speed, damage, and later hit logic.

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Common_Game_Code/Gameplay/CooldownTimer.h"
#include "Common_Game_Code/Gameplay/HealthComponent.h"
#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        struct WeaponConfig
        {
            // Tuning data is grouped here so balance changes do not require
            // rewriting the Weapon class logic.
            std::string name;
            int magazineSize = 6;
            int damage = 25;
            float bulletSpeed = 18.0f;
            float fireCooldownSeconds = 0.2f;
            float reloadSeconds = 1.0f;
        };

        struct Bullet
        {
            // A bullet stores only the data needed by the projectile update system.
            Vector2 position;
            Vector2 velocity;
            int damage = 0;
            bool active = true;
        };

        struct Target
        {
            // Targets are intentionally simple: position + hit radius + health.
            // This keeps the example close to what real hit detection needs.
            std::string name;
            Vector2 position;
            float hitRadius = 0.6f;
            HealthComponent health{ 100 };
        };

        class Weapon
        {
        public:
            explicit Weapon(WeaponConfig config) : weaponConfig(std::move(config)), ammoInMagazine(weaponConfig.magazineSize) {}

            void Update(float deltaSeconds)
            {
                fireCooldown.Update(deltaSeconds);
                reloadTimer.Update(deltaSeconds);

                if (isReloading && reloadTimer.IsReady())
                {
                    ammoInMagazine = weaponConfig.magazineSize;
                    isReloading = false;
                    std::cout << "    Reload complete: " << ammoInMagazine << " rounds ready\n";
                }
            }

            [[nodiscard]] bool TryFire(const Vector2& muzzlePosition, const Vector2& aimDirection, std::vector<Bullet>& bullets)
            {
                // A real game would also check recoil, spread, network authority, and animation state here.
                if (isReloading || !fireCooldown.IsReady() || ammoInMagazine <= 0)
                {
                    return false;
                }

                --ammoInMagazine;
                fireCooldown.Start(weaponConfig.fireCooldownSeconds);

                bullets.push_back({
                    muzzlePosition,
                    aimDirection.Normalized() * weaponConfig.bulletSpeed,
                    weaponConfig.damage,
                    true
                });

                return true;
            }

            void StartReload()
            {
                if (!isReloading && ammoInMagazine < weaponConfig.magazineSize)
                {
                    isReloading = true;
                    reloadTimer.Start(weaponConfig.reloadSeconds);
                    std::cout << "    Reload started\n";
                }
            }

            [[nodiscard]] int Ammo() const
            {
                return ammoInMagazine;
            }

        private:
            WeaponConfig weaponConfig;
            CooldownTimer fireCooldown;
            CooldownTimer reloadTimer;
            int ammoInMagazine = 0;
            bool isReloading = false;
        };

        void UpdateBullets(std::vector<Bullet>& bullets, std::vector<Target>& targets, float deltaSeconds)
        {
            for (Bullet& bullet : bullets)
            {
                if (!bullet.active)
                {
                    continue;
                }

                bullet.position += bullet.velocity * deltaSeconds;

                for (Target& target : targets)
                {
                    if (!target.health.IsAlive())
                    {
                        continue;
                    }

                    if (Distance(bullet.position, target.position) <= target.hitRadius)
                    {
                        target.health.ApplyDamage(bullet.damage);
                        bullet.active = false;
                        std::cout << "    Hit " << target.name << " for " << bullet.damage
                                  << " damage. HP=" << target.health.Current() << '/' << target.health.Max() << '\n';
                        break;
                    }
                }
            }
        }
    }

    void RunShooterDemo()
    {
        std::cout << "[02] Shooter Weapon and Bullet System\n";

        Weapon rifle({ "Training Rifle", 3, 35, 10.0f, 0.25f, 0.7f });
        std::vector<Bullet> bullets;
        std::vector<Target> targets = {
            { "Drone A", { 3.0f, 0.0f }, 0.55f, HealthComponent(70) },
            { "Drone B", { 7.0f, 0.0f }, 0.55f, HealthComponent(100) }
        };

        constexpr float deltaSeconds = 0.1f;
        for (int frame = 0; frame < 18; ++frame)
        {
            rifle.Update(deltaSeconds);

            if (frame == 0 || frame == 3 || frame == 6 || frame == 12)
            {
                const bool fired = rifle.TryFire({ 0.0f, 0.0f }, { 1.0f, 0.0f }, bullets);
                std::cout << "  Frame " << frame << (fired ? " fired" : " did not fire")
                          << " ammo=" << rifle.Ammo() << '\n';
            }

            if (frame == 7)
            {
                rifle.StartReload();
            }

            UpdateBullets(bullets, targets, deltaSeconds);
        }

        std::cout << '\n';
    }
}
