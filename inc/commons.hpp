#pragma once

#include <iostream>
#include <vector>
#include <sstream>


struct Square
{
    short c; // collumn
    short r; // row
    
    Square(int _c, int _r): c(_c), r(_r) {}
    bool isValid();
};

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
};
