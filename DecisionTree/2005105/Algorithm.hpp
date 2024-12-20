#include <cmath>
#include <limits>
#include <algorithm>
#include <random>
#include "DecisionTree.hpp"

#define ITERATIONS 20
#define TRAIN_PERCENTAGE 0.8

using namespace std;

map<string, int> count_values(vector<Car> &dataset, string attr)
{
    map<string, int> counts;
    for (auto &car : dataset)
    {
        counts[car.getCarAttribute(attr)]++;
    }
    return counts;
}

vector<Car> filter_dataset(vector<Car> &dataset, string attr, string value)
{
    vector<Car> filtered;
    for (auto &car : dataset)
    {
        if (car.getCarAttribute(attr) == value)
        {
            filtered.push_back(car);
        }
    }
    return filtered;
}

string plurality_value(vector<Car> &dataset)
{
    map<string, int> temp_map = count_values(dataset, class_attr);
    pair<string, int> return_pair = {"", -1};
    for (auto temp_pair : temp_map)
        if (temp_pair.second > return_pair.second)
            return_pair = temp_pair;
    return return_pair.first;
}

double information_gain(vector<Car> &dataset, string attr)
{
    int total_size = dataset.size();

    map<string, int> attr_counts = count_values(dataset, attr);
    double ig = 0.0;

    for (auto &[value, count] : attr_counts)
    {
        double p = static_cast<double>(count) / total_size;
        vector<Car> filtered = filter_dataset(dataset, attr, value);

        map<string, int> filtered_class_counts = count_values(filtered, class_attr);
        double entropy = 0.0;

        for (auto &[cls, fcount] : filtered_class_counts)
        {
            double q = static_cast<double>(fcount) / count;
            if (q > 0)
            {
                entropy -= q * log2(q);
            }
        }
        ig += p * entropy;
    }

    return ig;
}

double gini_impurity(vector<Car> &dataset, string attr) {
    int total_size = dataset.size();
    if (total_size == 0) return 0.0; 

    map<string, int> attr_counts = count_values(dataset, attr);
    double gini = 0.0;

    for (auto &[value, count] : attr_counts) {
        double p = static_cast<double>(count) / total_size;

        double impurity = 1.0;

        vector<Car> filtered = filter_dataset(dataset, attr, value);
        map<string, int> filtered_class_counts = count_values(filtered, class_attr);
        for (auto &[cls, fcount] : filtered_class_counts) {
            double q = static_cast<double>(fcount) / count;
            impurity -= q * q;
        }

        gini += p * impurity;
    }

    return gini;
}

Node *train_decision_tree(vector<Car> &dataset, vector<string> &attributes, vector<Car> &parent_dataset,
                          double (*metric)(vector<Car> &, string), int k=1, int min_size = 5, double threshold = 0.01)
{

    string majority_class = plurality_value(parent_dataset);
    if (dataset.empty())
    {
        Node *leaf = new Node();
        leaf->setIsLeaf(true);
        leaf->setLabel(majority_class);
        return leaf;
    }

    map<string, int> class_counts = count_values(dataset, class_attr);
    if (class_counts.size() == 1)
    {
        Node *leaf = new Node();
        leaf->setIsLeaf(true);
        leaf->setLabel(class_counts.begin()->first);
        return leaf;
    }

    majority_class = plurality_value(dataset);
    if (attributes.empty())
    {
        Node *leaf = new Node();
        leaf->setIsLeaf(true);
        leaf->setLabel(majority_class);
        return leaf;
    }

    // if (dataset.size() < min_size)
    // {
    //     Node *leaf = new Node();
    //     leaf->setIsLeaf(true);
    //     leaf->setLabel(majority_class);
    //     return leaf;
    // }

    string best_attribute;
    double best_score;

    vector<pair<string, double>> scores;
    for (auto &attr : attributes)
    {
        double score = metric(dataset, attr);
        scores.push_back({attr, score});
    }
    sort(scores.begin(), scores.end(), [](const pair<string, double> &a, const pair<string, double> &b)
         { return a.second < b.second; });

    if(k == 1){
        best_attribute = scores[0].first;
        best_score = scores[0].second;
    }
    else if ((k!=1) && (scores.size() >= k))
    {
        int random_pick = rand() % k;
        best_attribute = scores[random_pick].first;
        best_score = scores[random_pick].second;
    }
    else
    {
        best_attribute = scores[0].first;
        best_score = scores[0].second;
    }

    // if (best_score <= threshold)
    // {
    //     Node *leaf = new Node();
    //     leaf->setIsLeaf(true);
    //     // string majority_class = plurality_value(dataset);
    //     leaf->setLabel(majority_class);
    //     return leaf;
    // }

    Node *tree = new Node();
    tree->setNodeAttribute(best_attribute);
    tree->setLabel(majority_class);

    map<string, int> attr_values = count_values(dataset, best_attribute);

    for (auto &[value, _] : attr_values)
    {
        vector<Car> filtered = filter_dataset(dataset, best_attribute, value);
        vector<string> remaining_attributes;
        for (auto &attr : attributes)
        {
            if (attr != best_attribute)
            {
                remaining_attributes.push_back(attr);
            }
        }
        tree->setChild(value, train_decision_tree(filtered, remaining_attributes, dataset, metric, k));
    }

    return tree;
}

double getAccuracy(vector<Car> &cars, double (*metric)(vector<Car> &, string), default_random_engine rng, int k = 1)
{

    double total_accuracy = 0;

    for (int i = 0; i < ITERATIONS; i++)
    {
        shuffle(cars.begin(), cars.end(), rng);

        size_t train_size = static_cast<size_t>(cars.size() * TRAIN_PERCENTAGE);
        vector<Car> training_set(cars.begin(), cars.begin() + train_size);
        vector<Car> test_set(cars.begin() + train_size, cars.end());

        Node *root = train_decision_tree(training_set, attributes, training_set, metric, k);
        DecisionTree tree(root);

        string predicted, actual;
        int correct = 0;
        for (auto &car : test_set)
        {
            predicted = tree.getClassification(car);
            actual = car.getCarAttribute(class_attr);
            if (predicted == actual)
                correct++;
        }

        double accuracy = static_cast<double>(correct) / test_set.size();
        total_accuracy += accuracy;
    }

    double mean = total_accuracy / ITERATIONS;
    return mean * 100;
}