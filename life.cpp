#include "life.h"
#include <random>
#include <algorithm>



Life::Life(unsigned int weight_, unsigned int height_)
    : weight(weight_), height(height_)
{
    world.resize(weight);
    for(auto &x: world)
        x.resize(height);
}


void Life::init_world()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for(auto &h: world)
    {
        for(auto &w: h)
        {
            w.is_live = dis(gen) % 2;
        }
    }
}
