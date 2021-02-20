#include "piece.hpp"


std::set<Move, decltype(&MoveLComp)> Pawn::getMoves
(const Square& position, const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces)
{
    std::set<Move, decltype(&MoveLComp)> result(&MoveLComp);
    int enemy = 1 - 2*m_isWhite;
    
    Square straight  = position + Square( 0, -enemy  );
    Square doubled   = position + Square( 0, -enemy*2);
    Square diagonal1 = position + Square(-1, -enemy  );
    Square diagonal2 = position + Square( 1, -enemy  );
    
    if(straight.isValid()  && pieces.count(straight) == 0)
    {
	result.insert(Move{position, straight});
	if(position.r == 7 - 6*m_isWhite && doubled.isValid() && pieces.count(doubled) == 0)
	    result.insert(Move{position, doubled});
    }
    if(diagonal1.isValid() &&
       pieces.find(diagonal1) != pieces.end() &&
       pieces.find(diagonal1)->second->isWhite() != m_isWhite)
    {
	result.insert(Move{position, diagonal1});
    }
    if(diagonal2.isValid() &&
       pieces.find(diagonal2) != pieces.end() &&
       pieces.find(diagonal2)->second->isWhite() != m_isWhite)
    {
	result.insert(Move{position, diagonal2});
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
	if(Ls[i].isValid() && (pieces.find(Ls[i]) == pieces.end() ||
			       pieces.find(Ls[i])->second->isWhite() != m_isWhite))
	    result.insert(Move{position, Ls[i]});
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
	
	while(curr.isValid() && pieces.find(curr) == pieces.end())
	{
	    result.insert(Move{position, curr});
	    curr += dir;
	}
	if(curr.isValid() && pieces.find(curr)->second->isWhite() != m_isWhite)
	    result.insert(Move{position, curr});
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
	
	while(curr.isValid() && pieces.find(curr) == pieces.end())
	{
	    result.insert(Move{position, curr});
	    curr += dir;
	}
	if(curr.isValid() && pieces.find(curr)->second->isWhite() != m_isWhite)
	    result.insert(Move{position, curr});
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
	
	while(curr.isValid() && pieces.find(curr) == pieces.end())
	{
	    result.insert(Move{position, curr});
	    curr += dir;
	}
	if(curr.isValid() && pieces.find(curr)->second->isWhite() != m_isWhite)
	    result.insert(Move{position, curr});
    }

    // bishop moves
    for(int i = 0; i < 4; ++i)
    {
	Square dir(1-2*(i/2), 1-2*(i%2));
	Square curr = position + dir;
	
	while(curr.isValid() && pieces.find(curr) == pieces.end())
	{
	    result.insert(Move{position, curr});
	    curr += dir;
	}
	if(curr.isValid() && pieces.find(curr)->second->isWhite() != m_isWhite)
	    result.insert(Move{position, curr});
    }

    return result;
}

std::set<Move, decltype(&MoveLComp)> King::getMoves
(const Square& position, const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces)
{
    std::set<Move, decltype(&MoveLComp)> result(&MoveLComp);
    
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
	if(neighbours[i].isValid() &&
	   (pieces.find(neighbours[i]) == pieces.end() ||
	    pieces.find(neighbours[i])->second->isWhite() != m_isWhite))
	{
	    result.insert(Move{position, neighbours[i]});
	}
    }

    return result;
}
