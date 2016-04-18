
//  Drew Smegner 
#include <iostream>
#include "Piece.h"

namespace Gaming
{
    unsigned int Piece::__idGen = 0;

    Piece::Piece(const Game&g, const Position &p) : __game(g), __position(p)
        {
        __id = __idGen++;__finished = false;__turned = false;
        
        }
    Piece::~Piece()
        {
        __idGen--; __finished = true; __turned = true;
        
        }
    std::ostream &operator <<(std::ostream &os, const Piece &piece)
    {
        //os <<piece.__id;
        piece.print(os);
        return os;
    }
}
