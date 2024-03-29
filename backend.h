#include <stdbool.h> //bool type (uchar) and true/false defines
#define BONUS_TIMEOUT 200
#define RESPLODE_TIMEOUT 18
#define ANACONDA_TIMEOUT 500

//enum to handle movement directions
enum Direction { UP, DOWN, LEFT, RIGHT, SPACE, ENTER };
enum Status { SUCCESS, FAILURE };
//Simply linked list of (x,y) points 
// x, y are text screen coordinates
// (0, 0) is top-left corner
// x = column
// y = row

//struct Coords{int x; int y; Coords* next;};

struct PointList {
  int x;
  int y;
  struct PointList* next;
  
  // Variables for snakes
  char id;
  int score;
  int justScored; // delay countdown for score changed graphic
  enum Direction dir; // current direction
  int hasFireblock; // snake has picked up a fireblock bonus
  int usingSpecial; // special key has been pressed
  struct PointList* fireBlocks; // fireblock within a snake
  int anacondaCountdown; // to timeout anaconda mode
  int fireworksBeingFired;
  int fireworksTimeout;
  int fireworksReplodeCountdown; // for replode
  struct PointList* fireworkA;
  struct PointList* fireworkB;
  struct PointList* fireworkC;
  struct PointList* fireworkD;
  struct PointList* fireworkE;
  struct PointList* fireworkF;
  struct PointList* fireworkG;
  struct PointList* fireworkH;
  struct PointList* fireworkI;

  // Variables for fireblocks
  int beingFired; // if fireblock has been fired
  int independentOfSnake; // if fireblock is now travelling outside the snake
  int extinguishGraphics; // if the fireblock extinguish graphics need to be shown
  
  // Variable for fireworks
  int fwBeingFired;
  
};

typedef struct PointList PointList;


//struct containing the full game board info
typedef struct {
  PointList* snake; //list of snake points
  PointList* snakeB; 
  PointList* foods; //list of food points
  PointList* pvpbonus; // pvp bonus
  PointList* fireBonuses; // fireblocks bonus
  PointList* anacondaBonus; // anaconda super bonus
  PointList* fireworkBonus; // firework bonus
  PointList* maze; // list of maze blocks
  int xmax; //screen cols
  int ymax; // screen rows
} Board;

//Checks if two point structures have the same coordinates
bool is_same_place(PointList* cell1, PointList* cell2);
// Changes the x and y values of the fireblock
void newFireBlockPosition(PointList* fireblock, Board* board);
// Move each fireblock
void move_fireblocks(Board* board, PointList* snakePtr);
// Move fireworks for the snake in question
void move_fireworks(Board* board, PointList* snakePtr);
//This function tries to move the full snake in the given direction
enum Status move_snake(Board* board, enum Direction dir, PointList* snake, int * pvp);
//shift snake does the actual tranferring of x,y coords each snake part in turn
void shift_snake(PointList* snakePtr, PointList* beginning, int addTail);
//Creates and returns a new snake point (the new snake's head position)
PointList* next_move(Board* board, enum Direction dir, PointList* snake);
//Creates a new point element dynamically allocating it
PointList* create_cell(int x, int y);
//creates a cell in a random location
PointList* create_random_cell(int xmax, int ymax);
//Generates the initial snake (length 2)
PointList* create_snake();
//fills in the Board structure from the given lists and dimensions
Board* create_board(PointList* snake, PointList* snakeB, PointList* foods, PointList* pvpbonus, PointList* fireBonuses, PointList* anacondaBonus, PointList* fireworkBonus, PointList* maze, int xmax, int ymax);
// Checks if a given point is contained in a list of points
bool list_contains(PointList* cell, PointList* list);
// Checks if a given point is within anaconda points of a particular snake
bool list_contains_anaconda(PointList* beginning, PointList* list, PointList* snakePtr);
// Checks if a given point is within fireblock points
bool list_contains_fireblock(PointList* beginning, PointList* list);
// Checks if a list's points are in a snake's points, if so then decrement snake score and break snake at the point of contact (not in use currently)
bool list_contains_snake_then_break(PointList* fires, PointList* snakePtr);
//remove element elt from list, update list pointer
bool remove_from_list(PointList* elt, PointList** list);
// change item x and y to different values
bool change_coords(PointList* item, Board* board);
//Initialises firework coordinates to be that of their snake head
void initialiseFireworkCoordsAndSetBeingFired(PointList* snakePtr, int x, int y);
// Updates the item in list's coordinates if in anaconda, e.g. to 'respawn' food that gets eaten
void update_item_coords_in_list_if_in_anaconda(PointList* beginning, PointList* list, Board* board);
// Updates the item in lists's coordinates if in fireblock, e.g. to 'respawn' food that gets eaten
void update_item_coords_in_list_if_in_fireblock(PointList* beginning, PointList* list, Board* board);
// Updates the item in list's coordinates if same as new point
void update_item_coords_in_list(PointList* beginning, PointList* list, Board* board);
//Adds a new food point to the food list in a random location
void add_new_food(Board* board);
// Adds a new pvp bonus to the board
void add_new_bonus(Board* board);
// Adds a new fireblock bonus to the board
void add_new_firebonus(Board* board);
// Adds a new firework bonus to the board
void add_new_fireworkbonus(Board* board);
// Adds a new anaconda bonus to the board
void add_new_anacondabonus(Board* board);
// Adds a snake to the board, specifically playerB snakes if it is NULL
void add_snake(Board* board);
// Allocates memory and sets variables of a snake's fireblock 
void add_fireblock_tosnake(PointList* snakePtr);

