#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include<cmath>
#include <cstdlib>
#include <sstream>
#include <thread> // For simulating resource loading
#include <chrono>
#include<fstream>
#include <sstream>
#include<vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

//function definitions:

void openWindow(RenderWindow& window, Texture& mainBgTexture, Font& mainFont, int aiGrid[10][10], bool aimed[10][10]);
int loadEverything();
int handleEvents(RenderWindow& window, int screenManager, int aiGrid[10][10]);
void drawMainScreen(RenderWindow& window, Texture& mainBgTexture, Font& mainFont);
void writeText(RenderWindow& window, string name, Font& mainFont, int horizontal, int vertical, int width, int height, bool leaderBoard, bool caseScenario = 0);
FloatRect makeButtons(RenderWindow& window, Font& mainFont, string name, int width, int height, int vertical, int horizontal);
void screenDecide(RenderWindow& window, Texture& mainBgTexture, Font& mainFont, int aiGrid[10][10], bool aimed[10][10], int screenManager);
bool drawBoard(RenderWindow& window,int Array[10][10], int userNo, int height = 10, int width = 10);
void handleDrag(RenderWindow& window, Event& event, int Array[10][10], int userNo, int width, int height);
void gamePlayScreen(RenderWindow& window, int otherArray[10][10], bool aimed[10][10], bool multiYes, int width, int height);
bool readyToPlay(int rows, int columns, int shipWeight);
void pauseButton(RenderWindow& window);
void leaderBoardScreen(RenderWindow& window);
void mainLoadingScreen(RenderWindow& window);
void settings(RenderWindow& window);
void settingsMenu(RenderWindow& window);
void multiGame(RenderWindow& window, int secondArray[10][10], bool aimed[10][10]);

//GlobalVariables for button coordinates, updated in makeButtons and used in handleEvents

Texture airCraftCarrierTexture, battleShipTexture, submarineTexture, cruiserTexture, destroyerTexture;
Sprite airCraftCarrier, battleShip, submarine, cruiser, destroyer;
Sprite* draggedShip;
Texture setShips;
Texture gameBgTexture;
Texture crossHairTexture;
Texture pauseButtonTexture;
Texture dialogBoxTexture;
Texture LeaderBoardTexture;
Texture youLostTexture;

Texture youWonTexture;
Texture settingTexture;
Texture settingMenuTexture;
Texture multiEndTexture;

VideoMode desktopsize = VideoMode::getDesktopMode();

Event event;

SoundBuffer clickSoundBuffer;
SoundBuffer errorSoundBuffer;
SoundBuffer DestructSoundBuffer;
SoundBuffer missSoundBuffer;
SoundBuffer hitSoundBuffer;
Sound clickSound;
Sound errorSound;
Sound destructSound;
Sound missSound;
Sound hitSound;

FloatRect playGlobal;
FloatRect leaderboardGlobal;
FloatRect exitGlobal;
FloatRect multiGlobal;
FloatRect shipSetPlayGlobal;
FloatRect pauseContinue;
FloatRect pauseMenu;
FloatRect pauseExit;
FloatRect fireGlobal;
FloatRect musicRect, clickRect, errorRect, aimRect, delayRect, dectructRect;

bool shipCollision = false;
bool beingDragged = false;
bool shipInGrid = false;
bool shipInitialPos = true;
bool pauseButtonPressed = false;
bool oneTime = true;
int currentI = 0, currentJ = 0;
int prevState = 0, newState = 0;
int turn = 0;
bool settingIconPressed = false;
bool settingsArray[5];
bool clickSoundOn = 1, errorSoundOn = 1, musicOn = 1, aimConfirmOn = 1, destructionOn = 1;
int cpuDelayTime = 1;
bool validTargetFound = false;

int playerScore = 100, aiScore = 100;
Vector2f offset;
Vector2f InitialPos;

//some global variables that are to be used
int attackCoordX, attackCoordY, sizeAttack; // global variables that store the ai attack coordinates and the "size" of the ship being attacked
bool directionConfirmation; //  specifies whether or not the direction is confirmed
int currentCount = 0; // specifies what the count of loop is being run on the ai mode
int shipValueTargetted; // classifies the type of ship being targetted
int directionToTarget = 0; // 1 for north, 2 for east, 3 for south, 4 for west
bool targetMode = false;
bool didYouWin = false;
bool didYouLose = false;
bool multiEnded = false;
bool multiOneWon = false;
bool multiTwoWon = false;
int multiGameState = 0;
string userName;
const int GRID_SIZE = 10;
const int NUM_SHIPS = 5;
const int SHIP_SIZES[NUM_SHIPS] = { 5, 4, 3, 3, 2 }; // Carrier, Battleship, Cruiser, Submarine, Destroyer
const int SHIP_VALUES[NUM_SHIPS] = { 1, 2, 3, 4, 5 };


//FloatRect largestShipGlobal;
//FloatRect largeShipGlobal;
//FloatRect smallShipGlobal;
//FloatRect smallestShipGlobal;



int userArray[10][10] = { 0 };
int secondArray[10][10] = { 0 };


Font mainFont;

Texture mainLoading;
Texture mainBgTexture;


//JUST to show the grid in the form of array on console
void shipsInArray(int aiGrid[10][10], bool aimed[10][10], int rows = 10, int columns = 10) {
	cout << "Array\n";
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cout << userArray[i][j] << " ";
		}
		cout << endl;
	}

	cout << "Ai grid\n";

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cout << aiGrid[i][j] << " ";
		}
		cout << endl;
	}

	cout << "aimed\n";

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cout << aimed[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {


	
	//To implement Full Screen
	VideoMode desktop = VideoMode::getDesktopMode();
	RenderWindow window(desktop, "BATTLESHIPS", Style::Fullscreen);
	window.clear(Color(0, 0, 0));
	window.display();
	//Loading Textures & Fonts

	if (!mainLoading.loadFromFile("Made Without Unity.png"))
	{
		return -1;
	}

	mainLoadingScreen(window);
	
	if (loadEverything() == -1)
		return 0;
	

	int aiGrid[10][10] = { 0 };

	bool aimed[10][10] = { false };


	//opens the main menu
	openWindow(window, mainBgTexture, mainFont, aiGrid, aimed);


	shipsInArray(aiGrid, aimed);

}

bool readSettings() {
	ifstream settingRead("settingsMemory.txt");

	if (!settingRead.is_open()) {
		return 0;
	}

	for (int i = 0; i < 5; i++) {
		if (!(settingRead >> settingsArray[i])) {
			cerr << "Error: Failed to read setting " << i << " from file!" << endl;
			return 0;
		}
	}
	if (!(settingRead >> cpuDelayTime)) {
		cerr << "Error: Failed to read cpuDelayTime from file!" << endl;
		return 0;
	}
	musicOn = settingsArray[0];
	clickSoundOn = settingsArray[1];
	errorSoundOn = settingsArray[2];
	destructionOn = settingsArray[3];
	aimConfirmOn  = settingsArray[4];
	cout << "I'm done";

	settingRead.close();

	return 1;
}

bool writeSettings() {
	ofstream settingsWrite("settingsMemory.txt");
	if (!settingsWrite.is_open()) {
		return 0;
	}
	settingsArray[0] = musicOn;
		settingsArray[1] = clickSoundOn;
		settingsArray[2] = errorSoundOn;
		settingsArray[3] = destructionOn;
		settingsArray[4] = aimConfirmOn;
	for (int i = 0; i < 6; i++) {
		if (i < 5)
			settingsWrite << settingsArray[i]<<" ";
		else
			settingsWrite << cpuDelayTime;
	}

	settingsWrite.close();
	return 1;
}

void writeLeaderBoard() {
	ofstream leaderboarder("leaderboardFile.txt", ios::app);
	leaderboarder << userName << "\n" << playerScore << "\n";
	leaderboarder.close();
}

int loadEverything() {
	if (!airCraftCarrierTexture.loadFromFile("airCraftCarrierTexture.png") || !battleShipTexture.loadFromFile("battleShipTexture.png") || !submarineTexture.loadFromFile("submarineTexture.png") || !cruiserTexture.loadFromFile("cruiserTexture.png") || !destroyerTexture.loadFromFile("destroyerTexture.png"))
	{
		cout << "Ships not Loaded";
		return -1;
	}
	if (!mainBgTexture.loadFromFile("MainMenuFinalBg.png"))
	{
		return -1;
	}


	if (!setShips.loadFromFile("Set Ships.png"))
	{
		return -1;
	}

	if (!mainFont.loadFromFile("Paul-le1V.ttf"))
	{
		return -1;
	}
	if (!clickSoundBuffer.loadFromFile("minecraft-click.WAV"))
	{
		return -1;
	}
	clickSound.setBuffer(clickSoundBuffer);

	if (!errorSoundBuffer.loadFromFile("error-sound.WAV"))
	{
		return -1;
	}
	errorSound.setBuffer(errorSoundBuffer);

	if (!gameBgTexture.loadFromFile("GameBg.png"))
	{
		return -1;
	}
	if (!crossHairTexture.loadFromFile("crossHair.png"))
	{
		return -1;
	}
	if (!pauseButtonTexture.loadFromFile("pauseButton.png"))
	{
		return -1;
	}
	if (!dialogBoxTexture.loadFromFile("dialogBox.png"))
	{
		return -1;
	}
	if (!LeaderBoardTexture.loadFromFile("LEADERBOARD.png"))
	{
		return -1;
	}
	if (!youWonTexture.loadFromFile("youWon.png"))
	{
		return -1;
	}
	if (!settingTexture.loadFromFile("settings.png"))
	{
		return -1;
	}
	if (!settingMenuTexture.loadFromFile("settingsMenu.png"))
	{
		return -1;
	}
	if (!readSettings())
	{
		return -1;
	}
	if (!DestructSoundBuffer.loadFromFile("destruction.WAV"))
	{
		return -1;
	}
	destructSound.setBuffer(DestructSoundBuffer);
	if (!missSoundBuffer.loadFromFile("missBoat.WAV"))
	{
		return -1;
	}
	missSound.setBuffer(missSoundBuffer);
	if (!hitSoundBuffer.loadFromFile("hitBoat.WAV"))
	{
		return -1;
	}
	hitSound.setBuffer(hitSoundBuffer);
	/*if (!bgMusic.openFromFile("bgMusiccc.waptt.WAV"))
	{
		return -1;
	}
	bgMusic.setLoop(true);
	bgMusic.setVolume(50);
	if(musicOn)
		bgMusic.play();*/

	if (!youLostTexture.loadFromFile("youLost.png"))
	{
		return -1;
	}
	if (!multiEndTexture.loadFromFile("multiEnd.png"))
	{
		return -1;
	}


	
	return 0;
}

void mainLoadingScreen(RenderWindow& window) {
	Sprite mainLoader;
	mainLoader.setTexture(mainLoading);
	mainLoader.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);
	window.clear();
	window.draw(mainLoader);
	window.display();
	this_thread::sleep_for(std::chrono::seconds(3));
}

