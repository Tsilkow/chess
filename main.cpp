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
#include "board.hpp"


//using namespace std;

int main()
{
    Board board;

    board.findMoves();
    while(true)
    {
	auto moves = board.getMoves();

	auto it = moves.begin();
	for(int i = 0; i < moves.size(); ++i, ++it)
	{
	    std::cout << i+1 << ": " << it->from << it->to << std::endl;
	}

	getchar();
	it = moves.begin();
	++it; ++it; ++it;
	board.makeAMove(*it);
	
	board.findMoves();
    }
    
    return 0;
}
