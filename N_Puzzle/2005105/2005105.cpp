#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <cmath>
#include <fstream>

using namespace std;

vector<vector<int>> goal_state = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}};

// Function to check if the current state is the goal state
bool isGoalState(vector<vector<int>> &state)
{
    return state == goal_state;
}

// Possible moves in the puzzle for the blank tile
// Left, Right, Down, Up
int row_move[] = {-1, 1, 0, 0};
int col_move[] = {0, 0, -1, 1};

pair<int, int> findBlankTilePosition(const vector<vector<int>> &puzzle)
{
    int n = puzzle.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (puzzle[i][j] == 0)
            {
                return {i, j};
            }
        }
    }

    // No blank tile (invalid puzzle)
    return {-1, -1};
}

// Function to count inversions in the puzzle
int countInversions(const vector<int> &flat_puzzle)
{
    int inv_count = 0;
    int n = flat_puzzle.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (flat_puzzle[i] > flat_puzzle[j])
            {
                inv_count++;
            }
        }
    }
    return inv_count;
}

// Function to flatten the puzzle (convert 2D vector to 1D vector)
vector<int> flattenPuzzle(const vector<vector<int>> &puzzle)
{
    vector<int> flat_puzzle;
    for (const auto &row : puzzle)
    {
        for (const auto &tile : row)
        {
            if (tile != 0)
                flat_puzzle.push_back(tile);
        }
    }
    return flat_puzzle;
}

// Function to check if the puzzle is solvable
bool isPuzzleSolvable(const vector<vector<int>> &puzzle, int blank_x)
{
    int n = puzzle.size();
    vector<int> flat_puzzle = flattenPuzzle(puzzle);

    // Count the number of inversions
    int inv_count = countInversions(flat_puzzle);

    // If grid size is odd, return true if inversion count is even
    if (n % 2 != 0)
    {
        return (inv_count % 2 == 0);
    }
    else
    {
        // If grid size is even, find the row of the blank (0) tile from the bottom
        int blank_row = n - blank_x;

        // Check solvability for even grid size
        if ((blank_row + inv_count) % 2 != 0)
            return true;
        else
            return false;
    }
}

// Structure to represent a node in the search tree
struct Node
{
    vector<vector<int>> puzzle;
    int blank_x, blank_y;
    int cost;
    int heuristic;
    Node *parent;

    Node(vector<vector<int>> Puzzle, int X, int Y, int Cost, int Heuristic, Node *Parent = nullptr)
    {
        puzzle = Puzzle;
        blank_x = X;
        blank_y = Y;
        cost = Cost;
        heuristic = Heuristic;
        parent = Parent;
    }

    // Overloading the less than operator for priority queue
    bool operator<(const Node &other) const
    {
        return (cost + heuristic) > (other.cost + other.heuristic);
    }
};

// Function to calculate the Hamming distance heuristic
int calculateHammingDistance(const vector<vector<int>> &state)
{
    int distance = 0;
    int n = state.size();

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (state[i][j] != 0)
                if (state[i][j] != (i * n + j + 1))
                    distance++;

    return distance;
}

// Function to calculate the Manhattan distance heuristic
int calculateManhattanDistance(vector<vector<int>> &state)
{
    int distance = 0;
    int n = state.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (state[i][j] != 0)
            {
                int targetX = (state[i][j] - 1) / n;
                int targetY = (state[i][j] - 1) % n;
                distance += abs(i - targetX) + abs(j - targetY);
            }
        }
    }
    return distance;
}

// Function to print the puzzle
void printPuzzle(vector<vector<int>> &puzzle, ofstream &output_file)
{
    for (auto row : puzzle)
    {
        for (auto tile : row)
        {
            if (tile == 0)
                output_file << "*" << " ";
            else
                output_file << tile << " ";
        }
        output_file << endl;
    }
}

// Function to trace back the solution path
void printExpandedNodes(Node *node, ofstream &output_file)
{
    if (node == nullptr)
        return;
    printExpandedNodes(node->parent, output_file);
    printPuzzle(node->puzzle, output_file);
    output_file << endl;
}

