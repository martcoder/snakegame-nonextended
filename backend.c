#include "backend.h"
#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <ncurses.h>

/*
 * Removes from the list or returns false
 */
//Searches for element elt in the list 
//and removes it (updating the list pointers)
//As the original list is modified, the function accesses it
//via pointer (thus the **list)
bool remove_from_list(PointList* elt, PointList** list) {
  PointList *currP, *prevP;
  prevP = NULL;

  for (currP = *list;
      currP != NULL;
      prevP = currP, currP = currP->next) {
    if (is_same_place(currP, elt)) {
      if (prevP == NULL) {
        *list = currP->next;
      } else {
        prevP->next = currP->next;
      }
      free(currP);
      return true;
    }
  }
  return false;
}

// change existing item's coordinates to different values, e.g. to simulate an item being eaten and respawning somewhere else
bool change_coords(PointList* item, Board* board) {
	PointList* newcell;
	int foundNewLocation = 0;
  if(item){

    while(foundNewLocation == 0){
      newcell = create_cell(rand() % board->xmax, rand() % board->ymax);
      if( list_contains(newcell, board->maze) 
      || list_contains(newcell, board->pvpbonus) 
      || list_contains(newcell, board->foods ) 
      || list_contains(newcell, board->snake ) 
      || list_contains(newcell, board->snakeB) 
      || list_contains_fireblock(board->snake->fireBlocks,newcell)
      || (board->snakeB) == NULL ? 0 : list_contains_fireblock(board->snakeB->fireBlocks,newcell)
      || list_contains_anaconda( board->snake, newcell, board->snake) 
      || (board->snakeB) == NULL ? 0 : list_contains_anaconda(board->snakeB,newcell, board->snakeB) )
        foundNewLocation = 0;
      else
        foundNewLocation = 1;
    } // end of while
    item->x = newcell->x;
    item->y = newcell->y;
    return true;
  }
  return false;
}

// based on a new location, it goes through each anaconda head point and for each point loops through list to see if list point is in anaconda head, if so it updates list point coordinates 
void update_item_coords_in_list_if_in_anaconda(PointList* beginning, PointList* list, Board* board){
	

	PointList* listLocal = list; // e.g. food list, firebonus list, etc
	PointList* anacondaPoint = create_cell(beginning->x,beginning->y);
	
	// go through list and see if item in list matches an anaconda location
	while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(anacondaPoint->x == listLocal->x && anacondaPoint->y == listLocal->y) // location match
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
	}
	
	listLocal = list; // reset pointer
	anacondaPoint->x = anacondaPoint->x + 1;
	anacondaPoint->y = anacondaPoint->y + 1;

	// go through list and see if item in list matches an anaconda location
	while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(anacondaPoint->x == listLocal->x && anacondaPoint->y == listLocal->y)
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
	}
		
	listLocal = list; // reset pointer
	anacondaPoint->x = anacondaPoint->x + 1;
        anacondaPoint->y = anacondaPoint->y + 1;

	while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(anacondaPoint->x == listLocal->x && anacondaPoint->y == listLocal->y)
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
        }
	  	
        listLocal = list; // reset pointer
        anacondaPoint->x = anacondaPoint->x + -3;
        anacondaPoint->y = anacondaPoint->y + -3;

        while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(anacondaPoint->x == listLocal->x && anacondaPoint->y == listLocal->y)
	    change_coords(listLocal,board); 

	  listLocal = listLocal->next; // move to next point in list
        }
	  	
        listLocal = list; // reset pointer
        anacondaPoint->x = anacondaPoint->x + -1;
        anacondaPoint->y = anacondaPoint->y + -1;

        while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(anacondaPoint->x == listLocal->x && anacondaPoint->y == listLocal->y)
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
        }
}
// check all items in the list to see if they match beginning, if so change the coords of that item, e.g. that item will be eaten
void update_item_coords_in_list(PointList* beginning, PointList* list, Board* board){
	
	PointList* listLocal = list; // e.g. food list, firebonus list, etc
	PointList* point = create_cell(beginning->x,beginning->y);
	
	// go through list and see if item in list matches the beginning location
        while(listLocal){
	  // if this element in the list equals the point, change coords of element
	  if(point->x == listLocal->x && point->y == listLocal->y) // location match
		change_coords(listLocal,board); 
	  listLocal = listLocal->next;
	}
}

// based on a new location, it goes through each fireblock point plus aura shifts and for each point loops through list to see if list point is in fireblock, if so it updates list point coordinates 
void update_item_coords_in_list_if_in_fireblock(PointList* beginning, PointList* list, Board* board){
	
	PointList* listLocal = list; // e.g. food list, firebonus list, etc
	PointList* fireblockPoint = create_cell(beginning->x,beginning->y);
	
	// go through list and see if item in list matches an anaconda location
	while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(fireblockPoint->x == listLocal->x && fireblockPoint->y == listLocal->y) // location match
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
	}
	
	listLocal = list; // reset pointer
	fireblockPoint->x = beginning->x + 1;
	fireblockPoint->y = beginning->y + 1;
	// go through list and see if item in list matches an anaconda location
	while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(fireblockPoint->x == listLocal->x && fireblockPoint->y == listLocal->y)
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
	}

	listLocal = list; // reset pointer
	fireblockPoint->x = beginning->x + 1;
        fireblockPoint->y = beginning->y;
	while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(fireblockPoint->x == listLocal->x && fireblockPoint->y == listLocal->y)
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
        }
	  	

        listLocal = list; // reset pointer
        fireblockPoint->x = beginning->x - 1;
        fireblockPoint->y = beginning->y;
        while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(fireblockPoint->x == listLocal->x && fireblockPoint->y == listLocal->y)
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
        }
	  	

        listLocal = list; // reset pointer
        fireblockPoint->x = beginning->x ;
        fireblockPoint->y = beginning->y + 1;
        while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(fireblockPoint->x == listLocal->x && fireblockPoint->y == listLocal->y)
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
        }
  
  
        listLocal = list; // reset pointer
        fireblockPoint->x = beginning->x ;
        fireblockPoint->y = beginning->y - 1;
        while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(fireblockPoint->x == listLocal->x && fireblockPoint->y == listLocal->y)
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
        }
  

        listLocal = list; // reset pointer
        fireblockPoint->x = beginning->x - 1;
        fireblockPoint->y = beginning->y - 1;
        while(listLocal){
	  // if this element in the list equals an anaconda point, change coords of element
	  if(fireblockPoint->x == listLocal->x && fireblockPoint->y == listLocal->y)
	    change_coords(listLocal,board); 
	  listLocal = listLocal->next;
        }
  
}

