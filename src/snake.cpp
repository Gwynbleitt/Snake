#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

#include "snake.h"
#include "grid.h"

SnakeNode* snake::push(){
    SnakeNode* ptr = new SnakeNode;
    
    if(head){
        ptr->prev = head;
        head->next = ptr;
        head = ptr;
        if(add){add=0;length++;}
        else tail=tail->next;
    }else{
        head = ptr;
        tail = ptr;
    }  
    return ptr;
}

void snake::pop(Display* d, Window window){
    if(tail->prev){
        XClearArea(d,window,tail->prev->x,tail->prev->y,size,size,0);
        delete tail->prev;
        tail->prev=NULL;
    }
}

void snake::move(){
    SnakeNode* ptr = push();
    switch(direction){
        case(1):ptr->x=(ptr->prev->x)-cellsize; ptr->y=(ptr->prev->y); break;
        case(2):ptr->y=(ptr->prev->y)-cellsize; ptr->x=(ptr->prev->x); break;
        case(3):ptr->x=(ptr->prev->x)+cellsize; ptr->y=(ptr->prev->y); break;
        case(4):ptr->y=(ptr->prev->y)+cellsize; ptr->x=(ptr->prev->x); break;
        default:break;
    }
}

void snake::print(Display* d, Drawable drawable, GC gc){
    if (head) {
        XFillRectangle(d,drawable,gc,head->x,head->y,size,size);
        XFillRectangle(d,drawable,gc,tail->x,tail->y,size,size);
    }
}

bool snake::detectdeath(){
    SnakeNode *tmp = tail;
    while(tmp!=head){
        if(tmp->x == head->x && tmp->y == head->y && length>1) overlap = 1;
        tmp=tmp->next;
    }
    if((head->x<initialx || head->x>initialx+(cellnumber*cellsize) || head->y<initialy || head->y>initialy+(cellnumber*cellsize))
    || overlap){overlap = 0; return 1;}
    else return 0;
}


void snake::left(){ if(direction==4 || direction==2) direction = 1;};
void snake::right(){ if(direction==4 || direction==2) direction = 3;};
void snake::up(){ if(direction==1 || direction==3) direction = 2;};
void snake::down(){ if(direction==3 || direction==1) direction = 4;};

void snake::deletenode(SnakeNode* tmp){
    if(tmp){
        deletenode(tmp->prev);
        delete tmp;
    }
}

void snake::DeleteSnake(){
    tail = head;
    deletenode(head->prev);
    length = 0;
}