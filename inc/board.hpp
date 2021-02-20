#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

#include "commons.hpp"
#include "piece.hpp"


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
    std::set<Move, decltype(&MoveLComp)> m_moves;
    
    public:
    Board(FEN start = getDefaultStart());
    
    void findMoves();

    bool makeAMove(Move chosen);

    const std::set<Move, decltype(&MoveLComp)>& getMoves() {return m_moves; }
};
