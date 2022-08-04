#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcms.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#include "grid.h"

Display* display;
int screen, width, height;
Colormap colormap;
Window root, w1;
XWindowAttributes xattr;
GC gc;
XGCValues gc_data;
//Grid* grid;
Visual* truecolor;


void Connect(){
    /*establish connection to X server*/
    display = XOpenDisplay(NULL);

    if(display){
        screen = DefaultScreen(display);

        /*Create Window*/
        root = DefaultRootWindow(display);
        w1 = XCreateSimpleWindow(display,root,0,0,DisplayWidth(display,screen),DisplayHeight(display,screen),0,0,0);
        XMapWindow(display,w1);
        
        XFlush(display); //pass output buffer
    }else{
        std::cout << "connection to X server failed" << '\n';
    }
    
}

void close(){

    XFreeColormap(display,colormap);
    XCloseDisplay(display);
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
}

int main(){
    
    Connect();
    create_colormap();
    getwindowinfo();
    
    gc_data.foreground = get24pixval(255,255,255);
    gc_data.background = 0;
    gc_data.line_width = 40;
    gc = XCreateGC(display,w1,GCForeground | GCBackground | GCLineWidth, &gc_data);
    
    XDrawLine(display,w1,gc,0,0,500,500);
    
    /*
    int center[2] = {width/2,height/2}; //x,y
    
    grid = new Grid;
 
    grid->dimension = 15;
    grid->cell_dimension = 70;
    grid->center = center;
    grid->linewidth = gc_data.line_width;
    grid->draw_grid(display,gc,w1);
    XSetForeground(display,gc,get24pixval(255,50,50));

    for(int i = 0; i++ <= 5;){
        grid->createfood(display,gc,w1);
        XFlush(display);
        sleep(1);
        grid->eat(display,gc,w1);
        XFlush(display);
    }
    */
    
   
    XFlush(display);

    for(;;)sleep(1);
    return 0;
}