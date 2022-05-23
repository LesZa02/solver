#pragma once

#include <utility>
#include <vector>

class Segment {
public:

	Segment(int l, int r);
	int Length() const;

	std::pair<int, int> GetPair() const;

private:
	int l_, r_;
};

bool operator < (const Segment& lhs, const Segment& rhs);
Segment Intersection(Segment lhs, Segment rhs);

std::vector<Segment> Intersection(const std::vector<Segment>& first, 
								  const std::vector<Segment>& second);

std::vector<Segment> Merge(const std::vector<Segment>& first,
						   const std::vector<Segment>& second);
