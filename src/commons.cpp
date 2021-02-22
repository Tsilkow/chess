#include "commons.hpp"


bool Square::isValid()
{
    return (c >= 1 && c <= 8 && r >= 1 && r <= 8);
}

std::ostream& operator<<(std::ostream& out, const Square& a)
{
    switch(a.c)
    {
	case 1: out << "a"; break;
	case 2: out << "b"; break;
	case 3: out << "c"; break;
	case 4: out << "d"; break;
	case 5: out << "e"; break;
	case 6: out << "f"; break;
	case 7: out << "g"; break;
	case 8: out << "h"; break;
	default: out << "E"; break;
    }
    out << a.r;
    
    return out;
}

Square toSquare(std::string input)
{
    if(input.size() != 2 || input[0] < 'a' || input[0] > 'g' || input[1] < '1' || input[2] > '8')
	return getInvalidSquare();
    
    return Square(input[0]-'a'+1, input[1]-'1'+1);
}

bool operator==(const Square& a, const Square& b)
{
    return (a.c == b.c && a.r == b.r);
}

Square operator+(const Square& a, const Square& b)
{
    return Square(a.c + b.c, a.r + b.r);
}

void operator+=(Square& a, const Square& b)
{
    a.c += b.c;
    a.r += b.r;
}

bool operator==(const Move& a, const Move& b)
{
    return (a.from == b.from && a.to == b.to);
}

bool SquareLComp(const Square& a, const Square& b)
{
    if(a.c == b.c) return (a.r < b.r);
    else return (a.c < b.c);
}

bool MoveLComp(const Move& a, const Move& b)
{
    if(a.from == b.from) return SquareLComp(a.to, b.to);
    else return SquareLComp(a.from, b.from);
}
