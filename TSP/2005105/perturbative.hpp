#include "graph.hpp"

using namespace std;

// Two-Opt Heuristic
vector<int> twoOptHeuristic(const Graph& graph, vector<int> tour) {
    vector<int> optimized_tour = tour;
    bool improved_tour_found = true;

    while (improved_tour_found) {

        improved_tour_found = false;

        for (int i = 1; i < optimized_tour.size() - 2; ++i) { 
            for (int j = i + 1; j < optimized_tour.size() - 1; ++j) {

                if (i == j || abs(i - j) == 1)
                    continue;

                vector<int> temp_tour = optimized_tour;
                reverse(temp_tour.begin() + i, temp_tour.begin() + j);

                if (calculateTourDistance(graph, temp_tour) < calculateTourDistance(graph, optimized_tour)) {
                    optimized_tour = temp_tour;
                    improved_tour_found = true;
                }
            }
        }
    }

    return optimized_tour;
}

// Node Shift Heuristic
vector<int> nodeShiftHeuristic(const Graph &graph, vector<int> &initial_tour)
{
    vector<int> optimized_tour = initial_tour;
    double optimized_cost = calculateTourDistance(graph, optimized_tour);
    bool improved_tour_found = true;

    while (improved_tour_found)
    {
        improved_tour_found = false;
        //int n = optimized_tour.size();

        for (int i = 1; i < optimized_tour.size() - 1; ++i)
        {
            int current_node = optimized_tour[i];
            for (int j = 1; j < optimized_tour.size() - 1; ++j)
            {
                if (i == j)
                    continue;

                vector<int> temp_tour = optimized_tour;
                temp_tour.erase(temp_tour.begin() + i);
                temp_tour.insert(temp_tour.begin() + j, current_node);

                double temp_cost = calculateTourDistance(graph, temp_tour);

                if (temp_cost < optimized_cost)
                {
                    optimized_tour = temp_tour;
                    optimized_cost = temp_cost;
                    improved_tour_found = true;
                    break;
                }
            }

            if (improved_tour_found)
                break;
        }
    }
    return optimized_tour;
}

// Node Swap Heuristic
vector<int> nodeSwapHeuristic(const Graph& graph, vector<int>& tour) {

    vector<int> optimized_tour = tour;
    double optimized_cost = calculateTourDistance(graph, optimized_tour);

    bool improved_tour_found = true;

    while (improved_tour_found) {
        
        improved_tour_found = false;

        for (int i = 1; i < optimized_tour.size() - 1; ++i) { 
            for (int j = i + 1; j < optimized_tour.size() - 1; ++j) {
                if (i == j) continue;

                vector<int> temp_tour = optimized_tour;
                swap(temp_tour[i], temp_tour[j]);

                double temp_tour_cost = calculateTourDistance(graph, temp_tour);

                if (temp_tour_cost < optimized_cost) {
                    optimized_tour = temp_tour;
                    optimized_cost = temp_tour_cost;
                    improved_tour_found = true;
                }
            }
        }
    }

    return optimized_tour;
}

void run_perturbative_heuristics(const vector<Point> &points, vector<int> &initialTour, vector<string> &csv_row)
{   
    if (points.size() < 2)
    {
        cout << "Not enough points for heuristics." << endl;
        return;
    }
    Graph graph(points);

    if (initialTour.front() != initialTour.back())
    {
        initialTour.push_back(initialTour.front());
    }

    vector<int> two_opt_tour = twoOptHeuristic(graph, initialTour);
    csv_row.push_back(to_string(calculateTourDistance(graph, two_opt_tour)));
    cout << "2-opt Tour Distance: " << calculateTourDistance(graph, two_opt_tour) << endl;

    vector<int> node_shift_tour = nodeShiftHeuristic(graph, two_opt_tour);
    csv_row.push_back(to_string(calculateTourDistance(graph, node_shift_tour)));
    cout << "Node Shift Tour Distance: " << calculateTourDistance(graph, node_shift_tour) << endl;

    vector<int> node_swap_tour = nodeSwapHeuristic(graph, node_shift_tour);
    csv_row.push_back(to_string(calculateTourDistance(graph, node_swap_tour)));
    cout << "Node Swap Tour Distance: " << calculateTourDistance(graph, node_swap_tour) << endl;
}