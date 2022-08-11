#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "ui.h"

void button::print(Display* d, GC gc, Drawable drawable, char* string, unsigned long shadow,unsigned long light, short int x, short int y, int width, int height){
    XSetForeground(d,gc,light);
    XDrawLine(d,drawable,gc,x,y,x+width,y);
    XDrawLine(d,drawable,gc,x,y,x,y+height);
    XDrawString(d,drawable,gc,x+(width/2),y+(height/2),"Hello World",11);
    XSetForeground(d,gc,shadow);
    XDrawLine(d,drawable,gc,x+width,y+height,x+width,y);
    XDrawLine(d,drawable,gc,x,y+height,x+width,y+height);
}


void menu::redraw(Display* d, GC gc, Drawable drawable, unsigned long shadow,unsigned long light){
    XSetLineAttributes(d,gc,10,0,0,0);
    play.print(d,gc,drawable,NULL,shadow,light,centerx,centery,buttwidth,buttheight); 
}
