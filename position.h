#pragma once

class Position
{
    public:
        Position(int row, int col);
        bool operator==(const Position &other) const;
        bool operator!=(const Position &other) const;
    public:
        int row;
        int col;
};

