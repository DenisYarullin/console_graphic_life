#include "life.h"
#include <random>
#include <algorithm>



Life::Life(unsigned height_, unsigned width_)
    : height(height_), width(width_),live_cell_count(0)
{
    world.resize(height);
    for(auto &x: world)
        x.resize(width);
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
    unsigned counter = 0;

    for (signed h = currentCell.x - 1; h <= currentCell.x + 1; h++)
    {
        for (signed w = currentCell.y - 1; w <= currentCell.y; w++)
        {
            // верхняя граница мира
            if (h < 0)
            {
                // левый угол
                if (w < 0)
                {
                    neighbors[counter] = Cell(height - 1, width - 1);
                }
                // правый угол
                else if (w > width - 1)
                {
                    neighbors[counter] = Cell(height - 1, 0);
                }
                else
                {
                    neighbors[counter] = Cell(height - 1, w);
                }
            }
            // нижняя граница мира
            else if (h > height - 1)
            {
                // левый угол
                if (w < 0)
                {
                    neighbors[counter] = Cell(0, width - 1);
                }
                // правый угол
                else if (w > width - 1)
                {
                    neighbors[counter] = Cell(0, 0);
                }
                else
                {
                    neighbors[counter] = Cell(0, w);
                }
            }
            // левая граница мира
            else if (w < 0)
            {
                neighbors[counter] = Cell(h, width - 1);
            }
            // правая граница мира
            else if (w > width - 1)
            {
                neighbors[counter] = Cell(h, 0);
            }
            else
            {
                 neighbors[counter] = Cell(h, w);
            }
        }
    }
}



void Life::begin_simulation()
{
    initWorld();
    drawWorld();
}
