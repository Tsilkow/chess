#include "board.hpp"


FEN getDefaultStart()
{
    return FEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1"};
}

Board::Board(FEN startPos):
    m_pieces(&SquareLComp),
    m_moves(&MoveLComp),
    m_whiteToMove(true),
    m_wKCastle(false),
    m_wQCastle(false),
    m_bKCastle(false),
    m_bQCastle(false),
    m_enpassant(getInvalidSquare())
{
    // Catching obvious FEN mistakes
    
    for(int i = 0; i < 6; ++i)
    {
	if(startPos.segments[i] == "")
	{
	    startPos = getDefaultStart();
	    std::cout << "!ERROR! Given starting position had an empty FEN segment. Using default starting position." << std::endl;
	    break;
	}
    }

    // Processing FEN notation

    bool error = false;
    do{
	if(error)
	{
	    startPos = getDefaultStart();
	    m_pieces.clear();
	    m_whiteToMove = true;
	    m_wKCastle = false;
	    m_wQCastle = false;
	    m_bKCastle = false;
	    m_bQCastle = false;
	    m_enpassant = getInvalidSquare();
	    error = false;
	}

	// 1st segment
	int previousSlash = -1;
	for(int r = 8; r > 0; --r)
	{
	    int start = previousSlash+1;
	    int end = startPos.segments[0].find("/", start);
	    if(end == std::string::npos)
	    {
		if(r != 1)
		{
		    std::cout << "!ERROR! Too few rows in 1st segment of FEN in starting position. Using default starting position ..." << std::endl;
		    error = true;
		    break;
		}
		end = startPos.segments[0].size()-1;
	    }
	    previousSlash = end;
	    
	    int c = 1;
	    for(int pos = start; pos <= end && c <= 8; ++pos)
	    {
		if(c > 8)
		{
		    std::cout << "!ERROR! Too many squares in row " << r << " in 1st segment of FEN in starting position. Using default starting position ..." << std::endl;
		    error = true;
		    break;
		}
	    
		switch(startPos.segments[0][pos])
		{
		    case '8': ++c;
		    case '7': ++c;
		    case '6': ++c;
		    case '5': ++c;
		    case '4': ++c;
		    case '3': ++c;
		    case '2': ++c;
		    case '1': break;
		    case 'p':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Pawn>  (false)));
			break;
		    case 'n':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Knight>(false)));
			break;
		    case 'b':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Bishop>(false)));
			break;
		    case 'r':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Rook>  (false)));
			break;
		    case 'q':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Queen> (false)));
			break;
		    case 'k':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<King>  (false)));
			break;
		    case 'P':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Pawn>  (true)));
			break;
		    case 'N':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Knight>(true)));
			break;
		    case 'B':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Bishop>(true)));
			break;
		    case 'R':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Rook>  (true)));
			break;
		    case 'Q':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Queen> (true)));
			break;
		    case 'K':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<King>  (true)));
			break;
		    default:
			std::cout << "!ERROR! Invalid character in 1st segment of FEN in starting position. Using default starting position ..." << std::endl;
			error = true;
			break;
		}
		++c;
		if(error) break;
	    }
	    if(error) break;
	    if(c < 8)
	    {
		std::cout << "!ERROR! Too few squares in row " << r << " in 1st segment of FEN in starting position. Using default starting position ..." << std::endl;
		error = true;
		break;
	    }
	}

	if(!error)
	{
	    // 2nd segment
	    switch(startPos.segments[1][0])
	    {
		case 'w': m_whiteToMove = true; break;
		case 'b': m_whiteToMove = false; break;
		default:
		    std::cout << "!Error! Inalid notation in 2nd segment of FEN in starting postion. Using default starting position ..." << std::endl;
		    error = true;
		    break;
	    }
	}

	if(!error)
	{
	    // 3rd segment
	    if(startPos.segments[2] != "-")
	    {
		for(int i = 0; i < startPos.segments[2].size(); ++i)
		{
		    switch(startPos.segments[2][i])
		    {
			case 'k': m_bKCastle = true; break;
			case 'q': m_bQCastle = true; break;
			case 'K': m_wKCastle = true; break;
			case 'Q': m_wQCastle = true; break;
			default: std::cout << "!Error! Invalid notation in 3rd segment of FEN in starting postion. Using default starting position ..." << std::endl;
			    error = true;
			    break;
		    }
		}
	    }
	}
	
	if(!error)
	{
// 4th segment
	    if(toSquare(startPos.segments[3]) != getInvalidSquare()) m_enpassant = toSquare(startPos.segments[3]);
	    else if(startPos.segments[3] != "-")
	    {
		std::cout << "!Error! Invalid notation in 4th segment of FEN in starting position. Using default starting position ..." << std::endl;
		error = true;
		break;
	    }
	}

	
	if(!error)
	{
	    // 5th segment
	    m_movesSinceCapture = std::stoi(startPos.segments[4]);
	    if(m_movesSinceCapture < 0)
	    {
		m_movesSinceCapture = 0;
		std::cout << "!Error! Invalid notation in 5th segment of FEN in starting position. Using default starting position ..." << std::endl;
		error = true;
		break;
	    }
	}

	if(!error)
	{
	    // 6th segment
	    m_movesSinceStart = std::stoi(startPos.segments[4]);
	    if(m_movesSinceStart < 0)
	    {
		m_movesSinceStart = 0;
		std::cout << "!Error! Invalid notation in 6th segment of FEN in starting position. Using default starting position ..." << std::endl;
		error = true;
		break;
	    }
	}
	
    }while(error);
}

void Board::findMoves()
{
    m_moves.clear();
    
    for(auto &p: m_pieces)
    {
	if(p.second->isWhite() == m_whiteToMove)
	{
	    std::set<Move, decltype(&MoveLComp)> temp = p.second->getMoves(p.first, m_pieces);

	    m_moves.merge(temp);
	}
    }
}

bool Board::makeAMove(Move chosen)
{
    if(m_moves.find(chosen) != m_moves.end())
    {
	auto pieceToMove = m_pieces.extract(chosen.from);
	auto pieceToCapture = m_pieces.find(chosen.to);
	
	if(pieceToCapture != m_pieces.end()) m_pieces.erase(pieceToCapture);
	
	pieceToMove.key() = chosen.to;
	m_pieces.insert(std::move(pieceToMove));

	m_moves.clear();
	m_whiteToMove = !m_whiteToMove;

	return true;
    }

    return false;
}