//Sets the button at center of screen in accordance to width
int centerAlign(int screenWidth, int width) {
	int center = (screenWidth / 2) - (width / 2);
	return center;
}

void resetShips() {
	//making ships vertical for assembly
	float x = desktopsize.width / 25.0;

	//setting up the ships upon their origin
	airCraftCarrier.setOrigin(airCraftCarrier.getLocalBounds().width / 2, airCraftCarrier.getLocalBounds().height / 2);
	battleShip.setOrigin(battleShip.getLocalBounds().width / 2, battleShip.getLocalBounds().height / 2);
	cruiser.setOrigin(cruiser.getLocalBounds().width / 2, cruiser.getLocalBounds().height / 2);
	submarine.setOrigin(submarine.getLocalBounds().width / 2, submarine.getLocalBounds().height / 2);
	destroyer.setOrigin(destroyer.getLocalBounds().width / 2, destroyer.getLocalBounds().height / 2);


	airCraftCarrier.setPosition(Vector2f(x += desktopsize.height / 12.0, desktopsize.height / 2.0));
	battleShip.setPosition(Vector2f(x += desktopsize.height / 7.2, desktopsize.height / 2.0));
	cruiser.setPosition(Vector2f(x += desktopsize.height / 7.2, desktopsize.height / 2.0));
	submarine.setPosition(Vector2f(x += desktopsize.height / 7.2, desktopsize.height / 2.0));
	destroyer.setPosition(Vector2f(x += desktopsize.height / 7.2, desktopsize.height / 2.0));

	airCraftCarrier.setRotation(450);
	battleShip.setRotation(450);
	cruiser.setRotation(450);
	submarine.setRotation(450);
	destroyer.setRotation(450);

	airCraftCarrier.setColor(Color(255, 255, 255, 255));
	battleShip.setColor(Color(255, 255, 255, 255));
	cruiser.setColor(Color(255, 255, 255, 255));
	submarine.setColor(Color(255, 255, 255, 255));
	destroyer.setColor(Color(255, 255, 255, 255));
}

void shipCreator() {
	airCraftCarrier.setTexture(airCraftCarrierTexture);
	battleShip.setTexture(battleShipTexture);
	cruiser.setTexture(cruiserTexture);
	submarine.setTexture(submarineTexture);
	destroyer.setTexture(destroyerTexture);

	
	float currentSizeX = desktopsize.width / 24.836;
	float currentSizeY = desktopsize.width / 30.355;
	float difference = (currentSizeX - currentSizeY) / 2;


	//Setting the current ship size
	airCraftCarrier.scale((currentSizeX - difference) / 52, currentSizeY / 52);
	battleShip.scale((currentSizeX - difference) / 52, currentSizeY / 52);
	cruiser.scale((currentSizeX - difference) / 52, currentSizeY / 52);
	submarine.scale((currentSizeX - difference)/ 52, currentSizeY / 52);
	destroyer.scale((currentSizeX - difference)/ 52, currentSizeY / 52);
	
	resetShips();
}

//Contains The Game Loop
void openWindow(RenderWindow& window, Texture& mainBgTexture,  Font& mainFont, int aiGrid[10][10], bool aimed[10][10]) {
	int screenManager = 0;
	shipCreator();
		while (window.isOpen()) {
		screenDecide(window, mainBgTexture, mainFont, aiGrid, aimed, screenManager);
		screenManager = handleEvents(window, screenManager, aiGrid);
	 }
}


void screenDecide(RenderWindow& window, Texture& mainBgTexture, Font& mainFont, int aiGrid[10][10], bool aimed[10][10], int screenManager) {
	switch (screenManager)
	{
	case 0:
		drawMainScreen(window, mainBgTexture, mainFont);
		break;
	case 1:
		drawBoard(window, userArray, 0, 10, 10);
		break;
	case 2:
		gamePlayScreen(window, aiGrid, aimed, 0, 10, 10);
		break;
	case 3:
		leaderBoardScreen(window);
		break;
	case 4:
		settingsMenu(window);
		break;
	case 5:
		multiGame(window, aiGrid, aimed);
		break;
	}
}

void initializeGrid(int grid[GRID_SIZE][GRID_SIZE]) {
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			grid[i][j] = 0;
		}
	}
}

bool canPlaceShip(int grid[GRID_SIZE][GRID_SIZE], int x, int y, int size, bool horizontal) {
	if (horizontal) {
		if (y + size > GRID_SIZE) return false;
		for (int i = 0; i < size; ++i) {
			if (grid[x][y + i] != 0) return false;
		}
	}
	else {
		if (x + size > GRID_SIZE) return false;
		for (int i = 0; i < size; ++i) {
			if (grid[x + i][y] != 0) return false;
		}
	}
	return true;
}

void placeShip(int grid[GRID_SIZE][GRID_SIZE], int x, int y, int size, bool horizontal, int arrayAccess) {
	if (horizontal) {
		for (int i = 0; i < size; ++i) {
			grid[x][y + i] = SHIP_VALUES[arrayAccess];
		}
	}
	else {
		for (int i = 0; i < size; ++i) {
			grid[x + i][y] = SHIP_VALUES[arrayAccess];
		}
	}
}

