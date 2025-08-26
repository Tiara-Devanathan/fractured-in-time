#include "GraphicsManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
//for the correct, need for formatting float
#include <iomanip> // For std::setprecision
#include <sstream> // For std::ostringstream

using namespace std;

Graphics::Graphics(){

	//Set the window size according to user's desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	
	window.create(desktop ,"Fractured_In_Time", sf::Style::Fullscreen);
	loadFont("pixel");

	//store dimensions
	winWidth = static_cast<float>(window.getSize().x);
	winHeight = static_cast<float>(window.getSize().y);
	float buttonWidth = winWidth * 0.15f;  // 15% of screen width
	float buttonHeight = winHeight * 0.05f; // 5% of screen height

	//Back button
	//Create the button shape
	backButton.setSize(sf::Vector2f(buttonWidth,buttonHeight));
	backButton.setFillColor(sf::Color::White);
	backButton.setPosition(winWidth * 0.05f, winHeight * 0.85f); // 5% from left, 85% from top

	//Create the button text
	sf::Text backButtonText;
	backButtonText.setFont(font);
	backButtonText.setString("Back");
	backButtonText.setCharacterSize(static_cast<unsigned int>(winHeight * 0.03f)); // 3% of height;
	backButtonText.setFillColor(sf::Color::Black);
	// Center the text inside the button
	sf::FloatRect textBounds = backButtonText.getLocalBounds();
	backButtonText.setOrigin(textBounds.left + textBounds.width / 2.0f,
		textBounds.top + textBounds.height / 2.0f);
	backButtonText.setPosition(
		backButton.getPosition().x + buttonWidth / 2.0f,
		backButton.getPosition().y + buttonHeight / 2.0f
	);

	// Save button text 
	this->backButtonText = backButtonText;

	hWnd = window.getSystemHandle();

	//Stores the relevant sounds and images in the buffers
	if (!buffer.loadFromFile("Resources/sounds/click.mp3")) {
		throw runtime_error("Failed to load sound");
	}
	sound.setBuffer(buffer);
	
	if (!muteTexture.loadFromFile("Resources/images/icon/mute.png")) {
		throw std::runtime_error("Failed to load mute image");
	}
	if (!unmuteTexture.loadFromFile("Resources/images/icon/unmute.png")) {
		throw std::runtime_error("Failed to load unmute image");
	}
	muteButton.setTexture(&unmuteTexture);
	if (!exitQuizButtonTexture.loadFromFile("Resources/images/icon/exit.png")) {
		throw std::runtime_error("Failed to load exit image");
	}
	exitQuizButton.setTexture(&exitQuizButtonTexture);


}
void Graphics:: drawWelcomeScreen() {
	loadBackground(0);
	window.clear();
	window.draw(bgSprite);
	// Define button size as a percentage of window dimensions
	float buttonWidth = winWidth * 0.25f;  // 25% of window width
	float buttonHeight = winHeight * 0.07f; // 7% of window height
	float buttonY = winHeight * 0.7f;       // 70% down the screen

	//Create the button shape
	enterButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
	enterButton.setFillColor(sf::Color(255, 255, 255, 100));
	sf::FloatRect buttonBounds = enterButton.getLocalBounds();
	enterButton.setOrigin(buttonBounds.left + buttonBounds.width / 2.0f, 0);
	enterButton.setPosition(winWidth / 2.0f, buttonY);
	window.draw(enterButton);

	
	//Create the button text
	sf::Text buttonText;
	buttonText.setFont(font);
	buttonText.setString("Enter");
	float charSize = static_cast<unsigned int>(buttonHeight * 0.7f);
	buttonText.setCharacterSize(charSize);
	buttonText.setFillColor(sf::Color::Black);
	sf::FloatRect buttonTextBounds = buttonText.getLocalBounds();
	buttonText.setOrigin(buttonTextBounds.left + buttonTextBounds.width / 2.0f, 0);
	buttonText.setPosition(winWidth / 2.0f, buttonY);
	window.draw(buttonText);
	window.display();

}

