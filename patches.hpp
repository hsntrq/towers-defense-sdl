#include <iostream>
#include <tuple>
#include <list>
#include "object.hpp"
#include "towers/firetower.hpp"
#include "towers/icetower.hpp"
#include "towers/goldtower.hpp"
#include "towers/longbowtower.hpp"
#include "towers/repairtower.hpp"
#include "towers/bombtower.hpp"
#pragma once

class Patches
{
private:
    int x, y;

public:
    bool isAvailable;
    Patches(int, int);
    bool checkRange(int, int);
    std::tuple<int, int> location();
    void isClicked(std::list<Tower *> &, int, int, int);
};