
//  Drew Smegner 

#include "AggressiveAgentStrategy.h"


#include "Game.h"

namespace Gaming
{
    
    const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;
    AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy)
        {
        __agentEnergy = agentEnergy; }
    AggressiveAgentStrategy::~AggressiveAgentStrategy()
    { }
    ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const
    {
        std::vector<int> Advantages;
        std::vector<int> Food;
        std::vector<int> Emptyspot;
        std::vector<int> simple;
        std::vector<int> strategy;
        Position agentposition;
        agentposition.x=1;
        agentposition.y=1;
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
            
            else if(s.array[i]==STRATEGIC)
                strategy.push_back(i);
            }
             if(simple.size() > 0 && __agentEnergy >= DEFAULT_AGGRESSION_THRESHOLD)
            {
            newPos=Game::randomPosition(simple);
            return Game::reachSurroundings(agentposition,newPos);
            }
            else if(Advantages.size() > 0)
             {
             newPos=Game::randomPosition(Advantages);
            return Game::reachSurroundings(agentposition,newPos);
             }
             else if(Food.size() > 0)
             {
            newPos=Game::randomPosition(Food);
            return Game::reachSurroundings(agentposition,newPos);
             }
             else
            return STAY;
    }
    
    
}