// Update fireblock position based on direction of fireblock
void newFireBlockPosition(PointList* fireblock, Board* board){
	
    if(fireblock->dir == UP)
	  fireblock->y = fireblock->y - 1;
	if(fireblock->dir == DOWN)
	  fireblock->y = fireblock->y + 1;
	if(fireblock->dir == LEFT)
	  fireblock->x = fireblock->x - 1;
	if(fireblock->dir == RIGHT)
	  fireblock->x = fireblock->x + 1;
	  
	// Allow to wrap through walls
	/*if(fireblock->x > board->xmax)
	  fireblock->x = 1;
	if(fireblock->x < 0)
	  fireblock->x = board->xmax;
	if(fireblock->y > board->ymax)
	  fireblock->y = 1;
	if(fireblock->y < 0)
	  fireblock->y = board->ymax;
	*/
}

// This function moves each fireblock in its starting direction, with checks for if being fired, if eats a food, if reaches edge of board
void move_fireblocks(Board* board, PointList* snakePtr){
	
	if(snakePtr->fireBlocks->beingFired == 1){ // if fireblock being fired, go ahead and move it
		
		PointList* fires = snakePtr->fireBlocks;
		
		if(fires->independentOfSnake == 0){ // if not independent of snake yet, take x y and dir from snake
		  fires->x = snakePtr->x;
		  fires->y = snakePtr->y;
		  fires->dir = snakePtr->dir;
		}
		
		fires->independentOfSnake = 1; // activate
			
		// Update location based on direction of fireblock
		newFireBlockPosition(fires, board);
		
		// if fireblock will hit a food, increment snake score!!! remove that food, and add another new food
		if (list_contains_fireblock(fires,board->foods)) {
			snakePtr->score = snakePtr->score + 1; 
			snakePtr->justScored = 4;
			//remove_from_list(fires, &(board->foods));
			update_item_coords_in_list_if_in_fireblock(fires,board->foods,board);
		}

                // if fireblock hits a snake, cut the snake at that point and halve the snake's score
		/*list_contains_snake_then_break(fires, board->snake);
		if(board->snakeB)
     		  list_contains_snake_then_break(fires, board->snakeB);
                */

		// Extinguish fireblock if edge of terminal is reached
		if( (fires->x > board->xmax) || (fires->x < 0) || (fires->y > board->ymax) || (fires->y < 0) ){
		    fires->beingFired = 0; //deactivate
            fires->independentOfSnake = 0; //deactivate
            //fires->extinguishGraphics = 1;
		}
	}	
}

// Set all of a snake's fireworks coordinates to the x and y coordinate passed in as parameters, and activate firework->beingFired
void initialiseFireworkCoordsAndSetBeingFired(PointList* snakePtr, int x, int y){
	    snakePtr->fireworkA->x = x;
	    snakePtr->fireworkA->y = y;	
	    snakePtr->fireworkA->beingFired = 1;
	    snakePtr->fireworkB->x = x;
	    snakePtr->fireworkB->y = y;
	    snakePtr->fireworkB->beingFired = 1;
	    snakePtr->fireworkC->x = x;
	    snakePtr->fireworkC->y = y;
	    snakePtr->fireworkC->beingFired = 1;
	    snakePtr->fireworkD->x = x;
	    snakePtr->fireworkD->y = y;
	    snakePtr->fireworkD->beingFired = 1;
	    snakePtr->fireworkE->x = x;
	    snakePtr->fireworkE->y = y;
	    snakePtr->fireworkE->beingFired = 1;
	    snakePtr->fireworkF->x = x;
	    snakePtr->fireworkF->y = y;
	    snakePtr->fireworkF->beingFired = 1;
	    snakePtr->fireworkG->x = x;
	    snakePtr->fireworkG->y = y;
	    snakePtr->fireworkG->beingFired = 1;
	    snakePtr->fireworkH->x = x;
	    snakePtr->fireworkH->y = y;
	    snakePtr->fireworkH->beingFired = 1;
	    snakePtr->fireworkI->x = x;
	    snakePtr->fireworkI->y = y;
	    snakePtr->fireworkI->beingFired = 1;
}

