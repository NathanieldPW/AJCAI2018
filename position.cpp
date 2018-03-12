#include "position.h"

Position::Position(int row, int col)
{
    this->row = row;
    this->col = col;
}

bool Position::operator==(const Position &other) const
{
    return row == other.row && col == other.col;
}

bool Position::operator!=(const Position &other) const
{
    return !( (*this) == other );
}