void Graphics:: drawMenu() {
	loadBackground(2);
	window.clear();
	window.draw(bgSprite);

	// Set button size and spacing relative to window
	sf::Vector2f buttonSize(winWidth * 0.2f, winHeight * 0.07f); // 20% width, 7% height
	float buttonSpacing = winHeight * 0.02f; // 2% vertical spacing
	float centerX = winWidth / 2.0f;
	float startY = winHeight * 0.4f; // Start position at 40% of screen height

	float charSize = static_cast<unsigned int>(buttonSize.y * 0.6f);

	// Play Button
	playButton.setSize(buttonSize);
	playButton.setFillColor(sf::Color(255,255,255,100));
	sf::FloatRect playBounds = playButton.getLocalBounds();
	playButton.setOrigin(playBounds.width / 2.0f, 0);
	playButton.setPosition(centerX, startY);
	window.draw(playButton);

	sf::Text playText;
	playText.setFont(font);
	playText.setString("Play");
	playText.setCharacterSize(charSize);
	playText.setFillColor(sf::Color::Black);
	sf::FloatRect playTextBounds = playText.getLocalBounds();
	playText.setOrigin(playTextBounds.width / 2.0f, 0);
	playText.setPosition(centerX, startY);
	window.draw(playText);

	// Credits Button
	creditsButton.setSize(buttonSize);
	creditsButton.setFillColor(sf::Color(255, 255, 255, 100));
	sf::FloatRect creditsBounds = creditsButton.getLocalBounds();
	creditsButton.setOrigin(creditsBounds.width / 2.0f, 0);
	creditsButton.setPosition(centerX, startY + buttonSize.y + buttonSpacing);
	window.draw(creditsButton);

	sf::Text creditsText;
	creditsText.setFont(font);
	creditsText.setString("Credits");
	creditsText.setCharacterSize(charSize);
	creditsText.setFillColor(sf::Color::Black);
	sf::FloatRect creditsTextBounds = creditsText.getLocalBounds();
	creditsText.setOrigin(creditsTextBounds.width / 2.0f, 0);
	creditsText.setPosition(centerX, startY + buttonSize.y + buttonSpacing);
	window.draw(creditsText);

	// HowTo Button
	howToButton.setSize(buttonSize);
	howToButton.setFillColor(sf::Color(255, 255, 255, 100));
	sf::FloatRect helpBounds = howToButton.getLocalBounds();
	howToButton.setOrigin(helpBounds.width / 2.0f, 0);
	howToButton.setPosition(centerX, startY + 2 * (buttonSize.y + buttonSpacing));
	window.draw(howToButton);

	sf::Text helpText;
	helpText.setFont(font);
	helpText.setString("How to play?");
	helpText.setCharacterSize(charSize);
	helpText.setFillColor(sf::Color::Black);
	sf::FloatRect helpTextBounds = helpText.getLocalBounds();
	helpText.setOrigin(helpTextBounds.width / 2.0f, 0);
	helpText.setPosition(centerX, startY + 2 * (buttonSize.y + buttonSpacing));
	window.draw(helpText);

	//Exit button
	exitButton.setSize(buttonSize);
	exitButton.setFillColor(sf::Color(255, 255, 255, 100));
	sf::FloatRect exitButtonBounds = exitButton.getLocalBounds();
	exitButton.setOrigin(exitButtonBounds.width / 2.0f, 0);
	exitButton.setPosition(centerX, startY + 3 * (buttonSize.y + buttonSpacing));
	window.draw(exitButton);

	sf::Text exitText;
	exitText.setFont(font);
	exitText.setString("Exit");
	exitText.setCharacterSize(charSize);
	exitText.setFillColor(sf::Color::Black);
	sf::FloatRect exitTextBounds = exitText.getLocalBounds();
	exitText.setOrigin(exitTextBounds.width / 2.0f, 0);
	exitText.setPosition(centerX, startY + 3 * (buttonSize.y + buttonSpacing));
	window.draw(exitText);

	window.display();
}

void Graphics::drawAdditionalButtons(){
	// Dynamically size buttons (~5% of height)
	float buttonSize = winHeight * 0.05f;
	float padding = winWidth * 0.01f;

	// Exit button (top-left)
	exitQuizButton.setSize(sf::Vector2f(buttonSize, buttonSize));
	exitQuizButton.setPosition(padding, padding);

	// Mute button (top-right)
	muteButton.setSize(sf::Vector2f(buttonSize, buttonSize));
	muteButton.setPosition(winWidth - buttonSize - padding, padding);

	// Draw buttons
	window.draw(exitQuizButton);
	window.draw(muteButton);
}
void Graphics::drawTimer(float time) {
	// Dynamically size character (~4% of window height)
	unsigned int charSize = static_cast<unsigned int>(winHeight * 0.04f);

	sf::Text timerText;
	timerText.setFont(font);
	timerText.setString("Time left: " + std::to_string(static_cast<int>(time)) + "s");
	timerText.setCharacterSize(charSize);
	timerText.setFillColor(sf::Color::Green);

	// Center horizontally, position near top (e.g. 2% from top)
	sf::FloatRect bounds = timerText.getLocalBounds();
	timerText.setOrigin(bounds.left + bounds.width / 2.0f, 0);
	timerText.setPosition(winWidth / 2.0f, winHeight * 0.02f);

	window.draw(timerText);
}