void generateAIBoard(int grid[GRID_SIZE][GRID_SIZE]) {
	srand(static_cast<unsigned>(time(0)));
	for (int i = 0; i < NUM_SHIPS; ++i) {
		int size = SHIP_SIZES[i];
		bool placed = false;
		while (!placed) {
			int x = rand() % GRID_SIZE;
			int y = rand() % GRID_SIZE;
			bool horizontal = rand() % 2 == 0;
			if (canPlaceShip(grid, x, y, size, horizontal)) {
				placeShip(grid, x, y, size, horizontal, i);
				placed = true;
			}
		}
	}
}

void aiGridSetup(int aiGrid[10][10]) {
	initializeGrid(aiGrid);
	generateAIBoard(aiGrid);
}

void handleDrag(RenderWindow& window, Event& event, int Array[10][10], int userNo, int width, int height) {

	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));


	int screenwidth = desktopsize.width;
	int screenheight = desktopsize.height;

	float boxsize = screenwidth / 24.836;
	float actualboxsize = screenwidth / 30.355;


	int startingPointX = (screenwidth / 2);
	int startingPointY = (screenheight / 2) - (boxsize * 5);


	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {


		if (airCraftCarrier.getGlobalBounds().contains(mousePos)) {
			draggedShip = &airCraftCarrier;
		}
		else if (battleShip.getGlobalBounds().contains(mousePos)) {
			draggedShip = &battleShip;
		}
		else if (cruiser.getGlobalBounds().contains(mousePos)) {
			draggedShip = &cruiser;
		}
		else if (submarine.getGlobalBounds().contains(mousePos)) {
			draggedShip = &submarine;
		}
		else if (destroyer.getGlobalBounds().contains(mousePos)) {
			draggedShip = &destroyer;
		}

		if (draggedShip) {
			beingDragged = true;
			offset = mousePos - draggedShip->getPosition();
		}

	}




	if (shipInitialPos && draggedShip) {
		InitialPos = draggedShip->getPosition();
		shipInitialPos = false;
	}

	if (beingDragged && draggedShip && event.type == Event::MouseMoved) {
		draggedShip->setPosition(mousePos - offset);
	}


	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
		if (draggedShip) {
				FloatRect bounds = draggedShip->getGlobalBounds();
				Vector2f topLeft(bounds.left, bounds.top);

				if (shipInGrid && !shipCollision) {
					float snappedX = round((topLeft.x - startingPointX) / boxsize) * boxsize + startingPointX;
					float snappedY = round((topLeft.y - startingPointY) / boxsize) * boxsize + startingPointY;

					draggedShip->setPosition(snappedX + 2 + bounds.width / 2, snappedY + 2 + bounds.height / 2);

					if(clickSoundOn) clickSound.play();
				}
				else
				{
					draggedShip->setPosition(InitialPos);
					if(errorSoundOn) errorSound.play();
				}

				shipInitialPos = true;
				beingDragged = false;
				draggedShip = nullptr;
			}
		}



	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right) {

		Sprite* selectedShip = nullptr;
		int initialRotation;

		//determine which ship to rotate
		if (airCraftCarrier.getGlobalBounds().contains(mousePos)) {
			selectedShip = &airCraftCarrier;
		}
		else if (battleShip.getGlobalBounds().contains(mousePos)) {
			selectedShip = &battleShip;
		}
		else if (cruiser.getGlobalBounds().contains(mousePos)) {
			selectedShip = &cruiser;
		}
		else if (submarine.getGlobalBounds().contains(mousePos)) {
			selectedShip = &submarine;
		}
		else if (destroyer.getGlobalBounds().contains(mousePos)) {
			selectedShip = &destroyer;
		}

		if (selectedShip) {
		
			//rotate the ship by 90 degrees
			if (selectedShip->getRotation() == 90)
			{

				InitialPos = selectedShip->getPosition();
				initialRotation = 90;
				selectedShip->rotate(90);
			}
			else {
				InitialPos = selectedShip->getPosition();
				initialRotation = 180;
				selectedShip->rotate(-90);
			}

			Vector2f shipPos = selectedShip->getPosition();
			FloatRect bounds = selectedShip->getGlobalBounds();

			Vector2f topLeft(bounds.left, bounds.top);

			float snappedX = round((topLeft.x - startingPointX) / boxsize) * boxsize + startingPointX;
			float snappedY = round((topLeft.y - startingPointY) / boxsize) * boxsize + startingPointY;

			selectedShip->setPosition(snappedX + 2 + bounds.width / 2, snappedY + 2 + bounds.height / 2);


			drawBoard(window, Array, userNo);
			if (shipCollision) {
				selectedShip->setRotation(initialRotation);
				selectedShip->setPosition(InitialPos);
				if(errorSoundOn)
					errorSound.play();
			}
			else
				if(clickSoundOn) clickSound.play();

		}
		

	}

}

//Handles all the input Events
int handleEvents(RenderWindow& window, int screenManager, int aiGrid[10][10]) {
	while (window.pollEvent(event)) {

		Vector2i mousePos = Mouse::getPosition(window);

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			window.close();

		if (screenManager == 1)
		{
			if (!pauseButtonPressed) {
				handleDrag(window, event, userArray, 0,10, 10);
			}
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (pauseButtonPressed) {
					if (pauseExit.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						if(clickSoundOn) clickSound.play();
						window.close();

					}
					if (pauseContinue.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						pauseContinue = temp;
						pauseButtonPressed = false;
						if(clickSoundOn) clickSound.play();
					}
					if (pauseMenu.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						pauseMenu = temp;
						screenManager = 0;
						pauseButtonPressed = false;
						resetShips();
						//generate new grid here
						if(clickSoundOn) clickSound.play();
					}
				}
				else {
					if (shipSetPlayGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y)))) {
						if (readyToPlay(10, 10, 17))
						{
							aiGridSetup(aiGrid);
							FloatRect temp;
							shipSetPlayGlobal = temp;
							if(clickSoundOn) clickSound.play();
							screenManager = 2;
						}
						else
							if(errorSoundOn) 
								errorSound.play();
					}
				}

			}
		}

		else if (screenManager == 0) {
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (exitGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					window.close();
					if(clickSoundOn) clickSound.play();
				}
				else if (playGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					FloatRect temp;
					playGlobal = temp;
					screenManager = 1;
					if(clickSoundOn) clickSound.play();
				}
				else if (multiGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					aiGridSetup(aiGrid);
					FloatRect temp;
					multiGlobal = temp;
					screenManager = 5;
					if(clickSoundOn) clickSound.play();
				}
				else if (leaderboardGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					FloatRect temp;
					playGlobal = temp;
					screenManager = 3;
					if(clickSoundOn) clickSound.play();
				}
			}
			if (settingIconPressed)
				screenManager = 4;

		}

		else if (screenManager == 2) {
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (pauseButtonPressed) {
					if (pauseExit.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						window.close();
						if(clickSoundOn) clickSound.play();
					}
					if (pauseContinue.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						pauseContinue = temp;
						pauseButtonPressed = false;
						if(clickSoundOn) clickSound.play();
					}
					if (pauseMenu.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						pauseMenu = temp;
						playerScore = 100;
						aiScore = 100;
						screenManager = 0;
						pauseButtonPressed = false;
						resetShips();
						if(clickSoundOn) clickSound.play();
					}
				}
				if (didYouWin) {
					if (leaderboardGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						leaderboardGlobal = temp;
						writeLeaderBoard();
						screenManager = 3;
						if(clickSoundOn) clickSound.play();
						didYouWin = false;
						resetShips();
					}
					if (playGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						playGlobal = temp;
						screenManager = 0;
						playerScore = 100;
						aiScore = 100;
						if(clickSoundOn) clickSound.play();
						didYouWin = false;
						resetShips();
					}
				}
				if (didYouLose) {
					if (playGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						playGlobal = temp;
						screenManager = 0;
						playerScore = 100;
						aiScore = 100;
						if (clickSoundOn) clickSound.play();
						didYouLose = false;
						resetShips();
					}
				}
			}
			if (didYouWin && event.type == sf::Event::TextEntered) {
				// Handle ASCII characters only
				if (event.text.unicode < 128) {
					// Backspace handling
					if (event.text.unicode == '\b') { // Backspace ASCII code
						if (!userName.empty()) {
							userName.pop_back(); // Remove last character
						}
					}
					// Printable characters
					else {
						userName += static_cast<char>(event.text.unicode);
					}
				}
			}
		}

		else if (screenManager == 3)
		{
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (pauseButtonPressed) {
					if (pauseExit.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						if(clickSoundOn) clickSound.play();
						window.close();

					}
					if (pauseContinue.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						pauseContinue = temp;
						pauseButtonPressed = false;
						if(clickSoundOn) clickSound.play();
					}
					if (pauseMenu.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						pauseMenu = temp;
						screenManager = 0;
						pauseButtonPressed = false;
						aiScore = playerScore = 100;
						if(clickSoundOn) clickSound.play();
					}
				}

			}

		}

		else if (screenManager == 4) {
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (musicRect.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					musicOn = !musicOn;
					if (clickSoundOn)
						 clickSound.play();
				}
				else if (clickRect.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					clickSoundOn = !clickSoundOn;
					if (clickSoundOn)
						 clickSound.play();
				}
				else if (errorRect.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					errorSoundOn = !errorSoundOn;
					if (clickSoundOn)
						clickSound.play();
				}
				else if (aimRect.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					aimConfirmOn = !aimConfirmOn;
					if (clickSoundOn)
						clickSound.play();
				}
				else if (dectructRect.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					destructionOn = !destructionOn;
					if (clickSoundOn)
						clickSound.play();
				}
				else if (delayRect.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					if (cpuDelayTime < 2)
						cpuDelayTime++;
					else
						cpuDelayTime = 0;
					if (clickSoundOn)
						clickSound.play();
				}
				else if (exitGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					writeSettings();
					FloatRect temp;
					exitGlobal = temp;
					settingIconPressed = false;
					screenManager = 0;
					if (clickSoundOn)
						clickSound.play();
				}
				

			}
		}

		else if (screenManager == 5) {
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (pauseButtonPressed) {
					if (pauseExit.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						if (clickSoundOn) clickSound.play();
						window.close();

					}
					if (pauseContinue.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						pauseContinue = temp;
						pauseButtonPressed = false;
						if (clickSoundOn) clickSound.play();
					}
					if (pauseMenu.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						pauseMenu = temp;
						screenManager = 0;
						multiGameState = 0;
						playerScore = 100;
						aiScore = 100;
						pauseButtonPressed = false;
						resetShips();
						//generate new grid here
						if (clickSoundOn) clickSound.play();
					}
				}
				if (multiGameState == 0) {
					if (shipSetPlayGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						if (readyToPlay(10, 10, 17)) {
							FloatRect temp;
							shipSetPlayGlobal = temp;
							multiGameState = 1;
							resetShips();
						}
						if (clickSoundOn)
							clickSound.play();

					}
				}
				if (multiGameState == 1) {
					if (shipSetPlayGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						if (readyToPlay(10, 10, 17)) {
							FloatRect temp;
							shipSetPlayGlobal = temp;
							multiGameState = 2;
							resetShips();
						}
						if (clickSoundOn)
							clickSound.play();
					}
				}
				if (multiGameState == 2) {
				
					if (multiEnded) {
						if (playGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
						{
							FloatRect temp;
							playGlobal = temp;
							screenManager = 0;
							playerScore = aiScore = 100;
							if (clickSoundOn) clickSound.play();
							multiEnded = multiOneWon = multiTwoWon = false;
							multiGameState = 0;
							resetShips();
						}
					}
				}
			}
		}
	}
	return screenManager;
}

