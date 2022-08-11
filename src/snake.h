#include <X11/Xutil.h>
#include <X11/Xlib.h>

struct SnakeNode{
    int x,y;
    SnakeNode* prev,* next = NULL;
};

class snake{

    public:

    int length = 1;
    int direction = 3; /* 1-left, 2-up, 3-right, 4-down*/
    int size;

    int initialx, initialy;
    int cellsize, cellnumber;

    bool add,overlap = 0;

    SnakeNode* head,* tail = NULL;

   
    void print(Display* d, Drawable drawable, GC gc);
    SnakeNode* push();
    void pop(Display* d, Window window);

    bool detectdeath();
    
    void move();
    void left();
    void right();
    void up();
    void down();
    void deletenode(SnakeNode* tmp);
    void DeleteSnake();
    
};