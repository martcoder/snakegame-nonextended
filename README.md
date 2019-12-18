# snakegame non-extended

The code builds upon the code created by Julia Evans from this repository: https://github.com/jvns/snake 
I have used the same license as her, MIT license. Her code was also modified by Mario Mata (GCU) who 
added comments and adjusted existing code, after that the features listed below were created by myself, 
and I hope you will enjoy them.  

TO COMPILE:
 In linux run this:
 sh compile.sh

TO RUN:
  ./snakegame

TO PLAY:
PlayerA: To play you use the arrow keys for direction, and the Enter key for special. 
PlayerB: use WASD and the Spacebar for special, and ask playerA to eat the pvp bonus! 

FEATURES:
Features added by myself include:

- Showing the score at middle top of screen

- PVP : eat a pvp bonus and a 2nd snake appears which can be controlled with WASD 

- Fireblocks : eat a fireblock power up and you can shoot a fireblock which will consume food in its path
  ( Enter for playerA, Space for playerB )

- Fireworks : eat a fireworks power up and the firework explodes, and extinguishes at the edge 
  of the screen..... but the last remaining one will explode again, and so on. There is a TIMEOUT
  the stop it continuing forever. 
  
- Anaconda power up : transform into a monster snake which guzzles food easily

- Maze in the centre

- Splash screens for the start of the game, for when PvP bonus is eaten, for when Anaconda bonus is eaten

NOTES FOR DEVELOPERS:
- This is the non-extended version, which means that it doesn't used extended characterset for symbols or custom colours, which means some of the symbols used are different from the extended version, which is at https://github.com/martcoder/snakegame 

- The way I went about doing these features, apart from the coordinates method, was to add variables to the PointList struct, some for snakes, some for fireblocks, some for anaconda, the PointList struct is in backend.h, for example when a snake eats a fireblock the hasFireblock variable in the snake head PointList gets set to 1, when the user presses the special key the fireblock is fired from the snake so Fireblock variable independentOfSnake is set to 1, while the snake's hasFireblock is set back to 0. 

- I also changed the way that the snake is moved by dominoing the coords of each part to the next part of the snake, then updating the first part, the head, with the new coordinates of the next move point. 

- I changed the way food is eaten and a new one created, instead of freeing an item and creating a new one, I just changed the coordinates of the existing food item when it is eaten, simulating it being removed and a new one made. 

- For Anacondas I decided to keep the existing snakes points and just show additional blocks around it, this requires starting from the snake head block and then changing to the surrounding points to display blocks there too. This also meant that when eating food, I need to do extra checks for the snake head point and all surrounding points, which was a bit of a headache. It might have been better to make the anaconda actually have more blocks in the head rather than simulating it, but this is the way I did it. For example things like list_contains for anaconda needed a new function dedicated to anaconda checks. 

- For fireblocks I created new functions similar to the move_snake but for fireblocks. As they span a few points I did these a similar way to Anacondas which is to move the centre block and simulate the surrounding blocks. I also have these moving double speed, which is done by calling move on them twice for every one of move_snake. 

- For fireworks I did similarly to fireblocks but at normal speed, but also created 8 firework items in each snake, which are moved outward from the snake's head coordinates when a firework bonus is hit. The biggest difference is having them explode again, which requires code for checking how many are off the board, and if only 1 left, check it's not near the edge, if not then explode again by initialising firework coordinates to the a randomly selected one of the last remaining one's coords, and then moving out again from there.  
