#pragma once
#include <string>

struct Question {
	int QID;
	std::string questionText;
	bool hasImage;
	double timeLimt;
};
