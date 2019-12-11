#include <ncurses.h>
#include "backend.h"
#include "frontend.h"
#include "splash.h"

//print character "symbol" in a list of points
void display_points(Board* board,PointList* snake, const chtype symbol, int colorPair) {
	
	if(snake){
	
		  PointList* snakeLocal = snake;

          snakeLocal = snake; // reset pointer
		  if(snake->anacondaCountdown > 0){
			  
			  snakeLocal = snake; // reset pointer
			  
		      //attron(COLOR_PAIR(colorPair));
			  while(snakeLocal) { //while list not NULL
				//print symbol in point position
				/*mvaddch(snakeLocal->y + 1, snakeLocal->x + 1, 174);
				mvaddch(snakeLocal->y + 2, snakeLocal->x + 2, 174);
				mvaddch(snakeLocal->y - 1, snakeLocal->x - 1, 174);
				mvaddch(snakeLocal->y - 2, snakeLocal->x - 2, 174);*/
                                mvaddch(snakeLocal->y + 1, snakeLocal->x + 1, '@');
				mvaddch(snakeLocal->y + 2, snakeLocal->x + 2, '@');
				mvaddch(snakeLocal->y - 1, snakeLocal->x - 1, '@');
				mvaddch(snakeLocal->y - 2, snakeLocal->x - 2, '@');
				//point to next point in the list
				snakeLocal = snakeLocal->next;
			  }
			  snake->anacondaCountdown = snake->anacondaCountdown - 1;
		  }      
		  
          attron(COLOR_PAIR(colorPair));
		  snakeLocal = snake; // reset pointer
		  while(snakeLocal) { //while list not NULL
			//print symbol in point position
			mvaddch(snakeLocal->y, snakeLocal->x,	symbol);
			//point to next point in the list
			snakeLocal = snakeLocal->next;
		  }

		  if(snake->hasFireblock == 1){ // head of the snake which has eaten a fireblock and not fired it yet
                  short int color = 191;
		    //init_pair(20, COLOR_CYAN, color); // for if advanced colors are available
                    
		    attron(COLOR_PAIR(20));
		    mvaddch(snake->y, snake->x,	symbol);
		  }

          attron(COLOR_PAIR(colorPair));
		  
		  if(snake->independentOfSnake == 1){ // is a fireblock which is travelling
			  
			  mvaddch(snake->y + 1, snake->x + 1, 'o');
			  mvaddch(snake->y, snake->x + 1, 'o');
			  mvaddch(snake->y, snake->x - 1, 'o');
			  mvaddch(snake->y + 1, snake->x, 'o');
			  mvaddch(snake->y - 1, snake->x, 'o');
			  mvaddch(snake->y -1, snake->x - 1, 'o');
		  }
	}
}

//Handle keyboard commands in-game
void get_next_move(enum Direction previouspA,enum Direction previouspB ,enum Direction * pAdirptr,enum Direction * pBdirptr, PointList* pA, PointList* pB) {
  int ch = getch();
  switch (ch) {
    case KEY_LEFT:
      if (previouspA != RIGHT) *pAdirptr = LEFT;
	  else *pAdirptr = previouspA; //Avoid entering remaining cases
break;
    case KEY_RIGHT:
      if (previouspA != LEFT) *pAdirptr = RIGHT;
  	  else *pAdirptr = previouspA; //Avoid entering remaining cases
break;
    case KEY_DOWN:
      if (previouspA != UP) *pAdirptr = DOWN;
	  else *pAdirptr = previouspA; //Avoid entering remaining cases 
break;
    case KEY_UP:
      if (previouspA != DOWN) *pAdirptr = UP;
      else *pAdirptr = previouspA;
      break;
    case 10:
      pA->usingSpecial = 1; // activate it
      break;
    case 'a':
      if (previouspB != RIGHT) *pBdirptr = LEFT;
	  else *pBdirptr = previouspB; //Avoid entering remaining cases
break;
    case 'd':
      if (previouspB != LEFT) *pBdirptr = RIGHT;
  	  else *pBdirptr = previouspB; //Avoid entering remaining cases
break;
    case 's':
      if (previouspB != UP) *pBdirptr = DOWN;
          else *pBdirptr = previouspB;
break;
    case 'w':
      if (previouspB != DOWN) *pBdirptr = UP;
break;
    case 32:
      if(pB)
        pB->usingSpecial = 1; // activate it
      break;
    default: //nothing pressed or non-direction key pressed
      *pAdirptr = previouspA;
      *pBdirptr = previouspB;
  }
}

void showScores(PointList* thesnake, int xmax, int ymax){
	
	if(thesnake){
		if(thesnake->id == 'a'){
			init_pair(6, COLOR_RED, COLOR_BLACK);
			if(thesnake->justScored > 0){
				init_pair(6, COLOR_RED, COLOR_RED);
				thesnake->justScored = thesnake->justScored - 1; // decrement each time so it times out
			}

			attron(COLOR_PAIR(6));
			// for int to string https://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c
			char str[12];
			sprintf(str, "%d", thesnake->score);
			mvprintw(0, xmax/2 - 2, str); // from http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/printw.html (6.0 Output functions)
		}
		else if(thesnake->id == 'b'){
		  init_pair(7, COLOR_GREEN, COLOR_BLACK);
			
		  if(thesnake->justScored > 0){
		    init_pair(7, COLOR_GREEN, COLOR_GREEN);
			thesnake->justScored = thesnake->justScored - 1; // decrement each time so it times out
		  }
		 				
		  attron(COLOR_PAIR(7));
		  char strB[12];
		  sprintf(strB, "%d", thesnake->score);
		  mvprintw(0, xmax/2 + 4, strB);
		}
	}
}


void show_maze(PointList* maze){
  int c;
  while(maze){
    mvaddch(maze->y, maze->x, ACS_CKBOARD);
    maze = maze->next;
  }
}




