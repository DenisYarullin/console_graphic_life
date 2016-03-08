#include "life.h"
#include <random>
#include <algorithm>



Life::Life(unsigned weight_, unsigned height_)
    : weight(weight_), height(height_), live_cell_count(0)
{
    world.resize(weight);
    for(auto &x: world)
        x.resize(height);
}


void Life::initWorld()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for(auto &h: world)
        for(auto &w: h)
            w.is_live = dis(gen) % 2;
}


void Life::drawWorld() const
{
    for(const auto &h: world)
    {
        for(const auto &w: h)
        {
            if (w.is_live)
            {
                live_cell_count++;
                cout << '@';
            }
            else
                cout << ' ';
            cout << ' ';
        }
        cout << endl;
    }
};


unsigned  Life::liveCellsCount() const
{
    return live_cell_count;
}


void Life::currentCellNeighborsCoord(vector<Cell> &neighbors, Cell currentCell)
{
    for (signed h = currentCell.x - 1; h <= currentCell.x + 1; h++)
    {
        for (signed w = currentCell.y - 1; w <= currentCell.y; w++)
        {

        }
    }
}



void Life::begin_simulation()
{
    initWorld();
    drawWorld();
}