void solve_n_Puzzle_extract(vector<vector<int>> &start_state, int n, pair<int, int> blank_tile_position, char method, ofstream &output_file)
{

    // Necessary structures for the A* algorithm
    priority_queue<Node> pq;
    map<vector<vector<int>>, bool> visited;

    int number_of_expanded_nodes = 0;
    int number_of_explored_nodes = 0;

    // Initiating algorithm
    int initial_heuristic = 0;
    if (method == 'H')
        initial_heuristic = calculateHammingDistance(start_state);
    else if (method == 'M')
        initial_heuristic = calculateManhattanDistance(start_state);

    Node *start_node = new Node(start_state, blank_tile_position.first, blank_tile_position.second, 0, initial_heuristic);
    pq.push(*start_node);

    while (!pq.empty())
    {
        Node current = pq.top();
        pq.pop();
        number_of_expanded_nodes++;

        // Checking if we have reached the goal state
        if (isGoalState(current.puzzle))
        {
            output_file << "Solution found in " << current.cost << " moves:\n";
            printExpandedNodes(&current, output_file);

            output_file << "Number of expanded nodes " << number_of_expanded_nodes << endl;
            output_file << "Number of explored nodes " << number_of_explored_nodes << endl;
            return;
        }

        // Marking the current state as visited
        visited[current.puzzle] = true;

        // Generating all possible next moves
        // Maximum 4 moves possible for the blank tile
        for (int i = 0; i < 4; i++)
        {
            int newX = current.blank_x + row_move[i];
            int newY = current.blank_y + col_move[i];

            // Checking if the move is within bounds
            if (newX >= 0 && newX < n && newY >= 0 && newY < n)
            {
                vector<vector<int>> new_puzzle = current.puzzle;
                // Blank tile move operation \/
                swap(new_puzzle[current.blank_x][current.blank_y], new_puzzle[newX][newY]);

                // Skipping if this state has been visited
                if (visited.find(new_puzzle) != visited.end())
                    continue;

                int newHeuristic = 0;
                if (method == 'H')
                    newHeuristic = calculateHammingDistance(new_puzzle);
                else if (method == 'M')
                    newHeuristic = calculateManhattanDistance(new_puzzle);

                Node *new_node = new Node(new_puzzle, newX, newY, current.cost + 1, newHeuristic, new Node(current));
                pq.push(*new_node);
                number_of_explored_nodes++;
            }
        }
    }
}

// Function to solve the n-puzzle problem using A* algorithm
void solve_n_Puzzle(vector<vector<int>> &start_state)
{

    // State size and position of the blank tile in start state
    int n = start_state.size();
    pair<int, int> blank_tile_position = findBlankTilePosition(start_state);

    // Output file
    ofstream output_file("output.txt");
    if (!output_file)
        cerr << "Error opening output file" << endl;

    // Checking if puzzle is solvable
    if (!isPuzzleSolvable(start_state, blank_tile_position.first))
    {
        output_file << "No solution possible" << endl;
        return;
    }
    else
    {
        output_file << "The puzzle is solvable" << endl;
    }

    output_file << endl;

    output_file << "Using Hamming Distance - " << endl;
    solve_n_Puzzle_extract(start_state, n, blank_tile_position, 'H', output_file);

    output_file << endl;

    output_file << "Using Manhattan Distance - " << endl;
    solve_n_Puzzle_extract(start_state, n, blank_tile_position, 'M', output_file);
}

int main()
{

    // File input stream
    ifstream input_file("input.txt");
    if (!input_file)
    {
        cerr << "Input file not available" << endl;
        return -1;
    }

    // Reading start_state size
    int start_state_size;
    input_file >> start_state_size;

    // Reading start_state
    vector<vector<int>> start_state(start_state_size, vector<int>(start_state_size));
    string element;
    for (int i = 0; i < start_state_size; i++)
    {
        for (int j = 0; j < start_state_size; j++)
        {
            input_file >> element;
            if (element == "*")
                start_state[i][j] = 0;
            else
                start_state[i][j] = stoi(element);
        }
    }

    input_file.close();

    solve_n_Puzzle(start_state);
    return 0;
}