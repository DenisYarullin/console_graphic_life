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
        unsigned x;
        unsigned y;
        unsigned is_live : 1;
    };

    unsigned weight;
    unsigned height;
    mutable unsigned live_cell_count;

    vector<vector<Cell>> world;

    void initWorld();
    void drawWorld() const;
    unsigned liveCellsCount() const;
    void currentCellNeighborsCoord(vector<Cell> &neighbors, Cell currentCell);

public:

    Life(unsigned weight_, unsigned height_);

    void begin_simulation();


};


#endif // LIFE_H_

