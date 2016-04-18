
//  Drew Smegner
#include "Agent.h"
#include "Resource.h"

namespace Gaming
{
    const double Gaming::Agent::AGENT_FATIGUE_RATE = 0.3;

    //constructor
    Agent::Agent(const Game &g, const Position &p, double energy) : Piece(g,p)
        {
        __energy = energy; }
    
    void Agent::age()
        {
        __energy = __energy -  AGENT_FATIGUE_RATE;
        
        if(__energy <= 0)
        { finish();}
        }

    //destructor
    Agent::~Agent()
    { }

    //overloaded operators
    Piece &Agent::operator*(Piece &other)
    {
        return other.interact(this); }

    Piece &Agent::interact(Agent *agent)
    {
        if(__energy == agent->__energy)
        {
            agent->finish();
            finish();

        }
        else if(__energy > agent->__energy)
        {
            __energy -= agent->__energy;
            agent->finish();
        }
        else
        {
            agent->__energy -= __energy;
            finish();
        }
        return *this;
    }

    Piece &Agent::interact(Resource *Resoruce)
    {
        if(Resoruce->getType() == FOOD) __energy+=Resoruce->consume();

        else if(Resoruce->getType() == ADVANTAGE)
            __energy+=Resoruce->consume();
        return *this;

    }
}