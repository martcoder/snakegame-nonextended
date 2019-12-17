#include <ncurses.h>
//Handle keyboard commands in-game
void get_next_move(enum Direction previousA,enum Direction previousB,enum Direction * pAdirptr,enum Direction * pBdirptr,PointList* pA, PointList* pB);
//print character symbol in a list of points
void display_points(Board* board,PointList* snake, const chtype symbol, int colorPair);
// draw snake score 
void showScores(PointList* snake, int xmax, int ymax);
// show starting splash screen
void splashScreen(int xmax, int ymax);
// show pvp splash screen
void splash2player(int xmax, int ymax);
// draw the maze
void show_maze(PointList* maze);