// Move each of a snake's fireworks to their next position, 
// deactivate firework if off the board
// also check if only 1 remaining or if resplode countdown timed out, then choose a random one of the remaining and re-explode fireworks from its location 
void move_fireworks(Board* board, PointList* snakePtr){
	
	int allFinished = 1;
	int countFinished = 0;

	if(snakePtr->fireworkA->beingFired == 1){ // Left travelling firework is activated
		
	  //Left
	  snakePtr->fireworkA->x = snakePtr->fireworkA->x - 1; // move to next position
	  // Firework consumes food it runs into
	  if(list_contains(snakePtr->fireworkA,board->foods)){
		  snakePtr->score = snakePtr->score + 1; 
		  snakePtr->justScored = 4;
		  // update coords of foods which got consumed
		  update_item_coords_in_list_if_in_fireblock(snakePtr->fireworkA,board->foods,board);
	  }
	  // If firework is off the board, deactivate
	  if(snakePtr->fireworkA->x < -1 || snakePtr->fireworkA->x > board->xmax|| snakePtr->fireworkA->y < -1 || snakePtr->fireworkA->y > board->ymax)
	    snakePtr->fireworkA->beingFired = 0; 

	  else // else firework still on the board so not all finished display it yet
	    allFinished = 0;
	}
	else
	  countFinished++; // Keep a count of firework which has been deactivated, e.g. is off the board
	  
	if(snakePtr->fireworkB->beingFired == 1){ // Right travelling firework is activated
		
	  //Right
	  snakePtr->fireworkB->x = snakePtr->fireworkB->x + 1;
// Firework consumes food it runs into
	  if(list_contains(snakePtr->fireworkB,board->foods)){
		  snakePtr->score = snakePtr->score + 1; 
		  snakePtr->justScored = 4;
		  // update coords of foods which got consumed
		  update_item_coords_in_list_if_in_fireblock(snakePtr->fireworkB,board->foods,board);
	  }
	  
	  // If firework is off the board, deactivate
	  if(snakePtr->fireworkB->x < -1 || snakePtr->fireworkB->x > board->xmax|| snakePtr->fireworkB->y < -1 || snakePtr->fireworkB->y > board->ymax)
	    snakePtr->fireworkB->beingFired = 0; 
	    
	  
	  else // else firework still on the board so not all finished display it yet
	    allFinished = 0;
	}
	else
	  countFinished++;
	  
	  
	if(snakePtr->fireworkC->beingFired == 1){ // Up travelling firework is activated
		
	  // Up
	  snakePtr->fireworkC->y = snakePtr->fireworkC->y + 1;
	  if(list_contains(snakePtr->fireworkC,board->foods)){
		  snakePtr->score = snakePtr->score + 1; 
		  snakePtr->justScored = 4;
		  // update coords of foods which got consumed
		  update_item_coords_in_list_if_in_fireblock(snakePtr->fireworkC,board->foods,board);
	  }
	  
	  // If firework is off the board, deactivate
	  if(snakePtr->fireworkC->x < -1 || snakePtr->fireworkC->x > board->xmax|| snakePtr->fireworkC->y < -1 || snakePtr->fireworkC->y > board->ymax)
	    snakePtr->fireworkC->beingFired = 0; 

	  
	  else // else firework still on the board so not all finished display it yet
	    allFinished = 0;
	}
	else
	  countFinished++;
	  
	  
	if(snakePtr->fireworkD->beingFired == 1){// Down travelling firework is activated
		
		//Down
	  snakePtr->fireworkD->y = snakePtr->fireworkD->y - 1;
	  if(list_contains(snakePtr->fireworkD,board->foods)){
		  snakePtr->score = snakePtr->score + 1; 
		  snakePtr->justScored = 4;
		  // update coords of foods which got consumed
		  update_item_coords_in_list_if_in_fireblock(snakePtr->fireworkD,board->foods,board);
	  }
	  
	  // If firework is off the board, deactivate
	  if(snakePtr->fireworkD->x < -1 || snakePtr->fireworkD->x > board->xmax|| snakePtr->fireworkD->y < -1 || snakePtr->fireworkD->y > board->ymax)
	    snakePtr->fireworkD->beingFired = 0; 
	  
	  else // else firework still on the board so not all finished display it yet
	    allFinished = 0;
	}
	else
	  countFinished++;
	  
	  
	if(snakePtr->fireworkE->beingFired == 1){// Up and Left travelling firework is activated
		// Up and Left
	  snakePtr->fireworkE->x = snakePtr->fireworkE->x - 1;
	  snakePtr->fireworkE->y = snakePtr->fireworkE->y - 1;
	  if(list_contains(snakePtr->fireworkE,board->foods)){
		  snakePtr->score = snakePtr->score + 1; 
		  snakePtr->justScored = 4;
		  // update coords of foods which got consumed
		  update_item_coords_in_list_if_in_fireblock(snakePtr->fireworkE,board->foods,board);
	  }
	  
	  
	  // If firework is off the board, deactivate
	  if(snakePtr->fireworkE->x < -1 || snakePtr->fireworkE->x > board->xmax|| snakePtr->fireworkE->y < -1 || snakePtr->fireworkE->y > board->ymax)
	    snakePtr->fireworkE->beingFired = 0; 

	  else // else firework still on the board so not all finished display it yet
	    allFinished = 0;
    }
    else
      countFinished++;
	  
	  
	if(snakePtr->fireworkF->beingFired == 1){// Up and Right travelling firework is activated
		
	  // UP and Right
	  snakePtr->fireworkF->x = snakePtr->fireworkF->x + 1;
	  snakePtr->fireworkF->y = snakePtr->fireworkF->y - 1;
	  if(list_contains(snakePtr->fireworkF,board->foods)){
		  snakePtr->score = snakePtr->score + 1; 
		  snakePtr->justScored = 4;
		  // update coords of foods which got consumed
		  update_item_coords_in_list_if_in_fireblock(snakePtr->fireworkF,board->foods,board);
	  }
	  
	  
		// If firework is off the board, deactivate
	  if(snakePtr->fireworkF->x < -1 || snakePtr->fireworkF->x > board->xmax|| snakePtr->fireworkF->y < -1 || snakePtr->fireworkF->y > board->ymax)
	    snakePtr->fireworkF->beingFired = 0; 
	  
	  else // else firework still on the board so not all finished display it yet
	    allFinished = 0;
	}
	else
	  countFinished++;
	  
	if(snakePtr->fireworkG->beingFired == 1){// Down and Left travelling firework is activated
		
		// Down and Left
	  snakePtr->fireworkG->x = snakePtr->fireworkG->x - 1;
	  snakePtr->fireworkG->y = snakePtr->fireworkG->y + 1;
	  if(list_contains(snakePtr->fireworkG,board->foods)){
		  snakePtr->score = snakePtr->score + 1; 
		  snakePtr->justScored = 4;
		  // update coords of foods which got consumed
		  update_item_coords_in_list_if_in_fireblock(snakePtr->fireworkG,board->foods,board);
	  }
	  
	  // If firework is off the board, deactivate
	  if(snakePtr->fireworkG->x < -1 || snakePtr->fireworkG->x > board->xmax|| snakePtr->fireworkG->y < -1 || snakePtr->fireworkG->y > board->ymax)
	    snakePtr->fireworkG->beingFired = 0; 
	  
	  else // else firework still on the board so not all finished display it yet
	    allFinished = 0;
	}
	
	else
	  countFinished++;
	  
	if(snakePtr->fireworkG->beingFired == 1){// Down and Right travelling firework is activated
		
		// Down and Right
	  snakePtr->fireworkH->x = snakePtr->fireworkH->x + 1;
	  snakePtr->fireworkH->y = snakePtr->fireworkH->y + 1;
	  if(list_contains(snakePtr->fireworkH,board->foods)){
		  snakePtr->score = snakePtr->score + 1; 
		  snakePtr->justScored = 4;
		  // update coords of foods which got consumed
		  update_item_coords_in_list_if_in_fireblock(snakePtr->fireworkH,board->foods,board);
	  }
	  
	  // If firework is off the board, deactivate
	  if(snakePtr->fireworkH->x < -1 || snakePtr->fireworkH->x > board->xmax|| snakePtr->fireworkH->y < -1 || snakePtr->fireworkH->y > board->ymax)
	    snakePtr->fireworkH->beingFired = 0; 
	  
	  else // else firework still on the board so not all finished display it yet
	    allFinished = 0;
	}
	else
	  countFinished++;
	  
	
	// If only 1 remaining or replode countdown almost timed out, choose at random a remaing firework and  do the firework thing of exploding again...
	if(countFinished == 7 || snakePtr->fireworksReplodeCountdown == 1){
		int which = rand() % 8;
		if(which == 0)
		  which = 1;
		// The if statements should find a remaining firework, and also check that it's not really close to the board edge
		if(which == 1 && snakePtr->fireworkA->beingFired == 1 && ( snakePtr->fireworkA->x > 2 && snakePtr->fireworkA->x < board->xmax - 2 && snakePtr->fireworkA->y > 2 && snakePtr->fireworkA->y < board->ymax - 2 ) )
		  initialiseFireworkCoordsAndSetBeingFired(snakePtr, snakePtr->fireworkA->x, snakePtr->fireworkA->y); // Start from the current coordinates of the last firework
		if(which == 2 && snakePtr->fireworkB->beingFired == 1 && ( snakePtr->fireworkB->x > 2 && snakePtr->fireworkB->x < board->xmax - 2 && snakePtr->fireworkB->y > 2 && snakePtr->fireworkB->y < board->ymax - 2 ))
		  initialiseFireworkCoordsAndSetBeingFired(snakePtr, snakePtr->fireworkB->x, snakePtr->fireworkB->y); // Start from the current coordinates of the last firework
		if(which == 3 && snakePtr->fireworkC->beingFired == 1 && ( snakePtr->fireworkC->x > 2 && snakePtr->fireworkC->x < board->xmax - 2 && snakePtr->fireworkC->y > 2 && snakePtr->fireworkC->y < board->ymax - 2 ))
		  initialiseFireworkCoordsAndSetBeingFired(snakePtr, snakePtr->fireworkC->x, snakePtr->fireworkC->y); // Start from the current coordinates of the last firework
		if(which == 4 && snakePtr->fireworkD->beingFired == 1 && ( snakePtr->fireworkD->x > 2 && snakePtr->fireworkD->x < board->xmax - 2 && snakePtr->fireworkD->y > 2 && snakePtr->fireworkD->y < board->ymax - 2 ))
		  initialiseFireworkCoordsAndSetBeingFired(snakePtr, snakePtr->fireworkD->x, snakePtr->fireworkD->y); // Start from the current coordinates of the last firework
		if(which == 5 && snakePtr->fireworkE->beingFired == 1 && ( snakePtr->fireworkE->x > 2 && snakePtr->fireworkE->x < board->xmax - 2 && snakePtr->fireworkE->y > 2 && snakePtr->fireworkE->y < board->ymax - 2 ))
		  initialiseFireworkCoordsAndSetBeingFired(snakePtr, snakePtr->fireworkE->x, snakePtr->fireworkE->y); // Start from the current coordinates of the last firework
		if(which == 6 && snakePtr->fireworkF->beingFired == 1 && ( snakePtr->fireworkF->x > 2 && snakePtr->fireworkF->x < board->xmax - 2 && snakePtr->fireworkF->y > 2 && snakePtr->fireworkF->y < board->ymax - 2 ))
		  initialiseFireworkCoordsAndSetBeingFired(snakePtr, snakePtr->fireworkF->x, snakePtr->fireworkF->y); // Start from the current coordinates of the last firework
		if(which == 7 && snakePtr->fireworkG->beingFired == 1 && ( snakePtr->fireworkG->x > 2 && snakePtr->fireworkG->x < board->xmax - 2 && snakePtr->fireworkG->y > 2 && snakePtr->fireworkG->y < board->ymax - 2 ))
		  initialiseFireworkCoordsAndSetBeingFired(snakePtr, snakePtr->fireworkG->x, snakePtr->fireworkG->y); // Start from the current coordinates of the last firework
		if(which == 8 && snakePtr->fireworkH->beingFired == 1 && ( snakePtr->fireworkH->x > 2 && snakePtr->fireworkH->x < board->xmax - 2 && snakePtr->fireworkH->y > 2 && snakePtr->fireworkH->y < board->ymax - 2 ))
		  initialiseFireworkCoordsAndSetBeingFired(snakePtr, snakePtr->fireworkH->x, snakePtr->fireworkH->y); // Start from the current coordinates of the last firework
		
		allFinished = 0;
		snakePtr->fireworksReplodeCountdown = RESPLODE_TIMEOUT / 3;
    }

    if(allFinished == 1 || snakePtr->fireworksTimeout == 0){ // if all fireworks have reached edge of board or overall fireworks timeout reached
	  snakePtr->fireworksBeingFired = 0; // deactive snake fireworks
	  change_coords(board->fireworkBonus,board);  // respawn fireworks bonus in a new random location 
    }
}