void Graphics:: loadFont(string fontName) {
	if (!font.loadFromFile("Resources/fonts/"+fontName+".ttf")) {
		throw runtime_error("Failed to load font");
	}
}
void Graphics:: playSound(std::string name) {
	if (!buffer.loadFromFile("Resources/sounds/" + name + ".mp3")) {
		cout << "No sound attached to question." << endl;
		sound.resetBuffer();
	}

	else {
		cout << "Playing sound: " << name << endl;
		sound.setBuffer(buffer);
		sound.play();
	}

}

//Checks if audio is in the sounds folder and store in backgroundMusic
void Graphics::playBackgroundMusic(string musicName) {
	if (!backgroundMusic.openFromFile("Resources/sounds/" + musicName + ".mp3")) {
		throw runtime_error("Failed to load background music");
	}
	backgroundMusic.setLoop(true);
	backgroundMusic.play();
}

//Checks if picture is in the images/backgroundImage folder and store in backgroundMusic
bool Graphics::loadBackground(int QID) {
	clear();
	
	string imagePath = "Resources/images/backgroundImage/" + to_string(QID) + ".png";
	if (!bgTexture.loadFromFile(imagePath)) {
		return false;
	}
	else {
		bgSprite.setTexture(bgTexture);

		// Get the size of the texture (image)
		sf::Vector2u textureSize = bgTexture.getSize();

		// Calculate scale factors to fit the sprite to the window
		float scaleX = winWidth / textureSize.x;
		float scaleY = winHeight / textureSize.y;

		// Apply the scaling to the sprite
		bgSprite.setScale(sf::Vector2f(scaleX, scaleY));
		return true;
	}
}
void Graphics::drawBackground(){
	window.draw(bgSprite);;
}

bool Graphics::isOpen() {
	return window.isOpen();
}

void Graphics::clear() {
	window.clear();
}

void Graphics::display() {
	window.display();
}

void Graphics::close() {
	window.close();
}

//returns a wrapped version of the input text
string Graphics::wrapText(const std::string& text, sf::Font& font, unsigned int charSize, float maxWidth, float startX) {
	//creates a stream-like object from the string text, which allows you to read the string word by word
	istringstream iss(text);
	string word, line, result;
	sf::Text tester("", font, charSize);

	tester.setPosition(startX, 0);  // account for initial x offset

	float availableWidth = maxWidth - startX;

	while (iss >> word) {
		string testLine = line + (line.empty() ? "" : " ") + word;
		tester.setString(testLine);

		//If this line exceeds max width, add to result
		if (tester.getLocalBounds().width > availableWidth) {
			if (!line.empty()) {
				result += line + "\n";
				line = word;
			}
			else {
				// Word is too long even for an empty line: force break
				result += word + "\n";
				line.clear();
			}
			
		}
		else {
			line = testLine;
		}
	}
	if (!line.empty()) {
		result += line;
	}

	return result;

}

