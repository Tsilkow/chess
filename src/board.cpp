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
    m_moves(&MoveLComp),
    m_whiteToMove(true),
    m_wKCastle(false),
    m_wQCastle(false),
    m_bKCastle(false),
    m_bQCastle(false),
    m_enpassant(getInvalidSquare()),
    m_wKingPos(getInvalidSquare()),
    m_bKingPos(getInvalidSquare()),
    m_state(Normal)
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
			if(m_bKingPos != getInvalidSquare())
			{
			    std::cout << "!ERROR! Too many black kings on the board. Using default starting position ..." << std::endl;
			    error = true;
			}
			else
			{
			    m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<King>
							   (false, textures, Square(c, r))));
			    m_bKingPos = Square(c, r);
			}
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
			if(m_wKingPos != getInvalidSquare())
			{
			    std::cout << "!ERROR! Too many black kings on the board. Using default starting position ..." << std::endl;
			    error = true;
			}
			else
			{
			    m_pieces.insert(std::make_pair(Square(c, r), std::make_shared<King>
							   (true, textures, Square(c, r))));
			    m_wKingPos = Square(c, r);
			}
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
	m_marks[at].setPosition(sf::Vector2f((at.c-1)*GgetTileSize(), (8-at.r)*GgetTileSize()));
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
	m_highlights[at].setPosition(sf::Vector2f((at.c-1)*GgetTileSize(), (8-at.r)*GgetTileSize()));
	m_highlights[at].setColor(sf::Color(255, 255, 255, 128));
	return true;
    }
    return false;
}

std::vector<Square> Board::findAttackers(const Square& target, bool side)
{
    std::vector<Square> result;
    
    for(auto it = m_pieces.begin(); it != m_pieces.end(); ++it)
    {
	if(it->second->isWhite() != side)
	{
	    auto moves = it->second->getMoves(it->first, m_pieces);

	    for(auto jt = moves.begin(); jt != moves.end(); ++jt)
	    {
		if(jt->to == target)
		{
		    result.push_back(jt->from);
		    break;
		}
	    }
	}
    }

    return result;
}

