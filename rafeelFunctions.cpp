//#include<iostream>
//#include <cstdlib> // For rand() and srand()
//#include <ctime>   // For time()
//using namespace std;
//int main() {
//	cout << "Hello";
//}
////some global variables that are to be used
//int actualPlayerGrid[10][10] = { 0 };
//int attackCoordX, attackCoordY, sizeAttack; // global variables that store the ai attack coordinates and the "size" of the ship being attacked
//bool directionConfirmation; //  specifies whether or not the direction is confirmed
//int currentCount = 0; // specifies what the count of loop is being run on the ai mode
//int shipValueTargetted; // classifies the type of ship being targetted
//int directionToTarget = 0; // 1 for north, 2 for east, 3 for south, 4 for west
//bool targetMode = false;
//
//void playerFire(int x, int y, int aiGrid[10][10]) {
//	if (aiGrid[x][y] < 0) {
//		cout << "Try again"; // invalid input handle
//	}
//	if (aiGrid[x][y] == 0) {
//		aiGrid[x][y] = -1;
//		cout << "Missed!"; // replace box with red coloration
//	}
//	if (aiGrid[x][y] == 1) {
//		aiGrid[x][y] = -10; // destroyer value replacement
//	}
//	if (aiGrid[x][y] == 2) {
//		aiGrid[x][y] = -9; // warship
//	}
//	if (aiGrid[x][y] == 3) {
//		aiGrid[x][y] = -8; // sub
//	}
//	if (aiGrid[x][y] == 4) {
//		aiGrid[x][y] = -7; // battleship
//	}
//	if (aiGrid[x][y] == 5) {
//		aiGrid[x][y] = -6; // carrier idk
//	}
//}
//
//void aiFire(int playerGrid[10][10]) {
//	srand(static_cast<unsigned>(time(0)));
//	int x = rand() % 10;
//	int y = rand() % 10;
//	while (playerGrid[y][x] < 0) { // invalid input handle
//		x = rand() % 10;
//		y = rand() % 10;
//	}
//	if (targetMode == false) {
//		if (playerGrid[y][x] == 0) {
//			playerGrid[y][x] = -1;
//			cout << "Missed!"; // replace box with red coloration
//		}
//		if (playerGrid[y][x] == 1) {
//			playerGrid[y][x] = -10; // destroyer value replacement
//			attackCoordX = x;
//			attackCoordY = y;
//			targetMode = true;
//			sizeAttack = 5 - 1;
//			shipValueTargetted = 1;
//		}
//		if (playerGrid[y][x] == 2) {
//			playerGrid[y][x] = -9; // warship
//			attackCoordX = x;
//			attackCoordY = y;
//			targetMode = true;
//			sizeAttack = 4 - 1;
//			shipValueTargetted = 2;
//		}
//		if (playerGrid[y][x] == 3) {
//			playerGrid[y][x] = -8; // sub
//			attackCoordX = x;
//			attackCoordY = y;
//			targetMode = true;
//			sizeAttack = 3 - 1;
//			shipValueTargetted = 3;
//		}
//		if (playerGrid[y][x] == 4) {
//			playerGrid[y][x] = -7; // battleship
//			attackCoordX = x;
//			attackCoordY = y;
//			targetMode = true;
//			sizeAttack = 3 - 1;
//			shipValueTargetted = 4;
//		}
//		if (playerGrid[y][x] == 5) {
//			playerGrid[y][x] = -6; // carrier idk
//			attackCoordX = x;
//			attackCoordY = y;
//			targetMode = true;
//			sizeAttack = 2 - 1;
//			shipValueTargetted = 5;
//		}
//	}
//	if (targetMode == true) {
//		if (directionConfirmation == false) {
//
//			if (currentCount == 0) {
//				attackCoordY--;
//				if (playerGrid[attackCoordY][attackCoordX] == shipValueTargetted) { // handles if the block is "correct"
//					directionConfirmation = true;
//					directionToTarget = 1; // sets the direction to be north.
//					currentCount = 0; // resetting the targetting process
//					playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted; // works for all types of ship values being targetted
//					sizeAttack--; //decrements the size of ships being attacked for this specific one.
//					cout << "Hit.";
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] == 0) { // handles if the block is empty and its missed
//					directionConfirmation = false;
//					playerGrid[attackCoordY][attackCoordX] = -1;
//					attackCoordY++;
//					cout << "Miss.";
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] < 0) { // handles if the block is already hit or its missed.
//					currentCount++;
//					aiFire(actualPlayerGrid);
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] > 0 && playerGrid[attackCoordY][attackCoordX] != shipValueTargetted) {
//					int temp = playerGrid[attackCoordY][attackCoordX]; // handles if the block is filled with a ship of other type
//					playerGrid[attackCoordY][attackCoordX] = -11 + temp;
//					currentCount++;
//				}
//				else if (attackCoordY < 0) { // handles if the coordinate is out of bounds.
//					currentCount++;
//					aiFire(actualPlayerGrid);
//				}
//			}
//
//			if (currentCount == 1) {
//				attackCoordX++; // to check the right side
//				attackCoordY++; // to undo the decrement in previous block
//
//				if (playerGrid[attackCoordY][attackCoordX] == shipValueTargetted) { // handles if the block is "correct"
//					directionConfirmation = true;
//					directionToTarget = 2; // sets the direction to be east.
//					currentCount = 0; // resetting the targetting process
//					playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted; // works for all types of ship values being targetted
//					sizeAttack--; //decrements the size of ships being attacked for this specific one.
//					cout << "Hit.";
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] == 0) { // handles if the block is empty and its missed
//					directionConfirmation = false;
//					playerGrid[attackCoordY][attackCoordX] = -1;
//					attackCoordX--;
//					cout << "Miss.";
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] < 0) { // handles if the block is already hit or its missed.
//					currentCount++;
//					aiFire(actualPlayerGrid);
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] > 0 && playerGrid[attackCoordY][attackCoordX] != shipValueTargetted) {
//					int temp = playerGrid[attackCoordY][attackCoordX]; // handles if the block is filled with a ship of other type
//					playerGrid[attackCoordY][attackCoordX] = -11 + temp;
//					currentCount++;
//				}
//				else if (attackCoordX > 9) { // handles if the coordinate is out of bounds.
//					currentCount++;
//					aiFire(actualPlayerGrid);
//				}
//			}
//
//			if (currentCount == 2) {
//				attackCoordX--; //undoes the previous increment in the currentcount==1 block
//				attackCoordY++; // now checks if the direction is in the south.
//				if (playerGrid[attackCoordY][attackCoordX] == shipValueTargetted) { // handles if the block is "correct"
//					directionConfirmation = true;
//					directionToTarget = 3; // sets the direction to be south.
//					currentCount = 0; // resetting the targetting process
//					playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted; // works for all types of ship values being targetted
//					sizeAttack--; //decrements the size of ships being attacked for this specific one.
//					cout << "Hit.";
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] == 0) { // handles if the block is empty and its missed
//					directionConfirmation = false;
//					playerGrid[attackCoordY][attackCoordX] = -1;
//					attackCoordY--;
//					cout << "Miss.";
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] < 0) { // handles if the block is already hit or its missed.
//					currentCount++;
//					aiFire(actualPlayerGrid);
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] > 0 && playerGrid[attackCoordY][attackCoordX] != shipValueTargetted) {
//					int temp = playerGrid[attackCoordY][attackCoordX]; // handles if the block is filled with a ship of other type
//					playerGrid[attackCoordY][attackCoordX] = -11 + temp;
//					currentCount++;
//				}
//				else if (attackCoordY > 9) { // handles if the coordinate is out of bounds.
//					currentCount++;
//					aiFire(actualPlayerGrid);
//				}
//			}
//
//			if (currentCount == 3) {
//				attackCoordY--; //undoes the previous increment in the currentcount==2 block
//				attackCoordX--; // now checks if the direction is in the west
//				if (playerGrid[attackCoordY][attackCoordX] == shipValueTargetted) { // handles if the block is "correct"
//					directionConfirmation = true;
//					directionToTarget = 4; // sets the direction to be south.
//					currentCount = 0; // resetting the targetting process
//					playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted; // works for all types of ship values being targetted
//					sizeAttack--; //decrements the size of ships being attacked for this specific one.
//					cout << "Hit.";
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] == 0) { // handles if the block is empty and its missed
//					directionConfirmation = false;
//					playerGrid[attackCoordY][attackCoordX] = -1;
//					attackCoordX++;
//					cout << "Miss.";
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] < 0) { // handles if the block is already hit or its missed.
//					currentCount++;
//					aiFire(actualPlayerGrid);
//				}
//				else if (playerGrid[attackCoordY][attackCoordX] > 0 && playerGrid[attackCoordY][attackCoordX] != shipValueTargetted) {
//					int temp = playerGrid[attackCoordY][attackCoordX]; // handles if the block is filled with a ship of other type
//					playerGrid[attackCoordY][attackCoordX] = -11 + temp;
//					currentCount++;
//				}
//				else if (attackCoordX < 0) { // handles if the coordinate is out of bounds.
//					currentCount++;
//					aiFire(actualPlayerGrid);
//				}
//			}
//		}
//		if (sizeAttack == 0) {
//			directionConfirmation = false;
//			targetMode = false;
//			cout << shipValueTargetted << " has been completely destroyed.\n";
//		}
//		if (directionConfirmation == true) {
//			if (directionToTarget == 1) {
//				attackCoordY--;
//				playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted;
//				sizeAttack--;
//			}
//			if (directionToTarget == 2) {
//				attackCoordX++;
//				playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted;
//				sizeAttack--;
//			}
//			if (directionToTarget == 3) {
//				attackCoordY++;
//				playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted;
//				sizeAttack--;
//			}
//			if (directionToTarget == 4) {
//				attackCoordX--;
//				playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted;
//				sizeAttack--;
//			}
//			if (sizeAttack == 0) {
//				targetMode = false;
//				directionConfirmation = false;
//			}
//		}
//	}
//}