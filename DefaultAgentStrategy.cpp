
//  Drew Smegner 
//

#include "DefaultAgentStrategy.h"
#include "Strategic.h"
#include "Game.h"

namespace Gaming
{
    
    DefaultAgentStrategy::DefaultAgentStrategy()
    { }
    DefaultAgentStrategy::~DefaultAgentStrategy()
    { }
    ActionType DefaultAgentStrategy::operator()(const Surroundings &s) const
    { std::vector<int> Advantages; std::vector<int> Food;
        std::vector<int> Emptyspot; std::vector<int> simple;

        Position agentPos;
        agentPos.x=1;
        agentPos.y=1;
        PositionRandomizer Pos;
        Position newPos;

        for(int i=0;i < s.array.size();i++)
            {
            if(s.array[i]==ADVANTAGE)
                Advantages.push_back(i);
            
            else if(s.array[i]==FOOD)
                Food.push_back(i);
            
            else if(s.array[i]==EMPTY)
                Emptyspot.push_back(i);
            
            else if(s.array[i]==SIMPLE)
                simple.push_back(i);
            }
        
        if(simple.size() > 0)
            {
                newPos = Game::randomPosition(simple);
            return Game::reachSurroundings(agentPos,newPos);
            }
        else if(Advantages.size() > 0)
            {
            newPos = Game::randomPosition(Advantages);
            return Game::reachSurroundings(agentPos,newPos);
            }
        else if(Food.size() > 0)
                {
            newPos = Game::randomPosition(Food);
            return Game::reachSurroundings(agentPos,newPos);
            }
        else
            return STAY;
            }
}

