#pragma once
#include <list>
#include <unordered_set>
#include <vector>

class Player {
private:
    std::vector<int> levelScores = {0,0,0};
    std::unordered_set<int> inventory;
    int totalQuestions = 0;
    int finalScore = 0;
    int correctAnswers = 0;
public:
    Player();
    std::vector <int> getScore();
    void acquire(int inventoryItem);
    bool has(int inventoryItem);
    void operator + (int score);
    void updateCorrectnessRate(int score);
    float getCorrectnessPercentage();
    void reset();
    void addToCurrentScore(int index, int score);
    int getScoreAtIndex(int index);
    int getFinalScore();
};

