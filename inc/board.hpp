#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "commons.hpp"
#include "piece.hpp"
#include "resources.hpp"


struct FEN
{
    std::array<std::string, 6> segments;
};

FEN getDefaultStart();

class Board
{
    private:
    std::map<Square, std::shared_ptr<Piece>, decltype(&SquareLComp)> m_pieces;
    bool m_whiteToMove;
    bool m_wKCastle; // can white castle kingside
    bool m_wQCastle; // can white castle queenside
    bool m_bKCastle; // can black castle kingside
    bool m_bQCastle; // can black castle queenside
    Square m_enpassant; // en passant move in previous turn; if there wasn't one, it's set to InvalidSquare
    int m_movesSinceStart;
    int m_movesSinceCapture;
    std::vector<Move> m_moves;
    sf::Sprite m_boardSprite;
    
    public:
    Board(ResourceHolder<sf::Texture, std::string>& textures, FEN start = getDefaultStart());
    
    void findMoves();

    bool makeAMove(Move chosen);

    void draw(sf::RenderTarget& target);

    const std::vector<Move>& getMoves() {return m_moves; }
};
