#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include<cmath>
#include <sstream>
#include <thread> // For simulating resource loading
#include <chrono>
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
int handleEvents(RenderWindow& window, int screenManager);
void drawMainScreen(RenderWindow& window, Texture& mainBgTexture, Font& mainFont);
FloatRect makeButtons(RenderWindow& window, Font& mainFont, string name, int width, int height, int vertical, int horizontal);
void screenDecide(RenderWindow& window, Texture& mainBgTexture, Font& mainFont, int aiGrid[10][10], bool aimed[10][10], int screenManager);
bool drawBoard(RenderWindow& window, int height = 10, int width = 10);
void handleDrag(RenderWindow& window, Event& event, int width, int height);
void gamePlayScreen(RenderWindow& window, int aiGrid[10][10], bool aimed[10][10], int width, int height);
bool readyToPlay(int rows, int columns, int shipWeight);
void pauseButton(RenderWindow& window);
void leaderBoardScreen(RenderWindow& window);
void mainLoadingScreen(RenderWindow& window);

//GlobalVariables for button coordinates, updated in makeButtons and used in handleEvents
FloatRect playGlobal;
FloatRect leaderboardGlobal;
FloatRect exitGlobal;
FloatRect shipSetPlayGlobal;
FloatRect pauseContinue;
FloatRect pauseMenu;
FloatRect pauseExit;
FloatRect fireGlobal;


//FloatRect largestShipGlobal;
//FloatRect largeShipGlobal;
//FloatRect smallShipGlobal;
//FloatRect smallestShipGlobal;
bool shipCollision = false;
bool beingDragged = false;
bool shipInGrid = false;
bool shipInitialPos = true;
bool pauseButtonPressed = false;
bool oneTime = true;
int currentI = 0, currentJ = 0;
int prevState = 0, newState = 0;
bool playerTurn = true;

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

Texture airCraftCarrierTexture, battleShipTexture, submarineTexture, cruiserTexture, destroyerTexture;
Sprite airCraftCarrier, battleShip, submarine, cruiser, destroyer;
Sprite* draggedShip;
Font mainFont;
Texture mainBgTexture;
Texture setShips;
Texture gameBgTexture;
Texture crossHairTexture;
Texture pauseButtonTexture;
Texture dialogBoxTexture;
Texture LeaderBoardTexture;
Texture mainLoading;

VideoMode desktopsize = VideoMode::getDesktopMode();

Event event;

SoundBuffer clickSoundBuffer;
SoundBuffer errorSoundBuffer;
Sound clickSound;
Sound errorSound;

int userArray[10][10] = { 0 };




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

	cout << "Player Score = " << playerScore << "AI Score : " << aiScore << endl;
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
		screenManager = handleEvents(window, screenManager);
	}
}


void screenDecide(RenderWindow& window, Texture& mainBgTexture, Font& mainFont, int aiGrid[10][10], bool aimed[10][10], int screenManager) {
	switch (screenManager)
	{
	case 0:
		drawMainScreen(window, mainBgTexture, mainFont);
		break;
	case 1:
		drawBoard(window, 10, 10);
		break;
	case 2:
		gamePlayScreen(window, aiGrid, aimed, 10, 10);
		break;
	case 3:
		leaderBoardScreen(window);
		break;
	}
}


void handleDrag(RenderWindow& window, Event& event, int width, int height) {

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

					clickSound.play();
				}
				else
				{
					draggedShip->setPosition(InitialPos);
					errorSound.play();
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


			drawBoard(window);

			if (shipCollision) {

				selectedShip->setRotation(initialRotation);

				selectedShip->setPosition(InitialPos);

				errorSound.play();

			}
			else
				clickSound.play();

		}
		

	}

}

