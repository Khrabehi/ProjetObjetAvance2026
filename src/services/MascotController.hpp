#pragma once

#include <QString>
#include <vector>

namespace ElCalculator::services
{
    class MascotController
    {
    public:
        MascotController() = default;

        // Méthodes pour récupérer les bons dialogues selon l'état du jeu
        QString getStartOfGameMessage() const;
        QString getPreBossMessage(int bossNumber) const;
        QString getPostBossMessage(bool bossWon) const;
        QString getRandomFlavorLine() const;

    private:
        // Optionnel : tu pourrais charger ça depuis un fichier JSON plus tard
        // pour respecter totalement l'Open/Closed principle.
    };

} // namespace ElCalculator::services