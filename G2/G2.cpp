#include "G2.hpp"

#include "../PathHelper/PathHelper.hpp"
#include "../Form/Form.hpp"

#include <iostream>
#include <set>
#include <climits>
#include <cassert>
#include <fstream>

G2::G2(const G1* g1, G2Form form) : 
	g1_(g1), 
	form_(form),

	G1VertexNum_(),

	G1Field_(),
	Vertex_(),
	AdjMatrix_(),

	VertexCoat_()
{

	std::cout << "Start Constructor G2" << std::endl;
	std::cout << "G2 Form: " << "("<<form_.GetSizes().first<<", "<<form_.GetSizes().second << ")\n";
	std::cout << "G2 Start Place: " << "(" << form_.GetCoords().first << ", " << form_.GetCoords().second << ")\n";

	auto G1Coord = g1_->GetVertexCoord();
	for (int i = 0; i < G1Coord.size(); ++i)
		G1VertexNum_[G1Coord[i]] = i;

	std::cout << "Vertex Num Map OK" << std::endl;

	int maxX = 0, maxY = 0;
	for (const auto& coord : G1Coord) {
		maxX = std::max(maxX, coord.first);
		maxY = std::max(maxY, coord.second);
	}

	int w = form_.GetSizes().first,
		h = form_.GetSizes().second;

 	int AddX = w - 1,
 		AddY = h - 1;

 	G1Field_.resize((maxY + 1) + 2 * AddY, std::vector<bool>((maxX + 1) + 2 * AddX, 0));
	for (const auto& coord : G1Coord)
		G1Field_[coord.second + AddY][coord.first + AddX] = 1;

	int start_x = form_.GetCoords().first,
		start_y = form_.GetCoords().second;

	std::set<G2Vertex> MetVertex;
	for (int cur_x = start_x; cur_x < G1Field_[0].size(); cur_x += w)
		for (int cur_y = start_y; cur_y < G1Field_.size(); cur_y += h) {

			std::vector<std::pair<int, int>> g1_coords_in_vertex = GetG1FieldCoordsInForm(G1Field_,
															  							  G2Form(w, h, cur_x, cur_y));
			for (auto& coord : g1_coords_in_vertex) {
				coord.first -= AddX;
				coord.second -= AddY;
			}

			G2Vertex new_vertex;
			for (auto& coord : g1_coords_in_vertex)
				new_vertex.push_back(G1VertexNum_.at(coord));

			if (new_vertex.size() == 0)
				continue;

			if (MetVertex.find(new_vertex) == MetVertex.end()) {
				MetVertex.insert(new_vertex);
				Vertex_.push_back(new_vertex);
			}
		}

	int vertex_count_check = 0;
	for (int i = 0; i < Vertex_.size(); ++i)
		vertex_count_check += Vertex_[i].size();
	std::cout << vertex_count_check << " " << g1->GetVertexCoord().size() << std::endl; 
	//assert(vertex_count_check == g1->GetVertexCoord().size());

	std::cout << "G2 Vertex OK" << std::endl;

	for (int i = 0; i < Vertex_.size(); ++i) {
		assert(Vertex_[i].size() > 0);

		VertexCoat_.push_back(g1->GetVertexCoat()[Vertex_[i][0]]);
		for (int j = 1; j < Vertex_[i].size(); ++j) {
			auto cur_coat = VertexCoat_.back();
			VertexCoat_.pop_back();

			cur_coat = Merge(cur_coat, g1->GetVertexCoat()[Vertex_[i][j]]);
			VertexCoat_.push_back(cur_coat);
		}
	}

	std::cout << "G2 Vertex Coat OK" << std::endl;

	AdjMatrix_.resize(Vertex_.size(), std::vector<int>(Vertex_.size()));

	for (int i = 0; i < AdjMatrix_.size(); ++i)
		for (int j = 0; j < AdjMatrix_[i].size(); ++j)
			AdjMatrix_[i][j] = VertexCoat_[i].SquareSize() + 
							   VertexCoat_[j].SquareSize() -
							   2 * IntersectionSize(VertexCoat_[i], VertexCoat_[j]);


	std::cout << "Adjacency Matrix OK" << std::endl;

	for (int i = 0; i < AdjMatrix_.size(); ++i)
		AdjMatrix_[i][i] = INT_MAX;

	std::cout << "Adjacency Matrix Diagoanl Setting OK" << std::endl;
	std::cout << "G2 Constructor OK\n" << std::endl;
}

PathHelper G2::LinKernighan() {
	//make file with points in G2
	std::ofstream oout("G2Solver\\G2.txt", std::ios_base::out | std::ios_base::trunc);
	for(int i = 0; i < Vertex_.size(); ++i) {
		oout << "G2 num: " << i << "\n";
		for(int j = 0; j < Vertex_[i].size(); ++j) {
			int num = Vertex_[i][j];
			oout << Vertex_[i][j] << " : " << "(" << g1_->GetVertexCoord()[num].first << ", " << g1_->GetVertexCoord()[num].second << "), ";
		}
		oout << "\n";
	}
	oout.close();

	std::ofstream out("G2Solver\\in.txt", std::ios_base::out | std::ios_base::trunc);
	out << AdjMatrix_.size() << "\n";
	for (int i = 0; i < AdjMatrix_.size(); ++i) {
		for (int j = 0; j < AdjMatrix_[i].size(); ++j)
			out << AdjMatrix_[i][j] << " ";
		out << "\n";
	}
	out.close();

	std::cout << "Lin Kernighan Prepare OK" << std::endl;
	system("G2Solver\\G2Solver.exe G2Solver\\in.txt G2Solver\\out.txt > log.txt");

	std::ifstream in("G2Solver\\out.txt", std::ios::in);
	PathHelper path_helper(Vertex_, g1_, in);
	in.close();

	return path_helper;		
}

const std::vector<std::vector<bool>>& G2::GetG1Field() {
	return G1Field_;
}