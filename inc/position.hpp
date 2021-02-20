#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>

#include "commons.hpp"
#include "piece.hpp"


struct FEN
{
    std::array<std::string, 6> segments;
};

FEN getDefaultStart();

class Position
{
    private:
    std::vector<std::shared_ptr<Piece>> m_pieces;
    bool m_whiteToMove;
    bool m_wKCastle; // can white castle kingside
    bool m_wQCastle; // can white castle queenside
    bool m_bKCastle; // can black castle kingside
    bool m_bQCastle; // can black castle queenside
    Square m_enpassant; // en passant move in previous turn; if there wasn't one, it's set to InvalidSquare
    int m_moves;
    int m_movesSinceCapture;
    std::vector<std::vector<int>> m_obstacles;
    
    public:
    Position(FEN start = getDefaultStart());

    std::vector<Move> getMoves();
};
