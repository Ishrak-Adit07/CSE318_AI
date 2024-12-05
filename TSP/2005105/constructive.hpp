#include "perturbative.hpp"

// 1. Nearest Neighbour Heuristic
vector<int> nearestNeighborHeuristic(const Graph &graph)
{

    int n = graph.points.size();
    int start_node = rand() % n + 1;
    vector<int> tour;
    vector<bool> visited(n, false);

    int current = start_node;
    visited[current] = true;
    tour.push_back(current);

    for (int i = 1; i < n; ++i)
    {
        int nearest_neighbor = -1;
        double nearest_neighbor_distance = numeric_limits<double>::infinity();

        for (int j = 0; j < n; ++j)
        {
            if (!visited[j] && graph.getDistance(current, j) < nearest_neighbor_distance)
            {
                nearest_neighbor = j;
                nearest_neighbor_distance = graph.getDistance(current, j);
            }
        }

        tour.push_back(nearest_neighbor);
        visited[nearest_neighbor] = true;
        current = nearest_neighbor;
    }

    tour.push_back(start_node);
    return tour;
}

// 2. Greedy Heuristic Function
vector<int> greedyHeuristic(const Graph &graph)
{
    vector<int> optimized_tour;

    int n = graph.getSize();
    vector<bool> visited(n, false);

    srand(time(0));
    int start_node = rand() % n;
    optimized_tour.push_back(start_node);
    visited[start_node] = true;

    while (optimized_tour.size() < n)
    {
        int best_node = -1;
        double least_distance = numeric_limits<double>::infinity();

        for (int i = 0; i < n; ++i)
        {
            if (!visited[i] && graph.getDistance(optimized_tour.back(), i) < least_distance)
            {
                best_node = i;
                least_distance = graph.getDistance(optimized_tour.back(), i);
            }
        }

        optimized_tour.push_back(best_node);
        visited[best_node] = true;
    }

    optimized_tour.push_back(start_node);
    return optimized_tour;
}

// 3. Cheapest Insertion Heuristic
vector<int> cheapestInsertionHeuristic(const Graph &graph)
{
    int n = graph.getSize();
    srand(time(0));

    int start_node = rand() % n;
    vector<int> optimized_tour = {start_node};
    unordered_set<int> unvisited_nodes;
    
    for (int i = 0; i < n; ++i)
        if (i != start_node)
            unvisited_nodes.insert(i);

    double cheapest_distance = numeric_limits<double>::infinity();
    int cheapest_node = -1;
    for (int node : unvisited_nodes)
    {
        double temp_distance = graph.getDistance(start_node, node);
        if (temp_distance < cheapest_distance)
        {
            cheapest_distance = temp_distance;
            cheapest_node = node;
        }
    }

    optimized_tour.push_back(cheapest_node);
    unvisited_nodes.erase(cheapest_node);

    while (!unvisited_nodes.empty())
    {
        double best_cost = numeric_limits<double>::infinity();
        int best_node = -1;
        int best_node_position = -1;

        for (int node : unvisited_nodes)
        {
            for (int i = 0; i < optimized_tour.size() - 1; ++i)
            {
                int first_node = optimized_tour[i];
                int consequtive_second_node = optimized_tour[i + 1];
                double cost = graph.getDistance(first_node, node) +
                              graph.getDistance(node, consequtive_second_node) -
                              graph.getDistance(first_node, consequtive_second_node);
                if (cost < best_cost)
                {
                    best_cost = cost;
                    best_node = node;
                    best_node_position = i + 1;
                }
            }
        }

        optimized_tour.insert(optimized_tour.begin() + best_node_position, best_node);
        unvisited_nodes.erase(best_node);
    }

    optimized_tour.push_back(start_node);
    return optimized_tour;
}

// Run the heuristics
void run_constructive_heuristics(const vector<Point> &points, vector<string> &csv_row)
{

    if (points.size() < 2)
    {
        cout << "Not enough points to perform heuristics." << endl;
        return;
    }
    Graph graph(points);

    vector<int> nnh_tour = nearestNeighborHeuristic(graph);
    csv_row.push_back(to_string(calculateTourDistance(graph, nnh_tour)));
    cout << "Nearest Neighbour Tour Distance: " << calculateTourDistance(graph, nnh_tour) << endl;
    run_perturbative_heuristics(points, nnh_tour, csv_row);
    cout << endl;

    vector<int> greedy_tour = greedyHeuristic(graph);
    csv_row.push_back(to_string(calculateTourDistance(graph, greedy_tour)));
    cout << "Greedy Insertion Tour Distance: " << calculateTourDistance(graph, greedy_tour) << endl;
    run_perturbative_heuristics(points, greedy_tour, csv_row);
    cout << endl;

    vector<int> cheapest_insertion_tour = cheapestInsertionHeuristic(graph);
    csv_row.push_back(to_string(calculateTourDistance(graph, cheapest_insertion_tour)));
    cout << "Cheapest Insertion Tour Distance: " << calculateTourDistance(graph, cheapest_insertion_tour) << endl;
    run_perturbative_heuristics(points, cheapest_insertion_tour, csv_row);
    cout << endl;
}