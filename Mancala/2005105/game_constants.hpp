#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <numeric>

using namespace std;

const int INF = numeric_limits<int>::max();
const int NEG_INF = numeric_limits<int>::min();

const int PLAYER_1 = 0;
const int PLAYER_2 = 1;

const int NUMBER_OF_BINS = 6;
const int TOTAL_BINS = NUMBER_OF_BINS + NUMBER_OF_BINS + 2;

const int PLAYER_1_STORAGE = NUMBER_OF_BINS;
const int PLAYER_2_STORAGE = TOTAL_BINS - 1;