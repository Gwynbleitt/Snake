#include <X11/Xlib.h>
#include <X11/Xutil.h>

class Grid{

    /* data */

    public:

    int dimension, cell_dimension, linewidth, cells, foodx, foody, size, *cell_position_map, WinH, WinW;

    /* methods */

    void createfood();
    void createmap();
    void printfood(Display* d, GC gc, Drawable drawable);
    void draw_grid(Display* d, GC gc, Drawable drawable);
    void map_destructor();
    
};