//Handles all the input Events
int handleEvents(RenderWindow& window, int screenManager) {
	while (window.pollEvent(event)) {

		Vector2i mousePos = Mouse::getPosition(window);

		if (event.type == Event::KeyPressed && event.key.code ==  Keyboard::Escape)
			window.close();

		if (screenManager == 1)
		{
			if (!pauseButtonPressed) {
				handleDrag(window, event, 10, 10);
			}
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (pauseButtonPressed) {
					if (pauseExit.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						clickSound.play();
						window.close();
						
					}
					if (pauseContinue.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						pauseContinue = temp;
						pauseButtonPressed = false;
						clickSound.play();
					}
					if (pauseMenu.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						FloatRect temp;
						pauseMenu = temp;
						screenManager = 0;
						pauseButtonPressed = false;
						resetShips();
						//generate new grid here
						clickSound.play();
					}
				}
				else {
					if (shipSetPlayGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y)))) {
						if (readyToPlay(10, 10, 17))
						{
							FloatRect temp;
							shipSetPlayGlobal = temp;
							clickSound.play();
							screenManager = 2;
						}
						else
							errorSound.play();
					}
				}

			}
		}

		if (screenManager == 0) {
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (exitGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					window.close();
					clickSound.play();
				}
				else if (playGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					FloatRect temp;
					playGlobal = temp;
					screenManager = 1;
					clickSound.play();
				}
				else if (leaderboardGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					FloatRect temp;
					playGlobal = temp;
					screenManager = 3;
					clickSound.play();
				}
			}
			
		}

		if (screenManager == 2) {
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (pauseButtonPressed) {
					if (pauseExit.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						window.close();
						clickSound.play();
					}
					if (pauseContinue.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						pauseButtonPressed = false;
						clickSound.play();
					}
					if (pauseMenu.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					{
						screenManager = 0;
						pauseButtonPressed = false;
						resetShips();
						clickSound.play();
					}
				}
			}
		}

		if (screenManager == 3)
		{
			}
		if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
			if (pauseButtonPressed) {
				if (pauseExit.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					clickSound.play();
					window.close();

				}
				if (pauseContinue.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					FloatRect temp;
					pauseContinue = temp;
					pauseButtonPressed = false;
					clickSound.play();
				}
				if (pauseMenu.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
				{
					FloatRect temp;
					pauseMenu = temp;
					screenManager = 0;
					pauseButtonPressed = false;
					clickSound.play();
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

	int buttonWidth = desktopsize.width / 5, buttonHeight = buttonWidth / 4, topButton = desktopsize.height / 2.2, buttonDistance = desktopsize.height / 6;
	playGlobal = makeButtons(window, mainFont, "PLAY", buttonWidth, buttonHeight, topButton, 0);
	topButton += buttonDistance;
	leaderboardGlobal = makeButtons(window, mainFont, "LEADERBOARD", buttonWidth, buttonHeight, topButton, 0);
	topButton += buttonDistance;
	exitGlobal = makeButtons(window, mainFont,  "EXIT", buttonWidth, buttonHeight, topButton, 0);

	window.display();
}

//Called in the makeButtons Function. Displays and Aligns the Text
void writeText(RenderWindow& window, string name, Font& mainFont, int horizontal, int vertical, int width, int height, bool leaderBoard) {

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
			clickSound.play();
		}
	}*/



	return button.getGlobalBounds();
}

  //////////////////////////////////////////////
 //////////////////////////////////////////////
//////////////////////////////////////////////

bool readyToPlay(int rows, int columns, int shipWeight) {


	return true;


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

bool drawBoard(RenderWindow& window, int height, int width) {
	window.clear();


	shipCollision = false; 
	shipInGrid = false;   

	Sprite setShipBg;
	setShipBg.setTexture(setShips);
	setShipBg.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);
	window.draw(setShipBg);


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
					if (userArray[i][j] == 0) {
						userArray[i][j] = shipValue;
					}
					else if (userArray[i][j] != shipValue) {
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
			userArray[i][j] = 0;
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
			rect.setFillColor(userArray[i][j] == 0 ? Color(25, 25, 25, 255) : Color(50, 50, 100, 255));
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
		shipSetPlayGlobal = makeButtons(window, mainFont, "PLAY", desktopsize.width / 5, desktopsize.width / 20, desktopsize.height / 1.2, (desktopsize.width / 4) - (desktopsize.width / 10));
	}
	pauseButton(window);

	window.display();

	return true;
}

