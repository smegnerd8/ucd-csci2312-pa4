
//  Drew Smegner 

#include "Strategic.h"

namespace Gaming
{
   const char Strategic::STRATEGIC_ID= 'T';
    
    Strategic::Strategic(const Game &g, const Position &p, double energy, Strategy *s) : Agent(g,p,energy)
        {
        __strategy = s; }
    
    Strategic::~Strategic()
        { }
    
    
    void Strategic::print(std::ostream &os) const
        {
        os << STRATEGIC_ID << __id; }
    
    ActionType Strategic::takeTurn(const Surroundings &s) const
        {
       return (*__strategy)(s); }
}