//Displays the Main Menu
void drawMainScreen(RenderWindow& window, Texture&  mainBgTexture, Font& mainFont) {

	Sprite mainBg;
	mainBg.setTexture(mainBgTexture);

	//makes the background image suitable for all screens
	mainBg.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);

	window.clear();
	window.draw(mainBg);

	int buttonWidth = desktopsize.width / 5, buttonHeight = buttonWidth / 4, topButton = desktopsize.height / 2.2, buttonDistance = desktopsize.height / 9;
	playGlobal = makeButtons(window, mainFont, "SINGLE PLAYER", buttonWidth, buttonHeight, topButton, 0);
	topButton += buttonDistance;
	multiGlobal = makeButtons(window, mainFont, "MULTI-PLAYER", buttonWidth, buttonHeight, topButton, 0);
	topButton += buttonDistance;
	leaderboardGlobal = makeButtons(window, mainFont, "LEADERBOARD", buttonWidth, buttonHeight, topButton, 0);
	topButton += buttonDistance;
	exitGlobal = makeButtons(window, mainFont,  "EXIT", buttonWidth, buttonHeight, topButton, 0);
	

	settings(window);

	window.display();
}

//Called in the makeButtons Function. Displays and Aligns the Text
void writeText(RenderWindow& window, string name, Font& mainFont, int horizontal, int vertical, int width, int height, bool leaderBoard, bool caseScenario) {

	Text text;
	text.setFont(mainFont);
	if(leaderBoard)
		text.setStyle(Text::Bold);
	
	text.setCharacterSize(desktopsize.height / 24);


	if (!leaderBoard) {
		text.setCharacterSize(desktopsize.height / 24);
		text.setFillColor(Color(0, 0, 50));//Bluish-Black
	}
	else {
		text.setCharacterSize(desktopsize.height / 15);
		text.setFillColor(Color(239, 242, 255));//Button-white
	}

	if (caseScenario) {
		text.setCharacterSize(desktopsize.height / 9);
	}

	text.setString(name);//Button name

	FloatRect textRect = text.getLocalBounds(); //gets the coordinates of Text
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f); //now origin will be placed on the given location instead of top right corner

	text.setPosition(horizontal + width / 2.0f, vertical + height / 2.0f);//Finds centre of button and places text there
	
	window.draw(text);

}

//Makes any type of Buttons(CENTER-ALIGNED)
FloatRect makeButtons(RenderWindow& window, Font& mainFont, string name, int width, int height, int vertical, int horizontal) {
	//gets screen size info to set button on centre
	if (horizontal == 0) {
		horizontal = centerAlign(desktopsize.width, width);
	}


	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

	bool isClicked = false;

	RectangleShape button(Vector2f(width, height));
	button.setPosition(Vector2f(horizontal, vertical));
	button.setOutlineThickness(1);
	button.setFillColor(Color(239, 242, 255));
	button.setOutlineColor(Color::Black);
	if (button.getGlobalBounds().contains(mousePos)) {
		button.setFillColor(Color(200, 200, 240));
		if (Mouse::isButtonPressed(Mouse::Left)) {
			button.setFillColor(Color(150, 150, 190));
		}
	}
	window.draw(button);
	//writing the text
	writeText(window, name, mainFont, horizontal, vertical, width, height, false);
	
	/*if (Mouse::isButtonPressed(Mouse::Left)) {
		Vector2i mousePos = Mouse::getPosition(window);
		if (button.getGlobalBounds().contains(static_cast<Vector2f>(mousePos))) {
			std::cout << "Button clicked!" << std::endl;
			if(clickSoundOn) clickSound.play();
		}
	}*/



	return button.getGlobalBounds();
}


//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////


bool readyToPlay(int rows, int columns, int shipWeight) {


	//return true;


	int nonZero = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (userArray[i][j] != 0) {
				nonZero++;
			}
		}
	}
	
	if(nonZero == shipWeight)
		return true;
	return false;
	
}

bool isShipOutOfBounds(Sprite& ship, int gridWidth, int gridHeight, float boxsize, int startingPointX, int startingPointY) {
	FloatRect bounds = ship.getGlobalBounds();

	// Calculate grid limits
	float gridLeft = startingPointX;
	float gridRight = startingPointX + gridWidth * boxsize;
	float gridTop = startingPointY;
	float gridBottom = startingPointY + gridHeight * boxsize;

	// Check if the ship's bounds are outside the grid area
	if (bounds.left < gridLeft || bounds.left + bounds.width > gridRight ||
		bounds.top < gridTop || bounds.top + bounds.height > gridBottom) {
		return true;
	}

	return false;
}

