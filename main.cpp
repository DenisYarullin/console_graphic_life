#include <iostream>
#include "life.h"

using namespace std;

int main()
{
    Life life(10, 10);
    life.begin_simulation();

    cin.get();
    return 0;
}
