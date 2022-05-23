#include "G1.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
	G1 g1(argv[1]);

	auto mat = g1.GetAdjMatrix();
	auto coord = g1.GetVertexCoord();

	std::ofstream out("vertex_coord.txt", std::ios::out);
	for (int i = 0; i < coord.size(); ++i)
		out << coord[i].first << " " << coord[i].second << "\n";

	for (int i = 0; i < 40; ++i) {
		for (int j = 0; j < 40; ++j)
			std::cout << mat[i][j] << " ";
		std::cout << "\n";
	}

	std::cout << "\n";

	for (int i = 0; i < 10; ++i)
		std::cout << coord[i].first << " " << coord[i].second << "\n";

	std::cout << "\n";

	return 0;
}