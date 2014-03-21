##2048 Sovler

Feburary 2014

Author: Michael Denny


I like the game 2048: http://gabrielecirulli.github.io/2048/

But it seems like this game is really hard to solve. For example, if you have one of each type of tile between 2 and 1024, you have used up 10 of the 16 tiles. You run out of space pretty fast.

So I wanted to program my own 2048 game which will randomly select input until the game is lost or won.

I plan to run this in a batch scenario to get a handle on how easy/hard it is to beat 2048 (at least with random movements).

This is just for my own curiosity...

#Results (using random input):
* 6x6 grids are solved every time.
* 5x5 grids are solved about every 12th game. So 1/12 of the time.
* 4x4 grids (the actual game) are unsolvable with random input. I have ran over 15 million games with winning a single one. :(