//This function tries to move the full snake in the given direction
//returns FAILURE if a game termination condition arises 
enum Status move_snake(Board* board, enum Direction dir, PointList* snake, int * pvp) {
  
  PointList* snakePtr = snake; 
  snakePtr->dir = dir;

  // Create a new beginning for the snake (head) with next_move
  PointList* beginning = NULL;
  beginning = next_move(board, dir, snakePtr);
  if (beginning == NULL) { //wall hit, next_move returned NULL
    return FAILURE;
  }

  // If we've gone backwards, don't do anything
  if (snakePtr->next && is_same_place(beginning, snakePtr->next)) {
    beginning->next = NULL;
    free(beginning);
    return SUCCESS;
  }

  // Check for collisions of snake head with other snake points
  if (list_contains(beginning, snakePtr)) {
    return FAILURE; //snake collided with itself!
  }
  
  // Check for collision with maze walls
  if (list_contains(beginning, board->maze)){ // if snake has hit the maze
    // decrement score
    if(snakePtr->score > -9 )
      snakePtr->score = snakePtr->score - 1;
    snakePtr->justScored = 5; // set delay timer
    // dont do anything more 
    return SUCCESS;
  }
  
  // if snake consumes anaconda super bonus
  if (list_contains(beginning, board->anacondaBonus)){ // if snake has hit a superbonus
    remove_from_list( beginning, &(board->anacondaBonus) ); // remove bonus from board
    snakePtr->anacondaCountdown = ANACONDA_TIMEOUT; // activate Anaconda Mode by setting countdown value, counts down in frontend.c, gets checked in main.c
  }

