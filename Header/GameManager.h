#include "QuizManager.h"
#include "Player.h"
#include <vector>

using namespace std;

//Different states of the game
enum class GameState {
	WelcomeScreen,
	Menu,
	StoryScreen,
	HowToPlay,
	Quiz,
	Ending,
	Score,
	Credits,
	Exit
};

class GameManager
{
private: 
	GameState currentState;
	Graphics graphics;
	QuizManager quizManager;
	void prepareCutscene();
	void playCutscence();
	void transitionToCutscene();
	int videoIndex = 0;
	int endingThreshold = 2500;
	string getScoreRank();
	//paths of the videos being used
	vector<string> videoFilePath = { "Resources/videos/intro.avi",
		"Resources/videos/egypt.avi","Resources/videos/rome.avi", 
		"Resources/videos/good.avi", "Resources/videos/bad.avi" };
public:
	GameManager();
	void run();
	void handleState();
	Player gamePlayer;
	void resetGame();
	
};
