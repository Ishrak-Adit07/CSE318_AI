#include "game_choices.hpp"

int main()
{
    // Rand function for randomly setting game depth and heuristic choice
    srand(time(0));

    // Making game choice and calling specific function
    int game_choice;

    while (true)
    {
        cout << "Choose Game Mode:" << endl;
        cout << "1. Play vs. Computer" << endl;
        cout << "2. Run hundred-game simulation" << endl;
        cout << "3. Exit" << endl;

        cin >> game_choice;
        if (game_choice == 1)
            human_vs_computer_game();
        else if (game_choice == 2)
            run_hundred_game_simulation();
        else if (game_choice == 3)
            break;
        else
            cout << "Invalid choice" << endl;
    }

    return 0;
}