bool drawBoard(RenderWindow& window, int Array[10][10], int userNo, int height, int width) {
	window.clear();


	shipCollision = false; 
	shipInGrid = false;   

	Sprite setShipBg;
	setShipBg.setTexture(setShips);
	setShipBg.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);
	window.draw(setShipBg);

	if (userNo == 1) {
		writeText(window, "Player 1", mainFont, desktopsize.width / 1.4222, desktopsize.width / 24, 0, 0, 1);
	}
	else if (userNo == 2) {
		writeText(window, "Player 2", mainFont, desktopsize.width / 1.4222, desktopsize.width / 24, 0, 0, 1);
	}

	float boxsize = desktopsize.width / 24.836;
	RectangleShape rect(Vector2f(boxsize, boxsize));

	int startingPointX = desktopsize.width / 2;
	int startingPointY = (desktopsize.height / 2) - (boxsize * 5);

	//lambda
	auto processShip = [&](Sprite& ship, int shipValue) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				rect.setPosition(Vector2f(startingPointX + (boxsize * j), startingPointY + (boxsize * i)));

				//checking if current ship intersects a cell
				if (ship.getGlobalBounds().intersects(rect.getGlobalBounds())) {
					if (Array[i][j] == 0) {
						Array[i][j] = shipValue;
					}
					else if (Array[i][j] != shipValue) {
						shipCollision = true; 
					}
					if (!rect.getGlobalBounds().intersects(ship.getGlobalBounds())) {
						shipCollision = true;
					}
					if (isShipOutOfBounds(ship, width, height, boxsize, startingPointX, startingPointY)) 
						shipCollision = true;
					if (draggedShip == &ship) {
						shipInGrid = true; 
					}
				}
			}
		}
	};

	//resetting the userArray
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Array[i][j] = 0;
		}
	}

	processShip(airCraftCarrier, 1);
	processShip(battleShip, 2);
	processShip(cruiser, 3);
	processShip(submarine, 4);
	processShip(destroyer, 5);

	//drawing the grid
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			rect.setPosition(Vector2f(startingPointX + (boxsize * j), startingPointY + (boxsize * i)));
			rect.setFillColor(Array[i][j] == 0 ? Color(25, 25, 25, 255) : Color(50, 50, 100, 255));
			rect.setOutlineColor(Color(255, 255, 255, 255));
			rect.setOutlineThickness(1);
			window.draw(rect);
		}
	}

	window.draw(airCraftCarrier);
	window.draw(battleShip);
	window.draw(cruiser);
	window.draw(submarine);
	window.draw(destroyer);

	if (!pauseButtonPressed) {
		if(userNo!=1)
			shipSetPlayGlobal = makeButtons(window, mainFont, "PLAY", desktopsize.width / 5, desktopsize.width / 20, desktopsize.height / 1.2, (desktopsize.width / 4) - (desktopsize.width / 10));
		else
			shipSetPlayGlobal = makeButtons(window, mainFont, "NEXT", desktopsize.width / 5, desktopsize.width / 20, desktopsize.height / 1.2, (desktopsize.width / 4) - (desktopsize.width / 10));
	}
	pauseButton(window);

	window.display();

	return true;
}

bool transferShips(Sprite& ship, int startingPointX, int startingPointY, float boxsize, int i, int j, int width, int height, int blocks, bool horizon, bool player) {
	
	if (player) {
		if (horizon)
			ship.setRotation(180);
		else
			ship.setRotation(90);
	}

	if (ship.getRotation() == 90) {
		ship.setOrigin(0, height);
		//ship.setScale(ship.getGlobalBounds().width / (static_cast<float>(blocks) * boxsize - 1), ship.getGlobalBounds().height / (static_cast<float>(boxsize) - 1));
	}
	else {
		ship.setOrigin(width, height);
		//ship.setScale(ship.getGlobalBounds().height / (static_cast<float>(boxsize) - 1), ship.getGlobalBounds().width / (static_cast<float>(blocks) * boxsize - 1));

	}
	ship.setPosition(startingPointX + (boxsize * j), startingPointY + (boxsize * i));

	ship.setColor(Color :: Black);


	return false;
}

void playerFire(int x, int y, int aiGridValue, int aiGrid[10][10], bool player) {
	cout << "here i am";
	if (aiGridValue < 0) {
		cout << "Try again"; // invalid input handle
	}
	if (aiGridValue == 0) {
		aiGrid[x][y] = -1;
		cout << "Missed!";// replace box with red coloration
		if (!player)
			playerScore--;
		else
			aiScore--;
		if (destructionOn)
			missSound.play();
	}
	else if (aiGridValue == 1) {
		cout << "-10!";
		aiGrid[x][y] = -10; // destroyer value replacement
		if (destructionOn)
			hitSound.play();
	}
	else if (aiGridValue == 2) {
		aiGrid[x][y] = -9; // warship
		if (destructionOn)
			hitSound.play();
	}
	else if (aiGridValue == 3) {
		aiGrid[x][y] = -8; // sub
		if (destructionOn)
			hitSound.play();
	}
	else if (aiGridValue == 4) {
		aiGrid[x][y] = -7; // battleship
		if (destructionOn)
			hitSound.play();
	}
	else if (aiGridValue == 5) {
		aiGrid[x][y] = -6; // carrier idk
		if (destructionOn)
			hitSound.play();
	}
}

void aiFire(int playerGrid[10][10]) {
	srand(static_cast<unsigned>(time(0)));

	if (!targetMode) {
		// Random fire mode
		int x, y;
		do {
			x = rand() % 10;
			y = rand() % 10;
		} while (playerGrid[y][x] < 0); // Avoid previously fired cells

		if (playerGrid[y][x] == 0) {
			playerGrid[y][x] = -1; // Mark as missed
			cout << "Missed." << endl;
			aiScore--;
		}
		else if (playerGrid[y][x] > 0) {
			// Ship hit
			shipValueTargetted = playerGrid[y][x];
			playerGrid[y][x] = -11 + shipValueTargetted; // Mark as hit
			sizeAttack = 6 - shipValueTargetted; // Determine remaining ship size
			if (shipValueTargetted > 3) sizeAttack++; // Adjust for larger ships
			sizeAttack--;
			targetMode = true;
			directionConfirmation = false;
			attackCoordX = x;
			attackCoordY = y;
			currentCount = 0; // Reset current count
			cout << "Hit." << endl;
		}
	}
	else {
		// Targeting mode
		if (!directionConfirmation) {
			while (currentCount < 4) {
				// Try each direction until a valid target is found or direction is confirmed
				int newX = attackCoordX;
				int newY = attackCoordY;

				switch (currentCount) {
				case 0: newY--; break; // North
				case 1: newX++; break; // East
				case 2: newY++; break; // South
				case 3: newX--; break; // West
				}

				// Check if the new coordinates are within bounds and not already fired upon
				if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10 && playerGrid[newY][newX] >= 0) {
					if (playerGrid[newY][newX] == 0) {
						if (currentCount == 3) {
							currentCount = 0;
							targetMode = false;
						}
						playerGrid[newY][newX] = -1; // Mark as missed
						cout << "Missed." << endl;
						currentCount++;
						break;
					}
					else if (playerGrid[newY][newX] == shipValueTargetted) {
						playerGrid[newY][newX] = -11 + shipValueTargetted; // Mark as hit
						sizeAttack--;
						directionConfirmation = true;
						directionToTarget = currentCount + 1; // Set direction
						cout << "Hit." << endl;
						validTargetFound = true;
						if (sizeAttack == 0) {
							directionConfirmation = false;
							validTargetFound = false;
							currentCount = 0;
						}
						break;
					}
					else {
						currentCount++;
					}
				}
				else {
					currentCount++;
				}
			}
		}
		if (currentCount > 3) {
			currentCount = 0;
			targetMode = false;
			aiFire(userArray);
		}
		else if (validTargetFound && directionConfirmation) {
			// Continue in the confirmed direction
			int anewX = attackCoordX;
			int anewY = attackCoordY;

			switch (directionToTarget) {
			case 1: anewY--; break; // North
			case 2: anewX++; break; // East
			case 3: anewY++; break; // South
			case 4: anewX--; break; // West
			}

			if (anewX >= 0 && anewX < 10 && anewY >= 0 && anewY < 10 && playerGrid[anewY][anewX] >= 0) {
				if (playerGrid[anewY][anewX] == 0) {
					playerGrid[anewY][anewX] = -1; // Mark as missed
					cout << "Missed." << endl;
					targetMode = false; // Exit targeting mode if the end is reached
					directionConfirmation = false;
					validTargetFound = false;
				}
				else if (playerGrid[anewY][anewX] == shipValueTargetted) {
					playerGrid[anewY][anewX] = -11 + shipValueTargetted; // Mark as hit
					sizeAttack--;
					cout << "Hit." << endl;

					if (sizeAttack >= 0) {
						// Ship sunk, reset targeting mode
						targetMode = false;
						directionConfirmation = false;
						currentCount = 0;
						cout << "Ship sunk!" << endl;
					}
					else {
						attackCoordX = anewX;
						attackCoordY = anewY;
					}
				}
			}
			else {
				// Revert to random mode if targeting direction fails
				targetMode = false;
				directionConfirmation = false;
				validTargetFound = false;
				currentCount = 0;
			}
		}
	}
}

