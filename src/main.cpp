#include <iostream>
#include "life.h"
#include "display.h"
#include <thread>
#include <future>
#include <locale.h>
#include <ncursesw/ncurses.h>

using namespace std;

void Task(int height, int width, int heightOffset, int widthOffset)
{
    Console console(heightOffset, widthOffset);
    Life life(height, width);
    life.randomInitWorld();

    while(!console.endSimulation())
    {
         console.drawWorld(life);
         life.simulation();
    }
}

int main()
{
    unsigned long hardware_threads = std::thread::hardware_concurrency();

    if (hardware_threads >= 4)
        hardware_threads = 4;

    // используется для получения высоты и ширины терминала
    Console console(0, 0);

    int height = console.height() / hardware_threads;
    int width  = console.width() / hardware_threads;

//    vector<std::future<void>> results;
//    results.reserve(hardware_threads);

//    for(auto &f : results)
//    {
//        f = std::async(std::launch::async, Task, height, width);
//    }

    //std::future<void> f1 = std::async(std::launch::async, Task, height, width, 0, 0);


//    std::future<void> f2 = std::async(std::launch::async, Task, height, width, 0, width);


//    std::future<void> f3 = std::async(std::launch::async, Task, height, width, height, 0);


    std::future<void> f4 = std::async(std::launch::async, Task, height, width, height, width);
    //f1.get();
    //f2.get();
    //f3.get();
    f4.get();

    cin.get();
    return 0;
}
