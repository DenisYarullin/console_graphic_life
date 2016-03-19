/*
Правила игры

Первое поколение — это положение игровых клеток в начале игры. Будем заполнять его случайным образом. Каждое новое поколение рассчитывается на основе предыдущего по таким правилам:

    В пустой клетке, рядом с которой ровно три живые клетки, зарождается жизнь.
    Если у живой клетки есть две или три живые соседки, то эта клетки продолжает жить. Иначе, клетка умирает (от «одиночества» или «перенаселенности»).

Игра прекращается, если на поле не останется ни одной живой клетки или при очередном шаге ни одна клетка не меняет своего состояния (складывается стабильная конфигурация).
*/


#pragma once


#ifndef LIFE_H_
#define LIFE_H_


#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;


class Life
{
private:

    struct Cell
    {
        bool is_live_;
    };

    unsigned height_;
    unsigned width_;
    mutable unsigned live_cell_count_;
    mutable vector<Cell> neighbors_;

    vector<vector<Cell>> world_;
    vector<vector<Cell>> oldWorld_;

    void randomInitWorld();
    unsigned liveCellsCount() const;
    void currentCellNeighborsCoord(vector<Cell> &neighbors, int x, int y);
    unsigned currentCellLiveNeighborsCount(int x, int y);
    void createNextGeneration();
    bool compareWorlds();

public:

    Life(unsigned height, unsigned width);

    int height() const { return height_; }
    int width() const { return width_; }
    const vector<vector<Cell>> &world() const{ return world_; }

    void begin_simulation();
};


#endif // LIFE_H_