  if (list_contains(beginning, board->pvpbonus) || list_contains_anaconda(beginning,board->pvpbonus,snakePtr)){ // if snake has hit a bonus
    //take the pvpbonus off the board
    board->pvpbonus->x = -10;
    board->pvpbonus->y = -10;
    //add_new_bonus(board); // add a new bonus randomly on the board
    add_snake(board); // add the bonus snake to the board!!!
    *pvp = 0; // set pvp to active
  }
  
  //If snake is about to eat a firework bonus
  if(list_contains(beginning, board->fireworkBonus) || list_contains_anaconda(beginning,board->fireworkBonus,snakePtr) ){
	  initialiseFireworkCoordsAndSetBeingFired(snakePtr, snakePtr->x, snakePtr->y);
	  snakePtr->fireworksBeingFired = 1;
	  snakePtr->fireworksTimeout = BONUS_TIMEOUT / 2;
	  snakePtr->fireworksReplodeCountdown = RESPLODE_TIMEOUT;
	  board->fireworkBonus->x = -5; board->fireworkBonus->y = -5; // move off the board

  }
  
  // If snake is about to eat a firebonus
  if (list_contains(beginning, board->fireBonuses) || list_contains_anaconda(beginning,board->fireBonuses,snakePtr)){ // if snake has hit a firebonus
    //remove_from_list( beginning, &(board->fireBonuses) ); // remove bonus from board
    
    // remove anaconda eaten firebonus
    if(snakePtr->anacondaCountdown > 0){
		 update_item_coords_in_list_if_in_anaconda(beginning,board->fireBonuses,board);
		//update_item_coords_if_in_anaconda(beginning,board->fireBonuses,board);
	}
	else{
		add_new_firebonus(board);
	}
    add_fireblock_tosnake(snakePtr); // add the fireblock to the snake!!!
    snakePtr->hasFireblock = 1; // activate
    snakePtr->beingFired = 0; // set to inactive to start with
  }
  
  //Also do fireblock move 
  
  if(snakePtr->usingSpecial == 1 && snakePtr->hasFireblock == 1){ // using special activated, start 'firing' fireblock
    
	snakePtr->fireBlocks->beingFired = 1; // activate
	snakePtr->hasFireblock = 0; // snake no longer has fireblock
        move_fireblocks(board,snakePtr);
        snakePtr->usingSpecial = 0; // set back to inactive until next time special key pressed
  }
  
  snakePtr->usingSpecial = 0; // reset it wether or not the snake hasFireblock, to avoid 'queing up' a shot

  
  // Check for collision with food (snake eats it!)
  if (list_contains(beginning, board->foods) || list_contains_anaconda(beginning,board->foods,snakePtr) ) {
	//the snake length increments by 1 thanks to food consumed
	
    // remove anaconda eaten food
    if(snakePtr->anacondaCountdown > 0){
		update_item_coords_in_list_if_in_anaconda(beginning,board->foods,board); // changes coords of food item, simulating it being eaten and a new one added somewhere else
	}
	else{
	  //change coords of food item, simulating it being eaten and then adding a new one
      update_item_coords_in_list(beginning, board->foods, board);
	}

    // increment snake score for eating food
    snakePtr->score = snakePtr->score + 1;
    snakePtr->justScored = 2; // activate justScored variable of snake, used to flare the score
    
    shift_snake(snakePtr,beginning,1); // domino x,y coords through the entire snake and add a new tail

    return SUCCESS;
  }
  
  
  // No food was eaten, snake keeps its length
  // just need to shift all coords along the snake
  shift_snake(snakePtr,beginning,0); // dont need to add a new tail

  return SUCCESS; //all done for this movement
}

// check if this new location plus anaconda shifts are in the list
bool list_contains_anaconda(PointList* beginning, PointList* list, PointList* snakePtr){
	
	if(snakePtr){
		if(snakePtr->anacondaCountdown == 0)
		  return false;
	
		PointList* listLocal = list;

		while(listLocal){ // loop through the list
			
			  
			  if(beginning->x  == listLocal->x && beginning->y  == listLocal->y)
				return true;
			  if(beginning->x + 1 == listLocal->x && beginning->y + 1 == listLocal->y)
				return true;
			  if(beginning->x + 2 == listLocal->x && beginning->y + 2 == listLocal->y)
				return true;
			  if(beginning->x - 1 == listLocal->x && beginning->y - 1 == listLocal->y)
				return true;
			  if(beginning->x - 2 == listLocal->x && beginning->y - 2 == listLocal->y)
				return true;
				
			  listLocal = listLocal->next;
		}
	}
	return false;
}

