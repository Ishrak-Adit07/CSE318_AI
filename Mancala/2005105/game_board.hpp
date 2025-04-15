#include "game_constants.hpp"

struct Board
{
    vector<int> bins;

    Board() : bins(TOTAL_BINS, 4)
    {
        bins[PLAYER_1_STORAGE] = bins[PLAYER_2_STORAGE] = 0;
    }

    void print(ostream &output)
    {
        output << "Board state\n";
        for (int i = TOTAL_BINS - 2; i > NUMBER_OF_BINS; --i)
            output << bins[i] << " ";
        output << "\n"
               << bins[PLAYER_2_STORAGE] << "        " << bins[PLAYER_1_STORAGE] << "\n";
        for (int i = 0; i < NUMBER_OF_BINS; ++i)
            output << bins[i] << " ";
        output << "\n\n";
    }

    void print_board(ofstream &output_file) { print(output_file); } // output file printing
    void display_board() { print(cout); }                           // console display

    bool is_game_over()
    {
        return all_of(bins.begin(), bins.begin() + NUMBER_OF_BINS, [](int s)
                      { return s == 0; }) ||
               all_of(bins.begin() + NUMBER_OF_BINS + 1, bins.end() - 1, [](int s)
                      { return s == 0; });
    }

    int get_winner()
    {
        if (bins[PLAYER_1_STORAGE] != bins[PLAYER_2_STORAGE])
            return (bins[PLAYER_1_STORAGE] > bins[PLAYER_2_STORAGE]) ? PLAYER_1 : PLAYER_2;
        return -1; // Draw
    }

    void collect_remaining_stones()
    {
        for (int i = 0; i < NUMBER_OF_BINS; ++i)
            bins[PLAYER_1_STORAGE] += exchange(bins[i], 0);
        for (int i = NUMBER_OF_BINS + 1; i < TOTAL_BINS - 1; ++i)
            bins[PLAYER_2_STORAGE] += exchange(bins[i], 0);
    }

    pair<bool, int> make_move(int bin_index, int player)
    {
        if (bin_index < 0 || bin_index >= TOTAL_BINS || bins[bin_index] == 0)
            return {false, 0};

        int storage_index = (player == PLAYER_1) ? PLAYER_1_STORAGE : PLAYER_2_STORAGE;
        int opponent_storage = (player == PLAYER_1) ? PLAYER_2_STORAGE : PLAYER_1_STORAGE;

        int stones_in_bin_index = exchange(bins[bin_index], 0);
        int current_index = bin_index;
        int stones_captured = 0;

        while (stones_in_bin_index--)
        {
            current_index = (current_index + 1) % TOTAL_BINS;
            if (current_index == opponent_storage)
                continue;
            bins[current_index]++;
        }

        if ((player == PLAYER_1 && current_index < NUMBER_OF_BINS && bins[current_index] == 1) ||
            (player == PLAYER_2 && current_index > NUMBER_OF_BINS && current_index < TOTAL_BINS - 1 && bins[current_index] == 1))
        {
            int capture_index = TOTAL_BINS - 2 - current_index;
            stones_captured = bins[capture_index];
            bins[storage_index] += stones_captured + 1;
            bins[current_index] = bins[capture_index] = 0;
        }

        return {current_index == storage_index, stones_captured};
    }

    int evaluate(int player, int heuristic_type, int another_turn, int stones_captured)
    {
        int player_storage = (player == PLAYER_1) ? PLAYER_1_STORAGE : PLAYER_2_STORAGE;
        int opponent_storage = (player == PLAYER_1) ? PLAYER_2_STORAGE : PLAYER_1_STORAGE;
        int score = bins[player_storage] - bins[opponent_storage];

        int stones_on_my_side = accumulate(bins.begin(), bins.begin() + NUMBER_OF_BINS, 0);
        int stones_on_opponent_side = accumulate(bins.begin() + NUMBER_OF_BINS + 1, bins.end() - 1, 0);

        int bias[] = {rand() % 20 + 1, rand() % 30 + 1, rand() % 40 + 1, rand() % 30 + 1};

        if (heuristic_type >= 2)
            score = score * bias[0] + (stones_on_my_side - stones_on_opponent_side) * bias[1];
        if (heuristic_type >= 3)
            score += another_turn * bias[2];
        if (heuristic_type == 4)
            score += stones_captured * bias[3];

        return score;
    }
};

struct Heuristic_Pair
{

    int heuristic1, heuristic2;
    int heuristic1_win, heuristic2_win;

    Heuristic_Pair(int h1, int h2)
    {
        heuristic1 = h1;
        heuristic2 = h2;
        heuristic1_win = 0;
        heuristic2_win = 0;
    }

    void set_heuristic_1_win(int w) { heuristic1_win = w; }
    void set_heuristic_2_win(int w) { heuristic2_win = w; }

    int get_heuristic_1() { return heuristic1; }
    int get_heuristic_2() { return heuristic2; }
    int get_heuristic_1_win() { return heuristic1_win; }
    int get_heuristic_2_win() { return heuristic2_win; }
};