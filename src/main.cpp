#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcms.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "grid.h"
#include "snake.h"
#include "ui.h"
#include <string.h>

Display* display;
int screen, width, height;
Colormap colormap;
Window root, w1;
XWindowAttributes xattr;
GC gc;
XGCValues gc_data;
Grid* grid;
Visual* truecolor;
snake* XSnake;
menu Menu;


void Connect(){
    display = XOpenDisplay(NULL);
    if(display){
        screen = DefaultScreen(display);
        root = DefaultRootWindow(display);
        w1 = XCreateSimpleWindow(display,root,0,0,DisplayWidth(display,screen),DisplayHeight(display,screen),0,0,0);
        XMapWindow(display,w1);
        XSelectInput(display,w1, ExposureMask | KeyPressMask | ButtonPressMask);
    } else { std::cout << "connection to X server failed" << '\n';}
}

void close(){
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

/* converts rgb (0-255) to one 24 bit pixel vlaue that means 8bit per channel */
unsigned long get24pixval(int r, int g, int b){
    return  (r*65536) + (g*256) + b;
}

void getwindowinfo(){
    XGetWindowAttributes(display,w1,&xattr);
    width = xattr.width;
    height = xattr.height;
}

void create_colormap(){
    colormap = XCreateColormap(display,w1,GetVisualFromClass(TrueColor),AllocNone);
    XInstallColormap(display,colormap);
}

bool ded = 0;

void redraw(){
    XSetForeground(display,gc,get24pixval(5,30,5));
    grid->draw_grid(display,gc,w1);
    XSnake->initialx = (width-(grid->dimension*grid->cell_dimension))/2;
    XSnake->initialy = (height-(grid->dimension*grid->cell_dimension))/2;
    XSetForeground(display,gc,get24pixval(50,150,50));
    if(XSnake->head->x == (grid->foodx-((XSnake->size - grid->size)/2)) 
    && XSnake->head->y == (grid->foody-((XSnake->size - grid->size)/2))){
        XSnake->add=1; grid->createfood();
    }
    grid->printfood(display,gc,w1);
    XSnake->print(display,w1,gc);
    if(XSnake->detectdeath()){ded=0;XClearWindow(display,w1);}
}
void animation(){
    XSnake->move();
    XSnake->pop(display,w1);
    redraw();
}

int main(){
    
    Connect();
    create_colormap();

    XEvent event;

    /*set GC */
    
    
    gc_data.line_width = 2;
    gc = XCreateGC(display,w1,GCLineWidth ,&gc_data);
    
    /*XFontStruct* font = XLoadQueryFont(display, "*");
    if(font)std::cout << "font found" << '\n';
    */
    /*draw grid*/
  

    grid = new Grid;
    
    grid->dimension = 50;
    grid->cell_dimension = 20;
    grid->linewidth = gc_data.line_width;

    /*create snake*/

    XSnake = new snake;
    
    XSnake->size = (grid->cell_dimension)*0.7;
    XSnake->cellsize = grid->cell_dimension;
    XSnake->cellnumber = grid->dimension;
    XSnake->push();

    /*Button dimensions*/

    Menu.buttheight = 100;
    Menu.buttwidth = 300;

    /*initial menu event loop*/

    while(!ded){
        XNextEvent(display, &event);
        switch(event.type){
            case(Expose):
                getwindowinfo();
                /*set game scene*/
                grid->WinH = height;
                grid->WinW = width;
                XSnake->head->x=((width-(grid->dimension*grid->cell_dimension))/2)+((XSnake->cellsize-XSnake->size)/2);
                XSnake->head->y=((height-(grid->dimension*grid->cell_dimension))/2)+((XSnake->cellsize-XSnake->size)/2);
                /*menu redraw*/
                Menu.centerx = (width-Menu.buttwidth)/2;
                Menu.centery = (height-Menu.buttheight)/2;
                Menu.redraw(display,gc,w1,get24pixval(50,100,50),get24pixval(100,150,100));
                break;
            case(ButtonPress):
                if(event.xbutton.x > Menu.centerx && event.xbutton.x < (Menu.centerx + Menu.buttwidth) 
                && event.xbutton.y > Menu.centery && event.xbutton.y < (Menu.centery + Menu.buttheight)){
                    ded = 1;
                    Menu.redraw(display,gc,w1,get24pixval(100,150,100),get24pixval(50,100,50));
                    XFlush(display);
                    usleep(200000);
                    XClearWindow(display,w1);
                    grid->createfood();
                    XSetLineAttributes(display,gc,2,0,0,0);
                }
                break;
            default:break;
        }
    }

    /*Main Event Handling Loop*/

    for(;;){     
        if(XPending(display)){
            XNextEvent(display, &event);
            if(ded){
                switch(event.type){
                    case(Expose):
                        getwindowinfo();
                        grid->WinH = height;
                        grid->WinW = width;
                        redraw();
                        XSetForeground(display,gc,get24pixval(255,50,50));
                        break;
                        
                    case(KeyPress):
                        switch((event.xkey).keycode){
                            
                            case(113):case(38): XSnake->left(); break; //left
                            case(25):case(111): XSnake->up(); break; //up
                            case(39):case(116): XSnake->down(); break; //down
                            case(40):case(114): XSnake->right(); break; //right
                            case(24):close(); 
                            default:break;
                        }
                        break;

                    default: break;
                }
            }else{
                switch(event.type){
                    case(Expose):
                        getwindowinfo();
                        Menu.centerx = (width-Menu.buttwidth)/2;
                        Menu.centery = (height-Menu.buttheight)/2;
                        XClearWindow(display,w1);
                        break;
                    case(ButtonPress):
                        if(event.xbutton.x > Menu.centerx && event.xbutton.x < (Menu.centerx + Menu.buttwidth) 
                        && event.xbutton.y > Menu.centery && event.xbutton.y < (Menu.centery + Menu.buttheight)){
                            ded=1;
                            Menu.redraw(display,gc,w1,get24pixval(100,150,100),get24pixval(50,100,50));
                            XFlush(display);
                            usleep(200000);
                            XSnake->head->x=((width-(grid->dimension*grid->cell_dimension))/2)+((XSnake->cellsize-XSnake->size)/2);
                            XSnake->head->y=((height-(grid->dimension*grid->cell_dimension))/2)+((XSnake->cellsize-XSnake->size)/2);
                            XSnake->direction = 3;
                            grid->createfood();
                            XSetLineAttributes(display,gc,2,0,0,0);
                            XClearWindow(display,w1);
                            XSnake->DeleteSnake();
                        }
                        break;
                    default:break;
                }
            }
        }

        if(ded)animation();
        else Menu.redraw(display,gc,w1,get24pixval(50,100,50),get24pixval(100,150,100));
        
        if(!XPending(display)&&ded) usleep(200000);
    }

    return 0;
}