/*This method is responsible for drawing a quiz question on the SFML window. It receives:
questionText: the actual question as a string.
questionID: a unique identifier used to load a related image if present.
questionHasImage: a flag indicating if the question has an associated image.*/
void Graphics::drawQuestion(const std::string& questionText, int questionID, bool questionHasImage) {
	//store the provided question information in member variables for later use 
	currentQuestionText = questionText;
	currentQuestionID = questionID;
	hasImage = questionHasImage;

	// Dynamically sized character size and padding
	unsigned int charSize = static_cast<unsigned int>(winHeight * 0.035f); //font size will be 3.5% of height
	float sidePadding = winWidth * 0.02f;//left and right margin (2% of window width).
	float topPadding = winHeight * 0.08f;//space from the top of the window.
	float boxWidth = winWidth - 2 * sidePadding;//total width available for text after padding.

	sf::Text text;
	//wrap question text to fit inside allowed box width
	std::string wrapped = wrapText(questionText, font, charSize, boxWidth, sidePadding);
	text.setFont(font);
	text.setString(wrapped);
	text.setCharacterSize(charSize);
	text.setFillColor(textColor);
	text.setPosition(sidePadding, topPadding);

	sf::FloatRect textBounds = text.getGlobalBounds();

	// Dynamically sized background box based on text height
	sf::RectangleShape textBox(sf::Vector2f(boxWidth, textBounds.height + winHeight * 0.02f));
	textBox.setTexture(&questionTexture);
	textBox.setPosition(sidePadding, topPadding);
	window.draw(textBox);

	window.draw(text);

	// Offset for content below question text
	offSet = topPadding + textBounds.height + winHeight * 0.4f;

	// Draw image if present
	if (hasImage) {
		//Images will have a smaller offset
		offSet = topPadding + textBounds.height + winHeight * 0.08f;
		std::string imagePath = "Resources/images/hasImage/" + std::to_string(questionID) + ".png";
		if (!questionImageTexture.loadFromFile(imagePath)) {
			throw std::runtime_error("Failed to load image for question " + std::to_string(questionID));
		}

		questionImageSprite.setTexture(questionImageTexture);

		// Scale image to 30% of window width
		float targetWidth = winWidth * 0.3f;
		float scaleFactor = targetWidth / questionImageTexture.getSize().x;

		questionImageSprite.setScale(scaleFactor, scaleFactor);

		// Center image horizontally
		float imageWidth = questionImageTexture.getSize().x * scaleFactor;
		float xPos = (winWidth - imageWidth) / 2.0f;

		questionImageSprite.setPosition(xPos, offSet);
		window.draw(questionImageSprite);

		offSet += questionImageSprite.getGlobalBounds().height + winHeight * 0.03f; // update offset
	}
}

/*This method draws all answer buttons for a multiple-choice question.
It receives:
answersText: the text labels for each answer option.
answerIndicies: the corresponding index for each answer.
answersCorrectness: indicates whether each answer is correct (> 0 means correct).*/
void Graphics::drawAnswers(const vector<string>& answersText, const vector<int>& answerIndicies, 
	const vector<int>& answersCorrectness) {
	buttons.clear();// remove all elements from vector buttons

	float buttonWidth = winWidth * 0.4f;
	float buttonHeight = winHeight * 0.06f;
	float buttonSpacing = buttonHeight * 0.3f;
	unsigned int charSize = static_cast<unsigned int>(buttonHeight * 0.4f);

	//Checks if all buttons would overflow past 95% of the screen height.
	//If so, it halves the height of buttons to make everything fit vertically.
	if ((offSet + (buttonHeight + buttonSpacing) * answersText.size()) > winHeight * 0.95) {
		buttonHeight /= 2;
	}

	//Loop through each answer option
	for (int i = 0; i < answersText.size(); ++i) {
		//Horizontally center the button by calculating its x position.
		float xPos = (winWidth - buttonWidth) / 2.0f;

		// Button
		sf::RectangleShape buttonShape(sf::Vector2f(buttonWidth, buttonHeight));
		buttonShape.setTexture(&answerTexture);
		buttonShape.setPosition(xPos, offSet);

		// Text
		sf::Text buttonText;
		std::string wrapped = wrapText(answersText[i], font, charSize, buttonWidth, buttonHeight);
		buttonText.setFont(font);
		buttonText.setString(wrapped);
		buttonText.setCharacterSize(charSize);
		buttonText.setFillColor(answerColor);
		buttonText.setPosition(xPos + 10, offSet + 5);

		//Construct a button struct
		//Add the button to the buttons vector for interaction purposes
		Button b = { buttonShape, buttonText, answerIndicies[i], (answersCorrectness[i] > 0) };
		buttons.push_back(b);

		window.draw(buttonShape);
		window.draw(buttonText);

		//Update the offSet so the next button appears below the current one
		offSet += buttonHeight + buttonSpacing;

	}
	
}


