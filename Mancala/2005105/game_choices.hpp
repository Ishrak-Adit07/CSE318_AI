#include "game_algo.hpp"

int computer_vs_computer_simulation(int depth, int heuristic1, int heuristic2)
{
    Board board;

    while (!board.is_game_over())
    {
        // Player 1's turn
        int player1_move = get_best_move(board, depth, heuristic1, PLAYER_1);
        board.make_move(player1_move, PLAYER_1);

        if (board.is_game_over())
            break;

        // Player 2's turn
        int player2_move = get_best_move(board, depth, heuristic2, PLAYER_2);
        board.make_move(player2_move, PLAYER_2);
    }

    board.collect_remaining_stones();

    int game_winner = board.get_winner();
    return game_winner;
}

// Function for a human vs. computer game
int human_vs_computer_game()
{
    Board board;
    int depth = (rand() % 6) + 1;
    int heuristic = (rand() % 4) + 1;
    int current_player = PLAYER_1;

    while (!board.is_game_over())
    {
        board.display_board();

        if (current_player == PLAYER_1)
        {
            int human_move;
            cout << "Your turn! Choose a bin (0 to " << NUMBER_OF_BINS - 1 << "): ";
            cin >> human_move;

            while (human_move < 0 || human_move >= NUMBER_OF_BINS || board.bins[human_move] == 0)
            {
                cout << "Invalid move! Try again: ";
                cin >> human_move;
            }

            auto move_metrics = board.make_move(human_move, PLAYER_1);
            current_player = (move_metrics.first) ? PLAYER_1 : PLAYER_2;
        }
        else
        {
            // Computer player's turn
            cout << "Computer's turn!" << endl;
            int best_move = get_best_move(board, depth, heuristic, PLAYER_2);
            board.make_move(best_move, PLAYER_2);
            current_player = PLAYER_1;
        }
    }

    board.collect_remaining_stones();
    board.display_board();

    int winner = board.get_winner();
    if (winner == PLAYER_1)
        cout << "You won!" << endl;
    else if (winner == PLAYER_2)
        cout << "Computer won!" << endl;
    else
        cout << "It's a tie!" << endl;

    return winner;
}

// Function to update heuristic win statistics
void update_heuristic_wins(int heuristic1, int heuristic2, bool player1_won,
                           Heuristic_Pair &H12, Heuristic_Pair &H13, Heuristic_Pair &H14,
                           Heuristic_Pair &H23, Heuristic_Pair &H24, Heuristic_Pair &H34)
{
    auto update_wins = [&](Heuristic_Pair &pair, bool update_heuristic_1)
    {
        if (update_heuristic_1)
            pair.set_heuristic_1_win(pair.get_heuristic_1_win() + 1);
        else
            pair.set_heuristic_2_win(pair.get_heuristic_2_win() + 1);
    };

    if ((heuristic1 == 1 && heuristic2 == 2) || (heuristic1 == 2 && heuristic2 == 1))
        update_wins(H12, player1_won ? heuristic1 == 1 : heuristic2 == 1);
    else if ((heuristic1 == 1 && heuristic2 == 3) || (heuristic1 == 3 && heuristic2 == 1))
        update_wins(H13, player1_won ? heuristic1 == 1 : heuristic2 == 1);
    else if ((heuristic1 == 1 && heuristic2 == 4) || (heuristic1 == 4 && heuristic2 == 1))
        update_wins(H14, player1_won ? heuristic1 == 1 : heuristic2 == 1);
    else if ((heuristic1 == 2 && heuristic2 == 3) || (heuristic1 == 3 && heuristic2 == 2))
        update_wins(H23, player1_won ? heuristic1 == 2 : heuristic2 == 2);
    else if ((heuristic1 == 2 && heuristic2 == 4) || (heuristic1 == 4 && heuristic2 == 2))
        update_wins(H24, player1_won ? heuristic1 == 2 : heuristic2 == 2);
    else if ((heuristic1 == 3 && heuristic2 == 4) || (heuristic1 == 4 && heuristic2 == 3))
        update_wins(H34, player1_won ? heuristic1 == 3 : heuristic2 == 3);
}

// Run a hundred game simulations for analysis
void run_hundred_game_simulation()
{
    int player1_wins = 0;
    int player2_wins = 0;
    int draws = 0;
    const int total_simulations = 100;

    Heuristic_Pair H12(1, 2);
    Heuristic_Pair H13(1, 3);
    Heuristic_Pair H14(1, 4);
    Heuristic_Pair H23(2, 3);
    Heuristic_Pair H24(2, 4);
    Heuristic_Pair H34(3, 4);

    ofstream report_file("game_report.txt");
    for (int i = 0; i < total_simulations; i++)
    {
        int depth = (rand() % 6) + 1;
        int heuristic1 = (rand() % 4) + 1;
        int heuristic2 = (rand() % 4) + 1;

        report_file << "depth: " << depth << ", heuristic 1: " << heuristic1 << ", heuristic 2: " << heuristic2;

        int result = computer_vs_computer_simulation(depth, heuristic1, heuristic2);
        if (result == PLAYER_1)
        {
            player1_wins++;
            report_file << ", winner: player 1" << endl;
            report_file << "winner heuristic: " << heuristic1 << ", loser heuristic: " << heuristic2 << endl << endl;
            update_heuristic_wins(heuristic1, heuristic2, true, H12, H13, H14, H23, H24, H34);
        }
        else if (result == PLAYER_2)
        {
            player2_wins++;
            report_file << ", winner: player 2" << endl;
            report_file << "winner heuristic: " << heuristic2 << ", loser heuristic: " << heuristic1 << endl << endl;
            update_heuristic_wins(heuristic1, heuristic2, false, H12, H13, H14, H23, H24, H34);
        }
        else
        {
            draws++;
            report_file << ", match drawn" << endl << endl;
        }
    }

    double player1_win_percentage = (static_cast<double>(player1_wins) / total_simulations) * 100;
    double player2_win_percentage = (static_cast<double>(player2_wins) / total_simulations) * 100;
    double draw_percentage = (static_cast<double>(draws) / total_simulations) * 100;

    report_file << "Heuristic 1 vs. Heuristic 2 : " << H12.get_heuristic_1_win() << " - " << H12.get_heuristic_2_win() << endl;
    report_file << "Heuristic 1 vs. Heuristic 3 : " << H13.get_heuristic_1_win() << " - " << H13.get_heuristic_2_win() << endl;
    report_file << "Heuristic 1 vs. Heuristic 4 : " << H14.get_heuristic_1_win() << " - " << H14.get_heuristic_2_win() << endl;
    report_file << "Heuristic 2 vs. Heuristic 3 : " << H23.get_heuristic_1_win() << " - " << H23.get_heuristic_2_win() << endl;
    report_file << "Heuristic 2 vs. Heuristic 4 : " << H24.get_heuristic_1_win() << " - " << H24.get_heuristic_2_win() << endl;
    report_file << "Heuristic 3 vs. Heuristic 4 : " << H34.get_heuristic_1_win() << " - " << H34.get_heuristic_2_win() << endl;

    report_file << endl;

    report_file << "Player 1 win percentage: " << player1_win_percentage << "%" << endl;
    report_file << "Player 2 win percentage: " << player2_win_percentage << "%" << endl;
    report_file << "Draw percentage: " << draw_percentage << "%" << endl;

    report_file.close();
}