#include "life.h"
#include "display.h"
#include <random>
#include <algorithm>
#include <locale.h>
#include <ncursesw/ncurses.h>


#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #define msleep(msec) Sleep(msec)
#else
    #include <unistd.h>
    #define msleep(msec) usleep(msec*1000)
#endif


Life::Life(unsigned height, unsigned width)
    : height_(height), width_(width), live_cell_count_(0), neighbors_(8, Cell())
{
    world_.resize(height_, std::vector<Cell>(width_));
}


void Life::randomInitWorld()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (unsigned h = 0; h < height_; h++)
    {
        for(unsigned w = 0; w < width_; w++)
        {
            if (dis(gen) % 2)
            {
                world_[h][w].is_live_ = true;
                live_cell_count_++;
            }
            else
                 world_[h][w].is_live_ = false;
        }
    }
}


unsigned  Life::liveCellsCount() const
{
    return live_cell_count_;
}


void Life::currentCellNeighborsCoord(vector<Cell> &neighbors, int x, int y)
{
    unsigned counter = 0;

    for (int h = y - 1; h <= y + 1; h++)
    {
        for (int w = x - 1; w <= x + 1; w++)
        {
            if (h == y && w == x)
                continue;

            // верхняя граница мира
            if (h < 0)
            {
                // левый угол
                if (w < 0)
                    neighbors_[counter] = oldWorld_[height_ - 1][width_ - 1];
                // правый угол
                else if (w > width_ - 1)
                    neighbors_[counter] = oldWorld_[height_ - 1][0];
                else
                    neighbors_[counter] = oldWorld_[height_ - 1][w];
            }
            // нижняя граница мира
            else if (h > height_ - 1)
            {
                // левый угол
                if (w < 0)
                    neighbors_[counter] = oldWorld_[0][width_ - 1];
                // правый угол
                else if (w > width_ - 1)
                    neighbors_[counter] = oldWorld_[0][0];
                else
                    neighbors_[counter] = oldWorld_[0][w];
            }
            // левая граница мира
            else if (w < 0)
                neighbors_[counter] = oldWorld_[h][width_ - 1];
            // правая граница мира
            else if (w > width_ - 1)
                neighbors_[counter] = oldWorld_[h][0];
            else
                neighbors_[counter] = oldWorld_[h][w];

            counter++;
        }
    }
}


unsigned Life::currentCellLiveNeighborsCount(int x, int y)
{
    unsigned countLiveNeighbours = 0;

    currentCellNeighborsCoord(neighbors_, x, y);

    for(const auto &cell: neighbors_)
        if (cell.is_live_)
            countLiveNeighbours++;

    return countLiveNeighbours;
}


void Life::createNextGeneration()
{
    unsigned countLiveNeighbours = 0;

    for (unsigned h = 0; h < height_; h++)
    {
        for(unsigned w = 0; w < width_; w++)
        {
            countLiveNeighbours = currentCellLiveNeighborsCount(w, h);

            if (!oldWorld_[h][w].is_live_ && countLiveNeighbours == 3)
            {
                world_[h][w].is_live_ = true;
                live_cell_count_++;
            }
            else if (oldWorld_[h][w].is_live_ && (countLiveNeighbours < 2 || countLiveNeighbours > 3))
            {
                world_[h][w].is_live_ = false;
                live_cell_count_--;
            }
        }
    }
}

bool Life::compareWorlds()
{
    for (unsigned h = 0; h < height_; h++)
    {
        for(unsigned w = 0; w < width_; w++)
        {
            if (world_[h][w].is_live_ != oldWorld_[h][w].is_live_)
                return false;
        }
    }

    return true;
}


void Life::begin_simulation()
{
    Console console;
    int numberOfGenerations = 0;
    randomInitWorld();

    //while(!console.endSimulation())
    //{
        console.drawWorld(*this);
//        msleep(200);
//        oldWorld_ = world_;
//        createNextGeneration();

//       if (compareWorlds())
//       {
//           std::string message = "Optimal configuration detected: " + std::to_string(numberOfGenerations);
//           mvaddstr(height_ / 2,  0, message.c_str());
//           break;
//       }

//       if(!liveCellsCount())
//       {
//           std::string message = "All points died: " + std::to_string(numberOfGenerations);
//           mvaddstr(height_ / 2,  0, message.c_str());
//           break;
//       }

//       numberOfGenerations++;
    //}

     getch();
}
