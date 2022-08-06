#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

#include "snake.h"

void snake::addnode(){
    SnakeNode* ptr = new SnakeNode;
    SnakeNode* tmp = NULL;
    length++;
    if(!root){
        root = ptr;
      
    }else{
        tmp = root;
        while(tmp->prev){
            tmp = tmp->prev;
        }
        tmp->prev = ptr;
    }
}

void snake::move(){
    switch(direction){
        case(1):SnakeX--;break;
        case(2):SnakeY--;break;
        case(3):SnakeX++;break;
        case(4):SnakeY++;break;
        default:break;
    }
    
}

void snake::print(Display* d, Drawable drawable, GC gc){
    if(root){
        SnakeNode* tmp = root;

        for(int i = 0; i < length; i++){
            
                tmp -> x = initialx+((cellsize-size)/2)+(SnakeX*cellsize)-(i*cellsize);
                tmp -> y = initialy+((cellsize-size)/2)+(SnakeY*cellsize);
       

            XFillRectangle(d,drawable,gc,tmp->x,tmp->y,size,size);
            tmp = tmp->prev;
        }
    }
}

void snake::left(){ direction = 1;};
void snake::right(){ direction = 3;};
void snake::up(){ direction = 2;};
void snake::down(){ direction = 4;};