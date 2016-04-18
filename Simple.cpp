
//  Drew Smegner 
#include "Simple.h"

namespace Gaming
{
    const char Simple::SIMPLE_ID = 'S';
        Simple::Simple(const Game &g, const Position &p, double energy) :Agent(g,p,energy)
        {
        __energy = energy;
        
        }
        Simple::~Simple()
        { }
    
    void Simple::print(std::ostream &os) const
        {
        os << SIMPLE_ID << __id;
        }
    
        ActionType Simple::takeTurn(const Surroundings &s) const
            {
        std::vector<int> Resources;
        std::vector<int> Emptyspot;
        
        PositionRandomizer position;
        Position newPos;
        Position newPos1;
        newPos1.x=1;
        newPos1.y=1;
        
        for(int i=0; i < s.array.size(); ++i)
            {
            if (s.array[i] == FOOD || s.array[i] == ADVANTAGE)
                Resources.push_back(i);
            
            else if(s.array[i]==EMPTY && s.array[i]!= INACCESSIBLE&&s.array[i]!= SIMPLE && s.array[i]!= STRATEGIC)
                Emptyspot.push_back(i);
            
            }
        
        if(Resources.size()>0)
            {
            newPos = Game::randomPosition(Resources);
            return __game.reachSurroundings(newPos1,newPos);
            }
        else if(Emptyspot.size()>0)
            {
            newPos = Game::randomPosition(Emptyspot);
            return __game.reachSurroundings(newPos1, newPos);
            }
        else
            return STAY;
    }
}