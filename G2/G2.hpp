#pragma once

#include "../G1/G1.hpp"
#include "../Form/Form.hpp"

#include "../PathHelper/PathHelper.hpp"

#include <map>
#include <utility>

typedef std::pair<int, int> G1Coord;
typedef std::vector<int> G2Vertex;

class G2 {
public:
	G2(const G1 *g1, int k);
	G2(const G1* g1, G2Form form);

	const std::vector<std::vector<bool>>& GetG1Field();
	PathHelper LinKernighan();
private:
	const G1 *g1_;
	G2Form form_;

	std::map<G1Coord, int> G1VertexNum_;

	std::vector<std::vector<bool>> G1Field_;

	std::vector<std::vector<int>> Vertex_;
	std::vector<std::vector<int>> AdjMatrix_;

	std::vector<RArea> VertexCoat_;
};