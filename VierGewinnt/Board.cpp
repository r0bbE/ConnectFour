#include "Board.h"
#include <iostream>

Board::Board(int columns, int rows)
{
	sf::Vector2<float> position(0.0f, 0.0f);
	std::vector<Token*> slot;

	for(auto i = 0; i < columns; ++i)
	{
		for(auto j = 0; j < rows; ++j)
		{
			Token* token = new Token(position);
			token->setRow(j);
			token->SetColumn(i);
			slot.push_back(token);
			position.y += 100.0f;
		}
		position.y = 0.0f;
		position.x += 100.0f;
		board.push_back(slot);
		slot.clear();
	}

	win = false;
}

Board::Board(const Board& other)
{
	std::vector<Token*> slot;
	auto boardarray = other.getArray();

	for (auto i = 0; i < other.board.size(); ++i)
	{
		for (auto j = 0; j < other.board[i].size(); ++j)
		{
			Token* token = new Token(*boardarray[i][j]);
			slot.push_back(token);
		}
		board.push_back(slot);
		slot.clear();
	}

	this->lastPlayedToken = other.lastPlayedToken;
	this->win = other.win;
}

Board::~Board()
{
	for(auto i : board)
	{
		for (auto j : i)
		{
			delete j;
		}
	}
}

void Board::Render(sf::RenderWindow* window)
{
	for(auto i : board)
	{
		for(auto j : i)
		{
			j->Render(window);
		}
	}
}

int Board::SelectSlot(int oldSlot, int newSlot)
{
	if (newSlot >= 0 && newSlot < board.size())
	{
		for (auto i : board[oldSlot])
		{
			i->setSelected(false);
		}

		for(auto i : board[newSlot])
		{
			i->setSelected(true);
		}

		return newSlot;
	}
	
	return oldSlot;
}

bool Board::FourInARow(sf::Vector2<int> token, Ownership player)
{

	//Check horizontal
	int newTokenSlot = token.x;
	int newTokenRow = token.y;
	int counterMinus = 0, counterPlus = 0;

	for(int i = 0; i < 4; i++)
	{
		if(newTokenSlot - i >= 0)
		{
			if (board.at(newTokenSlot - i).at(newTokenRow)->getOwner() == player)
			{
				counterMinus++;
			} else
			{
				break;
			}
		}
	}

	for (int i = 1; i < 4; i++)
	{
		if (newTokenSlot + i < board.size())
		{
			if (board.at(newTokenSlot + i).at(newTokenRow)->getOwner() == player)
			{
				counterPlus++;
			}
			else
			{
				break;
			}
		}
	}
	
	if(counterMinus + counterPlus >= 4)
	{
		return true;
	}

	//Check vertical
	counterMinus = 0;
	counterPlus = 0;

	//Nach Unten - Unterstes Element Index 5
	for (int i = 0; i < 4; i++)
	{
		if (newTokenRow + i < board[0].size())
		{
			if (board.at(newTokenSlot).at(newTokenRow + i)->getOwner() == player)
			{
				counterPlus++;
			}
			else
			{
				break;
			}
		}
	}

	//Nach Oben - Unterstes Element Index 0
	for (int i = 1; i < 4; i++)
	{
		if (newTokenRow - i >= 0)
		{
			if (board.at(newTokenSlot).at(newTokenRow - i)->getOwner() == player)
			{
				counterMinus++;
			}
			else
			{
				break;
			}
		}
	}

	if (counterMinus + counterPlus >= 4)
	{
		return true;
	}

	//Check diagonal - links oben nach rechts unten
	counterMinus = 0;
	counterPlus = 0;

	//Nach Links und Oben
	for (int i = 0; i < 4; i++)
	{
		if (newTokenSlot - i >= 0 && newTokenRow - i >= 0)
		{
			if (board.at(newTokenSlot - i).at(newTokenRow - i)->getOwner() == player)
			{
				counterMinus++;
			}
			else
			{
				break;
			}
		}
	}

	//Nach Rechts und Unten
	for (int i = 1; i < 4; i++)
	{
		if (newTokenSlot + i < board.size() && newTokenRow + i < board[0].size())
		{
			if (board.at(newTokenSlot + i).at(newTokenRow + i)->getOwner() == player)
			{
				counterPlus++;
			}
			else
			{
				break;
			}
		}
	}

	if (counterMinus + counterPlus >= 4)
	{
		return true;
	}

	//Check diagonal - rechts oben nach links unten
	counterMinus = 0;
	counterPlus = 0;

	//Nach Rechts und Oben
	for (int i = 0; i < 4; i++)
	{
		if (newTokenSlot + i < board.size() && newTokenRow - i >= 0)
		{
			if (board.at(newTokenSlot + i).at(newTokenRow - i)->getOwner() == player)
			{
				counterMinus++;
			}
			else
			{
				break;
			}
		}
	}

	//Nach Links und Unten
	for (int i = 1; i < 4; i++)
	{
		if (newTokenSlot - i >= 0 && newTokenRow + i < board[0].size())
		{
			if (board.at(newTokenSlot - i).at(newTokenRow + i)->getOwner() == player)
			{
				counterPlus++;
			}
			else
			{
				break;
			}
		}
	}

	if (counterMinus + counterPlus >= 4)
	{
		return true;
	}

	return false;
}

bool Board::BoardIsFull()
{
	for(int i = 0; i < 7; i++)
	{
		if(hasEmptyToken(i))
		{
			return false;
		}
	}

	return true;
}

const bool Board::getWin() const
{
	return win;
}

Ownership Board::getOwnerOfToken(sf::Vector2<int> pos)
{
	return board[pos.x][pos.y]->getOwner();
}

bool Board::hasEmptyToken(int slotNumber)
{
	for (auto i = board[slotNumber].rbegin(); i != board[slotNumber].rend(); ++i)
	{
		if (!((*i)->isFilled()))
		{
			return true;
		}
	}

	return false;
}

int Board::getLastPlayedSlot()
{
	return lastPlayedToken.x;
}

const std::vector<std::vector<Token*>> Board::getArray() const
{
	return board;
}

bool Board::PutTokenInSlot(int slot, Ownership player)
{
	//Find Last empty Token and place chip
	for (auto i = board[slot].rbegin(); i != board[slot].rend(); ++i)
	{
		if (!((*i)->isFilled()))
		{
			(*i)->SetOwnership(player);
			lastPlayedToken = (*i)->getPosition();

			if (FourInARow(lastPlayedToken, PLAYER1) || BoardIsFull() || FourInARow(lastPlayedToken, PLAYER2))
			{
				win = true;
			}

			return true;
		}
	}

	return false;
}

