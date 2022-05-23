#include "PathHelper.hpp"

#include <stdexcept>
#include <algorithm>

int& v(std::tuple<int, int, int>& edge) {
	return std::get<0>(edge);
}

int& to(std::tuple<int, int, int>& edge) {
	return std::get<1>(edge);
}

int& weight(std::tuple<int, int, int>& edge) {
	return std::get<2>(edge);
}

std::ifstream& operator >> (std::ifstream& in, std::tuple<int, int, int>& edge) {
	in >> v(edge) >> to(edge) >> weight(edge);

	return in;
}

PathHelper::PathHelper(const std::vector<std::vector<int>>& vertex, const G1* g1,
					   std::ifstream& in) :
	vertex_(vertex),
	g1_(g1),

	edges_(),
	path_(),

	path_weight_(0)
{
	if (g1 == nullptr)
		throw std::invalid_argument("\n"
									"Class PathHelper\n"
									"Constructor\n"
									"G1 pointer is nullptr\n"
								   );

	if (not in.is_open())
		throw std::invalid_argument("\n"
									"Class PathHelper\n"
									"Constructor\n"
									"Input file at input stream is not open\n"
								   );

	int edges_count = 0;
	in >> edges_count;

	if (edges_count != (int)vertex.size())
		throw std::invalid_argument("\n"
									"Class PathHelper\n"
									"Constructor\n"
									"Wrong G2Solver output or G1 vertex vector\n"
									"edges_count != vertex size\n"
								   );

	edges_.resize(edges_count);
	for (auto& edge : edges_)
		if (not (in >> edge))
			throw std::invalid_argument("\n"
										"Class PathHelper\n"
										"Constructor\n"
										"Wrong G2Solver output\n"
										"Can not read all edges\n"
									   );

	int bigger_edge_num = 0,
		bigger_edge_weight = 0;
	for (int i = 0; i < edges_.size(); ++i) {
		if (weight(edges_[i]) > bigger_edge_weight) {
			bigger_edge_weight = weight(edges_[i]);
			bigger_edge_num = i; 
		}
	}
	
	for (int i = bigger_edge_num - 1; i >= 0; --i) {
		if (path_.empty())
			path_.push_back(to(edges_[i]));
		path_.push_back(v(edges_[i]));
	}

	for (int i = edges_.size() - 1; i > bigger_edge_num; --i) {
		if (path_.empty())
			path_.push_back(to(edges_[i]));
		path_.push_back(v(edges_[i]));
	}

	for (auto& edge : edges_)
		path_weight_ += weight(edge);
}

void PathHelper::WritePath(std::ofstream& out) {
	if (not out.is_open())
		throw std::invalid_argument("\n"
									"Class PathHelper\n"
									"Method WritePath\n"
									"Output file at output stream is not open\n"
								   );

	out << path_.size() << "\n\n";

	for (const auto& vertexG2 : path_) {
		out << vertex_[vertexG2].size() << "\n";
		for (const auto& vertexG1 : vertex_[vertexG2])
			out << g1_->GetVertexCoord()[vertexG1].first << " " 
				<< g1_->GetVertexCoord()[vertexG1].second << "\n";

		out << "\n";
	}
}

int PathHelper::GetPathWeight() {
	return path_weight_;
}