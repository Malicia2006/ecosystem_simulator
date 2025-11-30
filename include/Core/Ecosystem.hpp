#pragma once
#include "Entity.hpp"
#include "Structs.hpp"
#include <vector>
#include <memory>
#include <random>

namespace Ecosystem {
namespace Core {

class Ecosystem {
public:
    struct Statistics {
        int totalHerbivores{0};
        int totalCarnivores{0};
        int totalPlants{0};
        int totalFood{0};
        int deathsToday{0};
        int birthsToday{0};
    };

private:
    std::vector<std::unique_ptr<Entity>> mEntities;
    std::vector<Food> mFoodSources;
    float mWorldWidth;
    float mWorldHeight;
    int mMaxEntities;
    int mDayCycle;
    std::mt19937 mRandomGenerator;
    Statistics mStats;

    void UpdateStatistics();
    void SpawnRandomEntity(EntityType type);
    Vector2D GetRandomPosition() const;
    void HandlePlantGrowth(float deltaTime);

public:
    Ecosystem(float width, float height, int maxEntities = 500);
    ~Ecosystem();

    void Initialize(int initialHerbivores, int initialCarnivores, int initialPlants);
    void Update(float deltaTime);
    void SpawnFood(int count);
    void RemoveDeadEntities();
    void HandleReproduction();
    void HandleEating();
    void Render(SDL_Renderer* renderer) const;

    // management
    void AddEntity(std::unique_ptr<Entity> entity);
    void AddFood(Vector2D position, float energy = 25.0f);

    // getters
    int GetEntityCount() const { return static_cast<int>(mEntities.size()); }
    int GetFoodCount() const { return static_cast<int>(mFoodSources.size()); }
    Statistics GetStatistics() const { return mStats; }
    float GetWorldWidth() const { return mWorldWidth; }
    float GetWorldHeight() const { return mWorldHeight; }
};
} // namespace Core
} // namespace Ecosystem
