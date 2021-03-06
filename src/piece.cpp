#include "piece.hpp"


void guardSquare(std::vector< std::vector<SquareStatus> >& squareStatuses, Square at, bool isWhite)
{
    if(isWhite) ++atSquare(squareStatuses, at).guardedByWhite;
    else        ++atSquare(squareStatuses, at).guardedByBlack;
}

void unguardSquare(std::vector< std::vector<SquareStatus> >& squareStatuses, Square at, bool isWhite)
{
    if(isWhite) --atSquare(squareStatuses, at).guardedByWhite;
    else        --atSquare(squareStatuses, at).guardedByBlack;
}

Piece::Piece(std::string abbreviation, bool isWhite,
	     ResourceHolder<sf::Texture, std::string>* textures, Square startPos):
    m_abbreviation(abbreviation),
    m_isWhite(isWhite)
{
    if(m_isWhite) m_sprite.setTexture(textures->get("w" + m_abbreviation));
    else          m_sprite.setTexture(textures->get("b" + m_abbreviation));
    
    m_sprite.setPosition(sf::Vector2f((startPos.c-1) * m_sprite.getGlobalBounds().width,
				      (8-startPos.r) * m_sprite.getGlobalBounds().height));
}

void Piece::move(const Move& move, std::vector< std::vector<SquareStatus> >& squareStatuses)
{
    std::vector<Move> moves = getMoves(move.from, squareStatuses);

    for(int i = 0; i < moves.size(); ++i)
    {
	if(i == 0 || moves[i-1].to != moves[i].to) unguardSquare(squareStatuses, moves[i].to, m_isWhite);
    }
    atSquare(squareStatuses, move.from).occupied = 0;
    atSquare(squareStatuses, move.to  ).occupied = sideSign(m_isWhite);
    
    m_sprite.setPosition(sf::Vector2f((move.to.c-1) * m_sprite.getGlobalBounds().width,
				      (8-move.to.r) * m_sprite.getGlobalBounds().height));
}

void Piece::draw(sf::RenderTarget& target)
{
    target.draw(m_sprite);
}

std::vector<Move> Pawn::getMoves(const Square& position,
				 std::vector< std::vector<SquareStatus> >& squareStatuses)
{
    std::vector<Move> result;
    int enemy = 1 - 2*m_isWhite;
    
    Square straight  = position + Square( 0, -enemy  );
    Square doubled   = position + Square( 0, -enemy*2);
    Square diagonal1 = position + Square(-1, -enemy  );
    Square diagonal2 = position + Square( 1, -enemy  );
    
    if(straight.isValid() && atSquare(squareStatuses, straight).isEmpty())
    {
	if(straight.r != 8) result.push_back(Move(position, straight));
	else
	{
	    result.push_back(Move(position, straight, false, 'N'));
	    result.push_back(Move(position, straight, false, 'B'));
	    result.push_back(Move(position, straight, false, 'R'));
	    result.push_back(Move(position, straight, false, 'Q'));
	}
	    
	if(position.r == 7 - 5*m_isWhite && doubled.isValid() && atSquare(squareStatuses, doubled).isEmpty())
	    result.push_back(Move(position, doubled));
    }
    if(diagonal1.isValid())
    {
	guardSquare(squareStatuses, diagonal1, m_isWhite);
	if(atSquare(squareStatuses, diagonal1).isEnemy(m_isWhite))
	{
	    if(diagonal1.r != 8) result.push_back(Move(position, diagonal1, true));
	    else
	    {
		result.push_back(Move(position, straight, true, 'N'));
		result.push_back(Move(position, straight, true, 'B'));
		result.push_back(Move(position, straight, true, 'R'));
		result.push_back(Move(position, straight, true, 'Q'));
	    }
	}
    }
    if(diagonal2.isValid())
    {
	guardSquare(squareStatuses, diagonal2, m_isWhite);
	if(atSquare(squareStatuses, diagonal2).isEnemy(m_isWhite))
	{
	    if(diagonal2.r != 8) result.push_back(Move(position, diagonal2, true));
	    else
	    {
		result.push_back(Move(position, straight, true, 'N'));
		result.push_back(Move(position, straight, true, 'B'));
		result.push_back(Move(position, straight, true, 'R'));
		result.push_back(Move(position, straight, true, 'Q'));
	    }
	}
    }

    return result;
}

