#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcms.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "MACROS.h"
#include "grid.h"
#include "snake.h"

Display* display;
int screen, width, height, windowx, windowy;
Colormap colormap;
Window root;
Window w1;
XWindowAttributes xattr;
GC gc;
XGCValues gc_data;
Grid* grid;
Visual* truecolor;
snake* XSnake;


void Connect(){
    display = XOpenDisplay(NULL);
    if(display){
        screen = DefaultScreen(display);
        root = DefaultRootWindow(display);
        w1 = XCreateSimpleWindow(display,root,0,0,DisplayWidth(display,screen),DisplayHeight(display,screen),0,0,0);
        XMapWindow(display,w1);
        XSelectInput(display,w1, ExposureMask | KeyPressMask);
    }else{
        std::cout << "connection to X server failed" << '\n';
    }
}

void close(){
    grid->map_destructor();
    delete grid;
    XFreeColormap(display,colormap);
    XFreeGC(display, gc);
	XDestroyWindow(display,w1);
    XCloseDisplay(display);
    exit(1);
}

/* Returns pointer to  visual type (visual*) from visual class (int) */
Visual* GetVisualFromClass(int cclass){
    XVisualInfo temp;
    temp.c_class = cclass;
    int n;
    XVisualInfo* visualinfo = XGetVisualInfo(display,VisualClassMask,&temp,&n);
    return visualinfo->visual;
}
void create_colormap(){
    /* set visual type to true color (24bit depth)*/
    truecolor = GetVisualFromClass(TrueColor);
    colormap = XCreateColormap(display,root,truecolor,AllocNone);
    XInstallColormap(display,colormap);
    XGetWindowAttributes(display,w1,&xattr);
}

/* converts rgb (0-255) to one 24 bit pixel vlaue that means 8bit per channel */
unsigned long get24pixval(int r, int g, int b){
    return  (r*65536) + (g*256) + b;
}

void getwindowinfo(){
    XGetWindowAttributes(display,w1,&xattr);
    width = xattr.width;
    height = xattr.height;
    windowx = xattr.x;
    windowy = xattr.y;
}

void redraw(){
    XClearWindow(display,w1);
    /*redraw grid*/
    XSetForeground(display,gc,get24pixval(5,30,5));
    grid->draw_grid(display,gc,w1);
    /*redraw snake*/
    XSnake->initialx = (width-(grid->dimension*grid->cell_dimension))/2;
    XSnake->initialy = (height-(grid->dimension*grid->cell_dimension))/2;;
    XSetForeground(display,gc,get24pixval(50,150,50));
    XSnake->print(display,w1,gc);
}

void animation(){
    XSnake->move();
    redraw();
}

int main(){
    
    Connect();
    create_colormap();

    XEvent event;

    /*set GC */
    gc_data.foreground = get24pixval(30,30,30);
    gc_data.background = 0;
    gc_data.line_width = 2;
    gc = XCreateGC(display,w1,GCForeground | GCBackground | GCLineWidth, &gc_data);
    /*draw grid*/
    grid = new Grid;
    
    grid->dimension = 50;
    grid->cell_dimension = 20;
    grid->linewidth = gc_data.line_width;

    grid->draw_grid(display,gc,w1);

    /*create snake*/
    getwindowinfo();

    XSnake = new snake;
    
    XSnake->size = (grid->cell_dimension)*0.7;
    XSnake->cellsize = grid->cell_dimension;
    XSnake->cellnumber = grid->dimension;
    XSnake->addnode();
    XSnake->addnode();
    XSnake->addnode();
    
   
    //XSnake->SnakeX = grid->cell_position_map

    /*Event Handling*/

    for(;;){

        if(XPending(display)){
            XNextEvent(display, &event);
            switch(event.type){
                case(Expose):
                    getwindowinfo();
                    grid->WinH = height;
                    grid->WinW = width;
                    redraw();
                    XSetForeground(display,gc,get24pixval(255,50,50));
                    break;
                case(KeyPress):
                    XSnake->oldx = XSnake->SnakeX;
                    XSnake->oldy = XSnake->SnakeY;
                    switch((event.xkey).keycode){
                        
                        case(A): XSnake->left(); std::cout << "left" <<'\n'; break;
                        case(W): XSnake->up(); break;
                        case(S): XSnake->down(); break;
                        case(D): XSnake->right(); break;
                        case(Q): close(); 
                        default:break;
                    }
                    break;

                default: break;

            }
        }

        if(XSnake->SnakeX < 0) XSnake->SnakeX = 0;
        else if(XSnake->SnakeX == grid->dimension) XSnake->SnakeX = grid->dimension - 1;
        else if(XSnake->SnakeY < 0) XSnake->SnakeY = 0;
        else if(XSnake->SnakeY == grid->dimension) XSnake->SnakeY = grid->dimension - 1;
        else animation();
            
        if(!XPending(display)){
            usleep(200000);
        }
        
        
    }
        
    
    return 0;
}