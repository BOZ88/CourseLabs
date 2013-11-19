###Objective

The objective of this assignment together with Homework 5  is to make a playable version of the game of HEX.  Homework 4 is about drawing a HEX board and determining a legal position and a winning position. Homework 5 will be to add an AI that can play HEX well. In Homework 5 we will use ideas from the video lectures that implement a Monte Carlo determined best move that will let your final program possibly exceed human ability.

###The Game of Hex 
The game of Hex has been invented in 1942 by Piet Hein, reinvented in 1948 by John Nash, got its name in 1952 from a commercial distribution by Parker Brothers and has been popularized by Martin Gardner in 1957. It is similar to tic-tac-toe on a hexagonal 11x11 board. 


The blue player must make a connected set of blue hexagons from east to west. The red player must do the same from north to south. At each turn a player chooses an unoccupied hexagon and gives it their color.  Unlike tic-tac-toe the game cannot end in a draw. Indeed it can be proven that by best play the first player wins.(John Nash). However there is no known optimal strategy.

###Your Hex Program
```
	Your program should use a graph representation and treat the game as a path finding problem. Each internal node (hexagon) has six neighbors – so each would have 6 edges. The corners and the edges are special. A corner has either two or three neighbors and a non-corner edge has 4 neighbors.

   The player should be able to interact with the program and choose its “color” with blue going first. The program should have a convenient interface for entering a move, displaying the board, and then making its own move. The program should determine when the game is over and announce the winner.

 	A simple strategy could be to extend your current longest path or to block your opponent’s longest path. A very dumb strategy would be to play on an empty hexagon at random.

 	A simple board display would be to have an 11 x 11 printout with B, R, or a blank in each position. A simple way to input a move would be to have the player enter an (i,j) coordinate corresponding to a currently empty hexagon and have the program check that this is legal and if not ask for another choice.

 	HW 4 expectations:

 	Be able to draw the board using ASCII symbols and a given size, such as 7 by 7 or 11 by 11.
	Input a move and determine if a move is legal.
	Determine who won.
	Some suggestions:

 	The board can be drawn in ASCII as follows: (7 x 7 board)
	
 	X - . - . - . - . - . - . 
	 \ / \ / \ / \ / \ / \ / \ 
	  . - . - . - . - . - . - .
	   \ / \ / \ / \ / \ / \ / \ 
   	    . - . - . - . - . - . - .
         \ / \ / \ / \ / \ / \ / \ 
	      . - . - . - . - . - . - . 
	       \ / \ / \ / \ / \ / \ / \ 
	        . - . - . - . - . - . - . 
	         \ / \ / \ / \ / \ / \ / \ 
	          . - . - . - . - . - . - .
	           \ / \ / \ / \ / \ / \ / \ 
	            . - . - . - . - . - . - . 
	 
 	Here X indicates a move in the corner. The dots are empty cells. A  O indicates the second player. The community TA will provide further tips and advice on this issue. Remember that Homework 4 does not need to actually play the game ; this is reserved for Homework 5.
```