std::vector<Move> Knight::getMoves(const Square& position,
				   std::vector< std::vector<SquareStatus> >& squareStatuses)
{
    std::vector<Move> result;

    std::vector<Square> Ls =
	{
	    position + Square( 1,  2),
	    position + Square( 2,  1),
	    position + Square( 2, -1),
	    position + Square( 1, -2),
	    position + Square(-1, -2),
	    position + Square(-2, -1),
	    position + Square(-2,  1),
	    position + Square(-1,  2)
	};

    for(int i = 0; i < Ls.size(); ++i)
    {
	if(Ls[i].isValid())
	{
	    guardSquare(squareStatuses, Ls[i], m_isWhite);
	    if(     atSquare(squareStatuses, Ls[i]).isEmpty())
		result.push_back(Move(position, Ls[i], false));
	    else if(atSquare(squareStatuses, Ls[i]).isEnemy(m_isWhite))
		result.push_back(Move(position, Ls[i], true));
	}
    }

    return result;
}

std::vector<Move> Bishop::getMoves(const Square& position,
				   std::vector< std::vector<SquareStatus> >& squareStatuses)
{
    std::vector<Move> result;

    for(int i = 0; i < 4; ++i)
    {
	Square dir(1-2*(i/2), 1-2*(i%2));
	Square curr = position + dir;
	
	while(curr.isValid() && atSquare(squareStatuses, curr).isEmpty())
	{
	    guardSquare(squareStatuses, curr, m_isWhite);
	    result.push_back(Move(position, curr));
	    curr += dir;
	}
	if(curr.isValid())
	{
	    guardSquare(squareStatuses, curr, m_isWhite);
	    if(atSquare(squareStatuses, curr).isEnemy(m_isWhite)) result.push_back(Move(position, curr, true));
	}
    }

    return result;
}

std::vector<Move> Rook::getMoves(const Square& position,
				 std::vector< std::vector<SquareStatus> >& squareStatuses)
{
    std::vector<Move> result;

    for(int i = 0; i < 4; ++i)
    {
	Square dir(((i+1)%2) * (1 - i/2*2), (i%2) * (1 - i/2*2));
	Square curr = position + dir;
	
	while(curr.isValid() && atSquare(squareStatuses, curr).isEmpty())
	{
	    guardSquare(squareStatuses, curr, m_isWhite);
	    result.push_back(Move(position, curr));
	    curr += dir;
	}
	if(curr.isValid())
	{
	    guardSquare(squareStatuses, curr, m_isWhite);
	    if(atSquare(squareStatuses, curr).isEnemy(m_isWhite)) result.push_back(Move(position, curr, true));
	}
    }

    return result;
}

std::vector<Move> Queen::getMoves(const Square& position,
				  std::vector< std::vector<SquareStatus> >& squareStatuses)
{
    std::vector<Move> result;

    // rook moves
    for(int i = 0; i < 4; ++i)
    {
	Square dir(((i+1)%2) * (1 - i/2*2), (i%2) * (1 - i/2*2));
	Square curr = position + dir;
	
	while(curr.isValid() && atSquare(squareStatuses, curr).isEmpty())
	{
	    guardSquare(squareStatuses, curr, m_isWhite);
	    result.push_back(Move(position, curr));
	    curr += dir;
	}
	if(curr.isValid())
	{
	    guardSquare(squareStatuses, curr, m_isWhite);
	    if(atSquare(squareStatuses, curr).isEnemy(m_isWhite)) result.push_back(Move(position, curr, true));
	}
    }

    // bishop moves
    for(int i = 0; i < 4; ++i)
    {
	Square dir(1-2*(i/2), 1-2*(i%2));
	Square curr = position + dir;
	
	while(curr.isValid() && atSquare(squareStatuses, curr).isEmpty())
	{
	    guardSquare(squareStatuses, curr, m_isWhite);
	    result.push_back(Move(position, curr));
	    curr += dir;
	}
	if(curr.isValid())
	{
	    guardSquare(squareStatuses, curr, m_isWhite);
	    if(atSquare(squareStatuses, curr).isEnemy(m_isWhite)) result.push_back(Move(position, curr, true));
	}
    }

    return result;
}

std::vector<Move> King::getMoves(const Square& position,
				 std::vector< std::vector<SquareStatus> >& squareStatuses)
{
    std::vector<Move> result;
    
    std::vector<Square> neighbours =
	{
	    position + Square( 0,  1),
	    position + Square( 1,  1),
	    position + Square( 1,  0),
	    position + Square( 1, -1),
	    position + Square( 0, -1),
	    position + Square(-1, -1),
	    position + Square(-1,  0),
	    position + Square(-1,  1)
	};

    for(int i = 0; i < neighbours.size(); ++i)
    {
	if(neighbours[i].isValid() && !atSquare(squareStatuses, neighbours[i]).isGuarded(m_isWhite))
	{
	    guardSquare(squareStatuses, neighbours[i], m_isWhite);
	    if(      atSquare(squareStatuses,   neighbours[i]).isEmpty())
		result.push_back(Move(position, neighbours[i]));
	    else if((atSquare(squareStatuses,   neighbours[i]).isEnemy(m_isWhite)))
		result.push_back(Move(position, neighbours[i], true));
	}
    }

    return result;
}
