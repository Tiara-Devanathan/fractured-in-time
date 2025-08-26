#pragma once
#include <string>

struct Answer {
    int answerID;
    int fromQuestionID;
    int toQuestionID;
    int correctness;
    std::string answerText;
    int inventoryAnswerID;
    bool requiresInventory;
    bool removedAfterPassed;
};