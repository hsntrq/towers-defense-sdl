#include "projectile.hpp"

/**
 * \brief This class stores the attributes and methods specific to the bomb tower's projectile
 * 
 */
class BombTowerProjectile : public Projectile
{
private:
public:
    /**
     * Simple constructor that initilizes the x and y locations of the projectile
     * \param x is the projectile's x location
     * \param y is the projectile's y location
     */
    BombTowerProjectile(int, int, int, int, int);
    
    /**
     * Simple destructor
     */
    ~BombTowerProjectile();
};