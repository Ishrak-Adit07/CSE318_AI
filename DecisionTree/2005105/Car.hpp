#include "Attributes.hpp"

class Car
{
    map<string, string> attributes;

public:
    Car(const string &str)
    {
        stringstream ss(str);
        vector<string> attribute_values;
        while (ss.good())
        {
            string substr;
            getline(ss, substr, ',');
            attribute_values.push_back(substr);
        }

        assert(attribute_values.size() == 7);
        attributes[buy_attr] = attribute_values[0];
        attributes[maint_attr] = attribute_values[1];
        attributes[door_attr] = attribute_values[2];
        attributes[person_attr] = attribute_values[3];
        attributes[lug_attr] = attribute_values[4];
        attributes[safety_attr] = attribute_values[5];
        attributes[class_attr] = attribute_values[6];
    }

    map<string, string> getCarAttributes() { return attributes; }
    string getCarAttribute(const string &attr) { return attributes[attr]; }
};