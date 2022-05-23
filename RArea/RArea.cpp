#include "RArea.hpp"

#include <cassert>
#include <iostream>
#include <algorithm>	

RArea::RArea(const std::vector<std::string>& data) {
	std::vector<std::pair<int, std::vector<Segment>>> cringe;

	for (int i = 0; i < data.size(); ++i) {
		std::string temp = data[i].substr(63, 16);
		int line = std::stoi(temp.substr(0, 4));
		int l = std::stoi(temp.substr(4, 4));
		int r = std::stoi(temp.substr(12, 4));

		//lines_.push_back({Segment(l, r)});
		//lines_y_.push_back(line);

		cringe.push_back(std::make_pair(line, std::vector<Segment>{Segment(l, r)}));
	}

	std::sort(cringe.begin(), cringe.end());
	for(auto it : cringe) {
		lines_.push_back(it.second);
		lines_y_.push_back(it.first);
	}
}

int RArea::SquareSize() const {
	int ret = 0;
	for(int i = 0; i < lines_.size(); ++i) {
		for (const auto& cur_segment : lines_[i]) {
			assert(cur_segment.GetPair().second >= cur_segment.GetPair().first);
			ret += cur_segment.Length();
		}
	}
	return ret;
}

const std::vector<std::vector<Segment>>& RArea::GetLines() const {
	return lines_;
}

RArea Intersection(RArea r1, RArea r2) {
	int line1_it = 0, line1_end = r1.lines_.size();
	int line2_it = 0, line2_end = r2.lines_.size();

	RArea ret;

	while (line1_it != line1_end and line2_it != line2_end) {
		if (r1.lines_[line1_it].empty()) {
			line1_it++;
			continue;
		}

		if (r2.lines_[line2_it].empty()) {
			line2_it++;
			continue;
		}

		if (r1.lines_y_[line1_it] < r2.lines_y_[line2_it]) {
			line1_it++;
			continue;
		}

		if (r1.lines_y_[line1_it] > r2.lines_y_[line2_it]) {
			line2_it++;
			continue;
		}

		auto inters_segments = Intersection(r1.lines_[line1_it], r2.lines_[line2_it]);
		if (not inters_segments.empty()) {
			ret.lines_.push_back(inters_segments);
			ret.lines_y_.push_back(r2.lines_y_[line2_it]);
		}

		line1_it++;
		line2_it++;
	}

	return ret;
}

RArea Merge(RArea r1, RArea r2) {
	int line1_it = 0, line1_end = r1.lines_.size();
	int line2_it = 0, line2_end = r2.lines_.size();

	RArea ret;

	while (line1_it != line1_end or line2_it != line2_end) {
		if (line1_it == line1_end) {
			ret.lines_.push_back(r2.lines_[line2_it]);
			ret.lines_y_.push_back(r2.lines_y_[line2_it]);

			line2_it++;
			continue;
		}

		if (line2_it == line2_end) {
			ret.lines_.push_back(r1.lines_[line1_it]);
			ret.lines_y_.push_back(r1.lines_y_[line1_it]);

			line1_it++;
			continue;
		}

		if (r1.lines_y_[line1_it] < r2.lines_y_[line2_it]) {
			ret.lines_.push_back(r1.lines_[line1_it]);
			ret.lines_y_.push_back(r1.lines_y_[line1_it]);			

			line1_it++;
			continue;
		}

		if (r1.lines_y_[line1_it] > r2.lines_y_[line2_it]) {
			ret.lines_.push_back(r2.lines_[line2_it]);
			ret.lines_y_.push_back(r2.lines_y_[line2_it]);

			line2_it++;
			continue;
		}

		ret.lines_.push_back(Merge(r1.lines_[line1_it], r2.lines_[line2_it]));
		ret.lines_y_.push_back(r2.lines_y_[line2_it]);

		line1_it++;
		line2_it++;
	}

	return ret;
}

int IntersectionSize(RArea r1, RArea r2) {
	RArea res = Intersection(r1, r2);
	return res.SquareSize();
}