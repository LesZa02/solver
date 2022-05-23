#pragma once

#include "../RArea/RArea.hpp"

#include <string>
#include <vector>


class G1 {
public:
	G1(const std::string xps_path);

	const std::vector<std::vector<int>>& GetAdjMatrix() const;
	const std::vector<std::pair<int, int>>& GetVertexCoord() const;
	const std::vector<RArea>& GetVertexCoat() const;
	int GetVertexCount() const;

	//void AddFictivNode();
	//void DeleteFictivNode();
private:
	std::vector<std::vector<int>> AdjMatrix_;
	std::vector<std::pair<int, int>> VertexCoord_;

	std::vector<RArea> VertexCoat_;

	int VertexCount_;
	bool FictivExist_;
};