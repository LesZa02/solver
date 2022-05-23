g++ -O3 G1.cpp -c -o G1.o
g++ -O3 ../RArea/RArea.cpp -c -o RArea.o
g++ -O3 test.cpp -o test.exe G1.o RArea.o