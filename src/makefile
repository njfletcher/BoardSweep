boardsweep.exe: main.o MoveGeneration.o TargetGeneration.o Board.o Move.o UsefulConstants.o Eval.o EvalConstants.o Search.o Tester.o BitUtil.o BoardVisualization.o GamePlay.o LookupLibrary.o
	g++ -O3 -lstdc++ -o boardsweep.exe main.o MoveGeneration.o TargetGeneration.o Board.o Move.o UsefulConstants.o Eval.o EvalConstants.o Search.o Tester.o BitUtil.o BoardVisualization.o GamePlay.o LookupLibrary.o
main.o: main.cpp
	g++ -O3 -c main.cpp
MoveGeneration.o: MoveGeneration/MoveGeneration.cpp MoveGeneration/MoveGeneration.h
	g++ -O3 -c MoveGeneration/MoveGeneration.cpp
TargetGeneration.o: MoveGeneration/TargetGeneration.cpp MoveGeneration/TargetGeneration.h
	g++ -O3 -c MoveGeneration/TargetGeneration.cpp
Board.o: Representation/Board.cpp Representation/Board.h
	g++ -O3 -c Representation/Board.cpp
Move.o: Representation/Move.cpp Representation/Move.h
	g++ -O3 -c Representation/Move.cpp
UsefulConstants.o: Representation/UsefulConstants.cpp Representation/UsefulConstants.h
	g++ -O3 -c Representation/UsefulConstants.cpp
Eval.o: Search/Eval.cpp Search/Eval.h
	g++ -O3 -c Search/Eval.cpp
EvalConstants.o: Search/EvalConstants.cpp Search/EvalConstants.h
	g++ -O3 -c Search/EvalConstants.cpp
Search.o: Search/Search.cpp Search/Search.h
	g++ -O3 -c Search/Search.cpp
Tester.o: Testing/Tester.cpp Testing/Tester.h
	g++ -O3 -c Testing/Tester.cpp
BitUtil.o: BitUtil.cpp BitUtil.h
	g++ -O3 -c BitUtil.cpp
BoardVisualization.o: BoardVisualization.cpp BoardVisualization.h
	g++ -O3 -c BoardVisualization.cpp
GamePlay.o: GamePlay.cpp GamePlay.h
	g++ -O3 -c GamePlay.cpp
LookupLibrary.o: Representation/LookupLibrary.cpp Representation/LookupLibrary.h
	g++ -O3 -c Representation/LookupLibrary.cpp
clean:
	rm boardsweep.exe; rm -f *.o; rm valgrind-out.txt
memory: boardsweep
	 valgrind --leak-check=full          --show-leak-kinds=all          --track-origins=yes          --verbose          --log-file=valgrind-out.txt  ./boardsweep
perf: boardsweep
	valgrind --tool=callgrind ./boardsweep


