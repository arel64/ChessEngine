#ifndef MACRO_DIRECTION_HPP
#define MACRO_DIRECTION_HPP
enum DirectionOffsets
{
    NORTH = BOARD_DIM,
    SOUTH = -BOARD_DIM,
    EAST = 1,
    WEST = -1,
    NORTH_EAST = NORTH + EAST,
    NORTH_WEST = NORTH + WEST,
    SOUTH_EAST = SOUTH + EAST,
    SOUTH_WEST = SOUTH + WEST
};
class DirectionIteratorBase
{
    public:
        virtual int8_t getNextSquare() = 0;
        virtual int8_t getSquare() const = 0;
        virtual ~DirectionIteratorBase() = default;
};
template <DirectionOffsets D> class DirectionIterator : public DirectionIteratorBase
{
    public:
        DirectionIterator(int8_t square) {this->square = square;}
        int8_t getNextSquare() override
        {
            int8_t nextSquare = getSquare() + D;
            
            if(isSquareWithinDirection(nextSquare))
            {
                this->square = nextSquare;
                return square;
            }
            return -1;
        }
        int8_t getSquare() const override
        {
            return this->square;
        }
        private:
            bool isSquareWithinDirection(uint8_t squareInDirection)
            {
                switch(D)
                {
                    case NORTH:
                    case SOUTH:
                        return isSquareWithinBoard();
                        break;
                    case EAST:
                    case WEST:
                        return isSquaresWithinSameRow(squareInDirection);
                        break;
                    case NORTH_EAST:
                    case SOUTH_WEST:
                        return isOnSamePositiveDiagonal(squareInDirection);
                    case NORTH_WEST:
                    case SOUTH_EAST:
                        return isOnSameNegativeDiagonal(squareInDirection);
                    break;
                    
                }
                return false;
            }
            bool isSquareWithinBoard()
            {
                if(square >= BOARD_DIM*BOARD_DIM || square < 0)
                {
                    return false;
                }
                return true;
            }
            bool isSquaresWithinSameRow(uint8_t square)
            {
                if(this->square / BOARD_DIM == square / BOARD_DIM)
                {
                    return true;
                }
                return false;
            }
            bool isSquareOnRank(BoardRank rank)
            {
                if(this->square / BOARD_DIM == rank)
                {
                    return true;
                }
                return false;
            }
            bool isOnSamePositiveDiagonal(uint8_t squareInDirection)
            {
                uint8_t squareRank = square / BOARD_DIM;
                uint8_t squareFile = square % BOARD_DIM;
                uint8_t squareInDirectionRank = squareInDirection / BOARD_DIM;
                uint8_t squareInDirectionFile = squareInDirection % BOARD_DIM;
                if(squareRank - squareInDirectionRank == squareFile - squareInDirectionFile)
                {
                    return true;
                }
                return false;
            }
            bool isOnSameNegativeDiagonal(uint8_t squareInDirection)
            {
                uint8_t squareRank = square / BOARD_DIM;
                uint8_t squareFile = square % BOARD_DIM;
                uint8_t squareInDirectionRank = squareInDirection / BOARD_DIM;
                uint8_t squareInDirectionFile = squareInDirection % BOARD_DIM;
                
                if(squareRank - squareInDirectionRank ==  -(squareFile - squareInDirectionFile))
                {
                    return true;
                }
                return false;
            }
            int8_t square;
        
};
#endif