//returns the index of the selected answer
//handles click of all buttons on the Quiz screen
int Graphics::handleClick() {
	sf::Event event;

	while (window.pollEvent(event)) {
		//.pollEvent() : Handle events without blocking the program
		if (event.type == sf::Event::Closed) {
			window.close();
			return -1;
		}
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			
			sf::Vector2f mousePos(
				static_cast<float>(event.mouseButton.x),
				static_cast<float>(event.mouseButton.y)
			);
			// Check mute button
			if (muteButton.getGlobalBounds().contains(mousePos)) {
				playSound("mute");
				//backgroundMusic.getStatus() == sf::SoundSource::Paused
				if (isMuted) {
					// Unmute
					backgroundMusic.play();
					muteButton.setTexture(&unmuteTexture);
				}
				else {
					// Mute
					backgroundMusic.pause();
					muteButton.setTexture(&muteTexture);
				}
				
				isMuted = !isMuted;
				renderCurrentScreen();
				continue; // Continue waiting for a proper answer click
			}

			//Check close button
			if (exitQuizButton.getGlobalBounds().contains(mousePos)) {
				playSound("click");
				close();
				exit(0);
			}

			//Check answer button
			for (Button& b : buttons) {
				if (b.shape.getGlobalBounds().contains(mousePos)) {
					playSound("click");

					if (b.isCorrect) {
						b.shape.setFillColor(sf::Color::Green);
						playSound("correct"); 
					}
					else {
						b.shape.setFillColor(sf::Color::Red);
						playSound("wrong");
					}
					
					renderCurrentScreen();
					sf::sleep(sf::milliseconds(250)); // half-second delay

					return b.answerIndex;
				}
			}
		}
	}
	return -1;
}

void Graphics::setButtonColor(int r, int g, int b) {
	buttonColor = sf::Color(r, g, b);
}

void Graphics::setTextColor(int r, int g, int b) {
	textColor = sf::Color(r, g, b);
}

//This method redraws the current current screen when updates occur
void Graphics::renderCurrentScreen() {
	window.clear();
	window.draw(bgSprite);
	window.draw(muteButton);
	window.draw(exitQuizButton);

	drawQuestion(currentQuestionText, currentQuestionID, hasImage);

	for (const Button& b : buttons) {
		window.draw(b.shape);
		window.draw(b.text);
	}
	drawProgressBar((int)(currentQuestionID / 100) - 1);
	window.display();
}

/*This method draws a horizontal progress bar at the bottom of the screen.
levelIndex is used to determine the color scheme (for different game levels or themes).*/
void Graphics::drawProgressBar(int levelIndex)
{
	//Create three rectangle shapes:
	sf::RectangleShape background;//the inner area behind the filling
	sf::RectangleShape filling;//the actual progress portion representing completion.
	sf::RectangleShape border;//the outer border of the progress bar.

	int thickness = 2;
	int height = 0;
	int width = 0;

	
	int xPos = 0;//Sets the horizontal position to start at the left edge.
	int yPos = winHeight- winHeight / 40;//Sets the vertical position so the bar is near the bottom, with a height of 1/40th of the window.
	width = winWidth;//The bar spans the entire width of the screen
	height = winHeight - yPos;//height is the distance from yPos to the bottom of the window

	border.setPosition(xPos, yPos);
	border.setSize(sf::Vector2f(width, height));
	border.setFillColor(storedProgressBar.levelColours[levelIndex][0]);

	background.setPosition(xPos + thickness, yPos + thickness);
	background.setSize(sf::Vector2f(width - 2 * thickness, height - 2 * thickness));
	background.setFillColor(storedProgressBar.levelColours[levelIndex][1]);

	filling.setPosition(xPos + thickness, yPos + thickness);
	filling.setSize(sf::Vector2f((width - 2 * thickness) * storedProgressBar.getFullness(), height - 2 * thickness));
	filling.setFillColor(storedProgressBar.levelColours[levelIndex][2]);

	window.draw(border);
	window.draw(background);
	window.draw(filling);
}

void Graphics::setProgressBar(ProgressBar& progressBar)
{
	storedProgressBar = progressBar;
}

