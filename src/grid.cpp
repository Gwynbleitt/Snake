#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "grid.h"

void Grid::draw_grid(Display* d, GC gc, Drawable drawable){

    short int grid_x = (WinW-(dimension*cell_dimension))/2;
    short int grid_y = (WinH-(dimension*cell_dimension))/2;

    if(dimension && cell_dimension && linewidth){ 

        XSegment lines[dimension*2+2]; 

        /*columns*/
        for(int i = 0; i<=dimension;i++){ 
            lines[i].x1 = cell_dimension*i + grid_x;
            lines[i].y1 = grid_y;
            lines[i].x2 = cell_dimension*i + grid_x;
            lines[i].y2 = dimension*cell_dimension + grid_y;
        }
        /*rows*/
        for(int i = dimension+1; i<=dimension*2+1;i++){
            lines[i].x1 = grid_x-(linewidth/2);
            lines[i].y1 = cell_dimension*(i-dimension-1) + grid_y;
            lines[i].x2 = (grid_x+dimension*cell_dimension)+(linewidth/2);
            lines[i].y2 = cell_dimension*(i-dimension-1) + grid_y;
        }

        XDrawSegments(d,drawable,gc,lines,(dimension*2)+2);
    }
}

void Grid::createmap(){
    short int grid_x = (WinW-(dimension*cell_dimension))/2;
    short int grid_y = (WinH-(dimension*cell_dimension))/2;
    cells = dimension*dimension;

    cell_position_map = new int[cells*2];
    int c = 0;
    int r = 0;
    for(int i = 0; i < cells*2-1; i+=2){
        cell_position_map[i] = grid_x  + (c*cell_dimension); //x
        cell_position_map[i+1] = grid_y  + (r*cell_dimension); //y
        c++;
        if(c%dimension == 0){
            c=0;
            r++;
        }
    }
}

void Grid::createfood(){
    
    int random = rand()%(cells*2);
    size = cell_dimension/2;

    if(random%2==0){
        foodx = cell_position_map[random]+(size/2);
        foody = cell_position_map[random+1]+(size/2);
    }else{
        foodx = cell_position_map[random+1]+(size/2);
        foody = cell_position_map[random]+(size/2);
    }
}

void Grid::printfood(Display* d, GC gc, Drawable drawable){
    XFillArc(d,drawable,gc,foodx,foody,size,size,0,29440);
}

void Grid::map_destructor(){
    delete[] cell_position_map;
}
