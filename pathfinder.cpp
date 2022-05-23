#include "G2\G2.hpp"
#include "G1\G1.hpp"
#include "Form\Form.hpp"
#include "PathHelper\PathHelper.hpp"
#include <iostream>
#include <climits>

int main(int argc, char** argv) {

	G1* g1 = new G1(argv[1]);

	std::vector<G2Form> forms;
	forms.push_back(G2Form(1, 1, 0));

	int k = std::stoi(argv[2]);
	for (int i = 2; i <= k; ++i)
		for (int j = 0; j < i; ++j) {
			forms.push_back(G2Form(i, 1, (i-1) + j));
			forms.push_back(G2Form(1, i, (i-1) + j));
		}

	std::vector<G2> G2FormsResults;
	for (auto form : forms)
		G2FormsResults.push_back(G2(g1, form));

	std::vector<PathHelper> G2Paths;
	for (auto& g2 : G2FormsResults)
		G2Paths.push_back(g2.LinKernighan());

	int best_path_weight = INT_MAX,
		best_path_num    = 0;

	for (int i = 0; i < G2Paths.size(); ++i)
		if (G2Paths[i].GetPathWeight() < best_path_weight) {
			best_path_weight = G2Paths[i].GetPathWeight();
			best_path_num    = i;
		}

	// //////////
	std::cout << "Best form: (" << forms[best_path_num].GetSizes().first << ", " << forms[best_path_num].GetSizes().second << ")\n";
	std::cout << "Best form start place: (" << forms[best_path_num].GetCoords().first << ", " << forms[best_path_num].GetCoords().second << ")\n";
	G2 best_graph(g1, forms[best_path_num]);
	best_graph.LinKernighan();

	std::ofstream out("vis.txt", std::ios_base::out);
	out << g1->GetVertexCoord().size() << "\n";
	for (const auto& coord : g1->GetVertexCoord())
		out << coord.first << " " << coord.second << "\n";
	out << "\n";

	G2Paths[best_path_num].WritePath(out);
	out.close();

	delete g1;
	return 0;
}