#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;


//function definitions:

void openWindow(RenderWindow& window, Texture& mainBgTexture, Font& mainFont,int array[10][10]);
int loadShips();
int handleEvents(RenderWindow& window, int array[10][10], int screenManager);
void drawMainScreen(RenderWindow& window, Texture& mainBgTexture, Font& mainFont);
FloatRect makeButtons(RenderWindow& window, Font& mainFont, string name, int width, int height, int vertical, int horizontal);
void screenDecide(RenderWindow& window, Texture& mainBgTexture, Font& mainFont, int array[10][10], int screenManager);
bool drawBoard(RenderWindow& window, int array[10][10], int height = 10, int width = 10);
void handleDrag(RenderWindow& window, Event& event, int array[10][10], int width, int height);

//GlobalVariables for button coordinates, updated in makeButtons and used in handleEvents
FloatRect playGlobal;
FloatRect leaderboardGlobal;
FloatRect exitGlobal;

//FloatRect largestShipGlobal;
//FloatRect largeShipGlobal;
//FloatRect smallShipGlobal;
//FloatRect smallestShipGlobal;

bool beingDragged = false;
Vector2f offset;

Texture ship4, ship3, ship2, ship1;
Sprite largestShip, largeShip, smallShip, smallestShip;
Sprite* draggedShip;
Font mainFont;
Texture mainBgTexture;
Texture setShips;

int main() {


	
	//To implement Full Screen
	VideoMode desktop = VideoMode::getDesktopMode();
	RenderWindow window(desktop, "BATTLESHIPS", Style::Fullscreen);

	//Loading Textures & Fonts
	
	if (!mainBgTexture.loadFromFile("MainMenuFinalBg.png"))
	{
		return 0;
	}
	
	if (!setShips.loadFromFile("Set Ships.png"))
	{
		return 0;
	}

	if (!mainFont.loadFromFile("Paul-le1V.ttf"))
	{
		return 0;
	}

	if (loadShips() == -1)
		return 0;
	
	
	int array[10][10] = {0};
	//opens the main menu
	openWindow(window, mainBgTexture, mainFont, array);

}

int loadShips() {
	if (!ship4.loadFromFile("5ship.png") || !ship3.loadFromFile("4ship.png") || !ship2.loadFromFile("3ship.png") || !ship1.loadFromFile("2ship.png"))
	{
		cout << "Ships not Loaded";
		return -1;
	}
	return 0;
}

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
	largestShip.scale((currentSizeX - difference) / 50, currentSizeY / 50);
	largeShip.scale((currentSizeX - difference) / 50, currentSizeY / 50);
	smallestShip.scale((currentSizeX - difference) / 50, currentSizeY / 50);
	smallShip.scale((currentSizeX - difference)/ 50, currentSizeY / 50);
	
	//making ships vertical for assembly
	largestShip.setRotation(90);
	largeShip.setRotation(90);
	smallShip.setRotation(90);
	smallestShip.setRotation(90);


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
	}
}

void handleDrag(RenderWindow& window, Event& event, int array[10][10], int width, int height) {

	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

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

	if (beingDragged && draggedShip && event.type == Event::MouseMoved) {
		draggedShip->setPosition(mousePos - offset);
	}
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
		beingDragged = false;
		draggedShip = nullptr;
	}
	drawBoard(window, array);
}

//Handles all the input Events
int handleEvents(RenderWindow& window, int array[10][10], int screenManager){
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::KeyPressed && event.key.code ==  Keyboard::Escape)
			window.close();

		if (screenManager == 1)
			handleDrag(window, event, array, 10, 10);

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

//Used in the makeButtons Function. Sets the button at center of screen

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

void drawShip(RenderWindow& window){
	window.draw(largestShip);
	window.draw(largeShip);
	window.draw(smallShip);
	window.draw(smallestShip);

}

bool drawBoard(RenderWindow& window, int array[10][10], int height, int width) {

	window.clear();

	Sprite setShipBg;
	setShipBg.setTexture(setShips);

	window.draw(setShipBg);

	// Finding out the center of the window and its size
	VideoMode desktopsize = VideoMode::getDesktopMode();
	int screenwidth = desktopsize.width;
	int screenheight = desktopsize.height;

	float actualboxsize = screenwidth / 30.355 ;
	RectangleShape rect(Vector2f(actualboxsize, actualboxsize));

	float boxsize = screenwidth / 24.836;
	int startingPointX = screenwidth / 2; // (screenwidth * 3 / 4) - (boxsize * 5);
	int startingPointY = (screenheight / 2) - (boxsize * 5);

	// Drawing our grid
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			rect.setPosition(Vector2f(startingPointX + (boxsize * j), startingPointY + (boxsize * i)));
			if (array[i][j] == 0) {
				rect.setFillColor(Color(25, 25, 25, 255));
			}
			rect.setOutlineColor(Color(255, 255, 255, 255));
			rect.setOutlineThickness(1);
			window.draw(rect);

		}
	}

	drawShip(window);
	makeButtons(window, mainFont, "PLAY", desktopsize.width / 5, desktopsize.width / 20, 900, (desktopsize.width / 4) - (desktopsize.width / 10));
	window.display();
	return true;
}