#include "Graphics/Renderer.hpp"
#include <iostream> // Pour simuler UI texte, en réalité utiliser SDL_ttf pour texte

namespace Ecosystem
{
    namespace Graphics
    {

        void Renderer::RenderStatistics(const Core::Ecosystem::Statistics &stats)
        {
            // AJOUT : Simulation d'UI - en réalité, utiliser SDL_ttf pour texte. Ici, print console.
            std::cout << "UI Stats: H=" << stats.totalHerbivores << " C=" << stats.totalCarnivores << " P=" << stats.totalPlants << std::endl;
            // Pour graphique, dessiner barres (exemple)
            SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
            SDL_FRect bar = {10.0f, 10.0f, static_cast<float>(stats.totalHerbivores), 20.0f};
            SDL_RenderFillRect(mRenderer, &bar);
            // Ajouter plus pour autres stats...
        }

    } // namespace Graphics
} // namespace Ecosystem