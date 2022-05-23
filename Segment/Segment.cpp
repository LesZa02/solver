#include "Segment.hpp"

#include <algorithm>
#include <cassert>

Segment::Segment(int l, int r) :
	l_(l),
	r_(r)
{}

int Segment::Length() const {
	return std::max(0, r_ - l_ + 1);
}

std::pair<int, int> Segment::GetPair() const {
	return std::make_pair(l_, r_);
}

bool operator < (const Segment& lhs, const Segment& rhs) {
	return lhs.GetPair() < rhs.GetPair();
}

Segment Intersection(Segment lhs, Segment rhs) {
	if (rhs < lhs)
		std::swap(lhs, rhs);

	return Segment(rhs.GetPair().first, lhs.GetPair().second);
}

std::vector<Segment> Intersection(const std::vector<Segment>& first,
								  const std::vector<Segment>& second) {
	int r = first.back().GetPair().second;
	r = std::max(r, second.back().GetPair().second);

	int l = first.front().GetPair().first;
	l = std::min(l, second.front().GetPair().first);

	std::vector<int> inter(r - l + 1);
	for (auto it : first)
		for (int i = it.GetPair().first; i <= it.GetPair().second; ++i)
			inter[i - l]++;
	
	for (auto it : second)
		for (int i = it.GetPair().first; i <= it.GetPair().second; ++i)
			inter[i - l]++;

	std::vector<Segment> ret;
	int de = l;

	l = -1, r = -1;
	bool flag = false;

	for (int i = 0; i < inter.size(); ++i) {
		assert(inter[i] <= 2 and inter[i] >= 0);
		if (inter[i] == 2 and !flag) {
			l = i, r = i;
			flag = true;
		} else if (inter[i] == 2 and flag) {
			r++;
		} else {
			if (l != -1)
				ret.push_back(Segment(l + de, r + de));
			l = -1, r = -1;
			flag = false;
		}
	}

	if (l != -1)
		ret.push_back(Segment(l + de, r + de));

	return ret;
}
/*
std::vector<Segment> Intersection(const std::vector<Segment>& first, 
								  const std::vector<Segment>& second) {
	int cur_first  = 0,
		cur_second = 0;

	std::vector<Segment> pre_ret;

	while (cur_first < first.size() and cur_second < second.size()) {
		if (first[cur_first].GetPair().second < second[cur_second].GetPair().first) {
			cur_first++;
			continue;
		}

		if (first[cur_first].GetPair().first > second[cur_second].GetPair().second) {
			cur_second++;
			continue;
		}

		pre_ret.push_back(Segment(std::max(first[cur_first].GetPair().first, second[cur_second].GetPair().first), 
					   	   		  std::min(first[cur_first].GetPair().second, second[cur_second].GetPair().second)));

		if (first[cur_first].GetPair().second < second[cur_second].GetPair().second)
			cur_first++;
		else
			cur_second++;
	}

	std::vector<Segment> ret;

	for (const auto& cur_segment : pre_ret) {
		if (ret.empty()) {
			ret.push_back(cur_segment);
			continue;
		}

		assert(ret.back().GetPair().second <= cur_segment.GetPair().first);

		if (ret.back().GetPair().second == cur_segment.GetPair().first) {
			auto [l, r] = ret.back().GetPair();
			r = cur_segment.GetPair().second;

			ret.pop_back();
			ret.push_back(Segment(l, r));
		} else
			ret.push_back(cur_segment);
	}

	return ret;
}
*/

std::vector<Segment> Merge(const std::vector<Segment>& first,
						   const std::vector<Segment>& second)
{
	int r = first.back().GetPair().second;
	r = std::max(r, second.back().GetPair().second);

	int l = first.front().GetPair().first;
	l = std::min(l, second.front().GetPair().first);

	std::vector<int> inter(r - l + 1);
	for (auto it : first)
		for (int i = it.GetPair().first; i <= it.GetPair().second; ++i)
			inter[i - l]++;
	
	for (auto it : second)
		for (int i = it.GetPair().first; i <= it.GetPair().second; ++i)
			inter[i - l]++;

	std::vector<Segment> ret;
	int de = l;

	l = -1, r = -1;
	bool flag = false;

	for (int i = 0; i < inter.size(); ++i) {
		assert(inter[i] <= 2 and inter[i] >= 0);
		if (inter[i] >= 1 and !flag) {
			l = i, r = i;
			flag = true;
		} else if (inter[i] >= 1 and flag) {
			r++;
		} else {
			if (l != -1)
				ret.push_back(Segment(l + de, r + de));
			l = -1, r = -1;
			flag = false;
		}
	}

	if (l != -1)
		ret.push_back(Segment(l + de, r + de));

	return ret;	
}
/*
std::vector<Segment> Merge(const std::vector<Segment>& first,
						   const std::vector<Segment>& second)
{
	auto couple = first;
	couple.insert(couple.end(), second.begin(), second.end());
	std::sort(couple.begin(), couple.end());

	std::vector<Segment> ret;
	int l = couple[0].GetPair().first,
		r = couple[0].GetPair().second;

	for (int i = 1; i < couple.size(); ++i) {
		if (couple[i].GetPair().first > r) {
			ret.push_back(Segment(l, r));
			l = couple[i].GetPair().first;
			r = couple[i].GetPair().second;
			continue;
		}

		r = couple[i].GetPair().second;
	}
	ret.push_back(Segment(l, r));

	return ret;
}
*/