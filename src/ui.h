#include <X11/Xlib.h>
#include <X11/Xutil.h>

class button{
    public:
    void print(Display* d, GC gc, Drawable drawable, char* string, unsigned long shadow,unsigned long light,
    short int x, short int y, int wdith, int height);
    
};

class menu{
    public:
    char* text;
    button play;
    short int centerx, centery;
    int buttwidth, buttheight;
    
    void redraw(Display* d, GC gc, Drawable drawable, unsigned long shadow,unsigned long light);
    void gameover();

};

class stats{

    int hightscore, score;
    
    void print(Display* d, GC gc, Drawable drawable);
    void writehs(); //write highscore from file
    void readhs (); //read highscore from file
};