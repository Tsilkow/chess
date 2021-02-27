#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <sstream>


struct Square
{
    short c; // collumn
    short r; // row
    
    Square(int _c, int _r): c(_c), r(_r) {}
    bool isValid();
};

inline int GgetTileSize() {return 100; }

Square toSquare(std::string input);

inline Square getInvalidSquare() {return Square(0, 0); }

bool operator==(const Square& a, const Square& b);
inline bool operator!=(const Square& a, const Square& b) {return !(a==b); }
Square operator+(const Square& a, const Square& b);
void operator+=(Square& a, const Square& b);

std::ostream& operator<<(std::ostream& out, const Square& a);

template <typename T>
T& atSquare(std::vector< std::vector<T> >& data, Square square)
{
    return data[square.c][square.r];
}

template <typename T>
T atSquare(const std::vector< std::vector<T> >& data, Square square)
{
    return data[square.c][square.r];
}

struct Move
{
    Square from;
    Square to;
    bool capture;
    char promotion;
    bool kingCastle;
    bool queenCastle;

    Move(Square _from, Square _to, bool _capture=false, char _promotion=' ', bool _kingCastle=false,
	 bool _queenCastle=false):
	from(_from),
	to(_to),
	capture(_capture),
	promotion(_promotion),
	kingCastle(_kingCastle),
	queenCastle(_queenCastle)
	{}
};

inline Move getInvalidMove() {return Move{getInvalidSquare(), getInvalidSquare()}; }

bool operator==(const Move& a, const Move& b);
inline bool operator!=(const Move& a, const Move& b) {return !(a == b); }
bool SquareLComp(const Square& a, const Square& b);
bool MoveLComp(const Move& a, const Move& b);

std::ostream& operator<<(std::ostream& out, const Move& a);
