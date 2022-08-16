#include <X11/Xlib.h>
#include <X11/Xutil.h>

class Grid{

    /* data */

    public:

    int dimension, cell_dimension, linewidth, cells, foodx, foody, size, *cell_position_map, WinH, WinW;

    /* methods */

    Grid();
    ~Grid();

    void createfood();
    void printfood(Display* d, GC gc, Drawable drawable);
    void draw_grid(Display* d, GC gc, Drawable drawable);
    
};