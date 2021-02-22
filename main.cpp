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
#include "resources.hpp"


//using namespace std;

int main()
{
    ResourceHolder<sf::Texture, std::string> textures;

    textures.load("board", "data/board.png");
    textures.load("w" , "data/w.png" );
    textures.load("wN", "data/wN.png");
    textures.load("wB", "data/wB.png");
    textures.load("wR", "data/wR.png");
    textures.load("wQ", "data/wQ.png");
    textures.load("wK", "data/wK.png");
    textures.load("b" , "data/b.png" );
    textures.load("bN", "data/bN.png");
    textures.load("bB", "data/bB.png");
    textures.load("bR", "data/bR.png");
    textures.load("bQ", "data/bQ.png");
    textures.load("bK", "data/bK.png");
    
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");
    window.setFramerateLimit(60);
    
    
    Board board(textures);

    bool exit = false;

    while(!exit)
    {
	window.clear();
	board.draw(window);
	window.display();
	
	board.findMoves();
	std::vector<Move> moves = board.getMoves();
	int chosen;
	
	for(int i = 0; i < moves.size(); ++i)
	{
	    std::cout << i+1 << ": " << moves[i].from << moves[i].to << std::endl;
	}
	do{
	    std::cin >> chosen;
	}while(chosen < 1 && chosen > moves.size());
	board.makeAMove(moves[chosen-1]);
    }
    
    return 0;
}
