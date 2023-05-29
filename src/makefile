boardsweep: main.o MoveGeneration.o TargetGeneration.o Board.o Move.o UsefulConstants.o Eval.o EvalConstants.o Search.o Tester.o BitUtil.o BoardVisualization.o GamePlay.o LookupLibrary.o
	g++ -g -lstdc++ -o boardsweep main.o MoveGeneration.o TargetGeneration.o Board.o Move.o UsefulConstants.o Eval.o EvalConstants.o Search.o Tester.o BitUtil.o BoardVisualization.o GamePlay.o LookupLibrary.o
main.o: main.cpp
	g++ -c main.cpp
MoveGeneration.o: MoveGeneration/MoveGeneration.cpp MoveGeneration/MoveGeneration.h
	g++ -c MoveGeneration/MoveGeneration.cpp
TargetGeneration.o: MoveGeneration/TargetGeneration.cpp MoveGeneration/TargetGeneration.h
	g++ -c MoveGeneration/TargetGeneration.cpp
Board.o: Representation/Board.cpp Representation/Board.h
	g++ -c Representation/Board.cpp
Move.o: Representation/Move.cpp Representation/Move.h
	g++ -c Representation/Move.cpp
UsefulConstants.o: Representation/UsefulConstants.cpp Representation/UsefulConstants.h
	g++ -c Representation/UsefulConstants.cpp
Eval.o: Search/Eval.cpp Search/Eval.h
	g++ -c Search/Eval.cpp
EvalConstants.o: Search/EvalConstants.cpp Search/EvalConstants.h
	g++ -c Search/EvalConstants.cpp
Search.o: Search/Search.cpp Search/Search.h
	g++ -c Search/Search.cpp
Tester.o: Testing/Tester.cpp Testing/Tester.h
	g++ -c Testing/Tester.cpp
BitUtil.o: BitUtil.cpp BitUtil.h
	g++ -c BitUtil.cpp
BoardVisualization.o: BoardVisualization.cpp BoardVisualization.h
	g++ -c BoardVisualization.cpp
GamePlay.o: GamePlay.cpp GamePlay.h
	g++ -c GamePlay.cpp
LookupLibrary.o: Representation/LookupLibrary.cpp Representation/LookupLibrary.h
	g++ -c Representation/LookupLibrary.cpp
clean:
	rm boardsweep; rm -f *.o; rm valgrind-out.txt
memory: boardsweep
	 valgrind --leak-check=full          --show-leak-kinds=all          --track-origins=yes          --verbose          --log-file=valgrind-out.txt  ./boardsweep
perf: boardsweep
	valgrind --tool=callgrind ./boardsweep


