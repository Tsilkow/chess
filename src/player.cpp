#include "player.hpp"


void Player::givePosition(Board* board)
{
    m_board = board;
    m_move = getInvalidMove();
}

bool Human::processInput(int ticksPassed, sf::Vector2f mouse, sf::Event event)
{
    Square mouseAt = m_board->getSquareAt(mouse);
    //std::cout << mouseAt << "\n";
    
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
	if(m_activeMoves.size() != 0)
	{
	    for(auto &m: m_activeMoves)
	    {
		if(m.to == mouseAt)
		{
		    m_move = m;
		    m_activeMoves.clear();
		    m_board->resetMarks();
		    return true;
		}
	    }
	    m_activeMoves.clear();
	}
	
	if(m_activeMoves.size() == 0)
	{
	    std::vector<Move> moves = m_board->getMoves();
	    auto it = moves.begin();
	    bool resetAlready = false;
	
	    while(it != moves.end() && it->from != mouseAt) ++it;
	    while(it != moves.end() && it->from == mouseAt)
	    {
		if(!resetAlready)
		{
		    m_board->resetMarks();
		    resetAlready = true;
		}

		m_activeMoves.push_back(*it);
		m_board->mark(it->to);
		++it;
	    }
	    return true;
	}
    }
    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
	m_board->mark(mouseAt);
	return true;
    }
    return false;
}
