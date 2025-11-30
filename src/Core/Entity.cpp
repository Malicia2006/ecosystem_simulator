#include "Core/Entity.hpp"
#include <iostream>
#include <algorithm>

namespace Ecosystem
{
    namespace Core
    {

        Entity::Entity(EntityType type, Vector2D pos, std::string entityName)
            : mType(type), position(pos), name(std::move(entityName)),
              mRandomGenerator(std::random_device{}())
        {

            switch (mType)
            {
            case EntityType::HERBIVORE:
                mEnergy = 80.0f;
                mMaxEnergy = 150.0f;
                mMaxAge = 200;
                color = Color::Blue();
                size = 8.0f;
                break;
            case EntityType::CARNIVORE:
                mEnergy = 100.0f;
                mMaxEnergy = 200.0f;
                mMaxAge = 150;
                color = Color::Red();
                size = 12.0f;
                break;
            case EntityType::PLANT:
                mEnergy = 50.0f;
                mMaxEnergy = 100.0f;
                mMaxAge = 300;
                color = Color::Green();
                size = 6.0f;
                break;
            }
            mAge = 0;
            mIsAlive = true;
            mVelocity = GenerateRandomDirection();
            std::cout << "Entity created: " << name << " at (" << position.x << "," << position.y << ")\n";
        }

        Entity::Entity(const Entity &other)
            : mType(other.mType), position(other.position), name(other.name + "_child"),
              mEnergy(other.mEnergy * 0.6f), mMaxEnergy(other.mMaxEnergy),
              mAge(0), mMaxAge(other.mMaxAge), mIsAlive(true),
              mVelocity(other.mVelocity), color(other.color), size(other.size * 0.8f),
              mRandomGenerator(std::random_device{}())
        {
            std::cout << "Child entity created: " << name << "\n";
        }

        Entity::~Entity()
        {
            // debug destruction
            // std::cout << "Entity destroyed: " << name << " age=" << mAge << "\n";
        }

        void Entity::ConsumeEnergy(float deltaTime)
        {
            float base = 0.0f;
            switch (mType)
            {
            case EntityType::HERBIVORE:
                base = 1.5f;
                break;
            case EntityType::CARNIVORE:
                base = 2.0f;
                break;
            case EntityType::PLANT:
                base = -0.2f;
                break; // plants gain slowly
            }
            mEnergy -= base * deltaTime;
        }

        void Entity::AgeUp(float deltaTime)
        {
            mAge += static_cast<int>(deltaTime * 10.0f);
        }

        void Entity::CheckVitality()
        {
            if (mEnergy <= 0.0f || mAge >= mMaxAge)
            {
                mIsAlive = false;
                // std::cout << name << " died (" << (mEnergy<=0?"starvation":"old age") << ")\n";
            }
        }

        Vector2D Entity::GenerateRandomDirection()
        {
            std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
            return Vector2D(dist(mRandomGenerator), dist(mRandomGenerator));
        }

        Color Entity::CalculateColorBasedOnState() const
        {
            float r = color.r, g = color.g, b = color.b;
            float ratio = GetEnergyPercentage();
            if (ratio < 0.3f)
            {
                // fade towards red
                return Color(255, static_cast<uint8_t>(g * ratio), static_cast<uint8_t>(b * ratio));
            }
            return color;
        }

        void Entity::Update(float deltaTime)
        {
            if (!mIsAlive)
                return;
            ConsumeEnergy(deltaTime);
            AgeUp(deltaTime);
            Move(deltaTime);
            CheckVitality();
        }

        void Entity::Move(float deltaTime)
        {
            if (mType == EntityType::PLANT)
                return;
            std::uniform_real_distribution<float> chance(0.0f, 1.0f);
            if (chance(mRandomGenerator) < 0.02f)
                mVelocity = GenerateRandomDirection();
            position = position + mVelocity * deltaTime * 20.0f;
            // energy cost from movement
            float mot = std::sqrt(mVelocity.x * mVelocity.x + mVelocity.y * mVelocity.y);
            mEnergy -= mot * deltaTime * 0.05f;
        }

        void Entity::Eat(float energy)
        {
            mEnergy += energy;
            if (mEnergy > mMaxEnergy)
                mEnergy = mMaxEnergy;
        }

        bool Entity::CanReproduce() const
        {
            return mIsAlive && (mEnergy > mMaxEnergy * 0.8f) && (mAge > 20);
        }

        std::unique_ptr<Entity> Entity::Reproduce()
        {
            if (!CanReproduce())
                return nullptr;
            std::uniform_real_distribution<float> chance(0.0f, 1.0f);
            if (chance(mRandomGenerator) < 0.25f)
            {
                mEnergy *= 0.6f;
                return std::make_unique<Entity>(*this);
            }
            return nullptr;
        }

        void Entity::Render(SDL_Renderer *renderer) const
        {
            if (!mIsAlive)
                return;
            Color rc = CalculateColorBasedOnState();
            // convert float position/size to int rect (simple)
            SDL_FRect rect;
            rect.x = position.x - size / 2.0f;
            rect.y = position.y - size / 2.0f;
            rect.w = size;
            rect.h = size;

            SDL_SetRenderDrawColor(renderer, rc.r, rc.g, rc.b, rc.a);
            SDL_RenderFillRect(renderer, &rect);

            // energy bar
            if (mType != EntityType::PLANT)
            {
                float pct = GetEnergyPercentage();

                SDL_FRect eb;
                eb.x = rect.x;
                eb.y = rect.y - 4.0f;
                eb.w = rect.w * pct;
                eb.h = 3.0f;

                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &eb);
            }
        }

    } // namespace Core
} // namespace Ecosystem
