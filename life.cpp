#include "life.h"
#include <random>
#include <algorithm>


Life::Life(unsigned height_, unsigned width_)
    : height(height_), width(width_), live_cell_count(0), neighbors(8, Cell())
{
    world.resize(height);

    for (unsigned h = 0; h < height; h++)
    {
        world[h].resize(width_);
        for(unsigned w = 0; w < width; w++)
        {
            world[h][w].x = h;
            world[h][w].y = w;
        }
    }
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
    live_cell_count = 0;

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
}


unsigned  Life::liveCellsCount() const
{
    return live_cell_count;
}


void Life::currentCellNeighborsCoord(vector<Cell> &neighbors, Cell currentCell)
{
    unsigned counter = 0;

    for (int h = currentCell.x - 1; h <= currentCell.x + 1; h++)
    {
        for (int w = currentCell.y - 1; w <= currentCell.y + 1; w++)
        {
            if (h == currentCell.x && w == currentCell.y)
                continue;

            // верхняя граница мира
            if (h < 0)
            {
                // левый угол
                if (w < 0)
                    neighbors[counter] = world[height - 1][width - 1];
                // правый угол
                else if (w > width - 1)
                    neighbors[counter] = world[height - 1][0];
                else
                    neighbors[counter] = world[height - 1][w];
            }
            // нижняя граница мира
            else if (h > height - 1)
            {
                // левый угол
                if (w < 0)
                    neighbors[counter] = world[0][width - 1];
                // правый угол
                else if (w > width - 1)
                    neighbors[counter] = world[0][0];
                else
                    neighbors[counter] = world[0][w];
            }
            // левая граница мира
            else if (w < 0)
                neighbors[counter] = world[h][width - 1];
            // правая граница мира
            else if (w > width - 1)
                neighbors[counter] = world[h][0];
            else
                neighbors[counter] = world[h][w];

            counter++;
        }
    }
}


unsigned Life::currentCellLiveNeighborsCount(Life::Cell currentCell)
{
    unsigned countLiveNeighbours = 0;

    currentCellNeighborsCoord(neighbors, currentCell);

    for(const auto &cell: neighbors)
        if (cell.is_live)
            countLiveNeighbours++;

    return countLiveNeighbours;
}


void Life::createNextGeneration()
{
    unsigned countLiveNeighbours = 0;

    for(const auto &h: oldWorld)
    {
        for(const auto &w: h)
        {
            countLiveNeighbours = currentCellLiveNeighborsCount(w);

            if (!w.is_live && countLiveNeighbours == 3)
                world[w.x][w.y].is_live = true;
            else if (countLiveNeighbours < 2 || countLiveNeighbours > 3)
                world[w.x][w.y].is_live = false;
        }
    }
}

bool Life::compareWorlds()
{
    for (unsigned h = 0; h < height; h++)
    {
        for(unsigned w = 0; w < width; w++)
        {
            if (world[h][w].is_live != oldWorld[h][w].is_live)
                return false;
        }
    }

    return true;
}


void Life::begin_simulation()
{
    int numberOfGenerations = 0;
    initWorld();

    while(true)
    {
        drawWorld();
        cout << "----------------------------" << endl;
        oldWorld = world;
        createNextGeneration();

       if (compareWorlds())
       {
           cout << "Optimal configuration detected: " << numberOfGenerations << endl;
           break;
       }

       if(!liveCellsCount())
       {
           cout << "All points died: " << numberOfGenerations << endl;
           break;
       }

       numberOfGenerations++;
    }
}
