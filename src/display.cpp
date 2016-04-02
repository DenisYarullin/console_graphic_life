#include "display.h"
#include "life.h"
#include <cstring>
#include <algorithm>
#include <ncursesw/ncurses.h>
#include <ncursesw/menu.h>
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

    fileNames = {"Load", "Save", "Quit"};
    stillLifeNames = {"Block", "Bucket", "Boat", "Ship", "Snake", "Long snake", "Hive", "Load", "Pond",
                      "Long boat", "Long ship", "Barge", "Fishhook", "Hat", "Aircraft carrier", "Honeycomb",
                      "Sinking ship", "Baton"};

    oscillatorNames = {"Buoy", "Clock", "Toad", "Eight", "Pentadekatlon", "Toggle switch", "Pulsar", "Diamond ring",
                       "Whirligig", "Glider", "Space ship"};

    // stillLifesNames = {"Блок", "Бадья", "Лодка", "Корабль", "Змея",
    // "Длинная змея", "Улей", "Каравай", "Пруд", "Длинная лодка",
    // "Длинный корабль", "Баржа", "Рыболовный крючок", "Шляпа", "Авианосец",
    //"Соты", "Тонущий корабль", "Дубинка", "Длинная баржа"};

    //    oscillatorsNames = {"Бакен", "Часы", "Жаба", "Восьмерка", "Пентадекатлон",
    //                        "Тумблер", "Пульсар", "Бриллиантовое кольцо", "Вертушка",
    //                        "Глайдер", "Корабль"};
}


Console::~Console()
{
    //    echo();
    //    unpost_menu(fileMenu);
    //    free_menu(fileMenu);
    //    for(int i = 0; i < 3; i++)
    //        free_item(fileItems[i]);
    //    endwin();
}


bool Console::endSimulation() const
{
    return getch() == 'q';
}


void print_in_middle1(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
    int length, x, y;
    float temp;

    length = strlen(string);
    temp = (width - length)/ 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, starty, x, "%s", string);
    wattroff(win, color);
    refresh();
}


void Console::drawMenubar(WINDOW *menubar)
{
    wbkgd(menubar,COLOR_PAIR(4));

    print_in_middle1(menubar, 0, 0, 10, "File(F1)", COLOR_PAIR(5));
    wattron(menubar, COLOR_PAIR(5)); // Turn on the given attributes

    print_in_middle1(menubar, 0, 10, 22, "Still lifes(F2)", COLOR_PAIR(5));
    wattron(menubar, COLOR_PAIR(5));

    print_in_middle1(menubar, 0, 32, 19, "Oscillators(F3)", COLOR_PAIR(5));
    wattron(menubar, COLOR_PAIR(5));

    wattroff(menubar, COLOR_PAIR(5)); // Turn off the given attributes
    wrefresh(menubar);
}



void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
    int length, x, y;
    float temp;

    if(win == NULL) win = stdscr;
    getyx(win, y, x);
    if(startx != 0) x = startx;
    if(starty != 0) y = starty;
    if(width == 0) width = 80;

    length = strlen(string);
    temp = (width - length)/ 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}


