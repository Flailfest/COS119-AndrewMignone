#include "Vestal.h"
#include "Crusader.h"
#include "Highwayman.h"
#include "PlagueDoctor.h"
#include "Party.h"
#include "Battle.h"

int main()
{
    Crusader crusader;
    Highwayman highwayman;
    PlagueDoctor plagueDoctor;
    Vestal vestal;

    Party playerParty;

    playerParty.addMember(crusader, 1);
    playerParty.addMember(highwayman, 2);
    playerParty.addMember(plagueDoctor, 3);
    playerParty.addMember(vestal, 4);

    Battle battle(playerParty);

    battle.start();

    return 0;
}