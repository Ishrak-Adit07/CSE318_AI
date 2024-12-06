#include "Algorithm.hpp"
#include <algorithm>
#include <fstream>
#include <ctime>
#include <cmath>
#include <random>

#define ITERATIONS 20
#define TRAIN_PERCENTAGE 0.8

int main(int argc, char **argv) {
    srand(time(NULL)); // Seed the random number generator
    random_device rd;
    default_random_engine rng(rd());

    // Check for input file argument
    if (argc < 2) {
        cout << "Usage: ./main <input_file>" << endl;
        return 1;
    }

    string input_file = string(argv[1]);
    ifstream in(input_file);

    if (!in.is_open()) {
        cout << "Error: Unable to open file " << input_file << endl;
        return 1;
    }

    vector<Car> cars;

    // Read the dataset from the file
    while (true) {
        string str;
        getline(in, str);
        if (in.eof())
            break;

        // Construct a Car object from the input line
        Car car(str);
        cars.push_back(car);
    }
    in.close();

    if (cars.empty()) {
        cout << "Error: No data found in the input file." << endl;
        return 1;
    }

    double best_ig = getAccuracy(cars, information_gain, rng, 1);
    double top_3_ig = getAccuracy(cars, information_gain, rng, 3);
    double best_gini = getAccuracy(cars, gini_impurity, rng, 1);
    double top_3_gini = getAccuracy(cars, gini_impurity, rng, 3);

    // cout << best_ig << endl;
    // cout << top_3_ig << endl;
    // cout << best_gini << endl;
    // cout << top_3_gini << endl;

    print_accuracy_table(best_ig, best_gini, top_3_ig, top_3_gini);
    return 0;
}
