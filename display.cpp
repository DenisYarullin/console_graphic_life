#include "display.h"
#include "life.h"
#include <cstring>
#include <ncursesw/ncurses.h>
#include <locale.h>

#define ENTER 10
#define ESCAPE 27

Console::Console() : Display()
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    cbreak();
    curs_set(0);
    getmaxyx(stdscr, LINES, COLS);
    setWidth(COLS);
    setHeight(2* LINES);
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK); // This is an ordinary colours.
    init_pair(2, COLOR_BLACK, COLOR_MAGENTA); // This is reversed ones
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(4, COLOR_BLUE, COLOR_WHITE);
    init_pair(5, COLOR_RED, COLOR_WHITE);
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


void Console::drawMenubar(WINDOW *menubar)
{
    wbkgd(menubar,COLOR_PAIR(4));

    waddstr(menubar, "Файл");
    wattron(menubar, COLOR_PAIR(5)); // Turn on the given attributes
    waddstr(menubar, "(F1)");

    int lenFileName = strlen("Файл(F1)") + 10;
    wattroff(menubar, COLOR_PAIR(5)); // Turn off the given attributes
    wmove(menubar, 0,  lenFileName);

    waddstr(menubar, "Натюрморты");
    wattron(menubar, COLOR_PAIR(5));
    waddstr(menubar, "(F2)");

    int lenStillLife = strlen("Натюрморты(F2)") + lenFileName;
    wattroff(menubar, COLOR_PAIR(5));
    wmove(menubar, 0, lenStillLife);

    waddstr(menubar, "Осцилляторы");
    wattron(menubar, COLOR_PAIR(5));
    waddstr(menubar, "(F3)");

    wattroff(menubar, COLOR_PAIR(5));
}


WINDOW **Console::drawMenuFile(int start_col)
{
    items = new WINDOW *[4];

    items[0] = newwin(10, 19, 1, start_col);
    wbkgd(items[0], COLOR_PAIR(4));
    box(items[0], ACS_VLINE, ACS_HLINE);

    items[1] = subwin(items[0], 1, 17, 2, start_col + 1);
    wprintw(items[1], "Load");

    items[2] = subwin(items[0], 1, 17, 3, start_col + 1);
    wprintw(items[2], "Save");

    items[3] = subwin(items[0], 1, 17, 4, start_col + 1);
    wprintw(items[3], "Quit");

    wbkgd(items[1], COLOR_PAIR(1));
    wrefresh(items[0]);

    return items;
}


WINDOW **Console::drawMenuFigures(int start_col)
{
    items = new WINDOW *[4];

    items[0] = newwin(10, 19, 1, start_col);
    wbkgd(items[0], COLOR_PAIR(4));
    box(items[0], ACS_VLINE, ACS_HLINE);

    items[1] = subwin(items[0], 1, 17, 2, start_col + 1);
    wprintw(items[1], "Мигалка");

    items[2] = subwin(items[0], 1, 17, 3, start_col + 1);
    wprintw(items[2], "Улей, каравай, пруд, длинная лодка, длинный корабль");

    items[3] = subwin(items[0], 1, 17, 4, start_col + 1);
    wprintw(items[3], "Item%d");

    wbkgd(items[1], COLOR_PAIR(1));
    wrefresh(items[0]);
    return items;
}


void Console::deleteMenu(WINDOW **items, int count)
{
    for(int i = 0; i < count; i++)
        delwin(items[i]);
    delete [] items;
}


int Console::scrollMenu(WINDOW **items, int count, int menu_start_col)
{
    int key;
    int selected = 0;
    while(true)
    {
        key = getch();
        if(key == KEY_DOWN || key == KEY_UP)
        {
            wbkgd(items[selected + 1], COLOR_PAIR(4));
            wnoutrefresh(items[selected + 1]); // Copies window to virtual screen
            if (key == KEY_DOWN)
            {
                selected = (selected + 1) % count;
            }
            else
            {
                selected = (selected + count - 1) % count;
            }
            wbkgd(items[selected + 1], COLOR_PAIR(3));
            wnoutrefresh(items[selected + 1]);
            doupdate(); // Write all prepared refreshes to terminal
        }
        else if (key == KEY_LEFT || key == KEY_RIGHT)
        {
            deleteMenu(items, count + 1);
            touchwin(stdscr);
            refresh();
            if (!menu_start_col)
                items = drawMenuFigures(20);
            else
                items = drawMenuFile(0);
            return scrollMenu(items, 4, 20 - menu_start_col);
        }
        else if (key == ESCAPE)
        {
            return -1;
        }
        else if (key == ENTER)
        {
            return selected;
        }
    }
}


void Console::drawWindow()
{
    bkgd(COLOR_PAIR(3)); // Set background property for terminal screen
    menubar_ = subwin(stdscr, 1, 80, 0, 0);
    messagebar_ = subwin(stdscr, 1, 79, 23, 1);
    drawMenubar(menubar_);
    move(2, 1);
    printw("Press F1 or F2 to open the menus. ");
    printw("ESC quits.");
    refresh();

    int key;
    do {
        int selected_item;
        WINDOW **menu_items;
        key = getch();
        werase(messagebar_); // Erase terminal screen
        wrefresh(messagebar_);
        if (key == KEY_F(1))
        {
            menu_items = drawMenuFile(0);
            selected_item = scrollMenu(menu_items, 4, 0);
            deleteMenu(menu_items, 4);
            if (selected_item < 0)
                wprintw(messagebar_,"You haven't selected any item.");
            else
                wprintw(messagebar_, "You have selected menu item %d.",selected_item + 1);
            touchwin(stdscr);
            refresh();
        }
        else if (key == KEY_F(2))
        {
            menu_items = drawMenuFile(20);
            selected_item = scrollMenu(menu_items, 4, 20);
            deleteMenu(menu_items, 4);
            if (selected_item < 0)
                wprintw(messagebar_,"You haven't selected any item.");
            else
                wprintw(messagebar_, "You have selected menu item %d.", selected_item + 1);
            touchwin(stdscr);
            refresh();
        }
    } while (key != ESCAPE);

    delwin(menubar_);
    delwin(messagebar_);
}


void Console::drawWorld(const Life &life)
{
    drawWindow();

    //    const auto &world = life.world();

    //    for (unsigned w = 0; w < life.width(); w++)
    //    {
    //        for (unsigned h = 0; h < life.height() - 1; h += 2)
    //        {
    //            bool is_liveUpCell = world[h][w].is_live_;
    //            bool is_liveDownCell = world[h + 1][w].is_live_;

    //            cchar_t t;
    //            // For cells that has upper block (either full blocks or the upper one only) the reverse-colour trick is used.
    //            t.attr = is_liveUpCell ? COLOR_PAIR(2) : COLOR_PAIR(1);
    //            // For both upper and lower blocks either present or absent altogether we print space (i.e. full block).
    //            // Otherwise, lower half block is printed.
    //            t.chars[0] = (is_liveUpCell == is_liveDownCell) ? ' ' : 0x2584; // LOWER HALF BLOCK
    //            t.chars[1] = 0;

    //            mvadd_wch(h / 2, w, &t);
    //        }
    //    }
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
