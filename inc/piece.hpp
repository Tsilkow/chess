#pragma once

#include <set>
#include <map>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "commons.hpp"
#include "resources.hpp"


class Piece
{
    protected:
    std::string m_abbreviation;
    bool m_isWhite;
    sf::Sprite m_sprite;
    
    public:
    Piece() = delete;

    Piece(std::string abbreviation, bool isWhite, ResourceHolder<sf::Texture, std::string>& resources,
	  Square startPos);
    virtual std::vector<Move> getMoves
    (const Square& position, const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces)=0;
    // -1 = black, 0 = empty, 1 = white

    void move(Square moveTo);

    void draw(sf::RenderTarget& target);
    
    const std::string& getAbbrev() const {return m_abbreviation; }
    const bool isWhite() const {return m_isWhite; }
};

class Pawn: public Piece
{
    private:

    public:
    Pawn(bool isWhite, ResourceHolder<sf::Texture, std::string>& resources, Square startPos):
	Piece("", isWhite, resources, startPos) {};

    std::vector<Move> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};

class Knight: public Piece
{
    private:

    public:
    Knight(bool isWhite, ResourceHolder<sf::Texture, std::string>& resources, Square startPos):
	Piece("N", isWhite, resources, startPos) {};

    std::vector<Move> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};

class Bishop: public Piece
{
    private:

    public:
    Bishop(bool isWhite, ResourceHolder<sf::Texture, std::string>& resources, Square startPos):
	Piece("B", isWhite, resources, startPos) {};

    std::vector<Move> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};

class Rook: public Piece
{
    private:

    public:
    Rook(bool isWhite, ResourceHolder<sf::Texture, std::string>& resources, Square startPos):
	Piece("R", isWhite, resources, startPos) {};

    std::vector<Move> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};

class Queen: public Piece
{
    private:

    public:
    Queen(bool isWhite, ResourceHolder<sf::Texture, std::string>& resources, Square startPos):
	Piece("Q", isWhite, resources, startPos) {};

    std::vector<Move> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};

class King: public Piece
{
    private:

    public:
    King(bool isWhite, ResourceHolder<sf::Texture, std::string>& resources, Square startPos):
	Piece("K", isWhite, resources, startPos) {};

    std::vector<Move> getMoves
    (const Square& position,
     const std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)>& pieces) override;
};
