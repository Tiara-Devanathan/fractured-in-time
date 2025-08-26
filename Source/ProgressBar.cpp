#include "ProgressBar.h"

using namespace std;

//Shows the progress for the level
void ProgressBar::setFullness(const float& fullness)
{
	if (fullness <= 1 && fullness >= 0)
		this->fullness = fullness;
}

//Operator overloading
ProgressBar& ProgressBar::operator=(const ProgressBar& bar)
{
	if (this != &bar)
		fullness = bar.fullness;
	return *this;
}

float ProgressBar::getFullness() const
{
	return fullness;
}