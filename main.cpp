#include <iostream>
#include <vector>

#include "game.h"

using namespace std;

int  main() {
    /* Init game */ // NOTE: init game and special position
    GameManager GM;

    /* Init base */
    Position myBasePos, opBasePos;

    cin >> myBasePos; cin.ignore();
    opBasePos = GM.getMirrorPos(myBasePos);

    Base myBase(myBasePos), opBase(opBasePos);

    int heroes_per_player; // Always 3
    cin >> heroes_per_player; cin.ignore();

    /* Game loop */
    while(1) {
        GM.nextTurn();

        /* Turn input */
        vector<Hero> myHero, opHero;
        vector<Monster> mons;

        GM.readInBases(myBase, opBase);
        GM.readInEntities(mons, myHero, opHero);

        /* Attacker logic */
        Hero atk = myHero[0];
        vector<Monster> nearAtkMons, huntMons;

        for(auto& mon : mons) {
            if ( atk.isInView(mon.getPos()) ) {
                nearAtkMons.push_back(mon);

                if ( !mon.isThreatFor(2) ) {
                    huntMons.push_back(mon);
                }
            }
        }

        // Movement condition
        if(0){
        // defend
            atk.Wait("atk Def");
        }
        else if(0){
        // atk move
        }
        else if( !huntMons.empty() ){
        // hunt
            for(auto mon : huntMons){
                mon.printErr();
            }

            GM.getHuntPos(huntMons);
            atk.Wait("atk Hunt");
        }
        else{
        // patrol
            Position patrolPos = 
                (myBase.isBlue() ? GM.getPatrolPos(0)
                                 : GM.getMirrorPos( GM.getPatrolPos(0) ));

            if( atk.distance(patrolPos) < Param::HERO_MOVE_SPEED ){
                GM.switchPatrolPosIndex(0);
            }

            patrolPos = 
                (myBase.isBlue() ? GM.getPatrolPos(0)
                                 : GM.getMirrorPos( GM.getPatrolPos(0) ));

            atk.Move(patrolPos, "atk Patrol");
        }
        











        cout << "WAIT" << endl;
        cout << "WAIT" << endl;
    }







    return 0;
}
