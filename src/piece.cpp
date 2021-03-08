#include "piece.hpp"


bool isEmpty(std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)> pieces, const Square& square)
{
    return (pieces.count(square) == 0);
}

bool isEnemy(std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)> pieces,
	     const Square& square,
	     bool isWhite)
{
    return (!isEmpty(pieces, square) && pieces[square]->isWhite() != isWhite);
}

Piece::Piece(std::string abbreviation, bool isWhite,
	     ResourceHolder<sf::Texture, std::string>* textures, Square startPos):
	m_abbreviation(abbreviation),
	m_isWhite(isWhite)
{
    if(m_isWhite) m_sprite.setTexture(textures->get("w" + m_abbreviation));
    else          m_sprite.setTexture(textures->get("b" + m_abbreviation));

    move(startPos);
}

void Piece::move(Square moveTo)
{
    m_sprite.setPosition(sf::Vector2f((moveTo.c-1) * m_sprite.getGlobalBounds().width,
				      (8-moveTo.r) * m_sprite.getGlobalBounds().height));
}

void Piece::draw(sf::RenderTarget& target)
{
    target.draw(m_sprite);
}

std::set<Move, decltype(&MoveLComp)> Pawn::getMoves
(const Square& position, const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces)
{
    std::set<Move, decltype(&MoveLComp)> result(&MoveLComp);
    int enemy = 1 - 2*m_isWhite;
    
    Square straight  = position + Square( 0, -enemy  );
    Square doubled   = position + Square( 0, -enemy*2);
    Square diagonal1 = position + Square(-1, -enemy  );
    Square diagonal2 = position + Square( 1, -enemy  );
    
    if(straight.isValid()  && isEmpty(pieces, straight))
    {
	if(straight.r != 8) result.insert(Move(position, straight));
	else
	{
	    result.insert(Move(position, straight, false, 'N'));
	    result.insert(Move(position, straight, false, 'B'));
	    result.insert(Move(position, straight, false, 'R'));
	    result.insert(Move(position, straight, false, 'Q'));
	}
	    
	if(position.r == 7 - 5*m_isWhite && doubled.isValid() && isEmpty(pieces, doubled))
	    result.insert(Move(position, doubled));
    }
    if(diagonal1.isValid() && isEnemy(pieces, diagonal1, m_isWhite))
    {
	if(diagonal1.r != 8) result.insert(Move(position, diagonal1, true));
	else
	{
	    result.insert(Move(position, straight, true, 'N'));
	    result.insert(Move(position, straight, true, 'B'));
	    result.insert(Move(position, straight, true, 'R'));
	    result.insert(Move(position, straight, true, 'Q'));
	}
    }
    if(diagonal2.isValid() && isEnemy(pieces, diagonal2, m_isWhite))
    {
	if(diagonal2.r != 8) result.insert(Move(position, diagonal2, true));
	else
	{
	    result.insert(Move(position, straight, true, 'N'));
	    result.insert(Move(position, straight, true, 'B'));
	    result.insert(Move(position, straight, true, 'R'));
	    result.insert(Move(position, straight, true, 'Q'));
	}
    }

    return result;
}

std::set<Move, decltype(&MoveLComp)> Knight::getMoves
(const Square& position, const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces)
{
    std::set<Move, decltype(&MoveLComp)> result(&MoveLComp);

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
	    if     (isEnemy(pieces, Ls[i], m_isWhite)) result.insert(Move(position, Ls[i], true));
	    else if(isEmpty(pieces, Ls[i]))            result.insert(Move(position, Ls[i], false));
	}
    }

    return result;
}

std::set<Move, decltype(&MoveLComp)> Bishop::getMoves
(const Square& position, const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces)
{
    std::set<Move, decltype(&MoveLComp)> result(&MoveLComp);

    for(int i = 0; i < 4; ++i)
    {
	Square dir(1-2*(i/2), 1-2*(i%2));
	Square curr = position + dir;
	
	while(curr.isValid() && isEmpty(pieces, curr))
	{
	    result.insert(Move(position, curr));
	    curr += dir;
	}
	if(curr.isValid() && isEnemy(pieces, curr, m_isWhite)) result.insert(Move(position, curr, true));
    }

    return result;
}

std::set<Move, decltype(&MoveLComp)> Rook::getMoves
(const Square& position, const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces)
{
    std::set<Move, decltype(&MoveLComp)> result(&MoveLComp);

    for(int i = 0; i < 4; ++i)
    {
	Square dir(((i+1)%2) * (1 - i/2*2), (i%2) * (1 - i/2*2));
	Square curr = position + dir;
	
	while(curr.isValid() && isEmpty(pieces, curr))
	{
	    result.insert(Move(position, curr));
	    curr += dir;
	}
	if(curr.isValid() && isEnemy(pieces, curr, m_isWhite)) result.insert(Move(position, curr, true));
    }

    return result;
}

std::set<Move, decltype(&MoveLComp)> Queen::getMoves
(const Square& position, const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces)
{
    std::set<Move, decltype(&MoveLComp)> result(&MoveLComp);

    // rook moves
    for(int i = 0; i < 4; ++i)
    {
	Square dir(((i+1)%2) * (1 - i/2*2), (i%2) * (1 - i/2*2));
	Square curr = position + dir;
	
	while(curr.isValid() && isEmpty(pieces, curr))
	{
	    result.insert(Move(position, curr));
	    curr += dir;
	}
	if(curr.isValid() && isEnemy(pieces, curr, m_isWhite)) result.insert(Move(position, curr, true));
    }

    // bishop moves
    for(int i = 0; i < 4; ++i)
    {
	Square dir(1-2*(i/2), 1-2*(i%2));
	Square curr = position + dir;
	
	while(curr.isValid() && isEmpty(pieces, curr))
	{
	    result.insert(Move(position, curr));
	    curr += dir;
	}
	if(curr.isValid() && isEnemy(pieces, curr, m_isWhite)) result.insert(Move(position, curr, true));
    }

    return result;
}

std::set<Move, decltype(&MoveLComp)> King::getMoves
(const Square& position, const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces)
{
    std::set<Move, decltype(&MoveLComp)> result(&MoveLComp);
    
    std::vector<Square> Ns =
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

    for(int i = 0; i < Ns.size(); ++i)
    {
	if(Ns[i].isValid())
	{
	    if     (isEnemy(pieces, Ns[i], m_isWhite)) result.insert(Move(position, Ns[i], true));
	    else if(isEmpty(pieces, Ns[i]))            result.insert(Move(position, Ns[i], false));
	}
    }

    return result;
}
