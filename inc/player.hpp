#pragma once

#include "commons.hpp"
#include "board.hpp"

class Player
{
    protected:
    std::string m_name;
    Board* m_board;
    Move m_move;
    bool m_needsInput;

    public:
    Player(std::string name): m_name(name), m_board(nullptr), m_move(getInvalidMove()) {}

    void givePosition(Board* board);

    virtual void tick()=0;

    const std::string& getName() {return m_name; }
    const bool isMoveReady() {return (m_move != getInvalidMove()); }
    const bool needsInput() {return m_needsInput; }
    const Move& getMove() {return m_move; }
};

class Human: public Player
{
    sf::RenderTarget* m_window;
    std::vector<Move> m_activeMoves;
    
    public:
    Human(std::string name, sf::RenderTarget* window): Player(name), m_window(window) {};

    void tick() override {};

    bool processInput(int ticksPassed, sf::Vector2f mouse, sf::Event event);
};
