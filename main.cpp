// Tsilkow

#include <iostream>
#include <vector>
#include <time.h>
#include <memory>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "commons.hpp"
#include "piece.hpp"
#include "position.hpp"


//using namespace std;

int main()
{
    Position position;

    std::vector<Move> moves = position.getMoves();

    for(auto it = moves.begin(); it != moves.end(); ++it)
    {
	std::cout << it->from << it->to << std::endl;
    }
    
    return 0;
}
