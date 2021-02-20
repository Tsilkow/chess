#pragma once


#include "commons.hpp"


class Piece
{
    protected:
    std::string m_abbreviation;
    bool m_isWhite;
    Square m_square;
    
    public:
    Piece() = delete;

    Piece(std::string abbreviation, bool isWhite, Square square):
	m_abbreviation(abbreviation),
	m_isWhite(isWhite),
	m_square(square)
	{std::cout << "[" << m_abbreviation << ", " << m_isWhite << ", " << m_square << "]\n"; }

    bool move(Square moveTo);

    virtual std::vector<Move> getMoves(const std::vector<std::vector<int>>& obstacles)=0;
    // -1 = black, 0 = empty, 1 = white
    
    const std::string& getAbbrev() {return m_abbreviation; }
    const bool isWhite() {return m_isWhite; }
    const Square& getSquare() {return m_square; }
};

class Pawn: public Piece
{
    private:

    public:
    Pawn(bool isWhite, Square square): Piece("", isWhite, square) {};

    std::vector<Move> getMoves(const std::vector<std::vector<int>>& obstacles) override;
};

class Knight: public Piece
{
    private:

    public:
    Knight(bool isWhite, Square square): Piece("N", isWhite, square) {};

    std::vector<Move> getMoves(const std::vector<std::vector<int>>& obstacles) override;
};

class Bishop: public Piece
{
    private:

    public:
    Bishop(bool isWhite, Square square): Piece("B", isWhite, square) {};

    std::vector<Move> getMoves(const std::vector<std::vector<int>>& obstacles) override;
};

class Rook: public Piece
{
    private:

    public:
    Rook(bool isWhite, Square square): Piece("R", isWhite, square) {};

    std::vector<Move> getMoves(const std::vector<std::vector<int>>& obstacles) override;
};

class Queen: public Piece
{
    private:

    public:
    Queen(bool isWhite, Square square): Piece("Q", isWhite, square) {};

    std::vector<Move> getMoves(const std::vector<std::vector<int>>& obstacles) override;
};

class King: public Piece
{
    private:

    public:
    King(bool isWhite, Square square): Piece("K", isWhite, square) {};

    std::vector<Move> getMoves(const std::vector<std::vector<int>>& obstacles) override;
};
