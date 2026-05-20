#include "MascotController.hpp"
#include <random>

namespace ElCalculator::services
{

    QString MascotController::getStartOfGameMessage() const
    {
        return "Salut ! Je suis ton assistant mathématique. Prépare-toi, les calculs vont s'enchaîner !";
    }

    QString MascotController::getPreBossMessage(int bossNumber) const
    {
        return QString("Attention ! Le Boss n°%1 approche... Pas de QCM cette fois, prépare ton clavier !").arg(bossNumber);
    }

    QString MascotController::getPostBossMessage(bool bossWon) const
    {
        if (bossWon)
        {
            return "Incroyable ! Tu l'as pulvérisé ! Tu as même gagné un objet Skip !";
        }
        else
        {
            return "Aïe... Le boss a été trop fort. Mais ne te décourage pas, la série recommence !";
        }
    }

    QString MascotController::getRandomFlavorLine() const
    {
        std::vector<QString> lines = {
            "Tu te débrouilles bien !",
            "N'oublie pas d'utiliser tes objets si besoin.",
            "Les mathématiques, c'est magique !",
            "Garde le rythme !"
        };

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, lines.size() - 1);

        return lines[dis(gen)];
    }

} // namespace ElCalculator::services