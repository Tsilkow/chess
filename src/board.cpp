#include "board.hpp"


FEN getDefaultStart()
{
    return FEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1"};
}

Board::Board(ResourceHolder<sf::Texture, std::string>* textures, FEN startPos):
    m_textures(textures),
    m_marks(&SquareLComp),
    m_highlights(&SquareLComp),
    m_pieces(&SquareLComp),
    m_whiteToMove(true),
    m_wKCastle(false),
    m_wQCastle(false),
    m_bKCastle(false),
    m_bQCastle(false),
    m_enpassant(getInvalidSquare())
{
    m_boardSprite.setTexture(textures->get("board"));
    m_boardSprite.setPosition(sf::Vector2f(0, 0));
    
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
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Pawn>
						       (false, textures, Square(c, r))));
			break;
		    case 'n':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Knight>
						       (false, textures, Square(c, r))));
			break;
		    case 'b':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Bishop>
						       (false, textures, Square(c, r))));
			break;
		    case 'r':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Rook>
						       (false, textures, Square(c, r))));
			break;
		    case 'q':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Queen>
						       (false, textures, Square(c, r))));
			break;
		    case 'k':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<King>
						       (false, textures, Square(c, r))));
			break;
		    case 'P':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Pawn>
						       (true, textures, Square(c, r))));
			break;
		    case 'N':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Knight>
						       (true, textures, Square(c, r))));
			break;
		    case 'B':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Bishop>
						       (true, textures, Square(c, r))));
			break;
		    case 'R':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Rook>
						       (true, textures, Square(c, r))));
			break;
		    case 'Q':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<Queen>
						       (true, textures, Square(c, r))));
			break;
		    case 'K':
			m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<King>
						       (true, textures, Square(c, r))));
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

    findMoves();
}

bool Board::mark(Square at)
{
    auto temp = m_marks.find(at);
    if(temp == m_marks.end())
    {
	m_marks.insert(std::make_pair(at, sf::Sprite(m_textures->get("mark"))));
	m_marks[at].setPosition(sf::Vector2f((at.c-1)*GgetTileSize(), (at.r-1)*GgetTileSize()));
	return true;
    }
    m_marks.erase(temp);
    return false;
}

bool Board::highlight(Square at)
{
    if(m_highlights.find(at) == m_highlights.end())
    {
	m_highlights.insert(std::make_pair(at, sf::Sprite(m_textures->get("highlight"))));
	m_highlights[at].setPosition(sf::Vector2f((at.c-1)*GgetTileSize(), (at.r-1)*GgetTileSize()));
	m_highlights[at].setColor(sf::Color(255, 255, 255, 128));
	return true;
    }
    return false;
}

void Board::findMoves()
{
    m_moves.clear();
    
    for(auto &p: m_pieces)
    {
	if(p.second->isWhite() == m_whiteToMove)
	{
	    std::vector<Move> temp = p.second->getMoves(p.first, m_pieces);

	    m_moves.insert(m_moves.end(), temp.begin(), temp.end());

	    // special moves
	}
    }
    sort(m_moves.begin(), m_moves.end(), MoveLComp);
}

bool Board::makeAMove(Move chosen)
{
    bool found = false;
    for(auto &m: m_moves)
    {
	if(m == chosen)
	{
	    found = true;
	    break;
	}
    }
    if(!found) return false;

    m_pieces[chosen.from]->move(chosen.to);
    auto pieceToMove = m_pieces.extract(chosen.from);
    auto pieceToCapture = m_pieces.find(chosen.to);
	
    if(pieceToCapture != m_pieces.end()) m_pieces.erase(pieceToCapture);
	
    pieceToMove.key() = chosen.to;
    m_pieces.insert(std::move(pieceToMove));

    m_whiteToMove = !m_whiteToMove;
    findMoves();

    return true;
}

void Board::draw(sf::RenderTarget& target)
{
    target.draw(m_boardSprite);
    for(auto &h: m_highlights)
    {
	target.draw(h.second);
    }
    for(auto &p: m_pieces)
    {
	p.second->draw(target);
    }
    for(auto &m: m_marks)
    {
	target.draw(m.second);
    }
}
