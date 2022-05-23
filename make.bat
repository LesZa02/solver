g++ -std=c++17 -O2 G2Solver\G2Solver.cpp -o G2Solver\G2Solver.exe

g++ -std=c++17 -O3 G1\G1.cpp -c -o G1.o
g++ -std=c++17 -O3 G2\G2.cpp -c -o G2.o
g++ -std=c++17 -O3 RArea\RArea.cpp -c -o RArea.o
g++ -std=c++17 -O3 Segment\Segment.cpp -c -o Segment.o
g++ -std=c++17 -O3 Form\Form.cpp -c -o Form.o
g++ -std=c++17 -O3 PathHelper\PathHelper.cpp -c -o PathHelper.o

g++ -std=c++17 -O3 pathfinder.cpp -o pathfinder.exe G1.o G2.o RArea.o PathHelper.o Segment.o Form.o