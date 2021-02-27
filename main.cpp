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
#include "player.hpp"
#include "game.hpp"
#include "resources.hpp"


//using namespace std;

int main()
{
    ResourceHolder<sf::Texture, std::string> textures;

    textures.load("board"    , "data/board.png");
    textures.load("highlight", "data/highlight.png");
    textures.load("mark"     , "data/mark.png");
    textures.load("w"        , "data/w.png" );
    textures.load("wN"       , "data/wN.png");
    textures.load("wB"       , "data/wB.png");
    textures.load("wR"       , "data/wR.png");
    textures.load("wQ"       , "data/wQ.png");
    textures.load("wK"       , "data/wK.png");
    textures.load("b"        , "data/b.png" );
    textures.load("bN"       , "data/bN.png");
    textures.load("bB"       , "data/bB.png");
    textures.load("bR"       , "data/bR.png");
    textures.load("bQ"       , "data/bQ.png");
    textures.load("bK"       , "data/bK.png");
    
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");
    window.setFramerateLimit(60);

    std::shared_ptr<Human> first  = std::make_shared<Human>("amigo uno", &window);
    std::shared_ptr<Human> second = std::make_shared<Human>("amigo dos", &window);

    Game game(&window, first, second, &textures);

    game.playOut();
	
    window.close();
    
    return 0;
}
