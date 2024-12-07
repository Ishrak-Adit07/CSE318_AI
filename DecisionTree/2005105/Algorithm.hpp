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

double information_gain(vector<Car> &dataset, string attr, string class_attr)
{
    int total_size = dataset.size();

    map<string, int> class_counts = count_values(dataset, class_attr);
    double total_entropy = 0.0;

    for (auto &[cls, count] : class_counts)
    {
        double p = static_cast<double>(count) / total_size;
        total_entropy -= p * log2(p);
    }

    map<string, int> attr_counts = count_values(dataset, attr);
    double remainder = 0.0;

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
        remainder += p * entropy;
    }

    return total_entropy - remainder;
}

double gini_impurity(vector<Car> &dataset, string attr, string class_attr)
{
    int total_size = dataset.size();
    map<string, int> attr_counts = count_values(dataset, attr);
    double gini = 0.0;

    for (auto &[value, count] : attr_counts)
    {
        double p = static_cast<double>(count) / total_size;
        vector<Car> filtered = filter_dataset(dataset, attr, value);

        map<string, int> filtered_class_counts = count_values(filtered, class_attr);
        double impurity = 1.0;

        for (auto &[cls, fcount] : filtered_class_counts)
        {
            double q = static_cast<double>(fcount) / count;
            impurity -= q * q;
        }
        gini += p * impurity;
    }

    return gini;
}

Node *train_decision_tree(vector<Car> &dataset, vector<string> &attributes, string class_attr,
                          double (*metric)(vector<Car> &, string, string), int k = 1, int min_size = 5, double threshold = 0.01)
{
    if (dataset.empty())
    {
        Node *leaf = new Node();
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

    if (attributes.empty())
    {
        Node *leaf = new Node();
        leaf->setIsLeaf(true);
        string majority_class = max_element(class_counts.begin(), class_counts.end(),
                                            [](auto &a, auto &b)
                                            { return a.second < b.second; })
                                    ->first;
        leaf->setLabel(majority_class);
        return leaf;
    }

    if (dataset.size() < min_size)
    {
        Node *leaf = new Node();
        leaf->setIsLeaf(true);
        string majority_class = max_element(class_counts.begin(), class_counts.end(),
                                            [](auto &a, auto &b)
                                            { return a.second < b.second; })
                                    ->first;
        leaf->setLabel(majority_class);
        return leaf;
    }

    vector<pair<string, double>> scores;
    for (auto &attr : attributes)
    {
        double score = metric(dataset, attr, class_attr);
        scores.push_back({attr, score});
    }

    string best_attribute;
    double best_score;

    if (scores.size() >= k)
    {
        sort(scores.begin(), scores.end(), [](const pair<string, double> &a, const pair<string, double> &b)
             { return a.second < b.second; });

        vector<string> top_k_attributes;
        for (int i = 0; i < k && i < scores.size(); ++i)
        {
            top_k_attributes.push_back(scores[i].first);
        }

        int random_pick = rand() % k;
        best_attribute = top_k_attributes[random_pick];
        best_score = scores[random_pick].second;
    }
    else
    {
        best_attribute = scores[0].first;
        best_score = scores[0].second;
    }

    if (best_score <= threshold)
    {
        Node *leaf = new Node();
        leaf->setIsLeaf(true);
        string majority_class = max_element(class_counts.begin(), class_counts.end(),
                                            [](auto &a, auto &b)
                                            { return a.second < b.second; })
                                    ->first;
        leaf->setLabel(majority_class);
        return leaf;
    }

    Node *tree = new Node();
    tree->setNodeAttribute(best_attribute);

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
        tree->setChild(value, train_decision_tree(filtered, remaining_attributes, class_attr, metric, k));
    }

    return tree;
}

double getAccuracy(vector<Car> &cars, double (*metric)(vector<Car> &, string, string), default_random_engine rng, int k = 1)
{

    double total_accuracy = 0;

    for (int i = 0; i < ITERATIONS; i++) {
        shuffle(cars.begin(), cars.end(), rng);

        size_t train_size = static_cast<size_t>(cars.size() * TRAIN_PERCENTAGE);
        vector<Car> training_set(cars.begin(), cars.begin() + train_size);
        vector<Car> test_set(cars.begin() + train_size, cars.end());

        Node *root = train_decision_tree(training_set, attributes, class_attr, gini_impurity, 3);
        DecisionTree tree(root);

        string predicted, actual;
        int correct = 0;
        for (auto &car : test_set) {
            predicted = tree.getClassification(car);
            actual = car.getCarAttribute(class_attr);
            if (predicted == actual)
                correct++;
        }

        double accuracy = static_cast<double>(correct) / test_set.size();
        total_accuracy += accuracy;
    }

    double mean = total_accuracy / ITERATIONS;
    return mean;
}