#include "G1.hpp"

#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <climits>

#include <iostream>
#include <cassert>

bool IsUnsignedDigit(const std::string& str) {
	if (str.size() == 0)
		return false;

	if (str.size() == 1 and str[0] == '0')
		return true;

	if (str[0] == '0')
		return false;

	for (int i = 0; i < str.size(); ++i)
		if (str[i] < '0' or str[i] > '9')
			return false;

	return true;
}

bool IsVoid(const std::string& xps_line) {
	std::stringstream in(xps_line);
	
	std::string foo;
	return not (in >> foo);
}

int GetVertexX(const std::string& xps_line) {
	std::string info_block = xps_line.substr(29, 4);
	reverse(info_block.begin(), info_block.end());
	while (info_block.back() == ' ' or info_block.back() == '0')
		info_block.pop_back();
	reverse(info_block.begin(), info_block.end());

	if (not IsUnsignedDigit(info_block))
		throw std::invalid_argument(std::string() + "\n" + 
									"For Class G1\n" +
									"Function GetVertexX\n" +
									"Info block is not unsigned digit\n" +
									"xps_line: " + xps_line + "\n"
									"info_block: " + info_block + "\n"
								   );

	return std::stoi(info_block);
}

int GetVertexY(const std::string& xps_line) {
	std::string info_block = xps_line.substr(33, 4);
	reverse(info_block.begin(), info_block.end());
	while (info_block.back() == ' ' or info_block.back() == '0')
		info_block.pop_back();
	reverse(info_block.begin(), info_block.end());

	if (not IsUnsignedDigit(info_block))
		throw std::invalid_argument(std::string() + "\n" +
									"For Class G1\n" +
									"Function GetVertexY\n" +
									"Info block is not unsigned digit\n" +
									"xps_line: " + xps_line + "\n"
									"info_block: " + info_block + "\n"
								   );

	return std::stoi(info_block);
}

std::string GetLastInfoBlock(const std::string& xps_line) {
	std::string xps_line_copy = xps_line;

	reverse(xps_line_copy.begin(), xps_line_copy.end());

	std::stringstream in(xps_line_copy);
	std::string info_block;
	in >> info_block;

	reverse(info_block.begin(), info_block.end());

	return info_block;
}

void DeleteUnusedXPSLines(std::vector<std::string>& xps_lines) {
	std::vector<std::string> new_xps_lines;

	int cur_segment_size = 0;

	for (int i = 0; i < xps_lines.size(); ++i) {
		if (not new_xps_lines.empty() and 
			std::make_pair(GetVertexX(new_xps_lines.back()), GetVertexY(new_xps_lines.back())) !=
				std::make_pair(GetVertexX(xps_lines[i]), GetVertexY(xps_lines[i])))
		{
			i += cur_segment_size * 5;
			cur_segment_size = 0;

			if (i >= xps_lines.size())
				break;
		}

		new_xps_lines.push_back(xps_lines[i]);
		cur_segment_size++;
	}

	xps_lines = new_xps_lines;
}

