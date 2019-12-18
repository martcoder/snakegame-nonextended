#include <ncurses.h>
#include "backend.h"
#include "frontend.h"
#include "splash.h"

//print character "symbol" in a list of points
void display_points(Board* board,PointList* snake, const chtype symbol, int colorPair) {
	
	if(snake){
	
          PointList* snakeLocal = snake;

          snakeLocal = snake; // reset pointer
          // If PointList is a snake and has anacondaCountdown > 0 so is in anaconda mode, then draw the anaconda points around the snake
          if(snake->anacondaCountdown > 0){
			  
		snakeLocal = snake; // reset pointer
			  
		while(snakeLocal) { //while list not NULL
		   //print symbol in point position
                   mvaddch(snakeLocal->y + 1, snakeLocal->x + 1, '@');
		   mvaddch(snakeLocal->y + 2, snakeLocal->x + 2, '@');
		   mvaddch(snakeLocal->y - 1, snakeLocal->x - 1, '@');
		   mvaddch(snakeLocal->y - 2, snakeLocal->x - 2, '@');
		   //point to next point in the list
	           snakeLocal = snakeLocal->next;
		}
		snake->anacondaCountdown = snake->anacondaCountdown - 1; // decrement anaconda countdown
	  }     
          // Set the colour pair that was passed into function as a parameter
          attron(COLOR_PAIR(colorPair));
          snakeLocal = snake; // reset pointer
          while(snakeLocal) { //while list not NULL
		//print symbol in point position
		mvaddch(snakeLocal->y, snakeLocal->x,	symbol);
		//point to next point in the list
		snakeLocal = snakeLocal->next;
          }

          // head of the snake has eaten a fireblock and not fired it yet, draw a different colour to show fireblock is inside snake head waiting to be fired
          if(snake->hasFireblock == 1){ 
		attron(COLOR_PAIR(20));
		mvaddch(snake->y, snake->x,symbol);
	  }

          // Back to default colour pair
          attron(COLOR_PAIR(colorPair));
		  
          if(snake->independentOfSnake == 1){ // is a fireblock which is travelling
                // Draw the points around the central fireblock point
	        mvaddch(snake->y + 1, snake->x + 1, 'o');
		mvaddch(snake->y, snake->x + 1, 'o');
		mvaddch(snake->y, snake->x - 1, 'o');
		mvaddch(snake->y + 1, snake->x, 'o');
		mvaddch(snake->y - 1, snake->x, 'o');
		mvaddch(snake->y -1, snake->x - 1, 'o');
          }
	}// end of if (snake)
}

//Handle keyboard commands in-game
void get_next_move(enum Direction previouspA,enum Direction previouspB ,enum Direction * pAdirptr,enum Direction * pBdirptr, PointList* pA, PointList* pB) {
  int ch = getch();
  switch (ch) {
    case KEY_LEFT:
      if (previouspA != RIGHT) *pAdirptr = LEFT;
      else *pAdirptr = previouspA; //Direction key is opposite to current direction, so ignore and keep the previous direction
break;
    case KEY_RIGHT:
      if (previouspA != LEFT) *pAdirptr = RIGHT;
      else *pAdirptr = previouspA; //Direction key is opposite to current direction, so ignore and keep the previous direction
break;
    case KEY_DOWN:
      if (previouspA != UP) *pAdirptr = DOWN;
      else *pAdirptr = previouspA; //Direction key is opposite to current direction, so ignore and keep the previous direction 
break;
    case KEY_UP:
      if (previouspA != DOWN) *pAdirptr = UP;
      else *pAdirptr = previouspA; //Direction key is opposite to current direction, so ignore and keep the previous direction
      break;
    case 10: // Enter key is playerA special key, so activate playerA usingSpecial variable
      pA->usingSpecial = 1; // activate it
      break;
    case 'a':
      if (previouspB != RIGHT) *pBdirptr = LEFT;
      else *pBdirptr = previouspB; //Direction key is opposite to current direction, so ignore and keep the previous direction
break;
    case 'd':
      if (previouspB != LEFT) *pBdirptr = RIGHT;
      else *pBdirptr = previouspB; //Direction key is opposite to current direction, so ignore and keep the previous direction
break;
    case 's':
      if (previouspB != UP) *pBdirptr = DOWN;
      else *pBdirptr = previouspB; //Direction key is opposite to current direction, so ignore and keep the previous direction
break;
    case 'w':
      if (previouspB != DOWN) *pBdirptr = UP;
break;
    case 32: // Spacebar is playerB special key, so activate playerB usingSpecial variable
      if(pB)
        pB->usingSpecial = 1; // activate it
      break;
    default: //nothing pressed or non-direction key pressed, so keep existing directions
      *pAdirptr = previouspA;
      *pBdirptr = previouspB;
  }
}

// Function to show snake score in top middle of screen
void showScores(PointList* thesnake, int xmax, int ymax){
	
	if(thesnake){
		if(thesnake->id == 'a'){ // if playerA use playerA colours
			init_pair(6, COLOR_RED, COLOR_BLACK);
			if(thesnake->justScored > 0){ // if flaring has not timed out, show a solid colour over the score
				init_pair(6, COLOR_RED, COLOR_RED);
				thesnake->justScored = thesnake->justScored - 1; // decrement each time so it times out
			}

			attron(COLOR_PAIR(6));
                        
			// for int to string https://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c
			char str[12];
			sprintf(str, "%d", thesnake->score); // Conververt integer score to string
                        // Print score as a string
			mvprintw(0, xmax/2 - 2, str); // from http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/printw.html (6.0 Output functions)
		}
		else if(thesnake->id == 'b'){ // if playerB use playerB colours
		  init_pair(7, COLOR_GREEN, COLOR_BLACK);
			
		  if(thesnake->justScored > 0){// if flaring has not timed out, show a solid colour over the score
		    init_pair(7, COLOR_GREEN, COLOR_GREEN);
			thesnake->justScored = thesnake->justScored - 1; // decrement each time so it times out
		  }
		 				
		  attron(COLOR_PAIR(7));
		  char strB[12];
		  sprintf(strB, "%d", thesnake->score); // Conververt integer score to string
		  mvprintw(0, xmax/2 + 4, strB); // Print score as a string
		}
	}
}

// Function loops over each maze item and draws each
void show_maze(PointList* maze){
  int c;
  while(maze){
    mvaddch(maze->y, maze->x, ACS_CKBOARD);
    maze = maze->next;
  }
}




