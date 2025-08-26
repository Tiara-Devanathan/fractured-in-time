#include "GameManager.h"
#include "GraphicsManager.h"
#include "QuizManager.h"
#include "Player.h"

using namespace std;

GameManager::GameManager(){
   currentState = GameState::WelcomeScreen;
   graphics.playBackgroundMusic("menuMusic");
   graphics.backgroundMusic.setVolume(10);  
}


void GameManager::run() {
    while (graphics.isOpen() && currentState != GameState::Exit) {
        handleState();        
    }
}

//checks state of the game
void GameManager::handleState() {
    sf::Event event;

    switch (currentState) {

    case GameState::WelcomeScreen:
        graphics.clear();
        graphics.drawWelcomeScreen();
        // Wait for Enter click to go to Menu
        while (graphics.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                currentState = GameState::Exit;
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                //if enter button is clicked
                if (graphics.enterButton.getGlobalBounds().contains(mousePos)) {
                    graphics.playSound("click");
                    currentState = GameState::Menu;
                    return;
                }
            }
        }
        break;

    case GameState::Menu:
        graphics.clear();
        graphics.loadFont("pixel");
        graphics.drawMenu();
        while (graphics.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                currentState = GameState::Exit;
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                //if play button is clicked
                if (graphics.playButton.getGlobalBounds().contains(mousePos)) {
                    graphics.playSound("click");
                    resetGame();
                    graphics.backgroundMusic.stop();
                    transitionToCutscene();
                    currentState = GameState::StoryScreen;
                    return;
                }

                //if credit button is clicked
                if (graphics.creditsButton.getGlobalBounds().contains(mousePos)) {
                    graphics.playSound("click");
                    currentState = GameState::Credits;
                    return;
                }

                // if how to play button is clicked
                if (graphics.howToButton.getGlobalBounds().contains(mousePos)) {
                    graphics.playSound("click");
                    currentState = GameState::HowToPlay;
                    return;
                }

                //if exit button is clicked
                if (graphics.exitButton.getGlobalBounds().contains(mousePos)) {
                    graphics.playSound("click");
                    currentState = GameState::Exit;
                    return;
                }
            }
        }
        break;

    case GameState::HowToPlay:
        graphics.clear();
        graphics.drawHelpScreen();
        // Event handling loop for the story screen
        while (currentState == GameState::HowToPlay && graphics.window.isOpen()) {
            sf::Event event;

            // Poll for events
            while (graphics.window.pollEvent(event)) {
                //exit game if window is closed
                if (event.type == sf::Event::Closed) {
                    currentState = GameState::Exit;
                    return;
                }

                //return to menu screen
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    graphics.playSound("click");
                    currentState = GameState::Menu; 
                    return;
                }
            }

        }
        break;

    //transition screen from video to level
    case GameState::StoryScreen:
        graphics.clear();
        graphics.loadFont("pixel");
        graphics.drawStoryScreen(quizManager.getCurrentLevel());
        // Event handling loop for the story screen
        while (currentState == GameState::StoryScreen && graphics.window.isOpen()) {
            sf::Event event;

            //exit game is window is closed
            while (graphics.window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    currentState = GameState::Exit;  // Exit the game if the window is closed
                    return;
                }

                // Check if the enter is pressed to move to the next state
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    graphics.playSound("click");
                    currentState = GameState::Quiz;  
                    return;
                }
            }
        }
        break;
       
    case GameState::Credits:
        graphics.clear();
        graphics.resetCredits();
        while (currentState == GameState::Credits && graphics.isOpen()) {
            graphics.drawCredits();

            while (graphics.window.pollEvent(event)) {
                //exit if window is closed
                if (event.type == sf::Event::Closed) {
                    currentState = GameState::Exit;
                    return;
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                    //return back to menu
                    if (graphics.backButton.getGlobalBounds().contains(mousePos)) {
                        graphics.playSound("click");
                        currentState = GameState::Menu;
                        return;
                    }
                }
            }
        }
        break;

    case  GameState::Quiz:
        graphics.clear();
        if (quizManager.startQuiz(graphics)) {
            //play video when game starts
            transitionToCutscene();
            currentState = GameState::StoryScreen;
        }
        else {
            currentState = GameState::Ending;
        }
        break;

    case GameState::Ending:
        while (currentState == GameState::Ending && graphics.isOpen()) {
            //After the last video is played the index of vector will point to the good ending
            //If score not high enough -> increment to move index of vector to bad ending
            if (gamePlayer.getFinalScore() < endingThreshold)
                videoIndex++;
            transitionToCutscene();
            currentState = GameState::Score;
            return;
        }
        
        break;


    case GameState::Score:
        graphics.clear();
        graphics.drawScoreScreen(gamePlayer.getScore(), gamePlayer.getFinalScore(), getScoreRank(),gamePlayer.getCorrectnessPercentage());
        while (graphics.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                currentState = GameState::Exit;
                return;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                //return to menu
                if (graphics.backButton.getGlobalBounds().contains(mousePos)) {
                    graphics.playSound("click");
                    currentState = GameState::Menu;
                    return;
                }
            }
        }
        break;

    case GameState::Exit:
        graphics.close();
        break;
    }
}

//Allows for user to play the game again without exiting the window
void GameManager::resetGame()
{
    videoIndex = 0;
    gamePlayer.reset();
    quizManager.resetQuiz();
    quizManager.setPlayer(&gamePlayer);
    graphics.playBackgroundMusic("menuMusic");
    graphics.loadFont("pixel");
}

void GameManager::transitionToCutscene()
{
    prepareCutscene();
}

//Paints the screen black before the video plays
void GameManager::prepareCutscene()
{
    graphics.window.clear(sf::Color::Black);
	graphics.window.display();
	playCutscence();
}

//Plays the video
void GameManager::playCutscence()
{
    video vid;
    vid.display(videoFilePath[videoIndex], graphics.hWnd);
    videoIndex++;
    return;
}

string GameManager::getScoreRank()
{
    float finalPercent = gamePlayer.getCorrectnessPercentage();

    if (finalPercent >= 80)
        return "A";
    else if (finalPercent >= 70)
        return "B";
    else if (finalPercent >= 60)
        return "C";
    else if (finalPercent >= 50)
        return "D";
    else
        return "F";
}