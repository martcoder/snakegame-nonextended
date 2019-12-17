// Additional comments by Mario Mata, GCU, and Martin Kettle
#include "backend.h" //core functionalities and data structures
#include "frontend.h" //paint functions and key processing
#include "maze.h"
#include "splash.h"
#include <ncurses.h>

// initialise colour pairs, then called frontend.c functions to draw the blocks
void draw(Board* board){
    start_color();
    // from https://stackoverflow.com/questions/10487166/ncurses-multi-colors-on-screenv 
    init_pair(11, COLOR_RED, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(22, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(9, COLOR_WHITE, COLOR_BLUE);
    init_pair(8, COLOR_CYAN, COLOR_BLACK);
    init_pair(12, COLOR_CYAN, COLOR_GREEN);
    init_pair(14, COLOR_CYAN, COLOR_RED);
    init_pair(20, COLOR_CYAN, COLOR_YELLOW); 
    init_pair(34, COLOR_BLACK, COLOR_YELLOW);
    init_pair(38, COLOR_BLUE, COLOR_MAGENTA);
    init_pair(99, COLOR_YELLOW, COLOR_BLACK);
    init_pair(100, COLOR_YELLOW, COLOR_WHITE);
    init_pair(101, COLOR_MAGENTA, COLOR_BLACK);
    
    // draw the scores
    if(board->snake)
      showScores(board->snake,board->xmax,board->ymax);
    
    if(board->snakeB) // board->snakeB NULL to begin with, so need this check 
        showScores(board->snakeB,board->xmax,board->ymax); 

    // draw snake
    attron(COLOR_PAIR(11)); 
    display_points(board,board->snake, ACS_BLOCK,1); 

    // draw snakeB
    attron(COLOR_PAIR(22));
    if(board->snakeB){// board->snakeB NULL to begin with, so need this check 
      display_points(board,board->snakeB, ACS_BLOCK,2); 
    }
    
    // draw the maze blocks
    attron(COLOR_PAIR(34));
    show_maze(board->maze);
    
    // draw foods
    attron(COLOR_PAIR(3));
    display_points(board,board->foods, ACS_DIAMOND,3);
    
    // draw pvp bonus   
    attron(COLOR_PAIR(2));
    display_points(board,board->pvpbonus, 'S',2);
    
    // draw fireblock bonus
    attron(COLOR_PAIR(9));
    //display_points(board,board->fireBonuses, 230);
    display_points(board,board->fireBonuses, 'o',9);
    
    //draw firework bonus
    //attron(COLOR_PAIR(100));
    display_points(board,board->fireworkBonus, 'X',9);
    
    // draw anaconda bonus
    attron(COLOR_PAIR(38));
    //display_points(board,board->anacondaBonus, 198);
    display_points(board,board->anacondaBonus, ACS_PI,38);
    
    // draw fireblocks
    attron(COLOR_PAIR(8));
    if(board->snake->fireBlocks){
	  if(board->snake->fireBlocks->beingFired == 1)
            display_points(board,board->snake->fireBlocks, 'o',8);
            // display_points(board,board->snake->fireBlocks, 230); // for extended symbols 
    }
    if(board->snakeB){// board->snakeB NULL to begin with, so need this check 
      if(board->snakeB->fireBlocks){
          if(board->snakeB->fireBlocks->beingFired == 1)
            display_points(board,board->snakeB->fireBlocks, 'o',8);
            // display_points(board,board->snakeB->fireBlocks, 230); // for extended symbols 
      }
    }
    
    // draw fireworks
    attron(COLOR_PAIR(11));

	  if(board->snake->fireworksBeingFired == 1){
		    
            display_points(board,board->snake->fireworkA, '*',11);
            display_points(board,board->snake->fireworkB, '*',11);
            display_points(board,board->snake->fireworkC, '*',11);
            display_points(board,board->snake->fireworkD, '*',11);
            display_points(board,board->snake->fireworkE, '*',11);
            display_points(board,board->snake->fireworkF, '*',11);
            display_points(board,board->snake->fireworkG, '*',11);
            display_points(board,board->snake->fireworkH, '*',11);
            
            board->snake->fireworksTimeout = board->snake->fireworksTimeout - 1; // decrement fireworks timeout
            board->snake->fireworksReplodeCountdown = board->snake->fireworksReplodeCountdown - 1;
            
            //board->snake->fireworksBeingFired = 0;
            // display_points(board,board->snake->fireBlocks, 230); // for extended symbols 
    }
    if(board->snakeB){// board->snakeB NULL to begin with, so need this check 
      if(board->snakeB->fireworksBeingFired == 1){

            display_points(board,board->snakeB->fireworkA, '*',22);
            display_points(board,board->snakeB->fireworkB, '*',22);
            display_points(board,board->snakeB->fireworkC, '*',22);
            display_points(board,board->snakeB->fireworkD, '*',22);
            display_points(board,board->snakeB->fireworkE, '*',22);
            display_points(board,board->snakeB->fireworkF, '*',22);
            display_points(board,board->snakeB->fireworkG, '*',22);
            display_points(board,board->snakeB->fireworkH, '*',22);
            // display_points(board,board->snakeB->fireBlocks, 230); // for extended symbols 
            
            board->snakeB->fireworksTimeout = board->snakeB->fireworksTimeout - 1; // decrement fireworks timeout
            board->snakeB->fireworksReplodeCountdown = board->snakeB->fireworksReplodeCountdown - 1;
      }
    }
    
    refresh(); //paint screen
}
// function to call backend.c move_fireblocks which moves travelling fireblock to next position
void moveFireblocks(Board* board){

    if(board->snake->fireBlocks){
      if(board->snake->fireBlocks->beingFired == 1)
        move_fireblocks(board,board->snake);
    }
    
    if(board->snakeB){// board->snakeB NULL to begin with, so need this check 
      if(board->snakeB->fireBlocks){
        if(board->snakeB->fireBlocks->beingFired == 1)
          move_fireblocks(board,board->snakeB);
      }
    }
}

// function to call backend.c move_fireworks which moves fireworks to next position
void moveFireworks(Board* board){
   if(board->snake->fireworksBeingFired == 1)
	  move_fireworks(board, board->snake);
   if(board->snakeB){ // board->snakeB NULL to begin with, so need this check 
      if(board->snakeB->fireworksBeingFired == 1)
	    move_fireworks(board,board->snakeB);	
	}
}

int main() {
  //ncurses screen initialization
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE); // make ncurses handle key presses
  curs_set(0); // hide cursor
  
  int xmax;
  int ymax;
  //get current text screen size
  getmaxyx(stdscr, ymax, xmax); //it is a macro, no & needed in ymax, xmax

  // Start of game splash screen
  splashScreen(xmax,ymax);

  //start snake moving to the right
  enum Direction dir_playerA = RIGHT;
  enum Direction dir_playerB = DOWN;
  // create pointers to directions for player A and player B, to pass into get_next_move function
  enum Direction * dir_playerAptr = &dir_playerA; 
  enum Direction * dir_playerBptr = &dir_playerB; 

  //create snake list, and pass it to create_board.
  // snakeB doesnt exist yet, passing as NULL
  //Food list is not passed yet (NULL passed instead).
  // PvP Bonus list doesnt exist yet, passing as NULL.
  // Fireblock bonus list passing as NULL
  // Anaconda bonus passing as NULL
  // fireworks bonus passing as NULL
  // Creating maze and passing returned PointList
  // Passing xmax
  // Passing ymax 
  Board* board = create_board(create_snake(), NULL, NULL, NULL, NULL, NULL, NULL, create_maze(xmax,ymax), xmax, ymax);
  int i;
  if( (board->ymax ) < 10){ // if a small board
	  for (i = 0; i < 10; i++) { // create a minimum number of food items
        add_new_food(board); //add food pieces to the board
      }
  }
  else{ // if not a small board
	   for (i = 0; i < board->ymax ; i++) { // base the number of foods on the size of the board
		  add_new_food(board); //add food pieces to the board
		}
  }
  
  // Add pvp bonus to board
  add_new_bonus(board);
  // Add firesnake bonus to board
  add_new_firebonus(board);
  // Add firework bonus to board
  add_new_fireworkbonus(board);
  // Add anaconda super bonus to board
  add_new_anacondabonus(board);
  
  //MAIN LOOP
  while(true) {
    clear(); //clear screen
    //Draw snake and food lists
    draw(board);
    
    // first time - move fireblocks which are travelling independent of snake;
    moveFireblocks(board);
    
    // move fireworks blocks
    moveFireworks(board);

    // pvpactivator pointer gets passed to the move_snake function so it can be updated and checked later here in main to do pvp splash screen
    int pvpactivator = 1;
    int * pvp = &pvpactivator;
    
    // Read in the keyboard to see which direction is being pressed or if special key is pressed, reads for playerA and playerB for next move direction
    get_next_move(*dir_playerAptr, *dir_playerBptr, dir_playerAptr, dir_playerBptr,board->snake,board->snakeB ? board->snakeB : NULL); 

    //update snake direction
    enum Status status = move_snake(board, *dir_playerAptr,board->snake, pvp);
    
    if (status == FAILURE) break; //finish game if move not okay, e.g. snake ate itself

    if(board->snakeB){ // board->snakeB NULL to begin with, so need this check  
       status = move_snake(board, *dir_playerBptr,board->snakeB, pvp); // move player B snake
      if (status == FAILURE) break; //finish game if move not okay e.g. snake hit itself
    }
    
    //Move fireblocks for 2nd time to simulate double speed
    moveFireblocks(board);

    
    // Show 2 player splash screen if that bonus has been activated
    if(*pvp == 0){
	  	timeout(10000); //do not keep waiting for keypress longer than this
  
        //get current text screen size
        getmaxyx(stdscr, ymax, xmax); //it is a macro, no & needed in ymax, xmax

        // show pvp splash screen 
        splash2player(xmax,ymax);
        
        *pvp = 1; // deactivate again
	}
	
	// player B : If anaconda in play is about to expire, respawn anaconda bonus in centre of the maze
	if(board->snakeB){// board->snakeB NULL to begin with, so need this check 
	  if(board->snakeB->anacondaCountdown == ANACONDA_TIMEOUT)
	    splashAnaconda(board->xmax,board->ymax);
	  if(	board->snakeB->anacondaCountdown > 1 &&  board->snakeB->anacondaCountdown < 3 ){
	    add_new_anacondabonus(board);
	  }
	}
	// player A : If anaconda mode is active but about to end, respawn the Anaconda bonus in the centre of the maze
	if(board->snake->anacondaCountdown == ANACONDA_TIMEOUT)
	  splashAnaconda(board->xmax,board->ymax);
	if(board->snake->anacondaCountdown > 1 && board->snake->anacondaCountdown < 3 ){
	  	add_new_anacondabonus(board);
	}
  } // end MAIN LOOP
  //release ncurses resources
  endwin();

  return 0;
}
