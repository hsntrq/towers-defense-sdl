#include <SDL.h>
#include "object.hpp"
#include <string>
#pragma once

class Tower : public Object
{
protected:
    SDL_Rect srcSprite = {228, 24, 132, 174}; //The sprite configuration for the eggs(hatched as well)
    int towerPrice, towerHealth, towerAttackDamage, towerAttackSpeed, towerReloadTime, towerAttackRange;
    std::string towerName;

public: //function declarations
    Tower(); // constructors (overloaded)
    Tower(int x, int y);
    virtual void fireProjectile() = 0;
    virtual void computeHealth() = 0;
    virtual void repairTower() = 0;
    virtual bool checkEnemyInRange() = 0;
    virtual void upgradeTower() = 0;
    virtual void destroyTower() = 0;
    void draw(SDL_Renderer *gRenderer, SDL_Texture *assets);
};