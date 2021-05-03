#include "gameManager.hpp"

void gameManager::drawObjects() //iterating through the lists and drawing all of the instances
{
    elapsedFrames++;
    if (baseHP == 0)
    {
        state = 4;
        return;
    }
    else
    {
        state = 2;
    }
    if (!waves.empty())
    {
        if (!waves.front()->enemiesSpawned())
        {
            waves.front()->spawnEnemies(enemies, elapsedFrames);
        }
        if (waves.front()->waveComplete(elapsedFrames) && enemies.empty())
        {
            delete waves.front();
            waves.pop_front();
        }
    }
    if (waves.empty() && enemies.empty() && baseHP >= 0)
    {
        state = 3;
        std::cout << "Game Over" << std::endl;
        return;
    }
    for (list<Tower *>::iterator tower = towers.begin(); tower != towers.end(); ++tower)
    {
        if ((*tower)->towerID < 4)
        {
            auto [enemyX, enemyY] = (*tower)->checkEnemyInRange(enemies);
            if (enemyX && (*tower)->cooledDown)
            {
                (*tower)->fireProjectile(enemyX, enemyY, projectiles);
            }
        }
        (*tower)->updateCoolDownStatus(elapsedFrames); // necessary because the tower has to first fire before going into cooldown
        (*tower)->draw(gRenderer, assets);
    }
    for (list<Enemy *>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
    {
        if ((*enemy)->followPath())
        {
            delete (*enemy);
            enemies.erase(enemy--);
        }
        (*enemy)->draw(gRenderer, assets);
    }
    for (int i = 0; i < towerCards.size(); i++)
    {
        towerCards[i].draw(gRenderer, assets);
    }
    for (list<Projectile *>::iterator projectile = projectiles.begin(); projectile != projectiles.end(); ++projectile)
    {
        (*projectile)->shoot();
        (*projectile)->handleCollision(enemies);
        (*projectile)->draw(gRenderer, assets);
        if ((*projectile)->reachedTarget)
        {
            delete (*projectile);
            projectiles.erase(projectile--);
        }
    }
}

int gameManager::detectClick(int x, int y)
{
    std::cout << "Mouse clicked at: " << x << " -- " << y << std::endl;
    if (cardClicked)
    {
        for (list<Patches *>::iterator patch = patches.begin(); patch != patches.end(); ++patch)
        {
            (*patch)->isClicked(towers, towerSelected, x, y);
        }
        cardClicked = false;
        towerCards[towerSelected].isSelected = false;
        towerSelected = -1;
    }
    else
    {
        for (int i = 0; i < towerCards.size(); i++)
        {
            if (towerCards[i].isClicked(x, y))
            {
                cardClicked = true;
                towerSelected = i;
                towerCards[towerSelected].isSelected = true;
                break;
            }
        }
    }
    return state;
}

gameManager::gameManager(SDL_Renderer *renderer, SDL_Texture *asst)
{
    baseHP = 100;
    gRenderer = renderer;
    assets = asst;
    elapsedFrames = 0;

    patches.push_back(new Patches(64, 480));
    patches.push_back(new Patches(288, 416));
    patches.push_back(new Patches(288, 256));
    patches.push_back(new Patches(32, 64));
    patches.push_back(new Patches(672, 160));
    patches.push_back(new Patches(480, 352));
    patches.push_back(new Patches(544, 544));
    patches.push_back(new Patches(768, 320));
    patches.push_back(new Patches(992, 512));
    patches.push_back(new Patches(1152, 416));
    patches.push_back(new Patches(960, 224));

    towerCards.push_back(FireCard());
    towerCards.push_back(BombCard());
    towerCards.push_back(IceCard());
    towerCards.push_back(LongBowCard());
    towerCards.push_back(GoldCard());
    towerCards.push_back(RepairCard());

    cardClicked = false;
    towerSelected = -1;
    

    for (int i = 0; i < 2; i++) //fills up a list that stores 6 consecutive waves
    {
        waves.push_back(new Wave(i));
    }
}

gameManager::~gameManager() //destructor deletes all dynamically created objects traversing them in all the lists
{
}