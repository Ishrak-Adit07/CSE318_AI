#include "DecisionTree.hpp"
#include <math.h>

// Given a set of car_dataset and a particular attribute (possibly the classificaion as well), this
// will return a map mapping each value of that attribue to the number of occurences they are found.
map<string, int> count_specific_attribute(const vector<Car> &car_dataset, const string &attr)
{
    map<string, int> count_map;
    for (auto car_data : car_dataset)
        count_map[car_data.getCarAttribute(attr)]++;

    return count_map;
}

// This function will return the plurality value of an car_data set (along
// with the number of occurences for that value). A plurality value is the
// maximally occuring classificaion for a certain set of car_dataset.
pair<string, int> get_plurality_value(const vector<Car> &car_dataset)
{
    map<string, int> count_map = count_specific_attribute(car_dataset, class_attr);
    pair<string, int> ret = {"", -1};
    for (auto counted_attr : map)
        if (counted_attr.second > ret.second)
            ret = counted_attr;
    return ret;
}

// Returns a filtered vector of only those car_dataset who have attr_value for a certain attribute attr
vector<Car> filter_examples(const vector<Car> &car_dataset, const string attr, const string attr_value)
{
    vector<Car> filtered_dataset;
    for (auto car_data : car_dataset)
        if (car_data.getCarAttribute(attr) == attr_value)
            filtered_dataset.push_back(car_data);
    return filtered_dataset;
}

// Returns the information gain for a certain attribute attr
double get_remainder(const string &attr, const vector<Car> &car_dataset)
{
    const int dataset_size = car_dataset.size();

    map<string, int> count_map = count_specific_attribute(car_dataset, attr);
    double remainder = 0.0;
    for (auto counted_attr : count_map)
    {
        // counted_attr.first is an attribute value for attribute attr.
        // counted_attr.second is the number of occurences of that attribute value in car_dataset
        if (counted_attr.second == 0)
            continue;
        double p = (double)counted_attr.second / dataset_size;

        // filtered_examples is a vector of car_dataset that have attr_value for attribute attr
        vector<Car> filtered_examples = filter_examples(car_dataset, attr, counted_attr.first);
        map<string, int> filtered_map = count_specific_attribute(filtered_examples, class_attr);
        double entropy = 0.0;
        for (auto filtered_they : filtered_map)
        {
            double q = (double)filtered_they.second / counted_attr.second;
            if (q == 0)
                continue;
            entropy -= q * log2(q);
        }
        remainder += p * entropy;
    }

    return remainder;
}

Node *learn_decision_tree(const vector<Car> &car_dataset, const vector<string> &getCarAttributes(),
                          const vector<Car> parent_examples)
{
    if (car_dataset.empty())
    {
        Node *tree = new Node(true, get_plurality_value(parent_examples).first);
        return tree;
    }
    pair<string, int> pv = get_plurality_value(car_dataset);
    if (pv.second == car_dataset.size() || getCarAttributes().empty())
    {
        // all car_dataset have the same classification
        // or no getCarAttributes() left to check
        Node *tree = new Node(true, pv.first);
        return tree;
    }

    // find the attribute with the highest importance (min_remainder)
    double min_remainder = 1e9;
    string max_important_attr;
    for (auto attr : getCarAttributes())
    {
        double rem = get_remainder(attr, car_dataset);
        if (rem < min_remainder)
        {
            min_remainder = rem;
            max_important_attr = attr;
        }
    }

    // now create a new decision tree with root as max_important_attr
    Node *tree = new Node();
    tree->attribute_to_check = max_important_attr;
    vector<string> new_attributes;
    for (auto attr : getCarAttributes())
        if (attr != max_important_attr)
            new_attributes.push_back(attr);

    vector<string> attr_values = get_attribute_values(max_important_attr);
    for (auto attr_value : attr_values)
    {
        vector<Car> filtered_examples = filter_examples(car_dataset, max_important_attr, attr_value);
        Node *subtree = learn_decision_tree(filtered_examples, new_attributes, car_dataset);
        tree->child[attr_value] = subtree;
    }
    return tree;
}