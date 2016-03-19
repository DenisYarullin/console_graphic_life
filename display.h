#ifndef DISPLAY_H
#define DISPLAY_H

class Life;

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
    Console();
    virtual ~Console();

    virtual bool endSimulation() const;
    virtual void drawWorld(const Life &life);
};

#endif // DISPLAY_H