WINDOW **Console::drawMenuFile(int countElem, int start_col)
{
    int c;
    fileItems = (ITEM **)calloc(countElem + 1, sizeof(ITEM *));

    /* Create items */
    for (size_t i = 0; i < countElem; i++)
    {
        fileItems[i] = new_item(fileNames[i].c_str(), "");
    }
    fileItems[countElem] = (ITEM *)NULL;

    /* Create menu */
    fileMenu = new_menu((ITEM **)fileItems);
    set_menu_fore(fileMenu, COLOR_PAIR(3));
    set_menu_back(fileMenu, COLOR_PAIR(4));

    /* Create the window to be associated with the menu */
    fileMenuWin = newwin(countElem + 2, 10, 1, 0);
    wbkgd(fileMenuWin, COLOR_PAIR(4));
    keypad(fileMenuWin, TRUE);

    /* Set main window and sub window */
    set_menu_win(fileMenu, fileMenuWin);
    set_menu_sub(fileMenu, derwin(fileMenuWin, countElem, 7, 1, 1));

    /* Set menu mark to the string " * " */
    set_menu_mark(fileMenu, " ->");

    /* Print a border around the main window and print a title */
    box(fileMenuWin, 0, 0);
    //print_in_middle(fileMenuWin, 1, 0, 10, "File", COLOR_PAIR(5));
    //mvwaddch(fileMenuWin, 2, 0, ACS_LTEE);
    //mvwhline(fileMenuWin, 2, 1, ACS_HLINE, 10);
    //mvwaddch(fileMenuWin, 2, 9, ACS_RTEE);
    mvprintw(LINES - 2, 0, "F1 to exit");
    refresh();

    /* Post the menu */
    post_menu(fileMenu);
    wrefresh(fileMenuWin);

    // Process the user requests with a loop and do necessary updates to menu with menu_driver.
    while((c = wgetch(fileMenuWin)) != ESCAPE)
    {
        switch(c)
        {
        case KEY_DOWN:
            menu_driver(fileMenu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(fileMenu, REQ_UP_ITEM);
            break;
        }
        wrefresh(fileMenuWin);
    }


    //return items;
}


WINDOW **Console::drawMenuStillLifes(int countElem, int start_col)
{
    int c;
    stillLifeItems = (ITEM **)calloc(countElem + 1, sizeof(ITEM *));

    /* Create items */
    for (size_t i = 0; i < countElem; i++)
    {
        stillLifeItems[i] = new_item(stillLifeNames[i].c_str(), "");
    }
    stillLifeItems[countElem] = (ITEM *)NULL;

    /* Create menu */
    stillLifeMenu = new_menu((ITEM **)stillLifeItems);
    set_menu_fore(stillLifeMenu, COLOR_PAIR(3));
    set_menu_back(stillLifeMenu, COLOR_PAIR(4));

    /* Create the window to be associated with the menu */
    stillLifeMenuWin = newwin(countElem + 2, 22, 1, 10);
    wbkgd(stillLifeMenuWin, COLOR_PAIR(4));
    keypad(stillLifeMenuWin, TRUE);

    /* Set main window and sub window */
    set_menu_win(stillLifeMenu, stillLifeMenuWin);
    set_menu_sub(stillLifeMenu, derwin(stillLifeMenuWin, countElem, 20, 1, 1));

    /* Set menu mark to the string " * " */
    set_menu_mark(stillLifeMenu, " ->");

    /* Print a border around the main window and print a title */
    box(stillLifeMenuWin, 0, 0);
    //print_in_middle(stillLifeMenuWin, 1, 0, 22, "Still lifes", COLOR_PAIR(5));
//    mvwaddch(stillLifeMenuWin, 2, 0, ACS_LTEE);
//    mvwhline(stillLifeMenuWin, 2, 1, ACS_HLINE, 20);
//    mvwaddch(stillLifeMenuWin, 2, 21, ACS_RTEE);
    refresh();

    /* Post the menu */
    post_menu(stillLifeMenu);
    wrefresh(stillLifeMenuWin);

    // Process the user requests with a loop and do necessary updates to menu with menu_driver.
    while((c = wgetch(stillLifeMenuWin)) != ESCAPE)
    {
        switch(c)
        {
        case KEY_DOWN:
            menu_driver(stillLifeMenu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(stillLifeMenu, REQ_UP_ITEM);
            break;
        }
        wrefresh(stillLifeMenuWin);
    }



    //    size_t numStillLifesElem = stillLifesNames.size();
    //    items = new WINDOW *[numStillLifesElem + 1];

    //    items[0] = newwin(numStillLifesElem + 2, stillLifesOffset, 1, start_col);
    //    wbkgd(items[0], COLOR_PAIR(4));
    //    box(items[0], ACS_VLINE, ACS_HLINE);

    //    size_t maxLenName = std::max_element(stillLifesNames.begin(), stillLifesNames.end(),
    //                                         [](const std::string &str1, const std::string &str2)
    //                                         {return str1.length() < str2.length();})->length();

    //    for (size_t i = 0; i < numStillLifesElem; i++)
    //    {
    //        items[i + 1] = subwin(items[0], 1, maxLenName / 2 + 1, i + 2, start_col + 1);
    //        wprintw(items[i + 1], stillLifesNames[i].c_str());
    //    }

    //    wbkgd(items[1], COLOR_PAIR(1));
    //    wrefresh(items[0]);
    //    return items;
}


WINDOW **Console::drawMenuOscillators(int countElem, int start_col)
{
    int c;
    oscillatorItems = (ITEM **)calloc(countElem + 1, sizeof(ITEM *));

    /* Create items */
    for (size_t i = 0; i < countElem; i++)
    {
        oscillatorItems[i] = new_item(oscillatorNames[i].c_str(), "");
    }
    oscillatorItems[countElem] = (ITEM *)NULL;

    /* Create menu */
    oscillatorMenu = new_menu((ITEM **)oscillatorItems);
    set_menu_fore(oscillatorMenu, COLOR_PAIR(3));
    set_menu_back(oscillatorMenu, COLOR_PAIR(4));

    /* Create the window to be associated with the menu */
    oscillatorMenuWin = newwin(countElem + 2, 19, 1, 32);
    wbkgd(oscillatorMenuWin, COLOR_PAIR(4));
    keypad(oscillatorMenuWin, TRUE);

    /* Set main window and sub window */
    set_menu_win(oscillatorMenu, oscillatorMenuWin);
    set_menu_sub(oscillatorMenu, derwin(oscillatorMenuWin, countElem, 18, 1, 1));

    /* Set menu mark to the string " * " */
    set_menu_mark(oscillatorMenu, " ->");

    /* Print a border around the main window and print a title */
    box(oscillatorMenuWin, 0, 0);
//    print_in_middle(oscillatorMenuWin, 1, 0, 19, "Oscillators", COLOR_PAIR(5));
//    mvwaddch(oscillatorMenuWin, 2, 0, ACS_LTEE);
//    mvwhline(oscillatorMenuWin, 2, 1, ACS_HLINE, 17);
//    mvwaddch(oscillatorMenuWin, 2, 18, ACS_RTEE);
    refresh();

    /* Post the menu */
    post_menu(oscillatorMenu);
    wrefresh(oscillatorMenuWin);

    // Process the user requests with a loop and do necessary updates to menu with menu_driver.
    while((c = wgetch(oscillatorMenuWin)) != ESCAPE)
    {
        switch(c)
        {
        case KEY_DOWN:
            menu_driver(oscillatorMenu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(oscillatorMenu, REQ_UP_ITEM);
            break;
        }
        wrefresh(oscillatorMenuWin);
    }


    //    size_t numOscillatorsElem = oscillatorsNames.size();
    //    items = new WINDOW *[numOscillatorsElem + 1];

    //    items[0] = newwin(numOscillatorsElem + 2, oscillatorsOffset - stillLifesOffset, 1, start_col);
    //    wbkgd(items[0], COLOR_PAIR(4));
    //    box(items[0], ACS_VLINE, ACS_HLINE);

    //    size_t maxLenName = std::max_element(oscillatorsNames.begin(), oscillatorsNames.end(),
    //                                         [](const std::string &str1, const std::string &str2)
    //                                         {return str1.length() < str2.length();})->length();

    //    for (size_t i = 0; i < numOscillatorsElem; i++)
    //    {
    //        items[i + 1] = subwin(items[0], 1, maxLenName / 2 + 1, i + 2, start_col + 1);
    //        wprintw(items[i + 1], oscillatorsNames[i].c_str());
    //    }

    //    wbkgd(items[1], COLOR_PAIR(1));
    //    wrefresh(items[0]);
    //    return items;
}


void Console::deleteMenu(WINDOW *win, ITEM **items, MENU *menu, int count)
{
    echo();
    unpost_menu(menu);
    free_menu(menu);
    for(int i = 0; i < count; i++)
        free_item(items[i]);
    delwin(win);
}


int Console::scrollMenu(WINDOW **items, int count, int menu_start_col)
{
    //    int key;
    //    int selected = 0;
    //    while(true)
    //    {
    //        key = getch();
    //        if(key == KEY_DOWN || key == KEY_UP)
    //        {
    //            wbkgd(items[selected + 1], COLOR_PAIR(4));
    //            wnoutrefresh(items[selected + 1]); // Copies window to virtual screen
    //            if (key == KEY_DOWN)
    //            {
    //                selected = (selected + 1) % count;
    //            }
    //            else
    //            {
    //                selected = (selected + count - 1) % count;
    //            }
    //            wbkgd(items[selected + 1], COLOR_PAIR(3));
    //            wnoutrefresh(items[selected + 1]);
    //            doupdate(); // Write all prepared refreshes to terminal
    //        }
    //        else if (key == KEY_LEFT || key == KEY_RIGHT)
    //        {
    //            deleteMenu(items, count + 1);
    //            touchwin(stdscr);
    //            refresh();

    //            int count = 0;
    //            if (menu_start_col == stillLifesOffset)
    //            {
    //                items = drawMenuStillLifes(stillLifesOffset);
    //                count = stillLifesNames.size();
    //                return scrollMenu(items, count, oscillatorsOffset - menu_start_col);
    //            }
    //            else if (menu_start_col == oscillatorsOffset)
    //            {
    //                items = drawMenuOscillators(oscillatorsOffset);
    //                count = oscillatorsNames.size();
    //            }
    //            else
    //            {
    //                items = drawMenuFile(fileMenuOffset);
    //                count = 4;
    //            }
    //        }
    //        else if (key == ESCAPE)
    //        {
    //            return -1;
    //        }
    //        else if (key == ENTER)
    //        {
    //            return selected;
    //        }
    //    }
}


void Console::drawWindow()
{

    // Process the user requests with a loop and do necessary updates to menu with menu_driver.
    //    while((c = wgetch(fileMenuWin)) != KEY_F(1))
    //    {
    //        switch(c)
    //        {
    //        case KEY_DOWN:
    //            menu_driver(fileMenu, REQ_DOWN_ITEM);
    //            break;
    //        case KEY_UP:
    //            menu_driver(fileMenu, REQ_UP_ITEM);
    //

    bkgd(COLOR_PAIR(3)); // Set background property for terminal screen
    menubar_ = subwin(stdscr, 1, COLS, 0, 0);
    messagebar_ = subwin(stdscr, 1, COLS - 1, 23, 1);

    drawMenubar(menubar_);
    //    move(2, 1);
    //    printw("Press F1 or F2 to open the menus. ");
    //    printw("ESC quits.");
    refresh();

    int key;
    do {
        //        int selected_item;
        //        WINDOW **menu_items;
        key = getch();
        //        werase(messagebar_); // Erase terminal screen
        //        wrefresh(messagebar_);
        if (key == KEY_F(1))
        {
            size_t numFileMenuElem = fileNames.size();
            drawMenuFile(numFileMenuElem, 0);
            deleteMenu(fileMenuWin, fileItems, fileMenu, numFileMenuElem + 1);
            //            menu_items = drawMenuFile(fileMenuOffset);
            //            selected_item = scrollMenu(menu_items, 4, fileMenuOffset);
            //            deleteMenu(menu_items, 4);
            //            if (selected_item < 0)
            //                wprintw(messagebar_,"You haven't selected any item.");
            //            else
            //                wprintw(messagebar_, "You have selected menu item %d.",selected_item + 1);
            touchwin(stdscr);
            refresh();
        }
        else if (key == KEY_F(2))
        {
            size_t numStillLifeMenuElem = stillLifeNames.size();
            drawMenuStillLifes(numStillLifeMenuElem, 0);
            deleteMenu(stillLifeMenuWin, stillLifeItems, stillLifeMenu, numStillLifeMenuElem + 1);
            //            menu_items = drawMenuStillLifes(stillLifesOffset);
            //            selected_item = scrollMenu(menu_items, stillLifesNames.size(), stillLifesOffset);
            //            deleteMenu(menu_items, 4);
            //            if (selected_item < 0)
            //                wprintw(messagebar_,"You haven't selected any item.");
            //            else
            //                wprintw(messagebar_, "You have selected menu item %d.", selected_item + 1);
            touchwin(stdscr);
            refresh();
        }
        else if (key == KEY_F(3))
        {
            size_t numOscillatorMenuElem = oscillatorNames.size();
            drawMenuOscillators(numOscillatorMenuElem, 0);
            deleteMenu(oscillatorMenuWin, oscillatorItems, oscillatorMenu, numOscillatorMenuElem + 1);
            //            menu_items = drawMenuOscillators(oscillatorsOffset);
            //            selected_item = scrollMenu(menu_items, oscillatorsNames.size(), oscillatorsOffset);
            //            deleteMenu(menu_items, 4);
            //            if (selected_item < 0)
            //                wprintw(messagebar_,"You haven't selected any item.");
            //            else
            //                wprintw(messagebar_, "You have selected menu item %d.", selected_item + 1);
            touchwin(stdscr);
            refresh();
        }
    } while (key != ESCAPE);

    //    delwin(menubar_);
    //    delwin(messagebar_);
}


void Console::drawWorld(const Life &life)
{
    //drawMenuFile(0);
    //drawMenuStillLifes(0);
    //drawMenuOscillators(0);
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
