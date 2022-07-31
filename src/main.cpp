#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcms.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Display* display;
int screen_number;
Colormap colormap;
Window root;
Window w1;
Pixmap p1;
XWindowAttributes xattr;
XcmsCCC ccc;
GC gc;
XGCValues* gc_data;

XImage space;

void Connect(){
    /*establish connection to X server*/
    display = XOpenDisplay(NULL);
    screen_number = DefaultScreen(display);

    /*pixel values*/

    unsigned long white_pixel, black_pixel;
    white_pixel = WhitePixel(display,screen_number);
    black_pixel = BlackPixel(display,screen_number);

    /*Create Window*/
    root = DefaultRootWindow(display);
    w1 = XCreateSimpleWindow(display,root,0,0,DisplayWidth(display,screen_number),DisplayHeight(display,screen_number),0,black_pixel,black_pixel);
    XMapWindow(display,w1);
    

    /*createpixmap*/

    p1 = XCreatePixmap(display,w1,DisplayWidth(display,screen_number),DisplayHeight(display,screen_number),DefaultDepth(display,screen_number));

    /* create (default) Graphics Context (GC) */

    gc = XDefaultGC(display,screen_number);

    /*set cursor*/
    XCreateFontCursor(display,2);

    XFlush(display); //pass output buffer
}

void close(){
    XFreePixmap(display,p1);
    XcmsFreeCCC(ccc);
    XFreeColormap(display,colormap);
    XSetCloseDownMode(display,1); /*1(default) destroys all resources
    retein: (2) permament resources (3) temporarty resorces */
    XCloseDisplay(display);
}

Visual* truecolor;

/* Returns pointer to  visual type (visual*) from visual class (int) */

Visual* GetVisualFromClass(int cclass){
    XVisualInfo temp;
    temp.c_class = cclass;
    int n;
    XVisualInfo* visualinfo = XGetVisualInfo(display,VisualClassMask,&temp,&n);
    return visualinfo->visual;
}
void create_colormap(){
    /* set visual type to true color (24bit depth) colormap has allocated cells with set values */
    truecolor = GetVisualFromClass(TrueColor);
    colormap = XCreateColormap(display,root,truecolor,AllocNone);
    XInstallColormap(display,colormap);

    ccc = XcmsCCCOfColormap(display,colormap);

    /* print stuff */
    XGetWindowAttributes(display,w1,&xattr);
    //std::cout <<  << '\n';
}

void loadimage(XImage img){
    //&img = XCreateImage(display,truecolor,DefaultDepth(display,screen_number),ZPixmap,NULL,DisplayWidth(display,screen_number),DisplayHeight(display,screen_number))
    img.width = DisplayWidth(display,screen_number);
    img.height = DisplayHeight(display,screen_number);
    XInitImage(&img);
    XPutImage(display,w1,gc,&img,0,0,0,0,DisplayWidth(display,screen_number),DisplayHeight(display,screen_number));
}

int main(){
    
    Connect();
    create_colormap();

    //loadimage(space);

    for(;;)sleep(1);
    /*Atom* a =  XListProperties(display,w1,npr);
    pribnt array
    for(int i = 0; i <= *npr; i++){ std::cout << *(a+i) << '\n';} */
    return 0;
}