#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include "constructive.hpp"

namespace fs = std::filesystem;

vector<Point> parseTSPFile(const string &filename)
{
    vector<Point> points;
    ifstream file(filename);
    string line;

    while (getline(file, line))
        if (line == "NODE_COORD_SECTION")
            break;

    while (getline(file, line))
    {
        if (line == "EOF")
            break;

        stringstream ss(line);
        int node;
        double x, y;

        ss >> node >> x >> y;
        points.emplace_back(Point{x, y});
    }

    return points;
}

int main()
{
    srand(time(0));
    string csv_file_name = "tour_costs.csv";
    ofstream csv_file(csv_file_name, ios::out);
    csv_file << "File no,Filename,NearestNeighborHeuristic,2opt-1,NodeShift-1,NodeSwap-1,GreedyHeuristic,2opt-2,NodeShift-2,NodeSwap-2,CheapestInsertionHeuristic,2opt-3,NodeShift-3,NodeSwap-3\n";

    int number_of_files = 1;
    vector<string> csv_row;
    int csv_row_size;
    string directory = "data";
    for (const auto &entry : fs::directory_iterator(directory))
    {
            string filename = entry.path().string();
            cout << "Processing file no " << number_of_files << ": " << filename << endl;

            if(filename != "data\\st70.tsp"){
                continue;
            }

            csv_row.push_back(to_string(number_of_files));
            csv_row.push_back(filename);
            number_of_files++;

            vector<Point> points = parseTSPFile(filename);
            run_constructive_heuristics(points, csv_row);

            csv_row_size = csv_row.size();
            for(int i=0; i<csv_row_size; i++){
                csv_file << csv_row[i];
                if(i!=(csv_row_size-1)) csv_file << ",";
            }
            csv_file << endl;
            csv_row.clear();

            cout << "Finished processing: " << filename << endl;
            cout << endl;
    }

    csv_file.close();
    return 0;
}