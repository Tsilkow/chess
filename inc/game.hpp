#pragma once

#include "board.hpp"
#include "player.hpp"


class Game
{
    sf::RenderWindow* m_window;
    std::shared_ptr<Player> m_firstPlayer;
    std::shared_ptr<Player> m_secondPlayer;
    Board m_board;
    int m_ticksPassed;

    public:
    Game(sf::RenderWindow* window, std::shared_ptr<Player> firstPlayer, std::shared_ptr<Player> secondPlayer,
    ResourceHolder<sf::Texture, std::string>* m_textures);

    bool playOut();
};
