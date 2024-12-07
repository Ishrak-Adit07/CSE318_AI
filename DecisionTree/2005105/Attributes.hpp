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
vector<string> attributes = {buy_attr, maint_attr, door_attr, person_attr, lug_attr, safety_attr};

vector<string> buy_attr_values = {"vhigh", "high", "med", "low"};
vector<string> maint_attr_values = {"vhigh", "high", "med", "low"};
vector<string> door_attr_values = {"2", "3", "4", "5more"};
vector<string> person_attr_values = {"2", "4", "more"};
vector<string> lug_attr_values = {"small", "med", "big"};
vector<string> safety_attr_values = {"low", "med", "high"};
vector<string> class_attr_values = {"unacc", "acc", "good", "vgood"};

void print_accuracy_table(double best_ig, double best_gini, double top_3_ig, double top_3_gini){
 
    cout << "====================================================================================================" << endl;
    cout << "|                                                  |        Average Accuracy over 20 runs          |" << endl;
    cout << "====================================================================================================" << endl;
    cout << "|         Attribute Selection Strategy             |     Information Gain   |     Gini impurity    |" << endl;
    cout << "====================================================================================================" << endl;
    cout << "|       Always select the best attribute           |         "<<best_ig<<"        |       "<<best_gini<<"        |" << endl;
    cout << "====================================================================================================" << endl;
    cout << "|    Select one randomly from top 3 attributes     |         "<<top_3_ig<<"        |        "<<top_3_gini<<"       |" << endl;
    cout << "====================================================================================================" << endl;

}