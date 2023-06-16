# BoardSweep C++ Chess Engine

Instructions:

First download and make the source with whatever method you wish to use.

**Set a position:**

position [fen string]. By default, it will use the starting position.
You can change this position whenever you want using the same command.

**To display a set position: display.**

**For testing:**

perft [depth]. This will run a perft test for the set postion and given depth, and it will tell you the number of nodes it counted and the time the test took.

**To exit the program: quit**

**Game vs the engine:**

first start the game and say who you want to move first(0 for engine, 1 for you) with the command "play [moveFirst]"
I use a shortened notation for chess moves. For anything but promotions, simply enter the from square and to square in string notation.
Ex: a move bishop move from b1 to a2 would be "b1a2".
If the move is a promotion, append the promoted piece char to the end.(Q,B,N,R for white and the lowercase versions for black).
Ex: a pawn to queen promotion from a7 to a8 would be a7a8Q. 



