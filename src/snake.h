#include <X11/Xutil.h>
#include <X11/Xlib.h>

struct SnakeNode{

    int x,y;
    SnakeNode* previous;


};

class snake{

    public:

    int length;
    int direction = 3; /* 1-left, 2-up, 3-right, 4-down*/
    int size;

    int initialx, initialy;
    int cellsize, cellnumber;

    short int SnakeX, SnakeY = 0;

    SnakeNode* root = NULL;

   
    void print(Display* d, Drawable drawable, GC gc);
    void addnode();
    void move();
    void left();
    void right();
    void up();
    void down();
    
};