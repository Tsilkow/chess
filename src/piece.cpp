#include "piece.hpp"


bool Piece::move(Square moveTo)
{
    m_square = moveTo;
    return m_square.isValid();
}

std::vector<Move> Pawn::getMoves(const std::vector<std::vector<int>>& obstacles)
{
    std::vector<Move> result;
    
    Square straight  = m_square + Square( 0,  -1 + 2*m_isWhite   );
    Square doubled   = m_square + Square( 0, (-1 + 2*m_isWhite)*2);
    Square diagonal1 = m_square + Square(-1,  -1 + 2*m_isWhite   );
    Square diagonal2 = m_square + Square( 1,  -1 + 2*m_isWhite   );

    std::cout << straight << diagonal1 << diagonal2 << std::endl;
    
    if(straight.isValid()  && atSquare(obstacles, straight ) == 0)
    {
	result.push_back(Move{m_square, straight});
	if(m_square.r == 6 - 4*m_isWhite && doubled.isValid() && atSquare(obstacles, doubled) == 0)
	    result.push_back(Move{m_square, doubled});
    }
    if(diagonal1.isValid() && atSquare(obstacles, diagonal1) == 1 - 2*m_isWhite)
	result.push_back(Move{m_square, diagonal1});
    if(diagonal2.isValid() && atSquare(obstacles, diagonal2) == 1 - 2*m_isWhite)
	result.push_back(Move{m_square, diagonal2});

    return result;
}

std::vector<Move> Knight::getMoves(const std::vector<std::vector<int>>& obstacles)
{
    std::vector<Move> result;

    std::vector<Square> Ls =
    {
	m_square + Square( 1,  2),
	m_square + Square( 2,  1),
	m_square + Square( 2, -1),
	m_square + Square( 1, -2),
	m_square + Square(-1, -2),
	m_square + Square(-2, -1),
	m_square + Square(-2,  1),
	m_square + Square(-1,  2)
    };

    for(int i = 0; i < Ls.size(); ++i)
    {
	if(Ls[i].isValid() && atSquare(obstacles, Ls[i]) * (1 - 2*m_isWhite) >= 0)
	    result.push_back(Move{m_square, Ls[i]});
    }

    return result;
}

std::vector<Move> Bishop::getMoves(const std::vector<std::vector<int>>& obstacles)
{
    std::vector<Move> result;

    for(int i = 0; i < 4; ++i)
    {
	Square dir(1-2*(i/2), 1-2*(i%2));
	Square pos = m_square + dir;
	
	while(pos.isValid() && atSquare(obstacles, pos) == 0)
	{
	    result.push_back(Move{m_square, pos});
	    pos += dir;
	}
	if(pos.isValid() && atSquare(obstacles, pos) == (1 - 2*m_isWhite))
	    result.push_back(Move{m_square, pos});
    }

    return result;
}

std::vector<Move> Rook::getMoves(const std::vector<std::vector<int>>& obstacles)
{
    std::vector<Move> result;

    for(int i = 0; i < 4; ++i)
    {
	Square dir(((i+1)%2) * (1 - i/2*2), (i%2) * (1 - i/2*2));
	Square pos = m_square + dir;
	
	while(pos.isValid() && atSquare(obstacles, pos) == 0)
	{
	    result.push_back(Move{m_square, pos});
	    pos += dir;
	}
	if(pos.isValid() && atSquare(obstacles, pos) == (1 - 2*m_isWhite))
	    result.push_back(Move{m_square, pos});
    }

    return result;
}

std::vector<Move> Queen::getMoves(const std::vector<std::vector<int>>& obstacles)
{
    std::vector<Move> result;

    // rook moves
    for(int i = 0; i < 4; ++i)
    {
	Square dir(((i+1)%2) * (1 - i/2*2), (i%2) * (1 - i/2*2));
	Square pos = m_square + dir;
	
	while(pos.isValid() && atSquare(obstacles, pos) == 0)
	{
	    result.push_back(Move{m_square, pos});
	    pos += dir;
	}
	if(pos.isValid() && atSquare(obstacles, pos) == (1 - 2*m_isWhite))
	    result.push_back(Move{m_square, pos});
    }

    // bishop moves
    for(int i = 0; i < 4; ++i)
    {
	Square dir(1-2*(i/2), 1-2*(i%2));
	Square pos = m_square + dir;
	
	while(pos.isValid() && atSquare(obstacles, pos) == 0)
	{
	    result.push_back(Move{m_square, pos});
	    pos += dir;
	}
	if(pos.isValid() && atSquare(obstacles, pos) == (1 - 2*m_isWhite))
	    result.push_back(Move{m_square, pos});
    }

    return result;
}

std::vector<Move> King::getMoves(const std::vector<std::vector<int>>& obstacles)
{
    std::vector<Move> result;
    
    std::vector<Square> neighbours =
    {
	m_square + Square( 0,  1),
	m_square + Square( 1,  1),
	m_square + Square( 1,  0),
	m_square + Square( 1, -1),
	m_square + Square( 0, -1),
	m_square + Square(-1, -1),
	m_square + Square(-1,  0),
	m_square + Square(-1,  1)
    };

    for(int i = 0; i < neighbours.size(); ++i)
    {
	if(neighbours[i].isValid() && atSquare(obstacles, neighbours[i]) * (1 - 2*m_isWhite) >= 0)
	    result.push_back(Move{m_square, neighbours[i]});
    }

    return result;
}
