#include <iostream>
#include<cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;


//function definitions:

void openWindow(RenderWindow& window, Texture& mainBgTexture, Font& mainFont,int array[10][10]);
int loadEverything();
int handleEvents(RenderWindow& window, int array[10][10], int screenManager);
void drawMainScreen(RenderWindow& window, Texture& mainBgTexture, Font& mainFont);
FloatRect makeButtons(RenderWindow& window, Font& mainFont, string name, int width, int height, int vertical, int horizontal);
void screenDecide(RenderWindow& window, Texture& mainBgTexture, Font& mainFont, int array[10][10], int screenManager);
bool drawBoard(RenderWindow& window, int array[10][10], int height = 10, int width = 10);
void handleDrag(RenderWindow& window, Event& event, int array[10][10], int width, int height);
void gamePlayScreen(RenderWindow& window, int array[10][10], int width, int height);
bool readyToPlay(int array[10][10], int rows, int columns, int shipWeight);
void pauseButton(RenderWindow& window);

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
bool one = true, two = true, three = true, four = true, five = true;
int currentI = 0, currentJ = 0;
int prevState = 0;
Vector2f offset;
Vector2f InitialPos;

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

VideoMode desktopsize = VideoMode::getDesktopMode();

Event event;

SoundBuffer clickSoundBuffer;
SoundBuffer errorSoundBuffer;
Sound clickSound;
Sound errorSound;




//JUST to show the grid in the form of array on console
void shipsInArray(int array[10][10], int rows = 10, int columns = 10) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cout << array[i][j] << " ";
		}
		cout << endl;
	}
}


