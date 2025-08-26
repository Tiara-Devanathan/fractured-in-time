#include "QuizManager.h"
#include "Timer.h"

using namespace std;

//Check if question has a timer
bool QuizManager::questionHasTimer(int questionID) {
    int index = questionIndexMap[questionID];
    return (questions[index].timeLimt > 0);
}

int QuizManager::getAnswerForTimedQuestion(Graphics& graphics)
{
	graphics.setProgressBar(quizProgressBar);
    bool answered = false;
    int userChoice = -1;

    Timer questionTimer(questions[questionIndexMap[currentQuestionID]].timeLimt);

    while (questionTimer.isRunning()) {
        // Clear previous frame
        graphics.clear();

        // Redraw everything each frame
        if (!graphics.loadBackground(currentQuestionID)) {
            std::cout << "Error loading background image.";
        }
        graphics.drawBackground();
        graphics.drawAdditionalButtons();
        graphics.drawProgressBar(currentLevel);
        displayQuestionAndAnswers(currentQuestionID, graphics);

        //Draw the timer
        graphics.drawTimer((float)questionTimer.getRemainingTime());
        graphics.display();

        userChoice = getAnswerIndex(graphics);
        if (userChoice != -1) {
            answered = true;
            return userChoice;
        }
    }

    if (!answered) {
        userChoice = getWrongAnswerIndex(currentQuestionID);
    }

    return userChoice;

}