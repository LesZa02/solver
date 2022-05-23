#include "Form.hpp"

#include "../Segment/Segment.hpp"

#include <stdexcept>
#include <iostream>

Form::Form(int w, int h) :
	w_(w),
	h_(h)
{}

int Form::Square() const {
	return w_ * h_;
}

std::pair<int, int> Form::GetSizes() const {
	return std::make_pair(w_, h_);
}

std::ostream& operator << (std::ostream& out, Form form) {
	auto [w, h] = form.GetSizes();

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j)
			out << "1";
		out << "\n";
	}

	return out;
} 

G2Form::G2Form(int w, int h,
			   int x, int y) :
	Form(w, h),

	x_(x),
	y_(y)
{

	if (x - w + 1 < 0)
		throw std::invalid_argument("\n"
									"Class G2Form\n"
									"Constructor\n"
									"Incorrect args for G2Form\n"
								   );
}

G2Form::G2Form(int w, int h,
			   int Add) :
	Form(w, h),

	x_(0),
	y_(0)
{
	if (w > 1)
		x_ += Add;
	else
		y_ += Add;

	if (x_ - w + 1 < 0 or (w > 1 and h > 1))
		throw std::invalid_argument("\n"
									"Class G2Form\n"
									"Constructor\n"
									"Incorrect args for G2Form\n"
								   );			
}

std::pair<int, int> G2Form::GetCoords() const {
	return std::make_pair(x_, y_);
}

std::pair<int, int> GetFirstWHG1FieldIntersectionCoords(const std::vector<std::vector<bool>>& field,
															int w, int h)
{
	int x = w - 1,
		y = h - 1;

	if (field.size() < field.front().size())
		while (field[y][x] != 1)
			y++;
	else
		while (field[y][x] != 1)
			x++;

	return std::make_pair(x, y);
}

std::vector<std::pair<int, int>> GetG1FieldCoordsInForm(const std::vector<std::vector<bool>>&field,
														G2Form form)
{
	auto [x, y] = form.GetCoords();
	auto [w, h] = form.GetSizes();

	if (x - w + 1 < 0 or y - h + 1 < 0 or 
		y >= field.size() or
		x >= field.front().size())
	{
		std::cout << x << " " << y << " " << w << " " << h << std::endl;
		throw std::invalid_argument("\n"
									"Function GetG1FieldCoordsInForm\n"
									"Incorrect args\n"
								   );
	}

	std::vector<std::pair<int, int>> ret;

	for (int i = x; i > x - w; --i)
		for (int j = y; j > y - h; --j)
			if (field[j][i] == 1)
				ret.push_back(std::make_pair(i, j));

	return ret;
}