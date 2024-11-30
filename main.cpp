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
void gamePlayScreen(); 
bool readyToPlay(int array[10][10], int rows, int columns, int shipWeight);

//GlobalVariables for button coordinates, updated in makeButtons and used in handleEvents
FloatRect playGlobal;
FloatRect leaderboardGlobal;
FloatRect exitGlobal;
FloatRect shipSetPlayGlobal;

//FloatRect largestShipGlobal;
//FloatRect largeShipGlobal;
//FloatRect smallShipGlobal;
//FloatRect smallestShipGlobal;
bool shipCollision = false;
bool beingDragged = false;
bool shipInGrid = false;
bool shipInitialPos = true;
Vector2f offset;
Vector2f InitialPos;

Texture ship4, ship3, ship2, ship1;
Sprite largestShip, largeShip, smallShip, smallestShip;
Sprite* draggedShip;
Font mainFont;
Texture mainBgTexture;
Texture setShips;


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
	if (!ship4.loadFromFile("5ship.png") || !ship3.loadFromFile("4ship.png") || !ship2.loadFromFile("3ship.png") || !ship1.loadFromFile("2ship.png"))
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

	return 0;
}


//Sets the button at center of screen in accordance to width
int centerAlign(int screenWidth, int width) {
	int center = (screenWidth / 2) - (width / 2);
	return center;
}


void shipCreator() {
	largestShip.setTexture(ship4);
	largeShip.setTexture(ship3);
	smallShip.setTexture(ship2);
	smallestShip.setTexture(ship1);

	FloatRect boundsLargest = largestShip.getLocalBounds();
	FloatRect boundsLarge = largeShip.getLocalBounds();
	FloatRect boundsSmallest = smallestShip.getLocalBounds();
	FloatRect boundsSmall = smallShip.getLocalBounds();

	//setting up the ships upon their origin
	largestShip.setOrigin(boundsLargest.width/2, boundsLargest.height/2);
	largeShip.setOrigin(boundsLarge.width/2, boundsLarge.height/2);
	smallestShip.setOrigin(boundsSmallest.width/2, boundsSmallest.height/2);
	smallShip.setOrigin(boundsSmall.width/2, boundsSmall.height/2);
	

	VideoMode desktop = VideoMode::getDesktopMode();
	float x = desktop.width/25.0;
	
	float currentSizeX = desktop.width / 24.836;
	float currentSizeY = desktop.width / 30.355;
	float difference = (currentSizeX - currentSizeY) / 2;


	//Setting the current ship size
	largestShip.scale((currentSizeX - difference) / 52, currentSizeY / 52);
	largeShip.scale((currentSizeX - difference) / 52, currentSizeY / 52);
	smallestShip.scale((currentSizeX - difference) / 52, currentSizeY / 52);
	smallShip.scale((currentSizeX - difference)/ 52, currentSizeY / 52);
	
	//making ships vertical for assembly
	largestShip.setRotation(450);
	largeShip.setRotation(450);
	smallShip.setRotation(450);
	smallestShip.setRotation(450);


	largestShip.setPosition(Vector2f(x += 100, desktop.height / 2.0));
	largeShip.setPosition(Vector2f(x += 200, desktop.height / 2.0));
	smallShip.setPosition(Vector2f(x += 200, desktop.height / 2.0));
	smallestShip.setPosition(Vector2f(x += 200, desktop.height / 2.0));


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
		gamePlayScreen();
		break;
	}
}


void handleDrag(RenderWindow& window, Event& event, int array[10][10], int width, int height) {

	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));


	VideoMode desktopsize = VideoMode::getDesktopMode();
	int screenwidth = desktopsize.width;
	int screenheight = desktopsize.height;

	float boxsize = screenwidth / 24.836;
	float actualboxsize = screenwidth / 30.355;


	int startingPointX = (screenwidth / 2);
	int startingPointY = (screenheight / 2) - (boxsize * 5);


	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {


		if (largestShip.getGlobalBounds().contains(mousePos)) {
			draggedShip = &largestShip;
		}
		else if (largeShip.getGlobalBounds().contains(mousePos)) {
			draggedShip = &largeShip;
		}
		else if (smallShip.getGlobalBounds().contains(mousePos)) {
			draggedShip = &smallShip;
		}
		else if (smallestShip.getGlobalBounds().contains(mousePos)) {
			draggedShip = &smallestShip;
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
				}
				else
					draggedShip->setPosition(InitialPos);

				shipInitialPos = true;
				beingDragged = false;
				draggedShip = nullptr;
			}
		}



	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right) {

		Sprite* selectedShip = nullptr;
		int initialRotation;

		//determine which ship to rotate
		if (largestShip.getGlobalBounds().contains(mousePos)) {
			selectedShip = &largestShip;
		}
		else if (largeShip.getGlobalBounds().contains(mousePos)) {
			selectedShip = &largeShip;
		}
		else if (smallShip.getGlobalBounds().contains(mousePos)) {
			selectedShip = &smallShip;
		}
		else if (smallestShip.getGlobalBounds().contains(mousePos)) {
			selectedShip = &smallestShip;
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

			}

		}


	}

}


