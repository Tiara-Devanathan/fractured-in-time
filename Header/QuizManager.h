#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

#include "Answer.h"
#include "Question.h"
#include "Player.h"
#include "GraphicsManager.h"
#include "Graph.h"
#include "Tree.h"
#include "ProgressBar.h"

class Graph;
class Tree;


using namespace std;

class QuizManager {
private:
    vector<Question> questions;
    vector<Answer> answers;
    int currentQuestionID;
	Player* quizPlayer;

    //e.g. QID 100 -> index 0
    unordered_map<int, int> questionIndexMap;

    //e.g. fromQuestionID 100 -> 0, 1, 2 (answerID index)
    unordered_map<int, vector<int>> answerIndexMap;

    int currentLevel = 0;
    const int startingQID [3] = {100,200,301};

    void buildIndexMaps();
    void loadLevelAssets(Graphics& graphics);

    void load_Questions_Answers();
    void showQuestionAndAnswer(Graphics& graphics);
    int getAnswerIndex(Graphics& graphics);
    void displayQuestionAndAnswers(int currentQuestionID, Graphics& graphics);
    int waitForAnswer(Graphics& graphics);
    int getAnswerForTimedQuestion(Graphics& graphics);
    int branchToQuestion(int answerIndex);
    bool isInQuestionBank(int answerID);
    void displayRandomQuestions(int startID, int endID, int correctAnswers, Graphics& graphics);
    void tryToRemoveAnswer(int answerIndex);
    void removeChoice(int);
    int getWrongAnswerIndex(int qID);
    bool questionHasTimer(int questionIndex);
    int getAnswerCorrectness(int answerIndex);

    void nextLevel(int& currentQuestionID, Graphics& graphics);
    void checkForInventory(int answerIndex, Graphics& graphics);
    bool canRenderInventoryOption(int answerID);

    //progressBar
    ProgressBar quizProgressBar;
    Graph* graph;
    Tree* levelTree;

public:
    // Constructor
    QuizManager() {};
    void setPlayer(Player* p);   
    bool startQuiz(Graphics & graphics); 
    void resetQuiz();
    int getCurrentLevel();
    //progressBar
    void updateProgressBar();
    void loadGraph(const int& level);
    Tree* generateLevelTree(int level);
};
