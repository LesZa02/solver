#pragma once

#include "../Segment/Segment.hpp"

#include <string>
#include <vector>

class RArea {
public:
	RArea() {}
	RArea(const std::vector<std::string>& data);
	RArea(const std::vector<std::vector<Segment>>& lines);

	int Size() const { return lines_.size(); }
	int SquareSize() const;
	const std::vector<std::vector<Segment>>& GetLines() const; 

	friend RArea Intersection(RArea r1, RArea r2);
	friend RArea Merge(RArea r1, RArea r2);
	friend int IntersectionSize(RArea r1, RArea r2);

private:
	std::vector<std::vector<Segment>> lines_;
	std::vector<int> lines_y_;
};