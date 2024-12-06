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

// Function 3: Train a Decision Tree
Node *train_decision_tree(vector<Car> &dataset, vector<string> &attributes, string class_attr,
                          double (*metric)(vector<Car> &, string, string), int k = 1, int min_size = 5, double threshold = 0.01)
{
    if (dataset.empty())
    {
        Node *leaf = new Node();
        return leaf;
    }

    // Check if all examples belong to the same class
    map<string, int> class_counts = count_values(dataset, class_attr);
    if (class_counts.size() == 1)
    {
        Node *leaf = new Node();
        leaf->setIsLeaf(true);
        leaf->setLabel(class_counts.begin()->first);
        return leaf;
    }

    // If no attributes are left, return the majority class as a leaf
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

    // Checking if the dataset is too small to split
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

    // Find the top k attributes based on the chosen metric
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

        // Sort the attributes based on their score
        sort(scores.begin(), scores.end(), [](const pair<string, double> &a, const pair<string, double> &b)
             { return a.second < b.second; });

        // Select the top k attributes (if k is less than the number of attributes)
        vector<string> top_k_attributes;
        for (int i = 0; i < k && i < scores.size(); ++i)
        {
            top_k_attributes.push_back(scores[i].first);
        }

        // Randomly choose one of the top k attributes
        // random_device rd;
        // mt19937 gen(rd());
        // uniform_int_distribution<> dis(0, top_k_attributes.size() - 1);
        int random_pick = rand() % k;
        best_attribute = top_k_attributes[random_pick];
        best_score = scores[random_pick].second;
    }
    else
    {
        best_attribute = scores[0].first;
        best_score = scores[0].second;
    }

    // Check if the score is below a threshold
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

    // Create a new node for the best attribute
    Node *tree = new Node();
    tree->setNodeAttribute(best_attribute);

    // Get the unique values of the best attribute
    map<string, int> attr_values = count_values(dataset, best_attribute);

    // Recursively train the subtree for each attribute value
    for (auto &[value, _] : attr_values)
    {
        vector<Car> filtered = filter_dataset(dataset, best_attribute, value);

        // Remove the best attribute from the list of attributes
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

    // Perform multiple iterations of training and testing
    for (int i = 0; i < ITERATIONS; i++) {
        //cout << "Iteration : " << i << endl;
        // Shuffle the dataset for randomness
        // random_shuffle(cars.begin(), cars.end());
        shuffle(cars.begin(), cars.end(), rng);

        // Split the dataset into training and testing sets
        size_t train_size = static_cast<size_t>(cars.size() * TRAIN_PERCENTAGE);
        vector<Car> training_set(cars.begin(), cars.begin() + train_size);
        vector<Car> test_set(cars.begin() + train_size, cars.end());

        // Train the decision tree using information gain
        Node *root = train_decision_tree(training_set, attributes, class_attr, gini_impurity, 3);
        Tree tree(root);

        // Evaluate the decision tree on the test set
        string predicted, actual;
        int correct = 0;

        for (auto &car : test_set) {
            //cout << car.getCarAttribute(class_attr) << endl;
            predicted = tree.getClassification(car);
            actual = car.getCarAttribute(class_attr);
            if (predicted == actual)
                correct++;
            //cout << predicted << "  " << actual << endl;
        }

        // Calculate accuracy for this iteration
        double accuracy = static_cast<double>(correct) / test_set.size();
        //cout << "Iteration " << i + 1 << " Accuracy:\t" << accuracy << endl;

        total_accuracy += accuracy;
    }

    // Calculate mean accuracy and standard deviation
    double mean = total_accuracy / ITERATIONS;
    //cout << "Mean Accuracy: " << mean << endl;

    return mean;
}