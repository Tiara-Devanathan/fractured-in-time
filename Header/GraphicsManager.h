#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "videoplayback.h"
#include "ProgressBar.h"

#include <vector>
#include <string>

using namespace std;

class Graphics {
public:
    sf::RenderWindow window;
    HWND hWnd;

    sf::Sprite bgSprite;
    sf::Sprite questionImageSprite;

    sf::Font font;

    sf::SoundBuffer buffer;

    sf::Sound sound;

    sf::Music backgroundMusic;

    sf::Texture questionTexture;
    sf::Texture answerTexture;
    sf::Texture muteTexture;
    sf::Texture unmuteTexture;
    sf::Texture questionImageTexture;
    sf::Texture scrollTexture; 
    sf::Texture exitQuizButtonTexture;
    sf::Texture bgTexture;

    sf::Color answerColor; 

    sf::RectangleShape howToButton;
    sf::RectangleShape muteButton;
    sf::RectangleShape enterButton;
    sf::RectangleShape playButton;
    sf::RectangleShape creditsButton;
    sf::RectangleShape exitButton;
    sf::RectangleShape exitQuizButton;
    sf::RectangleShape continueButton;
    sf::RectangleShape backButton;

    sf::Text backButtonText;
    bool isMuted = false;

    struct Button {
        sf::RectangleShape shape;
        sf::Text text;
        int answerIndex;
        bool isCorrect;
        bool wasHovered;
    };
    std::vector<Button> buttons;
    Graphics();
    bool isOpen();
    void clear();
    void display();
    void close();

    void drawWelcomeScreen();
    void drawMenu();
    void drawHelpScreen();
    void drawStoryScreen(int level);
    void drawScoreScreen(vector<int> scores, int finalScore, string rank, float totalCorrect);

    std::string wrapText(const std::string& text, sf::Font& font, unsigned int charSize, float maxWidth, float startX);
    void drawQuestion(const std::string& questionText, int questionID = 9000, bool questionHasImage = false);
    void drawAnswers(const vector<string>& answersText, const vector<int>& answerIndicies, const vector<int>& answersCorrectness);

    void drawAdditionalButtons();
    void drawBackground();
    void drawProgressBar(int levelIndex);
    void drawTimer(float time);
    void loadFont(std::string fontName);
    bool loadBackground(int QID);
    void playSound(std::string name);
    void playBackgroundMusic(std:: string musicName);
    sf::Color buttonColor;
    sf::Color textColor;
    void setButtonColor(int r, int g, int b);
    void setTextColor(int r, int g, int b);
    void renderCurrentScreen();
    void setProgressBar(ProgressBar& progressBar);

    void resetCredits();
    vector<sf::Text> credits;
    sf::Clock creditsClock;
    void drawCredits();

    int handleClick();
private:
    
    int offSet;
    /*Variables stored to re-render screen*/
    std::string currentQuestionText;
    bool hasImage = false;
    int currentQuestionID;
    ProgressBar storedProgressBar;
    float winHeight;
    float winWidth;

    
};

