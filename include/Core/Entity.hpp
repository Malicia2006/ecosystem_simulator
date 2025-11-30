#pragma once
#include "Structs.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <random>
#include <string>
#include <vector>

namespace Ecosystem {
namespace Core {

enum class EntityType { HERBIVORE, CARNIVORE, PLANT };

class Entity {
private:
    float mEnergy;
    float mMaxEnergy;
    int mAge;
    int mMaxAge;
    bool mIsAlive;
    Vector2D mVelocity;
    EntityType mType;
    std::mt19937 mRandomGenerator;
    // private helpers
    void ConsumeEnergy(float deltaTime);
    void AgeUp(float deltaTime);
    void CheckVitality();
    Vector2D GenerateRandomDirection();
    Color CalculateColorBasedOnState() const;

public:
    Vector2D position;
    Color color;
    float size;
    std::string name;

    Entity(EntityType type, Vector2D pos, std::string entityName = "Unnamed");
    Entity(const Entity& other);
    ~Entity();

    void Update(float deltaTime);
    void Move(float deltaTime);
    void Eat(float energy);
    bool CanReproduce() const;
    std::unique_ptr<Entity> Reproduce();
    void Render(SDL_Renderer* renderer) const;

    // getters
    float GetEnergy() const { return mEnergy; }
    float GetEnergyPercentage() const { return (mMaxEnergy > 0) ? (mEnergy / mMaxEnergy) : 0.0f; }
    int GetAge() const { return mAge; }
    bool IsAlive() const { return mIsAlive; }
    EntityType GetType() const { return mType; }
};

} // namespace Core
} // namespace Ecosystem
