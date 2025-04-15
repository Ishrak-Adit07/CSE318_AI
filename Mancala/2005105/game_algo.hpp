#include "game_board.hpp"

// Minimax algorithm with alpha-beta pruning
int minimax(Board &board, int depth, int alpha, int beta, int player, int heuristic)
{
    // Base case: if the game is over or max depth is reached
    if (depth <= 0 || board.is_game_over())
        return board.evaluate(player, heuristic, 0, 0);

    // Maximizing player
    if (player == PLAYER_1)
    {
        int max_value = NEG_INF;

        for (int i = 0; i < NUMBER_OF_BINS; ++i)
        {
            if (board.bins[i] == 0)
                continue;

            Board temp_board = board;
            auto move_metrics = temp_board.make_move(i, PLAYER_1);

            // If the player gets another turn
            if (move_metrics.first)
                max_value = max(max_value, minimax(temp_board, depth, alpha, beta, PLAYER_1, heuristic));
            else
                max_value = max(max_value, minimax(temp_board, depth - 1, alpha, beta, PLAYER_2, heuristic));

            // Updating alpha for maximizing player
            alpha = max(alpha, max_value);
            if (beta <= alpha)
                break;  // Alpha-beta pruning
        }
        return max_value;
    }
    // Minimizing player
    else
    {
        int min_value = INF;

        for (int i = 0; i < NUMBER_OF_BINS; ++i)
        {
            if (board.bins[NUMBER_OF_BINS + i + 1] == 0)
                continue;

            Board temp_board = board;
            auto move_metrics = temp_board.make_move(NUMBER_OF_BINS + i + 1, PLAYER_2);

            // If the player gets another turn
            if (move_metrics.first)
                min_value = min(min_value, minimax(temp_board, depth, alpha, beta, PLAYER_2, heuristic));
            else
                min_value = min(min_value, minimax(temp_board, depth - 1, alpha, beta, PLAYER_1, heuristic));

            // Updating beta for minimizing player
            beta = min(beta, min_value);

            if (beta <= alpha)
                break;  // Alpha-beta pruning
        }
        return min_value;
    }
}

// Function to get the best move for the current player
int get_best_move(Board &board, int depth, int heuristic, int player)
{
    int best_bin_index = -1;
    int best_bin_value = (player == PLAYER_1) ? NEG_INF : INF;
    int opponent = (player == PLAYER_1) ? PLAYER_2 : PLAYER_1;

    for (int i = 0; i < NUMBER_OF_BINS; ++i)
    {
        int bin_index = (player == PLAYER_1) ? i : (NUMBER_OF_BINS + i + 1);

        if (board.bins[bin_index] == 0)
            continue;

        Board temp_board = board;
        auto [extra_turn, captured] = temp_board.make_move(bin_index, player);

        int value_of_move = minimax(temp_board, extra_turn ? depth : depth - 1, NEG_INF, INF, extra_turn ? player : opponent, heuristic);

        bool is_better_move = (player == PLAYER_1) ? (value_of_move > best_bin_value) : (value_of_move < best_bin_value);
        if (is_better_move)
        {
            best_bin_value = value_of_move;
            best_bin_index = bin_index;
        }
    }

    return best_bin_index;
}