int main() {


	
	//To implement Full Screen
	VideoMode desktop = VideoMode::getDesktopMode();
	RenderWindow window(desktop, "BATTLESHIPS", Style::Fullscreen);

	//Loading Textures & Fonts
	
	if (loadEverything() == -1)
		return 0;
	
	
	int array[10][10] = {0};

	//opens the main menu
	openWindow(window, mainBgTexture, mainFont, array);


	shipsInArray(array);
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

	return 0;
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
void openWindow(RenderWindow& window, Texture& mainBgTexture,  Font& mainFont, int array[10][10]) {
	int screenManager = 0;
	shipCreator();
	while (window.isOpen()) {
		screenDecide(window, mainBgTexture, mainFont, array, screenManager);
		screenManager = handleEvents(window, array, screenManager);
	}
}


void screenDecide(RenderWindow& window, Texture& mainBgTexture, Font& mainFont, int array[10][10], int screenManager) {
	switch (screenManager)
	{
	case 0:
		drawMainScreen(window, mainBgTexture, mainFont);
		break;
	case 1:
		drawBoard(window, array, 10, 10);
		break;
	case 2:
		gamePlayScreen(window, array, 10, 10);
		break;
	}
}


void handleDrag(RenderWindow& window, Event& event, int array[10][10], int width, int height) {

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


			drawBoard(window, array);

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
int handleEvents(RenderWindow& window, int array[10][10], int screenManager){
	while (window.pollEvent(event)) {

		Vector2i mousePos = Mouse::getPosition(window);

		if (event.type == Event::KeyPressed && event.key.code ==  Keyboard::Escape)
			window.close();

		if (screenManager == 1)
		{
			handleDrag(window, event, array, 10, 10);
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
						clickSound.play();
					}
				}
				else {
					if (shipSetPlayGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y)))) {
						if (readyToPlay(array, 10, 10, 17))
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
void writeText(RenderWindow& window, string name, Font& mainFont, int horizontal, int vertical, int width, int height, bool isClicked) {

	Text text;
	text.setFont(mainFont);
	text.setStyle(Text::Bold);
	text.setCharacterSize(desktopsize.height / 24);
	text.setFillColor(Color(0, 0, 50));//Bluish-Black

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
			isClicked = true;
		}
	}
	window.draw(button);
	//writing the text
	writeText(window, name, mainFont, horizontal, vertical, width, height, isClicked);
	
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

bool readyToPlay(int array[10][10], int rows, int columns, int shipWeight) {


	//return true;


	int nonZero = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (array[i][j] != 0) {
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

bool drawBoard(RenderWindow& window, int array[10][10], int height, int width) {
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
					if (array[i][j] == 0) {
						array[i][j] = shipValue;
					}
					else if (array[i][j] != shipValue) {
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

	//resetting the array
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			array[i][j] = 0;
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
			rect.setFillColor(array[i][j] == 0 ? Color(25, 25, 25, 255) : Color(50, 50, 100, 255));
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

	shipSetPlayGlobal = makeButtons(window, mainFont, "PLAY", desktopsize.width / 5, desktopsize.width / 20, desktopsize.height / 1.2 , (desktopsize.width / 4) - (desktopsize.width / 10));
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

void gamePlayScreen(RenderWindow& window, int array[10][10], int width = 10, int height = 10) {
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


	//left grid
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			rect.setPosition(Vector2f(startingPointX1 + (boxsize * j), startingPointY + (boxsize * i)));

				/*if (rect.getGlobalBounds().contains(mousePos) && Mouse::isButtonPressed(Mouse::Left)) {
					
				}*/
				if (array[i][j] == 0) {
					rect.setFillColor(Color(25, 25, 25, 255));
				}
				else if (array[i][j] == -1) {
					rect.setFillColor(Color(255, 0, 50));
				}
				else {
					rect.setFillColor(Color(50, 50, 100, 255));
				}
			
			
			rect.setOutlineColor(Color(255, 255, 255, 255));
			rect.setOutlineThickness(1);
			window.draw(rect);
			if (array[i][j] == 1 && one) {
				one = transferShips(airCraftCarrier, startingPointX1, startingPointY, boxsize, i, j, 274, 59, 5);
			}
			if (array[i][j] == 2 && two) {
				two = transferShips(battleShip, startingPointX1, startingPointY, boxsize, i, j, 219, 60, 4);
			}
			if (array[i][j] == 3 && three) {
				three = transferShips(cruiser, startingPointX1, startingPointY, boxsize, i, j, 163, 59, 3);
			}
			if (array[i][j] == 4 && four) {
				four = transferShips(submarine, startingPointX1, startingPointY, boxsize, i, j, 163, 59, 3);
			}
			if (array[i][j] == 5 && five) {
				five = transferShips(destroyer, startingPointX1, startingPointY, boxsize, i, j, 109, 60, 2);
			}


			
		}
	
	}
	window.draw(airCraftCarrier);
	window.draw(battleShip);
	window.draw(cruiser);
	window.draw(submarine);
	window.draw(destroyer);

	//right grid
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			rect.setPosition(Vector2f(startingPointX2 + (boxsize * j), startingPointY + (boxsize * i)));
			rect.setFillColor(Color(25, 25, 25, 255));
			rect.setOutlineColor(Color(255, 255, 255, 255));
			rect.setOutlineThickness(1);
			window.draw(rect);

			if (rect.getGlobalBounds().contains(mousePos) && Mouse::isButtonPressed(Mouse::Left)) {

				if (currentI >= 0) {
					array[currentI][currentJ] = prevState;/////////////////////////////////////
				}
				currentI = i; currentJ = j;
				prevState = array[i][j];///////////////////
				array[i][j] = 10;////////////////////

			}

			if (array[i][j] == 10) {
				crossHair.setPosition(mousePos);

				crossHair.setPosition(Vector2f(rect.getGlobalBounds().left + rect.getGlobalBounds().width / 2, rect.getGlobalBounds().top + rect.getGlobalBounds().height / 2));
			}

		}
	}
	window.draw(crossHair);
	pauseButton(window);
	
	fireGlobal = makeButtons(window, mainFont, "FIRE", desktopsize.width / 8.6, desktopsize.width / 23, desktopsize.height / 1.148, 0);

	window.display();
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