// This function is not being used yet, but is meant to be for if a snake gets hit by a fireblock, to reduce its score and remove some parts of it
bool list_contains_snake_then_break(PointList* fires, PointList* snakePtr){
	

  PointList* snakeLocal = snakePtr;
  snakeLocal = snakeLocal->next; // move past head
  int hit = 0;
  while(snakeLocal){
	if(snakeLocal->x == fires->x && snakeLocal->y == fires->y){
	  snakePtr->score = snakePtr->score / 2; // Halve the snake's score	
	  hit = 1;
	}
	snakeLocal = snakeLocal->next; 
  }
  
  // reset pointer
  snakeLocal = snakePtr;
  
  // now past the second segment
  snakeLocal = snakeLocal->next;
  snakeLocal = snakeLocal->next;
  
  while(snakeLocal){
		  
		  PointList* next = snakeLocal->next;
		  free(snakeLocal);
		  snakeLocal = next;
		  
  }
	
}


// check if beginning plus extra fireblock shifts against each item in the list
// it is designed so that if a newfireblock position has been created, e.g. beginning, this can be checked against say a list of foods to see if beginning is the same as any food items
// that way it means that the fireblock has effectively consumed those food items

// when being called from the create_new_foood and similar functions, it is called like (board->snake->fireblock, newcell) 
// because this will check the fireblock plus aura shifts against the list which in this case is jsut a single new item location
bool list_contains_fireblock(PointList* beginning, PointList* list){
	
		PointList* listLocal = list;
		
		if(list){
			if(beginning){
				
				while(listLocal){ // loop through the list
				
				  if(beginning->x + 1 == listLocal->x && beginning->y + 1 == listLocal->y)
					return true;
				  if(beginning->x + 1 == listLocal->x && beginning->y  == listLocal->y)
					return true;
				  if(beginning->x - 1 == listLocal->x && beginning->y  == listLocal->y)
					return true;
				  if(beginning->x == listLocal->x && beginning->y + 1 == listLocal->y)
					return true;
				  if(beginning->x == listLocal->x && beginning->y - 1 == listLocal->y)
					return true;
				  if(beginning->x - 1 == listLocal->x && beginning->y - 1 == listLocal->y)
					return true;
					
				  listLocal = listLocal->next;
			   }
			}
			
			
		}

		
	return false;
}

// Domino each snake PointList coordinate to the next point down the length of the entire snake, and put next move coordinates into snake head
// It is to move each point of the snake along, to make movement of the snake happen
// parameter addTail can be used to also signal that a new tail should be added, e.g. if a food has been eaten
void shift_snake(PointList* snakePtr, PointList* beginning, int addTail){
  	struct PointList* position = (struct PointList*) calloc(1,sizeof(struct PointList));
  struct PointList* storerPrev = (struct PointList *) calloc(1,sizeof(struct PointList)); 
  struct PointList* storerCurr = (struct PointList *) calloc(1,sizeof(struct PointList)); 

  // Set position to the head of snake
  position = snakePtr;
  position->next = snakePtr->next;
  position->x = snakePtr->x;
  position->y = snakePtr->y;
  
  // Put head values into storerPrev
  storerPrev->x = snakePtr->x; 
  storerPrev->y = snakePtr->y;
  
  // Put new beginning values in head
  snakePtr->x = beginning->x;
  snakePtr->y = beginning->y;
  
  // move position to the next part of snake
  //position = position->next;
  
  while(position->next){
    position = position->next;
    storerCurr->x = storerCurr->x ;
    position->x = position->x;
    
    storerCurr->x = position->x; // Store current position values
    storerCurr->y = position->y;
    position->x = storerPrev->x; // Write current position values with previous values
    position->y = storerPrev->y;
    storerPrev->x = storerCurr->x; // save stored current as previous
    storerPrev->y = storerCurr->y;
    //position = position->next; // move to next position in the list
  }
  
  if(addTail){
	  
	  // Construct new tail
	  struct PointList* newTail = (struct PointList*) calloc(1,sizeof(struct PointList));
	  newTail->x = storerPrev->x;
	  newTail->y = storerPrev->y;
	  newTail->next = NULL;

	  // Here adding the new tail
	  position->next = newTail; 
  }
}

//Checks if two point structures have the same coordinates
bool is_same_place(PointList* cell1, PointList* cell2) {
  return ((cell1->x == cell2->x) && (cell1->y == cell2->y));
}

//Creates and returns a new snake point (the new snake's head position)
//in the corresponding move direction
//If the snake hits the screen limits it wraps around to opposite side
PointList* next_move(Board* board, enum Direction dir, PointList* snake) {
  
  int new_x = snake->x;
  int new_y = snake->y;
  switch(dir) {
    case UP:
      new_y = new_y - 1;
      break;
    case DOWN:
      new_y = new_y + 1;
      break;
    case LEFT:
      new_x = new_x - 1;
      break;
    case RIGHT:
      new_x = new_x + 1;
      break;
  }

  // Allow snake to wrap around through walls
  if(new_x < 0 ){ new_x = board->xmax-1; }
	if(new_x == board->xmax){ new_x = 0;  }
	if(new_y < 0){ new_y = board->ymax-1;  }
	if(new_y == board->ymax){ new_y = 0;  }

  return create_cell(new_x, new_y);

}

//creates a cell in a random location
PointList* create_random_cell(int xmax, int ymax) {
  return create_cell(rand() % xmax, rand() % ymax);
}

//Adds a new food point to the food list in a random location
void add_new_food(Board* board) {
  PointList* new_food;
  //UPDATED by Mario: original code (now commented) was not releasing memory
  // for cells allocated in non-valid positions, they were just ignored (mem leak!)
   bool ok = false;
   while(!ok){
	new_food = create_random_cell(board->xmax, board->ymax);
	//calls to list_contains_fireblock have parameters in reverse compared to when calling for a newly created fireblock to see if fireblock should consume
	  // similar for list_contains_anaconda
	if(list_contains(new_food, board->foods) 
	|| list_contains(new_food, board->maze)
	|| list_contains(new_food, board->pvpbonus) 
	|| list_contains(new_food, board->fireworkBonus)
	|| list_contains(new_food, board->snake ) 
	|| list_contains(new_food, board->snakeB) 
	|| list_contains_anaconda(board->snake, new_food, board->snake) 
	|| list_contains_anaconda(board->snakeB, new_food, board->snakeB) 
	|| list_contains_fireblock(board->snake->fireBlocks,new_food) 
	|| ( (board->snakeB) == NULL ? 0 : list_contains_fireblock(board->snakeB->fireBlocks,new_food)) )
	  free(new_food); //not a valid location!
    else //valid location for new food
	  ok = true;
   }

  //update food list with the newly created food cell
  new_food->next = board->foods;
  board->foods = new_food;
}

