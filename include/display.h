#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>

class Life;
typedef struct _win_st WINDOW;
typedef struct tagMENU  MENU;
typedef struct tagITEM  ITEM;

class Display
{
public:
    Display() : width_(0), height_(0) {}
    //Display(int width, int height);
    virtual ~Display() {}

    int width() const { return width_; }
    int height() const { return height_; }
    void setWidth(int value) { width_ = value; }
    void setHeight(int value) { height_ = value; }

    virtual bool endSimulation() const = 0;
    virtual void drawWorld(const Life & life) = 0;

private:
    int width_;
    int height_;
};


class Console : public Display
{
public:
    Console(int heightOffset, int widthOffset);
    virtual ~Console();

    virtual bool endSimulation() const;
    virtual void drawWorld(const Life &life);
    void drawMenubar(WINDOW *menubar);
    WINDOW **drawMenuFile(int count, int start_col);
    WINDOW **drawMenuStillLifes(int count, int start_col);
    WINDOW **drawMenuOscillators(int count, int start_col);
    void drawWindow();
    void deleteMenu(WINDOW *win, ITEM **items, MENU *menu, int count);
    int scrollMenu(WINDOW **items, int count, int menu_start_col);

private:
    int key, keyExit;
    WINDOW *menubar_, *messagebar_;
    WINDOW *fileMenuWin, *stillLifeMenuWin, *oscillatorMenuWin;
    ITEM **fileItems, **stillLifeItems, **oscillatorItems;
    MENU *fileMenu, *stillLifeMenu, *oscillatorMenu;

    std::vector<std::string> stillLifeNames;
    std::vector<std::string> oscillatorNames;
    std::vector<std::string> fileNames;

    int heightOffset_, widthOffset_;
};

#endif // DISPLAY_H
