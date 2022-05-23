#pragma once

#include "../G1/G1.hpp"

#include <string>
#include <fstream>
#include <vector>
#include <tuple>

class PathHelper {
public:
	PathHelper(const std::vector<std::vector<int>>& vertex, const G1* g1,
			   std::ifstream& in);

	void WritePath(std::ofstream& out);
	int GetPathWeight();
private:
	const std::vector<std::vector<int>>& vertex_;
	const G1* g1_;

	std::vector<std::tuple<int, int, int>> edges_;
	std::vector<int> path_;

	int path_weight_;
};