// for PvP bonus
void add_new_bonus(Board* board){
  PointList* new_pvpbonus;
  
	  //UPDATED by Mario: original code (now commented) was not releasing memory
	  // for cells allocated in non-valid positions, they were just ignored (mem leak!)
	   bool ok = false;
	   while(!ok){
	  new_pvpbonus = create_random_cell(board->xmax, board->ymax);
	  //calls to list_contains_fireblock have parameters in reverse compared to when calling for a newly created fireblock to see if fireblock should consume
	  // similar for list_contains_anaconda
	  if( list_contains(new_pvpbonus, board->pvpbonus) 
	  || list_contains(new_pvpbonus, board->fireworkBonus)
	  || list_contains(new_pvpbonus, board->maze)
	  || list_contains(new_pvpbonus, board->foods ) 
	  ||list_contains(new_pvpbonus, board->snake ) 
	  || list_contains(new_pvpbonus, board->snakeB) 
	  || list_contains_anaconda( board->snake,new_pvpbonus, board->snake) 
	  || list_contains_anaconda(board->snakeB,new_pvpbonus, board->snakeB) 
	  || list_contains_fireblock(board->snake->fireBlocks,new_pvpbonus) 
	  || ((board->snakeB) == NULL ? 0 : list_contains_fireblock(board->snakeB->fireBlocks,new_pvpbonus) ) )
		free(new_pvpbonus); //not a valid location!
		else //valid location for new food
		ok = true;
	   }

	  //update pvpbonus list with the newly created cell
	  board->pvpbonus = new_pvpbonus;
      board->pvpbonus->next = NULL;
}


//if firebonus exists then update coords of existing firebonus, otherwise add a new one to the board
void add_new_firebonus(Board* board){
  PointList* new_firebonus;
  if(board->fireBonuses){
	  
	  bool ok = false;
	  PointList* newcell = create_random_cell(board->xmax,board->ymax);
	   while(!ok){
		  newcell->x = rand() % board->xmax;
		  newcell->y = rand() % board->ymax;
		  //calls to list_contains_fireblock have parameters in reverse compared to when calling for a newly created fireblock to see if fireblock should consume
	  // similar for list_contains_anaconda
		  // thorough checking of it the new location is within any of these game items, if so try a different location
		  if( list_contains(newcell, board->pvpbonus) 
		  || list_contains(newcell, board->fireworkBonus)
		  || list_contains(newcell, board->maze)
		  || list_contains(newcell, board->foods ) 
		  || list_contains(newcell, board->snake ) 
		  || list_contains(newcell, board->snakeB) 
		  || list_contains_anaconda(board->snake,newcell, board->snake) 
		  || list_contains_anaconda(board->snakeB,newcell, board->snakeB) 
		  || list_contains_fireblock(board->snake->fireBlocks,newcell) 
	      || ((board->snakeB) == NULL ? 0 : list_contains_fireblock(board->snakeB->fireBlocks,newcell) ))
			ok = false;
		   
		   else{
			 ok = true;
			 board->fireBonuses->x = newcell->x;
			 board->fireBonuses->y = newcell->y;
			 free(newcell);
		   }
	  }
  }
  else{
	  //UPDATED by Mario: original code (now commented) was not releasing memory
	  // for cells allocated in non-valid positions, they were just ignored (mem leak!)
	  
	  //calls to list_contains_fireblock have parameters in reverse compared to when calling for a newly created fireblock to see if fireblock should consume
	  // similar for list_contains_anaconda
	   bool ok = false;
	   while(!ok){
	  new_firebonus = create_random_cell(board->xmax, board->ymax);
          // thorough checking of if the new location is within any of these game items, if so try a different location
	  if( list_contains(new_firebonus, board->pvpbonus) 
	  || list_contains(new_firebonus, board->fireworkBonus)
	  || list_contains(new_firebonus, board->maze)
	  || list_contains(new_firebonus, board->foods ) 
	  || list_contains(new_firebonus, board->snake ) 
	  || list_contains(new_firebonus, board->snakeB) 
	  || list_contains_anaconda(board->snake,new_firebonus, board->snake) 
	  || list_contains_anaconda(board->snakeB,new_firebonus, board->snakeB) 
	  || list_contains_fireblock(board->snake->fireBlocks,new_firebonus) 
	  || ((board->snakeB) == NULL ? 0 : list_contains_fireblock(board->snakeB->fireBlocks,new_firebonus) ))
		free(new_firebonus); //not a valid location!
		else //valid location for new food
		ok = true;
	   }

	  //update food list with the newly created food cell
	  board->fireBonuses = new_firebonus;
  }
}

// Checks if a given point is contained in a list of points
// Checks if a given point is contained in a list of points
bool list_contains(PointList* cell, PointList* list) {
  PointList* s = list;
  while (s) { //while not NULL
    if (is_same_place(s, cell)) {
      return true;
    }
    s = s->next; //select next element in the list
  }
  return false;
}

//Creates a new point element dynamically allocating it
//Initializes x and y elements to the ones given
//pointer to next element is set to NULL so far
PointList* create_cell(int x, int y) {
  PointList* cell = malloc(sizeof(*cell));
  cell->x = x;
  cell->y = y;
  cell->next = NULL;
  return cell;
}

