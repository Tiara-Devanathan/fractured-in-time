#include "QuizManager.h"
#include "Player.h"
#include <vector>
#include <set>
#include "Timer.h"

using namespace std;

void QuizManager::setPlayer(Player* p)
{
    quizPlayer = p;
}

//Populates the Question and Answer vectors
void QuizManager:: buildIndexMaps() {
    questionIndexMap.clear();
    answerIndexMap.clear();

    //A map where key: QID and value: Index
    for (int i = 0; i < questions.size(); i++) {
        questionIndexMap[questions[i].QID] = i;
    }

    //A map where key: fromQuestionID and value: Indicies of answers
    // eg. 100: [0, 1]
    for (int i = 0; i < answers.size(); i++) {
        answerIndexMap[answers[i].fromQuestionID].push_back(i);
    }
}


// Display the current question and all possible answers
void QuizManager::showQuestionAndAnswer(Graphics & graphics) {
    if (!graphics.loadBackground(currentQuestionID)) {
        std::cout << "Error loading image" << endl;;
    }
    
    graphics.playSound(to_string(currentQuestionID)); 
    graphics.drawBackground();
    graphics.drawAdditionalButtons();
    graphics.setProgressBar(quizProgressBar);
    graphics.drawProgressBar(currentLevel);
    displayQuestionAndAnswers(currentQuestionID, graphics);
    graphics.display();
}

void QuizManager::displayQuestionAndAnswers(int questionID, Graphics& graphics) {
    if (questionIndexMap.find(questionID) == questionIndexMap.end()) {
        graphics.drawQuestion("Question not found!");
        return;
    }

    int questionIndex = questionIndexMap[questionID];
    const string& questionText = questions[questionIndex].questionText;
    bool questionHasImage = questions[questionIndex].hasImage;

    // Collect visible answers
    vector<string> answerText;
    vector<int> visibleIndices;
    vector<int> correctnesses;

    if (answerIndexMap.find(questionID) != answerIndexMap.end()) {
        for (int index : answerIndexMap[questionID]) {
            if (!answers[index].requiresInventory || canRenderInventoryOption(answers[index].answerID)) {
                answerText.push_back(answers[index].answerText);
                correctnesses.push_back(answers[index].correctness);
                visibleIndices.push_back(index);
                
            }
        }
    }

    graphics.drawQuestion(questionText,  questionID, questionHasImage);
    graphics.drawAnswers(answerText, visibleIndices, correctnesses);
}

//Used for the timed questions
int QuizManager::getWrongAnswerIndex(int qID)
{
    for (int index : answerIndexMap[qID]) {
        if (answers[index].correctness == 0)
        {
            return index;
        }
    }
    for (int index : answerIndexMap[qID])
        return index;
}

//Get answer index
int QuizManager::getAnswerIndex(Graphics &graphics) {
    return graphics.handleClick();
}

//Return index of next Question
int QuizManager::branchToQuestion(int answerIndex) {
        return answers[answerIndex].toQuestionID; // Next questionID
   
}

//move to next level
void QuizManager::nextLevel(int & QID, Graphics &graphics) {
    if (QID == 1) {
        currentLevel++;
        loadLevelAssets(graphics);
        QID = startingQID[currentLevel];
    } 
}

void QuizManager::removeChoice(int answerIndex) {
    //remove from answer struct and answerMap
    int fromQuestionID = answers[answerIndex].fromQuestionID;

    //Remove from answer struct
    answers.erase(answers.begin() + answerIndex);

    //Remove index from map
    auto& indexList = answerIndexMap[fromQuestionID];
    indexList.erase(remove(indexList.begin(), indexList.end(), answerIndex), indexList.end());

    //Shift elements in the map
    for (auto& pair : answerIndexMap)
    {
        vector<int>& indices = pair.second;
        for (int& index : indices)
        {
            if (index > answerIndex)
                --index;
        }
    }

    //Delete a map if there exists no corresponding answers for it
    if (answerIndexMap[fromQuestionID].empty())
        answerIndexMap.erase(fromQuestionID);

}

int QuizManager::waitForAnswer(Graphics& graphics) {
    int userChoice = -1;
    while (userChoice == -1) {
        userChoice = getAnswerIndex(graphics);
    }
    return userChoice;
}

int QuizManager::getAnswerCorrectness(int answerIndex)
{
    return answers[answerIndex].correctness;
}

