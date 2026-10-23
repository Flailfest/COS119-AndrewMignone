#include "Battle.h"

#include <iostream>
#include <random>

Battle::Battle(Party& playerParty)
    : playerParty(playerParty),
    enemyCount(0)
{
    for (int i = 0; i < 4; i++)
    {
        enemyRoster[i] = nullptr;
    }

    generateEnemyParty();
}


Battle::~Battle()
{
    for (int i = 0; i < 4; i++)
    {
        delete enemyRoster[i];
        enemyRoster[i] = nullptr;
    }
}


void Battle::generateEnemyParty()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    // Bosses are deliberately very rare: 1% chance per battle.
    std::uniform_int_distribution<int> bossChanceDistribution(1, 100);
    if (bossChanceDistribution(generator) == 1)
    {
        std::uniform_int_distribution<int> bossTypeDistribution(1, 2);
        const int bossType = bossTypeDistribution(generator);
        enemyCount = 1;

        if (bossType == 1)
        {
            enemyRoster[0] = new Enemy(
                "Necromancer",
                {
                    85, 8, 85, 4, 8, 10
                },
                100,
                150
            );
        }
        else
        {
            enemyRoster[0] = new Enemy(
                "Prophet",
                {
                    80, 3, 75, 4, 9, 8
                },
                100,
                160
            );
        }

        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << "  A RARE BOSS ENCOUNTER!\n";
        std::cout << "  The Ruins reveal their greatest horror: "
                  << enemyRoster[0]->getName() << ".\n";
        std::cout << "====================================\n";

        enemyParty.addMember(*enemyRoster[0], 1);
        return;
    }

    std::uniform_int_distribution<int> countDistribution(1, 4);
    enemyCount = countDistribution(generator);

    std::cout << "\n";
    std::cout << "Generating enemy party...\n";
    std::cout << "Enemy count: " << enemyCount << "\n";

    for (int i = 0; i < enemyCount; i++)
    {
        enemyRoster[i] = createRandomEnemy();

        if (enemyRoster[i] != nullptr)
        {
            enemyParty.addMember(
                *enemyRoster[i],
                i + 1
            );
        }
    }
}


Enemy* Battle::createRandomEnemy()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    // Common and elite enemy weights. The Bone Captain and Bone Bearer
    // are less common than the regular Ruins enemies.
    std::uniform_int_distribution<int> enemyDistribution(1, 100);
    const int enemyType = enemyDistribution(generator);

    if (enemyType <= 14)
    {
        return new Enemy(
            "Brigand",
            {
                20, 4, 80, 3, 6, 5
            },
            10,
            20
        );
    }

    if (enemyType <= 28)
    {
        return new Enemy(
            "Cultist",
            {
                18, 5, 85, 2, 5, 8
            },
            12,
            25
        );
    }

    if (enemyType <= 40)
    {
        return new Enemy(
            "Ghoul",
            {
                28, 3, 75, 5, 9, 3
            },
            15,
            30
        );
    }

    if (enemyType <= 52)
    {
        return new Enemy(
            "Bone Soldier",
            {
                10, 2, 75, 2, 4, 2
            },
            7,
            12
        );
    }

    if (enemyType <= 61)
    {
        return new Enemy(
            "Bone Courtier",
            {
                10, 8, 78, 2, 4, 10
            },
            8,
            15
        );
    }

    if (enemyType <= 71)
    {
        return new Enemy(
            "Bone Arbalist",
            {
                15, 5, 85, 3, 6, 8
            },
            10,
            20
        );
    }

    if (enemyType <= 81)
    {
        return new Enemy(
            "Bone Defender",
            {
                15, 1, 70, 2, 5, 2
            },
            12,
            18
        );
    }

    if (enemyType <= 89)
    {
        return new Enemy(
            "Bone Spearman",
            {
                22, 4, 80, 4, 7, 5
            },
            15,
            25
        );
    }

    if (enemyType <= 94)
    {
        return new Enemy(
            "Slavering Ghoul",
            {
                32, 3, 76, 5, 9, 4
            },
            18,
            32
        );
    }

    if (enemyType <= 97)
    {
        return new Enemy(
            "Bone Captain",
            {
                40, 1, 80, 4, 7, 4
            },
            20,
            35
        );
    }

    return new Enemy(
        "Bone Bearer",
        {
            35, 7, 75, 2, 5, 5
        },
        18,
        32
    );
}