//fills in the Board structure from the given lists and dimensions
Board* create_board(PointList* snake, PointList* snakeB, PointList* foods, PointList* pvpbonus, PointList* fireBonuses, PointList* anacondaBonus, PointList* fireworkBonus, PointList* maze, int xmax, int ymax) {
  Board* board = malloc(sizeof(*board));
  board->foods = foods;
  board->snake = snake;
  board->snakeB = snakeB;
  board->pvpbonus = pvpbonus;
  board->fireworkBonus = fireworkBonus;
  board->anacondaBonus = anacondaBonus;
  board->maze = maze;
  board->xmax = xmax;
  board->ymax = ymax;
  return board;
}

//Generates the initial snake (length 2)
PointList* create_snake() {
  //create the initial 2 point elements of the snake
  PointList* a = create_cell(2,3); //head
  PointList* b = create_cell(2,2); //tail
  a->id = 'a';
  a->next = b; //point head element to tail element
  a->hasFireblock = 0; // set to false initially 
  a->usingSpecial = 0; // set to false initially
  a->score = 0;
  a->justScored = 0;
  a->fireBlocks = NULL;
  a->fireworksBeingFired = 0; // set to false initially
  a->fireworksTimeout = 0; // inactive initially 
  a->beingFired = 0; // set to false initially
  a->anacondaCountdown = 0; // set to 0 initially
  a->fireworksReplodeCountdown = 0; // set to 0 initially
  a->fireworkA = (PointList*) malloc(sizeof(PointList)); // Allocate the firework memory
  a->fireworkA->fwBeingFired = 0; // initialise to inactive
  a->fireworkB = (PointList*) malloc(sizeof(PointList));
  a->fireworkB->fwBeingFired = 0;
  a->fireworkC = (PointList*) malloc(sizeof(PointList));
  a->fireworkC->fwBeingFired = 0;
  a->fireworkD = (PointList*) malloc(sizeof(PointList));
  a->fireworkD->fwBeingFired = 0;
  a->fireworkE = (PointList*) malloc(sizeof(PointList));
  a->fireworkE->fwBeingFired = 0;
  a->fireworkF = (PointList*) malloc(sizeof(PointList));
  a->fireworkF->fwBeingFired = 0;
  a->fireworkG = (PointList*) malloc(sizeof(PointList));
  a->fireworkG->fwBeingFired = 0;
  a->fireworkH = (PointList*) malloc(sizeof(PointList));
  a->fireworkH->fwBeingFired = 0;
  a->fireworkI = (PointList*) malloc(sizeof(PointList));
  a->fireworkI->fwBeingFired = 0;
  
  add_fireblock_tosnake(a); // add fireblock to the snake!!!
  return a; //return head element address (start of list)
}

// Adds a snake to the board consisting of two PointList items
void add_snake(Board* board){
  if(board->snakeB == NULL){ // if playerB snake is NULL, do that one
    board->snakeB = create_snake();
    board->snakeB->id = 'b';
    board->snakeB->justScored = 0;
  }
}

// Initialises a snake's fireblock with memory, and sets parameters
void add_fireblock_tosnake(PointList* snakePtr){
	struct PointList * new_fireblock;
	if(snakePtr->fireBlocks == NULL) // if snake fireblock not initislised yet, allocate memory
    	new_fireblock = (PointList* ) calloc( 1,sizeof(PointList) );
    else
        new_fireblock = snakePtr->fireBlocks; // if snake fireblock initialised, use that
    // set fireblock coordinates
	new_fireblock->x = 0;
	new_fireblock->y = 0;
	new_fireblock->beingFired = 0; // inactive initially 
	new_fireblock->independentOfSnake = 0; // inactive initially
	new_fireblock->extinguishGraphics = 0; // inactive initially 
	new_fireblock->next = NULL;
	snakePtr->fireBlocks = new_fireblock; // assign fireblock to the snake in question
}

// Add anaconda bonus to board
void add_new_anacondabonus(Board* board){
  PointList* new_bonus;
  bool ok = false;
  while(!ok){
  new_bonus = create_cell(board->xmax /2, board->ymax / 2 + 3);
  //calls to list_contains_fireblock have parameters in reverse compared to when calling for a newly created fireblock to see if fireblock should consume
  // similar for list_contains_anaconda
  // thorough checking of if the new location is within any of these game items, if so try a different location
  if(list_contains(new_bonus, board->pvpbonus) 
  || list_contains(new_bonus, board->fireworkBonus)
  || list_contains(new_bonus, board->maze)
  || list_contains(new_bonus, board->foods ) 
  || list_contains(new_bonus, board->snake ) 
  || list_contains(new_bonus, board->snakeB) 
  || list_contains_anaconda(board->snake,new_bonus, board->snake) 
  || list_contains_anaconda(board->snakeB,new_bonus, board->snakeB)  )
    free(new_bonus); //not a valid location!
    else //valid location for new food
    ok = true;
   }

  //update board anacondabonus as the newly created cell
  new_bonus->next = board->anacondaBonus;
  board->anacondaBonus = new_bonus;
}

// Add the fireworks bonus to the board
void add_new_fireworkbonus(Board* board){
	PointList* new_bonus;
   bool ok = false;
   while(!ok){
  new_bonus = create_random_cell(board->xmax, board->ymax);
  // thorough checking of if the new location is within any of these game items, if so try a different location
  //calls to list_contains_fireblock have parameters in reverse compared to when calling for a newly created fireblock to see if fireblock should consume
  // similar for list_contains_anaconda
  if(list_contains(new_bonus, board->pvpbonus) 
  || list_contains(new_bonus, board->maze)
  || list_contains(new_bonus, board->foods ) 
  || list_contains(new_bonus, board->snake ) 
  || list_contains(new_bonus, board->snakeB) 
  || list_contains_anaconda(board->snake,new_bonus, board->snake) 
  || list_contains_anaconda(board->snakeB,new_bonus, board->snakeB)  )
    free(new_bonus); //not a valid location!
    else //valid location for new food
    ok = true;
   }

  //update board fireworkBonus as the newly created cell
  new_bonus->next = board->fireworkBonus;
  board->fireworkBonus = new_bonus;
}


