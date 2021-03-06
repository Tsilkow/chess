#pragma once

#include <set>
#include <map>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "commons.hpp"
#include "resources.hpp"


inline int sideSign(bool isWhite) {return 2*isWhite - 1; }

struct SquareStatus
{
    int occupied; // -1 = black, 0 = empty, 1 = white
    int guardedByWhite;
    int guardedByBlack;

    bool isEnemy(bool isWhite) {return (occupied == sideSign(!isWhite)); }
    bool isEmpty() {return (occupied == 0); }
    bool isGuarded(bool isWhite) {return (isWhite && guardedByBlack > 0) || (!isWhite && guardedByWhite > 0); }
    bool isDoublyGuarded(bool isWhite)
	{return (isWhite && guardedByBlack > 1) || (!isWhite && guardedByWhite > 1); }
};

class Piece
{
    protected:
    std::string m_abbreviation;
    bool m_isWhite;
    sf::Sprite m_sprite;
    
    public:
    Piece() = delete;

    Piece(std::string abbreviation, bool isWhite,
	  ResourceHolder<sf::Texture, std::string>* textures, Square startPos);
    virtual std::vector<Move> getMoves
    (const Square& position, std::vector< std::vector<SquareStatus> >& squareStatuses)=0;
    // -1 = black, 0 = empty, 1 = white

    void move(const Move& move, std::vector< std::vector<SquareStatus> >& squareStatuses);

    void draw(sf::RenderTarget& target);
    
    const std::string& getAbbrev() const {return m_abbreviation; }
    const bool isWhite() const {return m_isWhite; }
};

class Pawn: public Piece
{
    private:

    public:
    Pawn(bool isWhite, ResourceHolder<sf::Texture, std::string>* textures, Square startPos):
	Piece("", isWhite, textures, startPos) {};

    std::vector<Move> getMoves
    (const Square& position, std::vector< std::vector<SquareStatus> >& squareStatuses) override;
};

class Knight: public Piece
{
    private:

    public:
    Knight(bool isWhite, ResourceHolder<sf::Texture, std::string>* textures,
	   Square startPos):
	Piece("N", isWhite, textures, startPos) {};

    std::vector<Move> getMoves
    (const Square& position, std::vector< std::vector<SquareStatus> >& squareStatuses) override;
};

class Bishop: public Piece
{
    private:

    public:
    Bishop(bool isWhite, ResourceHolder<sf::Texture, std::string>* textures,
	   Square startPos):
	Piece("B", isWhite, textures, startPos) {};

    std::vector<Move> getMoves
    (const Square& position, std::vector< std::vector<SquareStatus> >& squareStatuses) override;
};

class Rook: public Piece
{
    private:

    public:
    Rook(bool isWhite, ResourceHolder<sf::Texture, std::string>* textures,
	 Square startPos):
	Piece("R", isWhite, textures, startPos) {};

    std::vector<Move> getMoves
    (const Square& position, std::vector< std::vector<SquareStatus> >& squareStatuses) override;
};

class Queen: public Piece
{
    private:

    public:
    Queen(bool isWhite, ResourceHolder<sf::Texture, std::string>* textures,
	  Square startPos):
	Piece("Q", isWhite, textures, startPos) {};

    std::vector<Move> getMoves
    (const Square& position, std::vector< std::vector<SquareStatus> >& squareStatuses) override;
};

class King: public Piece
{
    private:

    public:
    King(bool isWhite, ResourceHolder<sf::Texture, std::string>* textures,
	 Square startPos):
	Piece("K", isWhite, textures, startPos) {};

    std::vector<Move> getMoves
    (const Square& position, std::vector< std::vector<SquareStatus> >& squareStatuses) override;
};