G1::G1(const std::string xps_path) :
	AdjMatrix_(),
	VertexCoord_(),
	VertexCoat_(),

	VertexCount_(0),
	FictivExist_(false)
{
	std::cout << "Start Constructor G1" << std::endl;

	std::ifstream in(xps_path);
	if (not in.is_open())
		throw std::runtime_error("\n"
								 "Class G1\n"
								 "Constructor\n"
								 "Can not open xps file\n"
								);

	std::vector<std::string> xps_lines;
	std::string xps_line;
	while (getline(in, xps_line)) {
		if (IsVoid(xps_line))
			continue;

		xps_lines.push_back(xps_line); 
	}

	in.close();

	std::cout << "Read XPS File OK" << std::endl;

	if (xps_lines.empty())
		throw std::runtime_error("\n"
								 "Class G1\n"
								 "Constructor\n"
								 "XPS file is empty or bad when reading\n"
								);

	DeleteUnusedXPSLines(xps_lines);

	std::cout << "Delete OK" << std::endl;

	int SubX = GetVertexX(xps_lines[0]),
		SubY = GetVertexY(xps_lines[0]);

	for (const auto& cur_xps_line : xps_lines) {
		int X = GetVertexX(cur_xps_line),
			Y = GetVertexY(cur_xps_line);

		if (VertexCoord_.empty() || VertexCoord_.back() != std::make_pair(X - SubX, Y - SubY)) {
			VertexCount_++;
			VertexCoord_.push_back({X - SubX, Y - SubY});
		}
	}

	std::cout << "Vertex Coord OK" << std::endl;

	std::vector<std::pair<int, int>> vertex_lines_segment;
	int cur_vertex_start = 0,
		cur_line_num     = 0,
		cur_vertex_num   = 0;

	for (const auto& cur_xps_line : xps_lines) {
		if (std::make_pair(GetVertexX(cur_xps_line) - SubX, GetVertexY(cur_xps_line) - SubY) != VertexCoord_[cur_vertex_num]) {
			vertex_lines_segment.push_back({cur_vertex_start, cur_line_num-1});
			cur_vertex_start = cur_line_num;
			cur_vertex_num++;
		}
		cur_line_num++;
	}
	vertex_lines_segment.push_back({cur_vertex_start, cur_line_num-1});

	assert(VertexCount_ == vertex_lines_segment.size());
	std::cout << "Vertex Lines Segments OK" << std::endl;

	for (const auto& cur_vertex_segment : vertex_lines_segment) {
		std::vector<std::string> cur_vertex_segment_lines;
		for (int i = cur_vertex_segment.first; i <= cur_vertex_segment.second; ++i)
			cur_vertex_segment_lines.push_back(xps_lines[i]);

		VertexCoat_.push_back(RArea(cur_vertex_segment_lines));
	}

	assert(VertexCount_ == VertexCoat_.size());
	std::cout << "Vertex Cout OK" << std::endl;

	AdjMatrix_.resize(VertexCount_, std::vector<int>(VertexCount_));

	for (int i = 0; i < VertexCount_; ++i)
		for (int j = 0; j < VertexCount_; ++j)
			AdjMatrix_[i][j] = VertexCoat_[i].SquareSize() + 
							   VertexCoat_[j].SquareSize() -
							   2 * IntersectionSize(VertexCoat_[i], VertexCoat_[j]);

	std::cout << "AAAAAAAAAAAAAAA " << VertexCoat_[0].SquareSize() << " " << VertexCoat_[1].SquareSize() << std::endl;  

	std::cout << "Adjacency Matrix OK" << std::endl;

	int gcd_x = std::__gcd(VertexCoord_[0].first, VertexCoord_[1].first);
	for (int i = 2; i < VertexCoord_.size(); ++i)
		gcd_x = std::__gcd(gcd_x, VertexCoord_[i].first);
	if (gcd_x > 0)
		for (int i = 0; i < VertexCoord_.size(); ++i)
			VertexCoord_[i].first /= gcd_x;

	int gcd_y = std::__gcd(VertexCoord_[0].second, VertexCoord_[1].second);
	for (int i = 2; i < VertexCoord_.size(); ++i)
		gcd_y = std::__gcd(gcd_y, VertexCoord_[i].second);
	if (gcd_y > 0)
		for (int i = 0; i < VertexCoord_.size(); ++i)
			VertexCoord_[i].second /= gcd_y;

	std::cout << "Vertex Coord Compression OK" << std::endl;

	for (int i = 0; i < VertexCount_; ++i)
		AdjMatrix_[i][i] = INT_MAX;

	std::cout << "Adjacency Matrix Diagoanl Setting OK" << std::endl;

	std::cout << "Constructor G1 OK\n" << std::endl;
}

const std::vector<std::vector<int>>& G1::GetAdjMatrix() const {
	return AdjMatrix_;
}

const std::vector<std::pair<int, int>>& G1::GetVertexCoord() const {
	return VertexCoord_;
}

int G1::GetVertexCount() const {
	return VertexCount_;
}

const std::vector<RArea>& G1::GetVertexCoat() const {
	return VertexCoat_;
}
/*
void G1::AddFictivNode() {
	if (FictivNodeExist_)
		throw std::runtime_error("\n"
								 "Class G1\n"
								 "Method AddFictivNode\n"
								 "Fictiv Node is exist but try add it\n"
								);

	for (int i = 0; i < VertexCount_; ++i)

}
*/