void colorGrid(RenderWindow& window, RectangleShape& rect, int currentValue, bool user) {
	if (!musicOn) {
		if (currentValue < -1) {
			rect.setFillColor(Color(222, 0, 0));
		}
		else if (currentValue == -1) {
			rect.setFillColor(Color(70, 114, 166));
		}
		else {
			rect.setFillColor(Color(25, 25, 25, 255));
		}

		if (user) {
			if (currentValue > 0) {
				rect.setFillColor(Color(67, 68, 100, 255));
			}
		}
	}
	else {
		if (currentValue < -1) {
			rect.setFillColor(Color(79, 6, 1));
		}
		else if (currentValue == -1) {
			rect.setFillColor(Color(27, 68, 212));
		}
		else {
			rect.setFillColor(Color(25, 25, 25, 255));
		}

		if (user) {
			if (currentValue > 0) {
				rect.setFillColor(Color(50, 50, 100, 255));
			}
		}
	}
	window.draw(rect);
}

bool oneAtATime(bool aimed[10][10]) {
	int count = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {

			
			if (aimed[i][j]) {
				count++;
			}

			if (count > 1) aimed[currentI][currentJ] = false;


			if (aimed[i][j]) {
				currentI = i;
				currentJ = j;
			}
		}
	}
	return count <= 1;
}

int gameEnder(int aiGrid[10][10]) {
	int userCount = 0, aiCount = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (userArray[i][j] < -1)
				userCount++;
			if (aiGrid[i][j] < -1)
				aiCount++;
		}
	}
	//return 1;
	//return 2;
	if (userCount == 17)
		return 2;
	if (aiCount == 17)
		return 1;
	return 0;
}

bool destroyedShips(RenderWindow& window, int aiGrid[10][10], Sprite& ship, int shipNo) {
	int count = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (aiGrid[i][j] == shipNo)
				count++;
		}
	}
	if (count == 0) {
		return true;
		if (destructionOn) {
			destructSound.play();
		}
	}
	return false;
}

bool handlePlayerInput(sf::RenderWindow& window, Sprite& crossHair, int otherArray[10][10], bool aimed[10][10], const sf::Vector2f& mousePos, int boxsize, int startingPointX2, int startingPointY, bool player) {
	Event aim;
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			sf::RectangleShape rect(Vector2f(boxsize, boxsize));
			rect.setPosition(Vector2f(startingPointX2 + (boxsize * j), startingPointY + (boxsize * i)));
			

			if (rect.getGlobalBounds().contains(mousePos)&& Mouse::isButtonPressed(Mouse::Left)&& otherArray[i][j] >= 0) {
				if (!aimConfirmOn) {
					playerFire(i, j, otherArray[i][j], otherArray, player);
					return true;
				}
				aimed[i][j] = true;
			}

			if (aimed[i][j] && oneAtATime(aimed)) {

				crossHair.setPosition(-100, -100);

				crossHair.setPosition(mousePos);

				crossHair.setPosition(Vector2f(rect.getGlobalBounds().left + rect.getGlobalBounds().width / 2, rect.getGlobalBounds().top + rect.getGlobalBounds().height / 2));
				
				static sf::Clock Clock;
				if (!pauseButtonPressed) {
					window.draw(crossHair);
					fireGlobal = makeButtons(window, mainFont, "FIRE", desktopsize.width / 14, (desktopsize.width / 27), (desktopsize.height / 2) + 2 - boxsize / 2, 0);

				}
				
				window.pollEvent(aim);
					if (fireGlobal.contains(mousePos) && aim.type == Event::MouseButtonPressed && aim.mouseButton.button == Mouse::Left)
					{
						cout << "inside it";
						playerFire(i, j, otherArray[i][j], otherArray, player);

						aimed[i][j] = false;
						return true;
					}
				
			}
		}
	}


	return false;
}

void renderGridScreen2(sf::RenderWindow& window, int aiGrid[10][10], int startX, int startY, float boxsize, bool isPlayerGrid, bool multi) {
	sf::RectangleShape rect(Vector2f(boxsize, boxsize));
	
	if (isPlayerGrid && !multi) {
		bool one = true, two = true, three = true, four = true, five = true;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				rect.setPosition(Vector2f(startX + (boxsize * j), startY + (boxsize * i)));

				colorGrid(window, rect, userArray[i][j], true);

				rect.setOutlineColor(Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				window.draw(rect);
				bool horizon;
				if (destroyedShips(window, userArray, airCraftCarrier, 1) && one && userArray[i][j] == -10) {
					if (userArray[i][j + 1] == -10)
						horizon = true;
					else
						horizon = false;

					one = transferShips(airCraftCarrier, startX, startY, boxsize, i, j, 274, 59, 5, horizon, 1);
				}
				else if (destroyedShips(window, userArray, battleShip, 2) && two && userArray[i][j] == -9) {
					if (userArray[i][j + 1] == -9)
						horizon = true;
					else
						horizon = false;

					two = transferShips(battleShip, startX, startY, boxsize, i, j, 219, 60, 4, horizon, 1); 

				}
				else if (destroyedShips(window, userArray, cruiser, 3) && three && userArray[i][j] == -8) {
					if (userArray[i][j + 1] == -8)
						horizon = true;
					else
						horizon = false;
					three = transferShips(cruiser, startX, startY, boxsize, i, j, 163, 59, 3, horizon, 1);
				}
				else if (destroyedShips(window, userArray, submarine, 4) && four && userArray[i][j] == -7) {
					if (userArray[i][j + 1] == -7)
						horizon = true;
					else
						horizon = false;
					four = transferShips(submarine, startX, startY, boxsize, i, j, 163, 59, 3, horizon, 1);

				}
				else if (destroyedShips(window, userArray, destroyer, 5) && five && userArray[i][j] == -6)  {
					if (userArray[i][j + 1] == -6)
						horizon = true;
					else
						horizon = false;
					five = transferShips(destroyer, startX, startY, boxsize, i, j, 109, 60, 2, horizon, 1);

				}


			}
		}
		window.draw(airCraftCarrier);
		window.draw(battleShip);
		window.draw(cruiser);
		window.draw(submarine);
		window.draw(destroyer);
	}


	else {
		bool one1 = true, two1 = true, three1 = true, four1 = true, five1 = true;

		//right grid
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				rect.setPosition(Vector2f(startX + (boxsize * j), startY + (boxsize * i)));
				colorGrid(window, rect, aiGrid[i][j], false);
				rect.setOutlineColor(Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				window.draw(rect);
				bool horizon;
				if (destroyedShips(window, aiGrid, airCraftCarrier, 1) && one1 && aiGrid[i][j] == -10) {
					if (aiGrid[i][j + 1] == -10)
						horizon = true;
					else
						horizon = false;

					one1 = transferShips(airCraftCarrier, startX, startY, boxsize, i, j, 274, 59, 5, horizon, 1);
				}
				else if (destroyedShips(window, aiGrid, battleShip, 2) && two1 && aiGrid[i][j] == -9) {
					if (aiGrid[i][j + 1] == -9)
						horizon = true;
					else
						horizon = false;

					two1 = transferShips(battleShip, startX, startY, boxsize, i, j, 219, 60, 4, horizon, 1);
					
				}
				else if (destroyedShips(window, aiGrid, cruiser, 3) && three1 && aiGrid[i][j] == -8) {
					if (aiGrid[i][j + 1] == -8)
						horizon = true;
					else
						horizon = false;
					three1 = transferShips(cruiser, startX, startY, boxsize, i, j, 163, 59, 3, horizon, 1);
				}
				else if (destroyedShips(window, aiGrid, submarine, 4) && four1 && aiGrid[i][j] == -7) {
					if (aiGrid[i][j + 1] == -7)
						horizon = true;
					else
						horizon = false;
					four1 = transferShips(submarine, startX, startY, boxsize, i, j, 163, 59, 3, horizon, 1);
				}
				else if (destroyedShips(window, aiGrid, destroyer, 5) && five1 && aiGrid[i][j] == -6) {
					if (aiGrid[i][j + 1] == -6)
						horizon = true;
					else
						horizon = false;
					five1 = transferShips(destroyer, startX, startY, boxsize, i, j, 109, 60, 2, horizon, 1);
				}

			}
		}
		window.draw(airCraftCarrier);
		window.draw(battleShip);
		window.draw(cruiser);
		window.draw(submarine);
		window.draw(destroyer);
	}
}

