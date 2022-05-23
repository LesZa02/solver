#pragma once

#include <utility>
#include <vector>
#include <fstream>

class Form {
public:
	Form(int w, int h);
	int Square() const;

	std::pair<int, int> GetSizes() const;

private:
	int w_, h_;
};

std::ostream& operator << (std::ostream& out, Form form);

class G2Form : public Form {
public:
	G2Form(int w, int h, 
		   int x, int y);

	G2Form(int w, int h,
		   int Add);

	std::pair<int, int> GetCoords() const;

private:
	int x_, y_;
};

G2Form Intersection(G2Form lhs, G2Form rhs);

bool CheckG2FormG1FieldIntersection(const std::vector<std::vector<bool>>& field,
			   					    G2Form form);

std::pair<int, int> GetFirstWHG1FieldIntersectionCoords(const std::vector<std::vector<bool>>& field,
														int w, int h);

std::vector<std::pair<int, int>> GetG1FieldCoordsInForm(const std::vector<std::vector<bool>>&field,
														G2Form form);