//Whole game runs here
bool QuizManager::startQuiz(Graphics & graphics) {
    graphics.clear();
    loadLevelAssets(graphics);

    if (levelTree != nullptr) {
        delete levelTree;
    }
    loadGraph(currentLevel + 1);

    levelTree = generateLevelTree(currentLevel + 1);

    bool reachedEnd = false;
  
    while (!reachedEnd) {
        updateProgressBar();
        showQuestionAndAnswer(graphics);
        int userChoice = -1;

        //Timed questions
        if (questionHasTimer(currentQuestionID)) {
            userChoice = getAnswerForTimedQuestion(graphics);
        }
        else {
            userChoice = waitForAnswer(graphics);
        }
       
        checkForInventory(userChoice, graphics);
        int nextQuestion;

        //Question bank
        if (isInQuestionBank(userChoice)) {
            if (currentQuestionID == 115) {
                displayRandomQuestions(117, 124, 3, graphics);
                nextQuestion = 125;
            }
            else {
                displayRandomQuestions(250, 266, 8, graphics);
                nextQuestion = 215;
            }
        }
        else {
            nextQuestion = branchToQuestion(userChoice);
        }

        quizPlayer->updateCorrectnessRate(getAnswerCorrectness(userChoice));
        quizPlayer->addToCurrentScore(currentLevel, getAnswerCorrectness(userChoice));

        //When nextQuestion = 1, then new era
        if (nextQuestion == 1) {
            *quizPlayer + quizPlayer->getScoreAtIndex(currentLevel);
            currentLevel++;
            loadLevelAssets(graphics);
            quizProgressBar.setFullness(0);
            currentQuestionID = startingQID[currentLevel];
            return true; // tell GameManager to switch to LevelEnd
        }
        tryToRemoveAnswer(userChoice);

        //Checks if end of game
        if (questionIndexMap.find(nextQuestion) == questionIndexMap.end()) {
            //last levels score gets added to final score
            *quizPlayer + quizPlayer->getScoreAtIndex(currentLevel);

            cout << "\nEnd of Game.\n";
            reachedEnd = true;
            graphics.playBackgroundMusic("menuMusic");
        }
        else {
            currentQuestionID = nextQuestion;
        }
    }

    return false;
}

//Checks if inventory is being used for question
void QuizManager::checkForInventory(int answerIndex, Graphics& graphics)
{
    int inventoryID = answers[answerIndex].inventoryAnswerID;
    if (inventoryID != 0)
    {
        quizPlayer->acquire(inventoryID);
		graphics.playSound("item");  
    }
}

void QuizManager::tryToRemoveAnswer(int answerIndex) {
    if (answers[answerIndex].removedAfterPassed)
        removeChoice(answerIndex);
}

bool QuizManager::canRenderInventoryOption(int answerID)
{
    return quizPlayer->has(answerID);
}

//Load items for each era: font, colour, music, images
void QuizManager::loadLevelAssets(Graphics& graphics) {
    if (currentLevel == 0) {
        graphics.loadFont("pixel");
        graphics.setTextColor(200, 100, 10);
        graphics.answerColor = sf::Color::White;
        graphics.playBackgroundMusic("egypt");
        graphics.backgroundMusic.setVolume(10);
        graphics.questionTexture.loadFromFile("Resources/images/buttons/egypt1.png");
        graphics.answerTexture.loadFromFile("Resources/images/buttons/egypt2.png");
    }
    else if (currentLevel == 1) {
        graphics.loadFont("skandal");
        graphics.setTextColor(0, 0, 0);
        graphics.answerColor = sf::Color::Black;
        graphics.playBackgroundMusic("rome");
        graphics.backgroundMusic.setVolume(10);
        graphics.questionTexture.loadFromFile("Resources/images/buttons/rome.png");
        graphics.answerTexture.loadFromFile("Resources/images/buttons/rome.png");

    }
    else if (currentLevel == 2) {
        graphics.loadFont("cyber");
        graphics.setTextColor(225, 225, 225);
        graphics.answerColor = sf::Color::White;
        graphics.playBackgroundMusic("cyber");
        graphics.backgroundMusic.setVolume(10);
        graphics.questionTexture.loadFromFile("Resources/images/buttons/cyber1.png");
        graphics.answerTexture.loadFromFile("Resources/images/buttons/cyber2.png");

    }
}

//Progress Bar
void QuizManager::updateProgressBar()
{
    float left = 1 - quizProgressBar.getFullness();

    int shortestPathLength = levelTree->path(currentQuestionID).size();
    float toAdd = (float(1) / shortestPathLength) * left;
    quizProgressBar.setFullness(quizProgressBar.getFullness() + toAdd);
}

void QuizManager::loadGraph(const int& level)
{
    std::vector<int> questionIDs;
    for (const auto q : questions) {
        if (q.QID / (level * 100) == 1) questionIDs.push_back(q.QID);
    }
    if (level == 3) {
        questionIDs.push_back(0);
    }
    else {
        questionIDs.push_back(1);
    }
    if (graph != nullptr) {
        delete graph;
    }

    std::vector<Answer> ans;
    for (const auto a : answers) {
        if (a.fromQuestionID / (level * 100) == 1) ans.push_back(a);
    }
    graph = new Graph(questionIDs, ans);
}

Tree* QuizManager::generateLevelTree(int level)
{
    if (level == 3) {
        return Tree::bfs(0, graph);
    }
    else {
        return Tree::bfs(1, graph);
    }


}

//Reset values for restart of game
void QuizManager::resetQuiz() {

    load_Questions_Answers();
    buildIndexMaps();
    currentQuestionID = 100;
    currentLevel = 0;
    quizProgressBar.setFullness(0);
}
int QuizManager::getCurrentLevel() {
    return currentLevel;
}
