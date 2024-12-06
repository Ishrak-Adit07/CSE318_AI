#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <map>
#include <string>

using namespace std;

string buy_attr = "buying";
string maint_attr = "maint";
string door_attr = "doors";
string person_attr = "persons";
string lug_attr = "lug_boot";
string safety_attr = "safety";
string class_attr = "class";

vector<string> buy_attr_values = {"vhigh", "high", "med", "low"};
vector<string> maint_attr_values = {"vhigh", "high", "med", "low"};
vector<string> door_attr_values = {"2", "3", "4", "5more"};
vector<string> person_attr_values = {"2", "4", "more"};
vector<string> lug_attr_values = {"small", "med", "big"};
vector<string> safety_attr_values = {"low", "med", "high"};
vector<string> class_attr_values = {"unacc", "acc", "good", "vgood"};

// Returns a vector of all possible values for a given attribute
vector<string> get_attribute_values(const string attr)
{
    if (attr == buy_attr)
        return buy_attr_values;
    else if (attr == maint_attr)
        return maint_attr_values;
    else if (attr == door_attr)
        return door_attr_values;
    else if (attr == person_attr)
        return person_attr_values;
    else if (attr == lug_attr)
        return lug_attr_values;
    else if (attr == safety_attr)
        return safety_attr_values;
    else
        assert(false);
}

// Assert that the attribute string is one of the specified attribute names
bool attribute_validity(const string &attr)
{
    return (attr == buy_attr || attr == maint_attr || attr == door_attr || attr == person_attr || attr == lug_attr ||
            attr == safety_attr || attr == class_attr);
}

// Assert that the attribute value is one of the specified attribute values
bool attribute_value_validity(const string &attr, const string &&attr_value)
{
    if (!attribute_validity(attr))
        return false;
    vector<string> value_vector = get_attribute_values(attr);

    for (auto value : value_vector)
        if (value == attr_value)
            return true;
    return false;
}