#include "Core/Ecosystem.hpp"
#include <algorithm>
#include <iostream>
#include <random>

namespace Ecosystem
{
    namespace Core
    {

        Ecosystem::Ecosystem(float width, float height, int maxEntities)
            : mWorldWidth(width), mWorldHeight(height), mMaxEntities(maxEntities),
              mDayCycle(0), mRandomGenerator(std::random_device{}())
        {
            mStats = {};
            std::cout << "Ecosystem created: " << width << "x" << height << "\n";
        }

        Ecosystem::~Ecosystem()
        {
            std::cout << "Ecosystem destroyed, remaining entities: " << mEntities.size() << "\n";
        }

        void Ecosystem::Initialize(int initialHerbivores, int initialCarnivores, int initialPlants)
        {
            mEntities.clear();
            mFoodSources.clear();
            mStats = {};
            for (int i = 0; i < initialHerbivores; ++i)
                SpawnRandomEntity(EntityType::HERBIVORE);
            for (int i = 0; i < initialCarnivores; ++i)
                SpawnRandomEntity(EntityType::CARNIVORE);
            for (int i = 0; i < initialPlants; ++i)
                SpawnRandomEntity(EntityType::PLANT);
            SpawnFood(20);
            std::cout << "Ecosystem initialized with " << mEntities.size() << " entities\n";
        }

        void Ecosystem::Update(float deltaTime)
        {
            for (auto &e : mEntities)
                e->Update(deltaTime);
            HandleEating();
            HandleReproduction();
            RemoveDeadEntities();
            HandlePlantGrowth(deltaTime);
            UpdateStatistics();
            ++mDayCycle;
        }

        void Ecosystem::SpawnFood(int count)
        {
            for (int i = 0; i < count; ++i)
            {
                if (mFoodSources.size() < 200)
                {
                    mFoodSources.emplace_back(GetRandomPosition(), 25.0f);
                }
            }
        }

        void Ecosystem::RemoveDeadEntities()
        {
            int before = static_cast<int>(mEntities.size());
            mEntities.erase(std::remove_if(mEntities.begin(), mEntities.end(),
                                           [](const std::unique_ptr<Entity> &e)
                                           { return !e->IsAlive(); }),
                            mEntities.end());
            int removed = before - static_cast<int>(mEntities.size());
            if (removed > 0)
                mStats.deathsToday += removed;
        }

        void Ecosystem::HandleReproduction()
        {
            std::vector<std::unique_ptr<Entity>> babies;
            for (auto &e : mEntities)
            {
                if (e->CanReproduce() && static_cast<int>(mEntities.size() + babies.size()) < mMaxEntities)
                {
                    auto baby = e->Reproduce();
                    if (baby)
                    {
                        babies.push_back(std::move(baby));
                        ++mStats.birthsToday;
                    }
                }
            }
            for (auto &b : babies)
                mEntities.push_back(std::move(b));
        }

        void Ecosystem::HandleEating()
        {
            // simple: if entity is herbivore and near food -> eat
            for (auto &e : mEntities)
            {
                if (!e->IsAlive())
                    continue;
                if (e->GetType() == EntityType::HERBIVORE)
                {
                    // find nearest food within small radius
                    int idx = -1;
                    float bestDist = 1e9f;
                    for (size_t i = 0; i < mFoodSources.size(); ++i)
                    {
                        float d = e->position.Distance(mFoodSources[i].position);
                        if (d < bestDist)
                        {
                            bestDist = d;
                            idx = static_cast<int>(i);
                        }
                    }
                    if (idx != -1 && bestDist < 20.0f)
                    {
                        e->Eat(mFoodSources[idx].energyValue);
                        // remove food
                        mFoodSources.erase(mFoodSources.begin() + idx);
                    }
                }
                else if (e->GetType() == EntityType::CARNIVORE)
                {
                    // simple predation: eat nearest herbivore if close
                    int victimIdx = -1;
                    float bestDist = 1e9f;
                    for (size_t i = 0; i < mEntities.size(); ++i)
                    {
                        if (mEntities[i].get() == e.get())
                            continue;
                        if (mEntities[i]->GetType() == EntityType::HERBIVORE && mEntities[i]->IsAlive())
                        {
                            float d = e->position.Distance(mEntities[i]->position);
                            if (d < bestDist)
                            {
                                bestDist = d;
                                victimIdx = static_cast<int>(i);
                            }
                        }
                    }
                    if (victimIdx != -1 && bestDist < 12.0f)
                    {
                        // carnivore eats victim
                        float gained = std::min(50.0f, mEntities[victimIdx]->GetEnergy() + 30.0f);
                        e->Eat(gained);
                        mEntities[victimIdx]->Eat(-10000.0f); // kill
                    }
                }
                else if (e->GetType() == EntityType::PLANT)
                {
                    // small passive energy gain already in Entity
                }
            }
        }

        void Ecosystem::UpdateStatistics()
        {
            mStats = {};
            mStats.totalFood = static_cast<int>(mFoodSources.size());
            for (const auto &e : mEntities)
            {
                if (e->GetType() == EntityType::HERBIVORE)
                    ++mStats.totalHerbivores;
                if (e->GetType() == EntityType::CARNIVORE)
                    ++mStats.totalCarnivores;
                if (e->GetType() == EntityType::PLANT)
                    ++mStats.totalPlants;
            }
        }

        void Ecosystem::SpawnRandomEntity(EntityType type)
        {
            if (static_cast<int>(mEntities.size()) >= mMaxEntities)
                return;
            Vector2D pos = GetRandomPosition();
            std::string name;
            switch (type)
            {
            case EntityType::HERBIVORE:
                name = "Herbivore_" + std::to_string(mStats.totalHerbivores);
                break;
            case EntityType::CARNIVORE:
                name = "Carnivore_" + std::to_string(mStats.totalCarnivores);
                break;
            case EntityType::PLANT:
                name = "Plant_" + std::to_string(mStats.totalPlants);
                break;
            }
            mEntities.push_back(std::make_unique<Entity>(type, pos, name));
        }

        Vector2D Ecosystem::GetRandomPosition() const
        {
            std::uniform_real_distribution<float> dx(0.0f, mWorldWidth);
            std::uniform_real_distribution<float> dy(0.0f, mWorldHeight);
            return Vector2D(dx(mRandomGenerator), dy(mRandomGenerator));
        }

        void Ecosystem::HandlePlantGrowth(float deltaTime)
        {
            std::uniform_real_distribution<float> chance(0.0f, 1.0f);
            if (chance(mRandomGenerator) < 0.02f && static_cast<int>(mEntities.size()) < mMaxEntities)
            {
                SpawnRandomEntity(EntityType::PLANT);
            }
        }

        void Ecosystem::Render(SDL_Renderer *renderer) const
        {
            // render food
            for (const auto &f : mFoodSources)
            {
                r.x = f.position.x - 3.0f;
                r.y = f.position.y - 3.0f;
                r.w = 6.0f;
                r.h = 6.0f;
                SDL_SetRenderDrawColor(renderer, f.color.r, f.color.g, f.color.b, f.color.a);
                SDL_RenderFillRect(renderer, &r);
            }
            // render entities
            for (const auto &e : mEntities)
                e->Render(renderer);
        }

        void Ecosystem::AddEntity(std::unique_ptr<Entity> entity) { mEntities.push_back(std::move(entity)); }
        void Ecosystem::AddFood(Vector2D position, float energy) { mFoodSources.emplace_back(position, energy); }

    } // namespace Core
} // namespace Ecosystem