void hideBoats(RenderWindow& window) {
	airCraftCarrier.setPosition(Vector2f(-100, -100));
	battleShip.setPosition(Vector2f(-100, -100));
	cruiser.setPosition(Vector2f(-100, -100));
	submarine.setPosition(Vector2f(-100, -100));
	destroyer.setPosition(Vector2f(-100, -100));

	window.draw(airCraftCarrier);
	window.draw(battleShip);
	window.draw(cruiser);
	window.draw(submarine);
	window.draw(destroyer);

}

void drawBackgroundAndGrids(sf::RenderWindow& window, int aiGrid[10][10], int boxsize, int startingPointX1, int startingPointX2, int startingPointY, bool multi) {
	// Draw background
	Sprite gameBg;
	gameBg.setTexture(gameBgTexture);
	gameBg.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);
	window.draw(gameBg);

	// Render left and right grids (reuse the existing logic)
	if (!multi) {
		
		renderGridScreen2(window, userArray, startingPointX1, startingPointY, boxsize, true, 0);
		renderGridScreen2(window, aiGrid, startingPointX2, startingPointY, boxsize, false, 0);
		hideBoats(window);
	}
	else {
		renderGridScreen2(window, userArray, startingPointX1, startingPointY, boxsize, true, 1);
		renderGridScreen2(window, aiGrid, startingPointX2, startingPointY, boxsize, false, 1);
		hideBoats(window);
	}
}

void winScreen(RenderWindow& window) {
	
		Sprite youWin;
		youWin.setTexture(youWonTexture);
		youWin.setOrigin(Vector2f(youWin.getGlobalBounds().width / 2, youWin.getGlobalBounds().height / 2));
		youWin.setPosition(Vector2f(desktopsize.width / 2, desktopsize.height / 2));
		window.draw(youWin);
		writeText(window, userName, mainFont, desktopsize.width / 2, desktopsize.height / 1.78, 0, 0, 1);
		playGlobal = makeButtons(window, mainFont, "Menu", desktopsize.width / 8, desktopsize.width / 22, desktopsize.height / 1.55, desktopsize.width / 2.9);
		leaderboardGlobal = makeButtons(window, mainFont, "Enter", desktopsize.width / 8, desktopsize.width / 22, desktopsize.height / 1.55, desktopsize.width / 1.9);
		window.display();
}

void loseScreen(RenderWindow& window) {
	Sprite youLose;
	youLose.setTexture(youLostTexture);
	youLose.setOrigin(Vector2f(youLose.getGlobalBounds().width / 2, youLose.getGlobalBounds().height / 2));
	youLose.setPosition(Vector2f(desktopsize.width / 2, desktopsize.height / 2));
	window.draw(youLose);
	playGlobal = makeButtons(window, mainFont, "Menu", desktopsize.width / 8, desktopsize.width / 22, desktopsize.height / 1.15, 0);
	window.display();
}

void multiEnd(RenderWindow& window) {
	Sprite youLose;
	youLose.setTexture(multiEndTexture);
	youLose.setOrigin(Vector2f(youLose.getGlobalBounds().width / 2, youLose.getGlobalBounds().height / 2));
	youLose.setPosition(Vector2f(desktopsize.width / 2, desktopsize.height / 2));
	window.draw(youLose);
	if(multiOneWon)
		writeText(window, "1", mainFont, desktopsize.width / 1.95, desktopsize.height / 2, 0, 0, 1, true);
	else  if(multiTwoWon)
		writeText(window, "2", mainFont, desktopsize.width / 1.95, desktopsize.height / 2, 0, 0, 1, true);
	playGlobal = makeButtons(window, mainFont, "Menu", desktopsize.width / 8, desktopsize.width / 22, desktopsize.height / 1.1, 0);
}

void endScreen(RenderWindow& window, bool multi) {
	if (!multi) {
		if (didYouWin)
			winScreen(window);
		else if (didYouLose)
			loseScreen(window);
	}
	else if(multiEnded){
		multiEnd(window);
	}
}

void gamePlayScreen(RenderWindow& window, int otherArray[10][10], bool aimed[10][10], bool multiYes,int width = 10, int height = 10) {
	window.clear();


	Sprite gameBg;
	gameBg.setTexture(gameBgTexture);
	gameBg.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);


	Sprite crossHair;
	crossHair.setTexture(crossHairTexture);
	crossHair.setOrigin(Vector2f(crossHair.getGlobalBounds().width / 2, crossHair.getGlobalBounds().height / 2));
	crossHair.setPosition(-20, -20);

	window.draw(gameBg);

	float boxsize = desktopsize.width / 24.836;
	crossHair.scale(Vector2f(boxsize / 55, boxsize / 55));
	RectangleShape rect(Vector2f(boxsize, boxsize));

	int startingPointX1 = desktopsize.width / 28;
	int startingPointX2 = desktopsize.width / 1.777;
	int startingPointY = (desktopsize.height / 2) - (boxsize * 5);

	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

	drawBackgroundAndGrids(window, otherArray, boxsize, startingPointX1, startingPointX2, startingPointY, multiYes);

static sf::Clock turnClock;

	switch (turn) {
	case 0: {
		// Allow player to aim and fire
		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		if (!pauseButtonPressed && !didYouWin && !didYouLose && !multiEnded) {
			if (handlePlayerInput(window, crossHair, otherArray, aimed, mousePos, boxsize, startingPointX2, startingPointY, 0)) {
				turn = 1;
				turnClock.restart(); // Start delay timer for transition
			}
		}
		break;
	}
	case 1: {
		// Short delay after player's turn for smooth transition
		if (turnClock.getElapsedTime().asSeconds() > cpuDelayTime) {
			turn = 2;
		}
		break;
	}
	case 2: {
		// AI fires and updates the player's grid
		if (!multiYes) {
			aiFire(userArray);
			turn = 0;
		}
		else {
			if(handlePlayerInput(window, crossHair, userArray, aimed, mousePos, boxsize, startingPointX1, startingPointY, 1))
				turn = 0;
		}
		//turnClock.restart(); // Start delay timer for transition
		break;
	}
	}

	stringstream userKaScore;
	userKaScore << playerScore;
	writeText(window, userKaScore.str(), mainFont, desktopsize.width / 5.3, desktopsize.height / 1.08, 20, 0, true);
	
	stringstream aiKaScore;
	aiKaScore << aiScore;
	writeText(window, aiKaScore.str(), mainFont, desktopsize.width / 1.23, desktopsize.height / 1.08, 20, 0, true);
	if (!multiYes) {
		if (turn == 0) {
			writeText(window, "YOUR TURN", mainFont, centerAlign(desktopsize.width, 40) + 5, desktopsize.height / 14, 20, 0, true);
		}
		else {
			writeText(window, "CPU's TURN", mainFont, centerAlign(desktopsize.width, 40) + 5, desktopsize.height / 14, 20, 0, true);

		}
	}
	else {
		if (turn == 0) {
			writeText(window, "PLAYER 1", mainFont, centerAlign(desktopsize.width, 40) + 5, desktopsize.height / 14, 20, 0, true);
		}
		else {
			writeText(window, "PLAYER 2", mainFont, centerAlign(desktopsize.width, 40) + 5, desktopsize.height / 14, 20, 0, true);
		}


		writeText(window, "Player 1", mainFont, desktopsize.width / 4.8, desktopsize.height / 10, 20, 0, true);

		writeText(window, "Player 2", mainFont, desktopsize.width / 1.275 , desktopsize.height / 10, 20, 0, true);
	}

	endScreen(window, multiYes);

	if(!didYouWin)
		pauseButton(window);

	if (!multiYes) {
		if (gameEnder(otherArray) == 1) {
			didYouWin = true;
		}
		else if (gameEnder(otherArray) == 2) {
			didYouLose = true;
		}
	}
	else {
		if (gameEnder(otherArray) == 1) {
			multiEnded = multiOneWon = true;
		}
		else if (gameEnder(otherArray) == 2) {
			multiEnded = multiTwoWon = true;
		}
	}

	window.display();


}