void Graphics::resetCredits() {
	credits.clear();
	creditsClock.restart();

	std::vector<std::string> members = {
		"Fractured in Time",
		"",
		"Created by:",
		"Erika Naidoo",
		"Mahir Syed",
		"Prashantan Naidoo",
		"Reyasen Naicker",
		"Staesha Sankar",
		"Tiara Devanathan",
		"Xander Links",
		"Yash Ramraj",
		"Zuriel Singh",
		"",
		"The End"
	};

	int charSize = static_cast<unsigned int>(winHeight * 0.035f); // 3.5% of height
	float lineSpacing = winHeight * 0.06f; // 6% of height between lines
	float startY = winHeight * 0.4f; // Start around 40% down the screen

	//Iterates through all member strings and initializes a new sf::Text object for each.
	//Sets font, content, character size, and color(black).
	for (int i = 0; i < members.size(); ++i) {
		sf::Text text;
		text.setFont(font);
		text.setString(members[i]);
		text.setCharacterSize(charSize);
		text.setFillColor(sf::Color::Black);

		// Center horizontally
		sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(bounds.left + bounds.width / 2.0f, 0);
		text.setPosition(winWidth / 2.0f, startY + i * lineSpacing);

		credits.push_back(text);
	}

}
/*This method renders the animated scrolling credits*/
void Graphics::drawCredits() {
	//Calculates the time elapsed since the last frame using the creditsClock.
	//This deltaTime is used to ensure smooth, frame rate - independent scrolling.
	//The clock is also restarted here to measure the time until the next frame.
	float deltaTime = creditsClock.restart().asSeconds();

	loadBackground(1);
	window.draw(bgSprite);

	// Move all text upward
	for (auto& text : credits) {
		//Moves each line of text upward on the screen at a speed of 50 pixels per second
		//scaled by deltaTime for smooth animation.
		text.move(0, -50 * deltaTime);
		window.draw(text);
	}
	window.draw(backButton);
	window.draw(backButtonText);
	window.display();
}


/*This method presents a score summary screen after the game ends, showing:
Individual level scores with corresponding images and names.
Final total score.
Player's overall rank.
A "Back" button for navigation*/
void Graphics::drawScoreScreen(vector<int> scores, int finalScore, string rank, float totalCorrect) {
	loadBackground(1);
	drawBackground();

	// Get window size
	sf::Vector2u windowSize = window.getSize();
	float winWidth = static_cast<float>(windowSize.x);
	float winHeight = static_cast<float>(windowSize.y);

	// Title
	sf::Text title("Score Summary:", font, winWidth * 0.05f); // 5% of width for font size
	title.setFillColor(sf::Color::Black);
	title.setPosition(winWidth * 0.35f, winHeight * 0.05f);   // 35% across, 5% down
	window.draw(title);

	// Load and draw images
	sf::Texture levelImages[3];
	sf::Sprite levelSprites[3];

	// Names for each level
	std::string levelNames[3] = { "Egypt", "Rome", "Night City" };

	float startX = winWidth * 0.15f; // 15% from left
	float spacingX = winWidth * 0.25f; // 25% between images
	float imageY = winHeight * 0.2f; // 20% from top
	for (int i = 0; i < 3; i++) {
		levelImages[i].loadFromFile("Resources/images/scoreScreen/" + std::to_string(i) + ".png");
		levelSprites[i].setTexture(levelImages[i]);

		// Scale images relative to window size
		float desiredWidth = winWidth * 0.20f;  // 15% of window width
		float desiredHeight = winHeight * 0.20f; // 15% of window height
		sf::Vector2u texSize = levelImages[i].getSize();
		levelSprites[i].setScale(desiredWidth / texSize.x, desiredHeight / texSize.y);

		levelSprites[i].setPosition(startX + i * spacingX, imageY);
		window.draw(levelSprites[i]);

		// Level Name
		sf::Text levelText(levelNames[i], font, winWidth * 0.02f);
		levelText.setFillColor(sf::Color::Black);
		levelText.setPosition(startX + i * spacingX, imageY + desiredHeight + winHeight * 0.02f);
		window.draw(levelText);

		// Level Score
		sf::Text scoreText("Score: " + std::to_string(scores[i]), font, winWidth * 0.02f);
		scoreText.setFillColor(sf::Color::Black);
		scoreText.setPosition(startX + i * spacingX, imageY + desiredHeight + winHeight * 0.06f);
		window.draw(scoreText);
	}


	// Total Score and Rank
	sf::Text totalScoreText("Total Score: " + std::to_string(finalScore), font, winWidth * 0.03f);
	totalScoreText.setFillColor(sf::Color::Black);
	totalScoreText.setPosition(winWidth * 0.1f, winHeight * 0.65f);
	window.draw(totalScoreText);


	std::ostringstream stream;
	stream << std::fixed << std::setprecision(2) << totalCorrect; // Format to 2 decimal places
	sf::Text totalPercentageText("Total Correct: " + stream.str() + "%", font, winWidth * 0.03f);

	//sf::Text totalPercentageText("Total Correct: " + std::to_string(totalCorrect), font, winWidth * 0.03f);
	totalPercentageText.setFillColor(sf::Color::Black);
	totalPercentageText.setPosition(winWidth * 0.1f, winHeight * 0.7f);
	window.draw(totalPercentageText);

	sf::Text rankText("Rank: " + rank, font, winWidth * 0.03f);
	rankText.setFillColor(sf::Color::Black);
	rankText.setPosition(winWidth * 0.1f, winHeight * 0.75f);
	window.draw(rankText);

	window.draw(backButton);
	window.draw(backButtonText);

	window.display();
}

