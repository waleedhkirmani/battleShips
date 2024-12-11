////#include <iostream>
////#include <cstdlib>
////#include <ctime>
////using namespace std;
////
////const int GRID_SIZE = 10;
////const int NUM_SHIPS = 5;
////const int SHIP_SIZES[NUM_SHIPS] = { 5, 4, 3, 3, 2 }; // Carrier, Battleship, Cruiser, Submarine, Destroyer
////const int SHIP_VALUES[NUM_SHIPS] = { 1, 2, 3, 4, 5 };
////void initializeGrid(int grid[GRID_SIZE][GRID_SIZE]) {
////    for (int i = 0; i < GRID_SIZE; ++i) {
////        for (int j = 0; j < GRID_SIZE; ++j) {
////            grid[i][j] = 0;
////        }
////    }
////}
////
////bool canPlaceShip(int grid[GRID_SIZE][GRID_SIZE], int x, int y, int size, bool horizontal) {
////    if (horizontal) {
////        if (y + size > GRID_SIZE) return false;
////        for (int i = 0; i < size; ++i) {
////            if (grid[x][y + i] != 0) return false;
////        }
////    }
////    else {
////        if (x + size > GRID_SIZE) return false;
////        for (int i = 0; i < size; ++i) {
////            if (grid[x + i][y] != 0) return false;
////        }
////    }
////    return true;
////}
////
////void placeShip(int grid[GRID_SIZE][GRID_SIZE], int x, int y, int size, bool horizontal, int arrayAccess) {
////    if (horizontal) {
////        for (int i = 0; i < size; ++i) {
////            grid[x][y + i] = SHIP_VALUES[arrayAccess];
////        }
////    }
////    else {
////        for (int i = 0; i < size; ++i) {
////            grid[x + i][y] = SHIP_VALUES[arrayAccess];
////        }
////    }
////}
////
////void generateAIBoard(int grid[GRID_SIZE][GRID_SIZE]) {
////    srand(static_cast<unsigned>(time(0)));
////    for (int i = 0; i < NUM_SHIPS; ++i) {
////        int size = SHIP_SIZES[i];
////        bool placed = false;
////        while (!placed) {
////            int x = rand() % GRID_SIZE;
////            int y = rand() % GRID_SIZE;
////            bool horizontal = rand() % 2 == 0;
////            if (canPlaceShip(grid, x, y, size, horizontal)) {
////                placeShip(grid, x, y, size, horizontal, i);
////                placed = true;
////            }
////        }
////    }
////}
////
////void printGrid(int grid[GRID_SIZE][GRID_SIZE]) {
////    for (int i = 0; i < GRID_SIZE; ++i) {
////        for (int j = 0; j < GRID_SIZE; ++j) {
////            cout << grid[i][j] << " ";
////        }
////        cout << endl;
////    }
////}
////
////int main() {
////    int grid[GRID_SIZE][GRID_SIZE];
////    initializeGrid(grid);
////    generateAIBoard(grid);
////    printGrid(grid);
////
////    return 0;
////}
//
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <string>
//#include <tuple>
//#include <algorithm>
//
//using namespace std;
//
//int main() {
//    ifstream file("leaderboardFile.txt");
//    if (!file) {
//        cerr << "Unable to open file." << endl;
//        return 1;
//    }
//
//    vector<pair<string, int>> leaderboard;
//    string username;
//    int score;
//
//    // Read the file and store usernames and scores in a vector
//    while (getline(file, username) && file >> score && file.ignore()) {
//        leaderboard.push_back(make_pair(username, score));
//    }
//
//    file.close();
//
//    // Sort the leaderboard based on scores in descending order
//    sort(leaderboard.begin(), leaderboard.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
//        return a.second > b.second;
//        });
//
//    // Variables to store the top three scores and usernames
//    tuple<string, int> topScore, midScore, bottomScore;
//
//    if (leaderboard.size() >= 1) {
//        topScore = make_tuple(leaderboard[0].first, leaderboard[0].second);
//    }
//    if (leaderboard.size() >= 2) {
//        midScore = make_tuple(leaderboard[1].first, leaderboard[1].second);
//    }
//    if (leaderboard.size() >= 3) {
//        bottomScore = make_tuple(leaderboard[2].first, leaderboard[2].second);
//    }
//
//    // Print the top three scores and usernames
//    cout << "Top Score: " << get<0>(topScore) << " - " << get<1>(topScore) << endl;
//    cout << "Mid Score: " << get<0>(midScore) << " - " << get<1>(midScore) << endl;
//    cout << "Bottom Score: " << get<0>(bottomScore) << " - " << get<1>(bottomScore) << endl;
//
//    return 0;
//}