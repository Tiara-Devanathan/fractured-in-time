#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

class ProgressBar {
private:
    float fullness = 0;//value between 0 and 1
    
public:
    //void draw(Graphics& g, int level);
    void setFullness(const float& fullness);
    ProgressBar& operator=(const ProgressBar& bar);
    float getFullness() const;
    //inner -> 0:border 1:background 2:filling outer -> 0:egypt 1:rome 2:future
    //egypt : gold, dark brown, turquoise
    //rome : gold, light grey, green
    //future: black, light grey, glitch purple
    std::vector<std::vector<sf::Color>> levelColours = { {sf::Color(212, 175, 55), sf::Color(50, 30, 10), sf::Color(0, 134, 139)},
        {sf::Color(212, 183, 20), sf::Color(220,220,220), sf::Color(41, 115, 38)},
        {sf::Color::Black, sf::Color(220,220,220), sf::Color(138, 43, 226)} };
};
