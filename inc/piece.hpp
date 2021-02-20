#pragma once

#include <set>
#include <map>
#include "commons.hpp"


class Piece
{
    protected:
    std::string m_abbreviation;
    bool m_isWhite;
    
    public:
    Piece() = delete;

    Piece(std::string abbreviation, bool isWhite):
	m_abbreviation(abbreviation),
	m_isWhite(isWhite)
	{std::cout << "[" << m_abbreviation << ", " << m_isWhite << "]\n"; }

    virtual std::set<Move, decltype(&MoveLComp)> getMoves
    (const Square& position, const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces)=0;
    // -1 = black, 0 = empty, 1 = white
    
    const std::string& getAbbrev() const {return m_abbreviation; }
    const bool isWhite() const {return m_isWhite; }
};

class Pawn: public Piece
{
    private:

    public:
    Pawn(bool isWhite): Piece("", isWhite) {};

    std::set<Move, decltype(&MoveLComp)> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};

class Knight: public Piece
{
    private:

    public:
    Knight(bool isWhite): Piece("N", isWhite) {};

    std::set<Move, decltype(&MoveLComp)> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};

class Bishop: public Piece
{
    private:

    public:
    Bishop(bool isWhite): Piece("B", isWhite) {};

    std::set<Move, decltype(&MoveLComp)> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};

class Rook: public Piece
{
    private:

    public:
    Rook(bool isWhite): Piece("R", isWhite) {};

    std::set<Move, decltype(&MoveLComp)> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};

class Queen: public Piece
{
    private:

    public:
    Queen(bool isWhite): Piece("Q", isWhite) {};

    std::set<Move, decltype(&MoveLComp)> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};

class King: public Piece
{
    private:

    public:
    King(bool isWhite): Piece("K", isWhite) {};

    std::set<Move, decltype(&MoveLComp)> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};
