#include <SDL.h>
#include <list>
#include <iostream>
#include <vector>
#include "../towers/tower.hpp"
#include "../towercards/towercard.hpp"
#include "../towercards/bombcard.hpp"
#include "../towercards/firecard.hpp"
#include "../towercards/goldcard.hpp"
#include "../towercards/repaircard.hpp"
#include "../towercards/icecard.hpp"
#include "../towercards/longbowcard.hpp"
#include "screen.hpp"
#include "../enemies/enemy.hpp"
#include "../enemies/highHPZombie.hpp"
#include "../enemies/highSpeedZombie.hpp"
#include "../enemies/normalZombie.hpp"
#include "../enemies/specialZombie.hpp"
#include "../enemies/weakZombie.hpp"
#include "../enemyFactory.hpp"
#include "../towerBuilder.hpp"
#include "../goldCoins/coinDigits.hpp"
#include "../goldCoins/coinSlab.hpp"

using namespace std;
#pragma once

/**
 * \brief This class stores the methods and attributes necessary to carry out game logic, main game screen
 */

class GameScreen : public Screen
{
    list<Tower *> towers;
    list<Projectile *> projectiles;
    list<TowerBuilder *> patches;
    list<Enemy *> enemies;
    vector<TowerCard> towerCards;
    int gameState; // 2-> continue // 3-> won // 4-> lose
    CoinDigits coins;
    int state;
    list<EnemyFactory *> waves;
    int towerSelected;
    bool cardClicked;
    int elapsedFrames;
    int delayBetweenEnemies;
    Button pause;
    bool paused;
    Base base;
    int baseDamage;
    CoinSlab slab;

public:
    /**
     * Simple Constructor, doesn't initalize anything
     */
    GameScreen();

    /**
     * Simple ocnstructor, initalizes all attributes to desired values
     */
    GameScreen(SDL_Renderer *, SDL_Texture *); //constructor
    
    /**
     * Function to draw objects on screen
     */
    int drawObjects() override;
    
    /**
     * Function to detect clicks and spawn objects as required by game
     * \param x is x axis of pixel cliked
     * \param y is xy axis of pixel cliked
     */
    void detectClick(int, int) override;
    
    /**
     * Simple Destructor that destroys all dynamic structures in game
     */
    ~GameScreen(); //destructor
};