
//  Drew Smegner 
#include "Resource.h"
#include "Agent.h"
#include <string>
#include <iostream>



    namespace Gaming{
    
        const double Resource::RESOURCE_SPOIL_FACTOR = 1.2;
    
        Resource::Resource(const Game&g, const Position&p, double capacity) : Piece(g,p), __capacity(capacity)
        { }
    
         Resource::~Resource() { }
    
    
        double Resource::consume()
        {
        finish();
        return __capacity;
        }
    
        void Resource::age()
            {
        __capacity = (__capacity / RESOURCE_SPOIL_FACTOR);
        if(!isViable())
        {finish();}}
         ActionType Resource::takeTurn(const Surroundings &s) const
            {
        return STAY; }
    
        Piece &Resource::operator*(Piece &other )
        {
       
        return other.interact(this); }
    
    
        Piece &Resource::interact(Agent *)
        {
        finish();
        return *this; }
        Piece &Resource::interact(Resource *p)
        {
            return *this; }
    }