bool transferShips(Sprite& ship, int startingPointX, int startingPointY, float boxsize, int i, int j, int width, int height, int blocks) {
	
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

void playerFire(int x, int y, int aiGridValue, int aiGrid[10][10]) {
	cout << "here i am";
	if (aiGridValue < 0) {
		cout << "Try again"; // invalid input handle
	}
	if (aiGridValue == 0) {
		aiGrid[x][y] = -1;
		cout << "Missed!";// replace box with red coloration
		playerScore--;
	}
	else if (aiGridValue == 1) {
		cout << "-10!";
		aiGrid[x][y] = -10; // destroyer value replacement
	}
	else if (aiGridValue == 2) {
		aiGrid[x][y] = -9; // warship
	}
	else if (aiGridValue == 3) {
		aiGrid[x][y] = -8; // sub
	}
	else if (aiGridValue == 4) {
		aiGrid[x][y] = -7; // battleship
	}
	else if (aiGridValue == 5) {
		aiGrid[x][y] = -6; // carrier idk
	}
}


void aiFire(int playerGrid[10][10]) {

		srand(static_cast<unsigned>(time(0)));
		int x = rand() % 10;
		int y = rand() % 10;
		while (playerGrid[y][x] < 0) { // invalid input handle
			x = rand() % 10;
			y = rand() % 10;
		}
		if (targetMode == false) {
			if (playerGrid[y][x] == 0) {
				playerGrid[y][x] = -1;
				aiScore--;
				// replace box with red coloration
			}
			if (playerGrid[y][x] == 1) {
				playerGrid[y][x] = -10; // airCraftCarrier value replacement
				attackCoordX = x;
				attackCoordY = y;
				targetMode = true;
				sizeAttack = 5 - 1;
				shipValueTargetted = 1;
			}
			if (playerGrid[y][x] == 2) {
				playerGrid[y][x] = -9; // warship
				attackCoordX = x;
				attackCoordY = y;
				targetMode = true;
				sizeAttack = 4 - 1;
				shipValueTargetted = 2;
			}
			if (playerGrid[y][x] == 3) {
				playerGrid[y][x] = -8; // battleship
				attackCoordX = x;
				attackCoordY = y;
				targetMode = true;
				sizeAttack = 3 - 1;
				shipValueTargetted = 3;
			}
			if (playerGrid[y][x] == 4) {
				playerGrid[y][x] = -7; // submarine
				attackCoordX = x;
				attackCoordY = y;
				targetMode = true;
				sizeAttack = 3 - 1;
				shipValueTargetted = 4;
			}
			if (playerGrid[y][x] == 5) {
				playerGrid[y][x] = -6; // destroyer
				attackCoordX = x;
				attackCoordY = y;
				targetMode = true;
				sizeAttack = 2 - 1;
				shipValueTargetted = 5;
			}
		}
		if (targetMode == true) {
			if (directionConfirmation == false) {

				if (currentCount == 0) {
					attackCoordY--;
					if (playerGrid[attackCoordY][attackCoordX] == shipValueTargetted) { // handles if the block is "correct"
						directionConfirmation = true;
						directionToTarget = 1; // sets the direction to be north.
						currentCount = 0; // resetting the targetting process
						playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted; // works for all types of ship values being targetted
						sizeAttack--; //decrements the size of ships being attacked for this specific one.
						cout << "Hit.";
					}
					else if (playerGrid[attackCoordY][attackCoordX] == 0) { // handles if the block is empty and its missed
						directionConfirmation = false;
						playerGrid[attackCoordY][attackCoordX] = -1;
						attackCoordY++;
						cout << "Miss.";
					}
					else if (playerGrid[attackCoordY][attackCoordX] < 0) { // handles if the block is already hit or its missed.
						currentCount++;
						aiFire(userArray);
					}
					else if (playerGrid[attackCoordY][attackCoordX] > 0 && playerGrid[attackCoordY][attackCoordX] != shipValueTargetted) {
						int temp = playerGrid[attackCoordY][attackCoordX]; // handles if the block is filled with a ship of other type
						playerGrid[attackCoordY][attackCoordX] = -11 + temp;
						currentCount++;
					}
					else if (attackCoordY < 0) { // handles if the coordinate is out of bounds.
						currentCount++;
						aiFire(userArray);
					}
				}

				if (currentCount == 1) {
					attackCoordX++; // to check the right side
					attackCoordY++; // to undo the decrement in previous block

					if (playerGrid[attackCoordY][attackCoordX] == shipValueTargetted) { // handles if the block is "correct"
						directionConfirmation = true;
						directionToTarget = 2; // sets the direction to be east.
						currentCount = 0; // resetting the targetting process
						playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted; // works for all types of ship values being targetted
						sizeAttack--; //decrements the size of ships being attacked for this specific one.
						cout << "Hit.";
					}
					else if (playerGrid[attackCoordY][attackCoordX] == 0) { // handles if the block is empty and its missed
						directionConfirmation = false;
						playerGrid[attackCoordY][attackCoordX] = -1;
						attackCoordX--;
						cout << "Miss.";
					}
					else if (playerGrid[attackCoordY][attackCoordX] < 0) { // handles if the block is already hit or its missed.
						currentCount++;
						aiFire(userArray);
					}
					else if (playerGrid[attackCoordY][attackCoordX] > 0 && playerGrid[attackCoordY][attackCoordX] != shipValueTargetted) {
						int temp = playerGrid[attackCoordY][attackCoordX]; // handles if the block is filled with a ship of other type
						playerGrid[attackCoordY][attackCoordX] = -11 + temp;
						currentCount++;
					}
					else if (attackCoordX > 9) { // handles if the coordinate is out of bounds.
						currentCount++;
						aiFire(userArray);
					}
				}

				if (currentCount == 2) {
					attackCoordX--; //undoes the previous increment in the currentcount==1 block
					attackCoordY++; // now checks if the direction is in the south.
					if (playerGrid[attackCoordY][attackCoordX] == shipValueTargetted) { // handles if the block is "correct"
						directionConfirmation = true;
						directionToTarget = 3; // sets the direction to be south.
						currentCount = 0; // resetting the targetting process
						playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted; // works for all types of ship values being targetted
						sizeAttack--; //decrements the size of ships being attacked for this specific one.
						cout << "Hit.";
					}
					else if (playerGrid[attackCoordY][attackCoordX] == 0) { // handles if the block is empty and its missed
						directionConfirmation = false;
						playerGrid[attackCoordY][attackCoordX] = -1;
						attackCoordY--;
						cout << "Miss.";
					}
					else if (playerGrid[attackCoordY][attackCoordX] < 0) { // handles if the block is already hit or its missed.
						currentCount++;
						aiFire(userArray);
					}
					else if (playerGrid[attackCoordY][attackCoordX] > 0 && playerGrid[attackCoordY][attackCoordX] != shipValueTargetted) {
						int temp = playerGrid[attackCoordY][attackCoordX]; // handles if the block is filled with a ship of other type
						playerGrid[attackCoordY][attackCoordX] = -11 + temp;
						currentCount++;
					}
					else if (attackCoordY > 9) { // handles if the coordinate is out of bounds.
						currentCount++;
						aiFire(userArray);
					}
				}

				if (currentCount == 3) {
					attackCoordY--; //undoes the previous increment in the currentcount==2 block
					attackCoordX--; // now checks if the direction is in the west
					if (playerGrid[attackCoordY][attackCoordX] == shipValueTargetted) { // handles if the block is "correct"
						directionConfirmation = true;
						directionToTarget = 4; // sets the direction to be south.
						currentCount = 0; // resetting the targetting process
						playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted; // works for all types of ship values being targetted
						sizeAttack--; //decrements the size of ships being attacked for this specific one.
						cout << "Hit.";
					}
					else if (playerGrid[attackCoordY][attackCoordX] == 0) { // handles if the block is empty and its missed
						directionConfirmation = false;
						playerGrid[attackCoordY][attackCoordX] = -1;
						attackCoordX++;
						cout << "Miss.";
					}
					else if (playerGrid[attackCoordY][attackCoordX] < 0) { // handles if the block is already hit or its missed.
						currentCount++;
						aiFire(userArray);
					}
					else if (playerGrid[attackCoordY][attackCoordX] > 0 && playerGrid[attackCoordY][attackCoordX] != shipValueTargetted) {
						int temp = playerGrid[attackCoordY][attackCoordX]; // handles if the block is filled with a ship of other type
						playerGrid[attackCoordY][attackCoordX] = -11 + temp;
						currentCount++;
					}
					else if (attackCoordX < 0) { // handles if the coordinate is out of bounds.
						currentCount++;
						aiFire(userArray);
					}
				}
			}
			if (sizeAttack == 0) {
				directionConfirmation = false;
				targetMode = false;
				cout << shipValueTargetted << " has been completely destroyed.\n";
			}
			if (directionConfirmation == true) {
				if (directionToTarget == 1) {
					attackCoordY--;
					playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted;
					sizeAttack--;
				}
				if (directionToTarget == 2) {
					attackCoordX++;
					playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted;
					sizeAttack--;
				}
				if (directionToTarget == 3) {
					attackCoordY++;
					playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted;
					sizeAttack--;
				}
				if (directionToTarget == 4) {
					attackCoordX--;
					playerGrid[attackCoordY][attackCoordX] = -11 + shipValueTargetted;
					sizeAttack--;
				}
				if (sizeAttack == 0) {
					targetMode = false;
					directionConfirmation = false;
				}
			}
		}
}

void colorGrid(RenderWindow& window, RectangleShape& rect, int currentValue, bool user) {
	if (currentValue < -1) {
		rect.setFillColor(Color(110, 0, 0));
	}
	else if (currentValue == -1) {
		rect.setFillColor(Color(70, 120, 160));
	}
	else {
		rect.setFillColor(Color(25, 25, 25, 255));
	}

	if (user) {
		if (currentValue > 0) {
			rect.setFillColor(Color(50, 50, 100, 255));
		}
	}
	window.draw(rect);
}

bool oneAtATime(bool aimed[10][10]) {
	int count = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (aimed[i][j]) count++;
			if (count > 1) aimed[i][j] = false; // Reset excess
		}
	}
	return count <= 1;
}

