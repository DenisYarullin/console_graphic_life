#include <iostream>
#include "life.h"
#include <locale.h>
#include <ncursesw/ncurses.h>

using namespace std;

int main()
{
    Life life(50, 50);
    life.begin_simulation();

    cin.get();
    return 0;
}
