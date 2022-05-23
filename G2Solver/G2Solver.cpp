#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <string>

int main(int argc, char** argv) {

	if (argc < 3)
		throw std::invalid_argument("arc < 3");

	const int inf = 1e9;

	std::ifstream in(argv[1], std::ios::in);
	if (!in.is_open())
		throw std::runtime_error(std::string(argv[1]) + " can not open file");

	std::vector<std::vector<int>> matrix;

	int n;
	in >> n;

	matrix.resize(n);

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			int x;
			in >> x;

			matrix[i].push_back(x);
		}	
	in.close();

	while (n < 9) {
		for (int i = 0; i < n-1; ++i)
			matrix[i].push_back(inf);
		matrix[n-1].push_back(0);
		matrix.push_back({});
		for (int i = 0; i < n+1; ++i)
			matrix[n].push_back(inf);
		matrix[n][n-1] = 0;
		n++;

		if (n == 9) {
			matrix[0][n-1] = 0;
			matrix[n-1][0] = 0;
		}
	}

	for (int i = 0; i < n; i++)
		matrix[i][i] = inf;

	std::ofstream out("tsp_file.tsp", std::ios::out);
	if (!out.is_open())
		throw std::runtime_error("tsp_file.tsp can not open file");

	out << "DIMENSION : " << n << "\n";
	out << "EDGE_WEIGHT_TYPE : EXPLICIT\n";
	out << "EDGE_WEIGHT_FORMAT : FULL_MATRIX\n";
	out << "EDGE_WEIGHT_SECTION\n";
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			out << matrix[i][j] << " ";

		out << "\n";
	}
	out << "EOF\n";
	out.close();

	system((std::string() + "VoyagerSolver\\LinKernighan.exe -o " + std::string(argv[2]) + " tsp_file.tsp").c_str());
	system("del tsp_file.tsp");

	in.open(argv[2], std::ios::in);
	if (!in.is_open())
		throw std::runtime_error(std::string(argv[2]) + " can not open file");

	int t;
	in >> t >> t;

	std::vector<std::vector<int>> edges;

	for (int i = 0; i < n; ++i) {
		int v, to, weight;
		in >> v >> to >> weight;

		if (weight != 0) {
			edges.push_back({v, to, weight});
		}
	}
	in.close();

	out.open(argv[2], std::ios::out | std::ios::trunc);
	if (!out.is_open())
		throw std::runtime_error(std::string(argv[2]) + " can not open file ");

	out << edges.size() << "\n";
	for (auto it : edges) {
		out << it[0] << " " << it[1] << " " << it[2] << "\n";
	}
	out.close();

	return 0;
}