bool gameEnder(int aiGrid[10][10]) {
	int userCount = 0, aiCount = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (userArray[i][j] < -1)
				userCount++;
			if (aiGrid[i][j] < -1)
				aiCount++;
		}
	}
	if (userCount == 17 || aiCount == 17)
		return true;
	return false;
}

bool destroyedShips(RenderWindow& window, int aiGrid[10][10], Sprite& ship, int shipNo) {
	int count = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (aiGrid[i][j] == shipNo)
				count++;
		}
	}
	if (count == 0) 
		return true;
	return false;
}

void gamePlayScreen(RenderWindow& window, int aiGrid[10][10], bool aimed[10][10], int width = 10, int height = 10) {
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

	if (oneTime) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				aiGrid[i][j] = userArray[i][j];
			}
		}

		oneTime = false;
	}

	if (!playerTurn) {
		aiFire(userArray);

		playerTurn = true;
		sf::Clock clock;
		while (clock.getElapsedTime().asSeconds() < 1.0f) {
		}
	}

	bool one = true, two = true, three = true, four = true, five = true;
	//left grid
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			rect.setPosition(Vector2f(startingPointX1 + (boxsize * j), startingPointY + (boxsize * i)));

			colorGrid(window, rect, userArray[i][j], true);

			rect.setOutlineColor(Color(255, 255, 255, 255));
			rect.setOutlineThickness(1);
			window.draw(rect);
			if ((userArray[i][j] == 1 || userArray[i][j] == -10) && one) {
				one = transferShips(airCraftCarrier, startingPointX1, startingPointY, boxsize, i, j, 274, 59, 5);
			}
			if ((userArray[i][j] == 2 || userArray[i][j] == -9) && two) {
				two = transferShips(battleShip, startingPointX1, startingPointY, boxsize, i, j, 219, 60, 4);
			}
			if ((userArray[i][j] == 3 || userArray[i][j] == -8) && three) {
				three = transferShips(cruiser, startingPointX1, startingPointY, boxsize, i, j, 163, 59, 3);
			}
			if ((userArray[i][j] == 4 || userArray[i][j] == -7) && four) {
				four = transferShips(submarine, startingPointX1, startingPointY, boxsize, i, j, 163, 59, 3);
			}
			if ((userArray[i][j] == 5 || userArray[i][j] == -6) && five) {
				five = transferShips(destroyer, startingPointX1, startingPointY, boxsize, i, j, 109, 60, 2);
			}


			
		}
	
	}

	window.draw(airCraftCarrier);
	window.draw(battleShip);
	window.draw(cruiser);
	window.draw(submarine);
	window.draw(destroyer);


	bool one1 = true, two1 = true, three1 = true, four1 = true, five1 = true;

	//right grid
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			rect.setPosition(Vector2f(startingPointX2 + (boxsize * j), startingPointY + (boxsize * i)));
			colorGrid(window, rect, aiGrid[i][j], false);
			rect.setOutlineColor(Color(255, 255, 255, 255));
			rect.setOutlineThickness(1);
			window.draw(rect);

			if (destroyedShips(window, aiGrid, airCraftCarrier, 1) && one1 && aiGrid[i][j] == -10) {
				one1 = transferShips(airCraftCarrier, startingPointX2, startingPointY, boxsize, i, j, 274, 59, 5);
			}
			if (destroyedShips(window, aiGrid, battleShip, 2) && two1 && aiGrid[i][j] == -9) {
				two1 = transferShips(battleShip, startingPointX2, startingPointY, boxsize, i, j, 274, 59, 5);
			}
			if (destroyedShips(window, aiGrid, cruiser, 3) && three1 && aiGrid[i][j] == -8) {
				three1 = transferShips(cruiser, startingPointX2, startingPointY, boxsize, i, j, 274, 59, 5);
			}
			if (destroyedShips(window, aiGrid, submarine, 4) && four1 && aiGrid[i][j] == -7) {
				four1 = transferShips(submarine, startingPointX2, startingPointY, boxsize, i, j, 274, 59, 5);
			}
			if (destroyedShips(window, aiGrid, destroyer, 5) && five1 && aiGrid[i][j] == -6) {
				five1 = transferShips(destroyer, startingPointX2, startingPointY, boxsize, i, j, 274, 59, 5);
			}

			if (playerTurn && rect.getGlobalBounds().contains(mousePos) && Mouse::isButtonPressed(Mouse::Left) && aiGrid[i][j] >= 0 && !pauseButtonPressed) {
				aimed[i][j] = true;
			}

			oneAtATime(aimed);

			if (aimed[i][j] && oneAtATime(aimed)) {

				crossHair.setPosition(mousePos);

				crossHair.setPosition(Vector2f(rect.getGlobalBounds().left + rect.getGlobalBounds().width / 2, rect.getGlobalBounds().top + rect.getGlobalBounds().height / 2));

				if (fireGlobal.contains(mousePos) && Mouse::isButtonPressed(Mouse::Left)) {
					cout << "inside it";
					playerFire(i, j, aiGrid[i][j], aiGrid);

					aimed[i][j] = false;
					playerTurn = false;
				}
			}
		}
	}

	window.draw(airCraftCarrier);
	window.draw(battleShip);
	window.draw(cruiser);
	window.draw(submarine);
	window.draw(destroyer);

	
	
	
	if (!pauseButtonPressed && playerTurn) {
		window.draw(crossHair);
		fireGlobal = makeButtons(window, mainFont, "FIRE", desktopsize.width / 8.6, desktopsize.width / 23, desktopsize.height / 1.148, 0);
	}

	stringstream userKaScore;
	userKaScore << playerScore;
	writeText(window, userKaScore.str(), mainFont, desktopsize.width / 5.3, desktopsize.height / 1.08, 20, 0, true);
	
	stringstream aiKaScore;
	aiKaScore << aiScore;
	writeText(window, aiKaScore.str(), mainFont, desktopsize.width / 1.23, desktopsize.height / 1.08, 20, 0, true);

	if (playerTurn) {
		writeText(window, "YOUR TURN", mainFont, centerAlign(desktopsize.width, 40) + 5, desktopsize.height / 14, 20, 0, true);
	}
	else {
		writeText(window, "CPU's TURN", mainFont, centerAlign(desktopsize.width, 40) + 5, desktopsize.height / 14, 20, 0, true);

	}
	pauseButton(window);

	window.display();

	if (gameEnder(aiGrid))
		window.close();

}