void Board::findMoves()
{
    bool doubleCheck = false;
    Square kingPos = getInvalidSquare();
    if(m_whiteToMove) kingPos = m_wKingPos;
    else kingPos = m_bKingPos;
    m_moves.clear();
    
    for(auto &p: m_pieces)
    {
	if(p.second->isWhite() == m_whiteToMove)
	{
	    std::set<Move, decltype(&MoveLComp)> temp = p.second->getMoves(p.first, m_pieces);

	    m_moves.insert(temp.begin(), temp.end());

	    // special moves
	}
    }

    {
	// check detection
	std::vector<Square> attackers = findAttackers(kingPos, m_whiteToMove);

	std::cout << "attackers: ";
	for(int i = 0; i < attackers.size(); ++i) std::cout << attackers[i] << " ";
	std::cout << "\n";
	
	switch(attackers.size())
	{
	    case 2: doubleCheck = true;
	     // only king moves can be legal
	    case 1: m_state = BoardState::Check; break;
             // only king moves, captures of attackers and blocks can be legal
	    case 0: m_state = BoardState::Normal; break;
	}

	if(m_state == BoardState::Check)
	{
	    std::cout << "check!\n";
	    std::set<Move, decltype(&MoveLComp)> newMoves(&MoveLComp);
	    
	    // king moves
	    for(auto it = m_moves.begin(); it != m_moves.end(); ++it)
	    {
		if(it->from == kingPos)
		{
		    std::cout << *it << std::endl;
		    newMoves.insert(*it);
		}
	    }
	    
	    if(!doubleCheck)
	    {
		// captures of the attacker
		for(auto it = m_moves.begin(); it != m_moves.end(); ++it)
		{
		    if(it->to == attackers[0]) newMoves.insert(*it);
		}

		// moves that block the check
		if(m_pieces[attackers[0]]->getAbbrev() == "B" ||
		   m_pieces[attackers[0]]->getAbbrev() == "Q")
		{
		    if(kingPos.getDiag1() == attackers[0].getDiag1())
		    {
			for(auto it = m_moves.begin(); it != m_moves.end(); ++it)
			{
			    if(it->to.getDiag1() == kingPos.getDiag1() &&
			       ((it->to.getDiag2() < kingPos.getDiag2() &&
				 it->to.getDiag2() > attackers[0].getDiag2()) ||
				(it->to.getDiag2() > kingPos.getDiag2() &&
				 it->to.getDiag2() < attackers[0].getDiag2())))
				newMoves.insert(*it);
			}
		    }
		    else if(kingPos.getDiag2() == attackers[0].getDiag2())
		    {
			for(auto it = m_moves.begin(); it != m_moves.end(); ++it)
			{
			    if(it->to.getDiag2() == kingPos.getDiag2() &&
			       ((it->to.getDiag1() < kingPos.getDiag1() &&
				 it->to.getDiag1() > attackers[0].getDiag1()) ||
				(it->to.getDiag1() > kingPos.getDiag1() &&
				 it->to.getDiag1() < attackers[0].getDiag1())))
				newMoves.insert(*it);
			}
		    }
		}
		
		if(m_pieces[attackers[0]]->getAbbrev() == "R" ||
		   m_pieces[attackers[0]]->getAbbrev() == "Q")
		{
		    if(kingPos.r == attackers[0].r)
		    {
			for(auto it = m_moves.begin(); it != m_moves.end(); ++it)
			{
			    if(it->to.r == kingPos.r &&
			       ((it->to.c < kingPos.c && it->to.c > attackers[0].c) ||
				(it->to.c > kingPos.c && it->to.c < attackers[0].c)))
				newMoves.insert(*it);
			}
		    }
		    else if(kingPos.c == attackers[0].c)
		    {
			for(auto it = m_moves.begin(); it != m_moves.end(); ++it)
			{
			    if(it->to.c == kingPos.c &&
			       ((it->to.r < kingPos.r && it->to.r > attackers[0].r) ||
				(it->to.r > kingPos.r && it->to.r < attackers[0].r)))
				newMoves.insert(*it);
			}
		    }
		}
	    }
	    m_moves = newMoves;
	}

    }

    // detection of king moves into check squares
    for(auto it = m_moves.begin(); it != m_moves.end();)
    {
	if(it->from == kingPos && findAttackers(it->to, m_whiteToMove).size() > 0) it = m_moves.erase(it);
	else ++it;
    }

    // detection of moves breaking absolute pins
    {
	for(auto it = m_pieces.begin(); it != m_pieces.end(); ++it)
	{
	    if(it->second->isWhite() == m_whiteToMove &&
	       (it->second->getAbbrev() == "B" || it->second->getAbbrev() == "Q"))
	    {
		if(kingPos.getDiag1() == it->first.getDiag1())
		{
		    for(auto jt = m_moves.begin(); jt != m_moves.end();)
		    {
			if(jt->from.getDiag1() == kingPos.getDiag1() && // same diagonal as king
			   ((jt->from.getDiag2() < kingPos.getDiag2() && // start between king and pinner
			     jt->from.getDiag2() > it->first.getDiag2()) ||
			    (jt->from.getDiag2() > kingPos.getDiag2() &&
			     jt->from.getDiag2() < it->first.getDiag2())) &&
			   !((jt->to.getDiag2()  < kingPos.getDiag2() && // end not between king and pinner
			     jt->to.getDiag2()   > it->first.getDiag2()) ||
			    (jt->to.getDiag2()   > kingPos.getDiag2() &&
			     jt->to.getDiag2()   < it->first.getDiag2())))
			{
			    jt = m_moves.erase(jt);
			}
			else ++jt;
		    }
		}
		else if(kingPos.getDiag2() == it->first.getDiag2())
		{
		    for(auto jt = m_moves.begin(); jt != m_moves.end();)
		    {
			if(jt->from.getDiag2() == kingPos.getDiag2() && // same diagonal as king
			   ((jt->from.getDiag1() < kingPos.getDiag1() && // start between king and pinner
			     jt->from.getDiag1() > it->first.getDiag1()) ||
			    (jt->from.getDiag1() > kingPos.getDiag1() &&
			     jt->from.getDiag1() < it->first.getDiag1())) &&
			   !((jt->to.getDiag1()  < kingPos.getDiag1() && // end not between king and pinner
			      jt->to.getDiag1()   > it->first.getDiag1()) ||
			     (jt->to.getDiag1()   > kingPos.getDiag1() &&
			      jt->to.getDiag1()   < it->first.getDiag1())))
			{
			    jt = m_moves.erase(jt);
			}
			else ++jt;
		    }
		}
	    }

	    // I HAVE TO BE FUCKING CERTAIN TAHT THIS IS THE ONLY PIECE THAT IS BETWEEN POTENTIAL PINNER AND KING, OTHERWISE THE POTENTIAL PINNER IS NOT A FUCKING PINNER

	    if(it->second->isWhite() == m_whiteToMove &&
	       (it->second->getAbbrev() == "R" || it->second->getAbbrev() == "Q"))
	    {
		if(kingPos.c == it->first.c)
		{
		    for(auto jt = m_moves.begin(); jt != m_moves.end();)
		    {
			if(jt->from.c == kingPos.c && // same collumn as king
			   ((jt->from.r < kingPos.r && // start between king and pinner
			     jt->from.r > it->first.r) ||
			    (jt->from.r > kingPos.r &&
			     jt->from.r < it->first.r)) &&
			   !((jt->to.r  < kingPos.r && // end not between king and pinner
			     jt->to.r   > it->first.r) ||
			    (jt->to.r   > kingPos.r &&
			     jt->to.r   < it->first.r)))
			{
			    jt = m_moves.erase(jt);
			}
			else ++jt;
		    }
		}
		else if(kingPos.r == it->first.r)
		{
		    for(auto jt = m_moves.begin(); jt != m_moves.end();)
		    {
			if(jt->from.r == kingPos.r && // same row as king
			   ((jt->from.c < kingPos.c && // start between king and pinner
			     jt->from.c > it->first.c) ||
			    (jt->from.c > kingPos.c &&
			     jt->from.c < it->first.c)) &&
			   !((jt->to.c  < kingPos.c && // end not between king and pinner
			      jt->to.c   > it->first.c) ||
			     (jt->to.c   > kingPos.c &&
			      jt->to.c   < it->first.c)))
			{
			    jt = m_moves.erase(jt);
			}
			else ++jt;
		    }
		}
	    }
	}	
    }
    

    if(m_moves.size() == 0)
    {
	std::cout << "NO MOVES\n";
	if(m_state == Check) m_state = Checkmate;
	else m_state = Stalemate;
    }
}

bool Board::makeAMove(Move chosen)
{
    bool found = false;
    for(auto &m: m_moves)
    {
	if(m == chosen)
	{
	    found = true;
	    std::cout << m << " " << chosen << "\n";
	    break;
	}
    }
    if(!found) return false;

    if(m_whiteToMove)
    {
	if(chosen.from == m_wKingPos) m_wKingPos = chosen.to;	
    }
    else if(chosen.from == m_bKingPos) m_bKingPos = chosen.to;
    
    m_pieces[chosen.from]->move(chosen.to);
    auto pieceToMove = m_pieces.extract(chosen.from);
    auto pieceToCapture = m_pieces.find(chosen.to);
	
    if(pieceToCapture != m_pieces.end()) m_pieces.erase(pieceToCapture);
	
    pieceToMove.key() = chosen.to;
    m_pieces.insert(std::move(pieceToMove));

    if(!m_whiteToMove) ++m_movesSinceStart;
    if(chosen.capture) m_movesSinceCapture = 0;
    else if(!m_whiteToMove) ++m_movesSinceCapture;
    
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
