#pragma once

#include <SDL3/SDL.h>
#include "../Core/Ecosystem.hpp"

namespace Ecosystem
{
    namespace Graphics
    {

        class Renderer
        {
        private:
            SDL_Renderer *mRenderer;

        public:
            Renderer(SDL_Renderer *renderer) : mRenderer(renderer) {}

            void RenderStatistics(const Core::Ecosystem::Statistics &stats);
        };

    } // namespace Graphics
} // namespace Ecosystem