void pauseButton(RenderWindow& window) {
	Sprite pause;
	pause.setTexture(pauseButtonTexture);
	pause.scale(1.3, 1.3);
	pause.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);
	pause.setPosition(static_cast<float>(desktopsize.width) / 96.0, static_cast<float>(desktopsize.height) / 54.0);
	window.draw(pause);

	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

	Sprite dialogBox;
	dialogBox.setTexture(dialogBoxTexture);
	dialogBox.setOrigin(Vector2f(dialogBox.getGlobalBounds().width / 2, dialogBox.getGlobalBounds().height / 2));
	dialogBox.setPosition(Vector2f(desktopsize.width / 2, desktopsize.height / 2));

	if (pause.getGlobalBounds().contains(mousePos) && Mouse::isButtonPressed(Mouse::Left)) {
		
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

	Sprite leaderboard;
	string first = "Placeholder1", second = "Placeholder2", third = "Placeholder3";
	leaderboard.setTexture(LeaderBoardTexture);
	leaderboard.scale(static_cast<float>(desktopsize.width) / 1920.0, static_cast<float>(desktopsize.height) / 1080.0);
	window.clear();
	window.draw(leaderboard);
	writeText(window, first, mainFont, centerAlign(desktopsize.width, 40), desktopsize.height / 2.48, 20, 0, true);
	writeText(window, second, mainFont, centerAlign(desktopsize.width, 40), desktopsize.height / 1.7, 20, 0, true);
	writeText(window, third, mainFont, centerAlign(desktopsize.width, 40), desktopsize.height / 1.29, 20, 0, true);
	pauseButton(window);
	window.display();
}
