#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <unordered_set>

using namespace std;

#define TOP_K_SELECTION 3

struct Point
{
    double x, y;
    Point(double x_, double y_) : x(x_), y(y_) {}
};

class Graph
{
public:
    vector<Point> points;
    vector<vector<double>> distMatrix;

    Graph(const vector<Point> &pts) : points(pts)
    {
        int n = points.size();
        distMatrix.resize(n, vector<double>(n));
        computeDistances();
    }

    double euclideanDistance(const Point &a, const Point &b)
    {
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    }

    void computeDistances()
    {
        for (int i = 0; i < points.size(); ++i)
            for (int j = 0; j < points.size(); ++j)
                distMatrix[i][j] = euclideanDistance(points[i], points[j]);
    }

    double getDistance(int i, int j) const
    {
        return distMatrix[i][j];
    }
    
    int getSize() const{
        return points.size();
    }
};

// Function to print the tour
void printTourPoints(const Graph& graph, const vector<int>& tour) {
    cout << "Tour Points: ";
    for (int city : tour) {
        cout << "(" << graph.points[city].x << ", " << graph.points[city].y << ") ";
    }
    cout << endl;
}

// Function to calculate total tour distance
double calculateTourDistance(const Graph& graph, const vector<int>& tour) {
    double totalDistance = 0.0;
    for (int i = 0; i < tour.size() - 1; ++i) {
        totalDistance += graph.getDistance(tour[i], tour[i + 1]);
    }
    return totalDistance;
}