/*This method displays a story screen between game levels.
It reads a story text file corresponding to the current level.
Renders the lines on the screen along with a prompt to continue.*/
void Graphics::drawStoryScreen(int level) {
	loadBackground(1);  // Load the background for the help screen
	window.draw(bgSprite);

	// Dynamic sizing
	int charSize = static_cast<unsigned int>(winHeight * 0.035f); // ~3.5% of height
	float sidePadding = winWidth * 0.05f;
	float topStart = winHeight * 0.25f; // Start story text at 25% of screen height
	float lineSpacing = winHeight * 0.06f;

	string path = "Resources/text/" + to_string(level) + ".txt";//build path
	ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open text" << std::endl;
		return;
	}

	string line;
	int lineNumber = 0;
	vector<sf::Text> storyLines;
	//Reads each line from the story file.
	//Each line is wrapped to fit the screen width and converted into an sf::Text object with appropriate formatting.
	//Text is positioned with line spacing to simulate paragraphs
	//Each line is added to the storyLines vector for later drawing.
	while (std::getline(file, line)) {
		sf::Text text;
		text.setFont(font);
		text.setString(wrapText(line, font, charSize, winWidth - 2 * sidePadding, lineSpacing));
		text.setCharacterSize(charSize);
		text.setFillColor(sf::Color::Black);
		text.setPosition(sidePadding, topStart + lineNumber * lineSpacing);
		storyLines.push_back(text);
		++lineNumber;
	}
	file.close();

	// Draw all story lines
	for (auto& text : storyLines) {
		window.draw(text);
	}

	// Draw "Press Enter to Continue"
	sf::Text continueText;
	continueText.setFont(font);
	continueText.setString("Press Enter to Continue");
	continueText.setCharacterSize(charSize);
	continueText.setFillColor(sf::Color::Black);

	// Center horizontally and place near bottom
	sf::FloatRect bounds = continueText.getLocalBounds();
	continueText.setPosition((winWidth - bounds.width) / 2, winHeight - charSize * 2.5f);
	window.draw(continueText);

	window.display();
	
}

/*This function displays an instructions screen to the player.
It reads text from a file and renders it, along with a message prompting the user to press Enter to return*/
void Graphics::drawHelpScreen() {
	loadBackground(1);  // Load and draw the background
	window.draw(bgSprite);

	// Dynamic sizing
	int charSize = static_cast<unsigned int>(winHeight * 0.035f); // ~3.5% of screen height
	float sidePadding = winWidth * 0.05f;
	float topStart = winHeight * 0.15f;
	float lineSpacing = winHeight * 0.06f;

	// Open help file
	ifstream file("Resources/text/help.txt");
	if (!file.is_open()) {
		cerr << "Failed to open help.txt" << std::endl;
		return;
	}

	string line;
	int lineNumber = 0;
	vector<sf::Text> helpTextLines;

	while (getline(file, line)) {
		sf::Text text;
		text.setFont(font);
		text.setString(wrapText(line, font, charSize, winWidth - 2 * sidePadding, lineSpacing));
		text.setCharacterSize(charSize);
		text.setFillColor(sf::Color::Black);
		text.setPosition(sidePadding, topStart + lineNumber * lineSpacing);
		helpTextLines.push_back(text);
		++lineNumber;
	}
	file.close();

	// Draw the help text lines
	for (auto& text : helpTextLines) {
		window.draw(text);
	}

	// Draw "Press Enter to Return" message
	sf::Text returnText;
	returnText.setFont(font);
	returnText.setString("Press Enter to Return");
	returnText.setCharacterSize(charSize);
	returnText.setFillColor(sf::Color::Black);

	// Center the return text horizontally
	sf::FloatRect bounds = returnText.getLocalBounds();
	returnText.setPosition((winWidth - bounds.width) / 2, winHeight - charSize * 2.5f);
	window.draw(returnText);

	window.display();
}

