#include "Algorithm.hpp"
#include <algorithm>
#include <fstream>
#include <ctime>
#include <cmath>
#include <random>

#define ITERATIONS 20
#define TRAIN_PERCENTAGE 0.8

vector<Car> readDataset(ifstream &instream)
{

    vector<Car> cars;
    while (true)
    {
        string str;
        getline(instream, str);
        if (instream.eof())
            break;

        Car car(str);
        cars.push_back(car);
    }

    return cars;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    random_device rd;
    default_random_engine rng(rd());

    if (argc < 2)
    {
        cout << "Usage: ./main <input_file>" << endl;
        return 1;
    }

    string input_file = string(argv[1]);
    ifstream instream(input_file);

    if (!instream.is_open())
    {
        cout << "Error: Unable to open file " << input_file << endl;
        return 1;
    }
    vector<Car> cars = readDataset(instream);
    instream.close();
    if (cars.empty())
    {
        cout << "Error: No data found in the input file." << endl;
        return 1;
    }

    double best_ig = getAccuracy(cars, information_gain, rng, 1);
    double top_3_ig = getAccuracy(cars, information_gain, rng, 3);
    double best_gini = getAccuracy(cars, gini_impurity, rng, 1);
    double top_3_gini = getAccuracy(cars, gini_impurity, rng, 3);

    print_accuracy_table(best_ig, best_gini, top_3_ig, top_3_gini);
    return 0;
}
