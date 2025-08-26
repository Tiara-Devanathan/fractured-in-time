#include "QuizManager.h"

using namespace std;

//Check if QuestionID is part of the question bank
bool QuizManager::isInQuestionBank(int answerID)
{
    return ((currentQuestionID == 115 && branchToQuestion(answerID) == 117)
        || currentQuestionID == 214);
}

void QuizManager::displayRandomQuestions(int startID, int endID, int correctAnswers, Graphics& graphics) {
    int correct = 0;
    int numOfQuestions = endID - startID + 1;
    unordered_set<int> questionIDs;
    srand(time(0));

    //Randomize the questions in vector questionIDs
    while (questionIDs.size() < numOfQuestions)
    {
        int randID = rand() % numOfQuestions + startID;
        questionIDs.insert(randID);
    }

    auto questionBankIT = questionIDs.begin();

    //Loop through the vector to display random questions
    while (questionBankIT != questionIDs.end() && correct < correctAnswers)
    {
        currentQuestionID = *questionBankIT;

        showQuestionAndAnswer(graphics);
        int answerIndex = waitForAnswer(graphics);
        int correctness = getAnswerCorrectness(answerIndex);
        quizPlayer->addToCurrentScore(currentLevel, correctness);
        quizPlayer->updateCorrectnessRate(correctness);
        if (correctness == 100)
            correct++;

        auto toErase = questionBankIT;
        questionBankIT = questionIDs.erase(toErase);
    }
}