//Handles all the input Events
int handleEvents(RenderWindow& window, int array[10][10], int screenManager){
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::KeyPressed && event.key.code ==  Keyboard::Escape)
			window.close();

		if (screenManager == 1)
		{
			handleDrag(window, event, array, 10, 10);
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2i mousePos = Mouse::getPosition(window);
				if (shipSetPlayGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y)))) {
					if (readyToPlay(array, 10, 10, 14))
						screenManager = 2;
				}

			}
		}

		if (screenManager == 0) {
		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
			Vector2i mousePos = Mouse::getPosition(window);
				if (exitGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					window.close();
				else if (playGlobal.contains((static_cast<float>(mousePos.x)), (static_cast<float>(mousePos.y))))
					screenManager = 1;
			}
			
		}
	}
	return screenManager;
}


//Displays the Main Menu
void drawMainScreen(RenderWindow& window, Texture&  mainBgTexture, Font& mainFont) {

	Sprite mainBg;
	mainBg.setTexture(mainBgTexture);
	window.clear();
	window.draw(mainBg);

	VideoMode desktop = VideoMode::getDesktopMode();
	int buttonWidth = desktop.width / 5, buttonHeight = buttonWidth / 4, topButton = desktop.height / 2.2, buttonDistance = desktop.height / 6;
	playGlobal = makeButtons(window, mainFont, "PLAY", buttonWidth, buttonHeight, topButton, 0);
	topButton += buttonDistance;
	leaderboardGlobal = makeButtons(window, mainFont,"LEADERBOARD", buttonWidth, buttonHeight, topButton, 0);
	topButton += buttonDistance;
	exitGlobal = makeButtons(window, mainFont, "EXIT", buttonWidth, buttonHeight, topButton, 0);

	window.display();
}


//Called in the makeButtons Function. Displays and Aligns the Text
void writeText(RenderWindow& window, string name, Font& mainFont, int horizontal, int vertical, int width, int height) {

	Text text;
	text.setFont(mainFont);
	text.setStyle(Text::Bold);
	text.setCharacterSize(45);
	text.setFillColor(Color(0, 0, 50)); //Bluish-Black
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
		VideoMode desktop = VideoMode::getDesktopMode();
		horizontal = centerAlign(desktop.width, width);
	}

	RectangleShape button(Vector2f(width, height));
	button.setPosition(Vector2f(horizontal, vertical));
	button.setOutlineThickness(1);
	button.setFillColor(Color(240, 235, 255));
	button.setOutlineColor(Color::Black);
	window.draw(button);
	//writing the text
	writeText(window, name, mainFont, horizontal, vertical, width, height);
	return button.getGlobalBounds();
}

bool readyToPlay(int array[10][10], int rows, int columns, int shipWeight) {

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
	window.draw(setShipBg);

	VideoMode desktopsize = VideoMode::getDesktopMode();
	int screenwidth = desktopsize.width;
	int screenheight = desktopsize.height;

	float boxsize = screenwidth / 24.836;
	RectangleShape rect(Vector2f(boxsize, boxsize));

	int startingPointX = screenwidth / 2;
	int startingPointY = (screenheight / 2) - (boxsize * 5);

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

	processShip(largestShip, 1);
	processShip(largeShip, 2);
	processShip(smallShip, 3);
	processShip(smallestShip, 4);

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

	window.draw(largestShip);
	window.draw(largeShip);
	window.draw(smallShip);
	window.draw(smallestShip);

	shipSetPlayGlobal = makeButtons(window, mainFont, "PLAY", desktopsize.width / 5, desktopsize.width / 20, 900, (desktopsize.width / 4) - (desktopsize.width / 10));
	window.display();

	return true;
}


void gamePlayScreen() {
	cout << "Here I am";
}
