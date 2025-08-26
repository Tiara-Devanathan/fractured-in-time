#include "Player.h"
#include <iostream>

using namespace std;

Player::Player() {
    inventory = unordered_set<int>();
}

std::vector <int> Player::getScore()
{
    return levelScores;
}

//Stores item in inventory
void Player::acquire(int inventoryItem) {
    inventory.insert(inventoryItem);
}

//Check if item is in inventory
bool Player::has(int inventoryItem) {
    return inventory.find(inventoryItem) != inventory.end();
}

//Operator Overloading
void Player::operator+(int score) {
    finalScore += score;
}

void Player::updateCorrectnessRate(int score)
{
    if (score > 0) {
        correctAnswers++;
        
    }
    totalQuestions++; 
}

float Player::getCorrectnessPercentage()
{
    return (float(correctAnswers) / totalQuestions) * 100.0;
}

//Reset score after each game run
void Player::reset()
{
    if (!inventory.empty()) {
        levelScores = { 0,0,0 };
        inventory.clear();
        finalScore = 0;
        totalQuestions = 0;
        correctAnswers = 0;
    }
}

void Player::addToCurrentScore(int index, int score)
{
    if (index < 3 && index > -1)
    {
        levelScores[index] += score;
        if (levelScores[index] < 0) levelScores[index] = 0;
    }
}

int Player::getScoreAtIndex(int index)
{
    return levelScores[index];
}

int Player::getFinalScore()
{
    return finalScore;
}