void pauseButton(RenderWindow& window) {
	Sprite pause;
	pause.setTexture(pauseButtonTexture);
	pause.scale(1.3, 1.3);
	pause.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);
	pause.setPosition(static_cast<float>(desktopsize.width) / 96.0, static_cast<float>(desktopsize.height) / 54.0);
	//pause.setPosition(static_cast<float>(desktopsize.width) / 70.0, static_cast<float>(desktopsize.height) / 39.38);
	window.draw(pause);

	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

	Sprite dialogBox;
	dialogBox.setTexture(dialogBoxTexture);
	dialogBox.setOrigin(Vector2f(dialogBox.getGlobalBounds().width / 2, dialogBox.getGlobalBounds().height / 2));
	dialogBox.setPosition(Vector2f(desktopsize.width / 2, desktopsize.height / 2));

	if (pause.getGlobalBounds().contains(mousePos) && Mouse::isButtonPressed(Mouse::Left)) {
		
		if (clickSoundOn)
			clickSound.play();
		pauseButtonPressed = true;
	}

	if (pauseButtonPressed) {
		window.draw(dialogBox);
		pauseContinue = makeButtons(window, mainFont, "Continue", desktopsize.height / 2.7, desktopsize.height / 13.5, desktopsize.height / 2.4, 0);
		pauseMenu = makeButtons(window, mainFont, "Back to Menu", desktopsize.height / 2.7, desktopsize.height / 13.5, desktopsize.height / 1.9636, 0);
		pauseExit = makeButtons(window, mainFont, "Exit", desktopsize.height / 2.7, desktopsize.height / 13.5, desktopsize.height / 1.6615, 0);
	}
}

void leaderBoardScreen(RenderWindow& window) {

	Sprite leaderBoard;
	string first = "Placeholder1", second = "Placeholder2", third = "Placeholder3";
	leaderBoard.setTexture(LeaderBoardTexture);
	leaderBoard.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);

	//@rafeel idhr se shuru hoja

	ifstream file("leaderboardFile.txt");
	if (!file) {
		cerr << "Unable to open file." << endl;
	}

	vector<pair<string, int>> leaderboard;
	string username;
	int score;

	while (getline(file, username) && file >> score && file.ignore()) {
		leaderboard.push_back(make_pair(username, score));
	}

	// Sort the leaderboard based on scores in descending order
	sort(leaderboard.begin(), leaderboard.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
		return a.second > b.second;
		});

	// Variables to store the top three scores and usernames
	tuple<string, int> topScore, midScore, bottomScore;

	if (leaderboard.size() >= 1) {
		topScore = make_tuple(leaderboard[0].first, leaderboard[0].second);
	}
	if (leaderboard.size() >= 2) {
		midScore = make_tuple(leaderboard[1].first, leaderboard[1].second);
	}
	if (leaderboard.size() >= 3) {
		bottomScore = make_tuple(leaderboard[2].first, leaderboard[2].second);
	}


	String firstName, secondName, thirdName, firstScore, secondScore, thirdScore;
	firstName = get<0>(topScore);
	secondName = get<0>(midScore);
	thirdName = get<0>(bottomScore);

	firstName = firstName + "\t\t";
	secondName = secondName + "\t\t";
	thirdName = thirdName + "\t\t";

	firstScore = to_string(get<1>(topScore));
	secondScore = to_string(get<1>(midScore));
	thirdScore = to_string(get<1>(bottomScore));

	first = firstName + firstScore;
	second = secondName + secondScore;
	third = thirdName + thirdScore;

	/*first = tupleToString(topScore);
	second = tupleToString(midScore);
	third = tupleToString(bottomScore);*/

	//@rafeel idhr khatam hoja

	window.clear();
	window.draw(leaderBoard);
	writeText(window, first, mainFont, centerAlign(desktopsize.width, 40), desktopsize.height / 2.48, 20, 0, true);
	writeText(window, second, mainFont, centerAlign(desktopsize.width, 40), desktopsize.height / 1.7, 20, 0, true);
	writeText(window, third, mainFont, centerAlign(desktopsize.width, 40), desktopsize.height / 1.29, 20, 0, true);
	pauseButton(window);
	window.display();
}

void settings(RenderWindow& window) {
	Sprite settingIcon;
	settingIcon.setTexture(settingTexture);
	settingIcon.scale(1.3, 1.3);
	settingIcon.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);
	settingIcon.setPosition(static_cast<float>(desktopsize.width) / 70.0, static_cast<float>(desktopsize.height) / 39.38);
	window.draw(settingIcon);

	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

	if (settingIcon.getGlobalBounds().contains(mousePos) && Mouse::isButtonPressed(Mouse::Left)) {
		if(clickSoundOn)
			clickSound.play();
		settingIconPressed = true;
	}
}

void settingsMenu(RenderWindow& window) {
	Sprite settings;
	settings.setTexture(settingMenuTexture);
	settings.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);
	window.clear();
	window.draw(settings);

	string music, click, error, aimConfirm, cpuDelay, destruct;
	musicOn ? music = "ON" : music = "OFF";
	clickSoundOn ? click = "ON" : click = "OFF";
	errorSoundOn ? error = "ON" : error = "OFF";
	aimConfirmOn ? aimConfirm = "ON" : aimConfirm = "OFF";
	destructionOn ? destruct = "ON" : destruct = "OFF";
	if (cpuDelayTime == 1)
		cpuDelay = "1 sec";
	else if (cpuDelayTime == 2)
		cpuDelay = "2 sec";
	else cpuDelay = "0 sec";
	
	musicRect = makeButtons(window, mainFont, music, desktopsize.height / 10.8, desktopsize.height / 14.4, desktopsize.height / 3.323, desktopsize.width / 3);
	clickRect = makeButtons(window, mainFont, click, desktopsize.height / 10.8, desktopsize.height / 14.4, desktopsize.height / 2.018, desktopsize.width / 3);
	errorRect = makeButtons(window, mainFont, error, desktopsize.height / 10.8, desktopsize.height / 14.4, desktopsize.height / 1.449, desktopsize.width / 3);

	dectructRect = makeButtons(window, mainFont, destruct, desktopsize.height / 10.8, desktopsize.height / 14.4, desktopsize.height / 3.323, desktopsize.width / 1.25);
	aimRect = makeButtons(window, mainFont, aimConfirm, desktopsize.height / 10.8, desktopsize.height / 14.4, desktopsize.height / 2.018, desktopsize.width / 1.25);
	delayRect = makeButtons(window, mainFont, cpuDelay, desktopsize.height / 10.8, desktopsize.height / 14.4, desktopsize.height / 1.449, desktopsize.width / 1.25);
	exitGlobal = makeButtons(window, mainFont, "Back", desktopsize.height / 6.171, desktopsize.height / 15.428, desktopsize.height/1.1, 0);


	window.display();
	
}

void multiGame(RenderWindow& window, int secondArray[10][10], bool aimed[10][10]) {
	window.clear();
	if (multiGameState == 0) {
		drawBoard(window, userArray, 1, 10, 10);
		handleDrag(window, event, userArray, 1, 10, 10);
	}
	else if (multiGameState == 1) {
		drawBoard(window, secondArray, 2, 10, 10);
		handleDrag(window, event, secondArray, 2, 10, 10);
	}
	else if (multiGameState == 2)
		gamePlayScreen(window, secondArray, aimed, 1, 10, 10);
	window.display();
}