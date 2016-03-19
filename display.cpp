#include "display.h"
#include "life.h"
#include <ncursesw/ncurses.h>
#include <locale.h>


Console::Console() : Display()
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    getmaxyx(stdscr, LINES, COLS);
    setWidth(COLS);
    setHeight(2* LINES);
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK); // This is an ordinary colours.
    init_pair(2, COLOR_BLACK, COLOR_MAGENTA); // This is reversed ones
}


Console::~Console()
{
    echo();
    endwin();
}


bool Console::endSimulation() const
{
    return getch() == 'q';
}


void Console::drawWorld(const Life &life)
{
    const auto &world = life.world();

    for (unsigned w = 0; w < life.width(); w++)
    {
        for (unsigned h = 0; h < life.height() - 1; h += 2)
        {
            bool is_liveUpCell = world[h][w].is_live_;
            bool is_liveDownCell = world[h + 1][w].is_live_;

            cchar_t t;
            // For cells that has upper block (either full blocks or the upper one only) the reverse-colour trick is used.
            t.attr = is_liveUpCell ? COLOR_PAIR(2) : COLOR_PAIR(1);
            // For both upper and lower blocks either present or absent altogether we print space (i.e. full block).
            // Otherwise, lower half block is printed.
            t.chars[0] = (is_liveUpCell == is_liveDownCell) ? ' ' : 0x2584; // LOWER HALF BLOCK
            t.chars[1] = 0;

            mvadd_wch(h / 2, w, &t);
        }
    }
    refresh();
}

//for (unsigned w = 0; w < life.width(); w++)
//{
//    for (unsigned h = 0; h < life.height() - 1; h += 2)
//    {
//        bool is_liveUpCell = world[h][w].is_live_;
//        bool is_liveDownCell = world[h + 1][w].is_live_;
//        cchar_t t;

//        t.attr = COLOR_PAIR(1);
//        if (is_liveUpCell && is_liveDownCell)
//            t.chars[0] = 0x2588 ; // full block
//       else if (is_liveUpCell)
//            t.chars[0] = 0x2580 ; // upper half block
//        else if (is_liveDownCell)
//            t.chars[0] = 0x2584 ; // lower half block
//        else
//            t.chars[0] = ' ';

//        t.chars[1] = 0;

//        mvadd_wch(h / 2, w, &t);
//    }
//}
