#include "game.hpp"

Game::Game(sf::RenderWindow* window, std::shared_ptr<Player> firstPlayer, std::shared_ptr<Player> secondPlayer, 
    ResourceHolder<sf::Texture, std::string>* m_textures):
    m_window(window),
    m_firstPlayer(firstPlayer),
    m_secondPlayer(secondPlayer),
    m_board(m_textures),
    m_ticksPassed(0)
{
    m_firstPlayer->givePosition( &m_board);
    m_secondPlayer->givePosition(&m_board);
}

bool Game::playOut()
{
    while(!isConcluded(m_board.getState()))
    {
	sf::Event event;
	m_window->clear();

	while(m_window->pollEvent(event))
	{
	    switch(event.type)
	    {
		case sf::Event::Closed:
		    return false;
		    break;
		case sf::Event::KeyPressed:
		    if(m_window->hasFocus())
		    {
			switch(event.key.code)
			{
			    case sf::Keyboard::Escape:
				return false;
				break;
			}
		    }
	    }

	    if(m_window->hasFocus())
	    {
		if(m_firstPlayer->needsInput())
		{
		    std::static_pointer_cast<Human>(m_firstPlayer)->processInput(
			   m_ticksPassed,
			   m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)),
			   event);
		}
	        if(m_secondPlayer->needsInput())
		{
		    std::static_pointer_cast<Human>(m_secondPlayer)->processInput(
			   m_ticksPassed,
			   m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)),
			   event);
		}
	    }
	}

	if(m_board.isWhiteToMove())
	{
	    if(m_firstPlayer->isMoveReady() ) m_board.makeAMove(m_firstPlayer->getMove());
	}
	else
	{
	    if(m_secondPlayer->isMoveReady()) m_board.makeAMove(m_secondPlayer->getMove());
	}

	m_board.draw(*m_window);
	m_window->display();

	++m_ticksPassed;
    }

    return true;
}
