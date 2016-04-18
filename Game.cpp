

//  Drew Smegner
#include "Game.h"
#include "Piece.h"
#include "Simple.h"
#include "Resource.h"
#include "Strategic.h"
#include "Advantage.h"
#include "Food.h"
#include <iomanip>
#include <sstream>

namespace Gaming
{
    const unsigned int Game::NUM_INIT_AGENT_FACTOR =4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR =2;
    const unsigned Game::MIN_WIDTH = 3;
    const unsigned Game::MIN_HEIGHT = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;
    PositionRandomizer Game::__posRandomizer;

    void Game::populate()
    {
        std::default_random_engine gen;
        std::uniform_int_distribution<int> d(0, __width * __height);
        __numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
        __numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;

        //strategic agents
        unsigned int numStrategic = __numInitAgents / 2;
        while (numStrategic > 0) {
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Strategic(*this, pos, Game::STARTING_AGENT_ENERGY);
                numStrategic --;
            }
        }

        //sample agents
        unsigned int numSimple = __numInitAgents - numStrategic;
        while (numSimple > 0) {
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Simple(*this, pos, Game::STARTING_AGENT_ENERGY);
                numSimple --;
            }
        }

        // advtantage agents
        unsigned int numAdvantages = __numInitResources / 4;
        while (numAdvantages > 0) {
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Advantage(*this, pos, Advantage::ADVANTAGE_MULT_FACTOR);
                numAdvantages --;
            }
        }

        //food resources
        unsigned int numFoods = __numInitResources - numAdvantages;
        while (numFoods > 0) {
            int i = d(gen);
            if (__grid[i] == nullptr)
            {
                Position pos(i / __width, i % __width);
                __grid[i] = new Food(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
                numFoods --;
            }
        }



    }

    Game::Game(unsigned width, unsigned height, bool manual) :
            __grid(width*height,nullptr),
            __round(0),
            __status(NOT_STARTED)
    {
        __width = width; __height = height;
        __numInitAgents = (__width * __height) /NUM_INIT_AGENT_FACTOR;
        __numInitResources = (__width * __height) /NUM_INIT_RESOURCE_FACTOR;
        __verbose = false;

        if (width < MIN_WIDTH || height < MIN_HEIGHT)
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);

        if (! manual) void populate();
    }

    Game::Game() : Game(3,3)
    { }
    Game::~Game()
    { }
    Game::Game(const Game &another)
    {
        std::vector<Piece *> *vecPtr = new std::vector<Piece *>(another.__width * another.__height);
        __width = another.__width;__height = another.__height;__round = another.__round;__status = another.__status;__verbose = another.__verbose;
        __grid = *vecPtr;

        delete vecPtr;
    }
//getters
    unsigned int Game::getNumPieces() const
    {
        unsigned int numPieces = (getNumResources() + getNumAgents());
        return numPieces; }

    unsigned int Game::getNumAgents() const
    {
        unsigned int numAgents = (getNumSimple() + getNumStrategic());
        return numAgents; }


    unsigned int Game::getNumSimple() const
    {
        unsigned int numAgents = 0; std::vector<Piece*>::const_iterator it;
        for ( it = __grid.begin(); it != __grid.end(); ++it)
        {
            Agent *agent = dynamic_cast<Simple*>(*it);
            if (agent) numAgents ++;
        }
        return numAgents;
    }


    unsigned int Game::getNumStrategic() const
    {
        unsigned int numAgents = 0; std::vector<Piece*>::const_iterator it;
        for ( it = __grid.begin(); it != __grid.end(); ++it)
        {
            Agent *agent = dynamic_cast<Strategic*>(*it);
            if (agent) numAgents ++;
        }

        return numAgents;
    }

    unsigned int Game::getNumResources() const
    {
        unsigned int numResources = 0;
        for (auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            Piece *Piece = dynamic_cast<Resource*>(*it);
            if (Piece) numResources ++;
        }
        return numResources;
    }

    const Piece *Game::getPiece(unsigned int x, unsigned int y) const {
        int place = x * __width + y;

        if (y >= __width || x >= __height)throw OutOfBoundsEx(__width, __height, x, y);

        if (__grid[place] == nullptr) throw PositionEmptyEx(x, y);

        return __grid[place];
    }

    void Game::addSimple(const Position &position)
    {

        if((__grid[position.x * __width + position.y] != nullptr))
                {

                                }
                else
                {
                            Simple *simPtr = new Simple(*this, position, STARTING_AGENT_ENERGY);
                            __grid[position.x * __width + position.y] = simPtr;

                        }
    }

    void Game::addSimple(const Position &position, double energy)
    {

        int place = position.y + position.x * __width;

        if (position.y >= __width || position.x >= __height)throw OutOfBoundsEx(__width, __height, position.x, position.y);

        if (__grid[place])throw PositionNonemptyEx(position.x, position.y);

        __grid[place] = new Simple(*this, position, energy);
    }

    void Game::addSimple(unsigned x, unsigned y)
    {

        if((__grid[x * __width + y]) != nullptr)
                {

                                }
                else
                {
                            Position pos(x,y);
                            Simple *simPtr = new Simple(*this, pos, STARTING_AGENT_ENERGY);
                            __grid[x * __width + y] = simPtr;

                        }
    }

    void Game::addSimple(unsigned y, unsigned x, double energy)
    {
        int place = y + x * __width;

        if (y >= __width || x >= __height)throw OutOfBoundsEx(__width, __height, x, y);

        if (__grid[place])throw PositionNonemptyEx(x, y);

        __grid[place] = new Simple(*this, Position(x, y), energy);
    }

    void Game::addStrategic(const Position &position, Strategy *s)
    {
        int place = position.y + position.x * __width;

        if (position.y >= __width || position.x >= __height)throw OutOfBoundsEx(__width, __height, position.x, position.y);

        if (__grid[place])throw PositionNonemptyEx(position.x, position.y);

        __grid[place] = new Strategic(*this, position, STARTING_AGENT_ENERGY, s);
    }

    void Game::addStrategic(unsigned x, unsigned y, Strategy *s)
    {
        int place = y + x * __width;

        if (y >= __width || x >= __height)throw OutOfBoundsEx(__width, __height, x, y);

        if (__grid[place])throw PositionNonemptyEx(x, y);

        __grid[place] = new Strategic(*this, Position(x, y), STARTING_AGENT_ENERGY, s);
    }

    void Game::addFood(const Position &position)
    {
        if ((__grid[position.x * __width + position.y]) != nullptr)
        {

        }
        else
        {
            Food *foodPtr = new Food(*this, position, STARTING_RESOURCE_CAPACITY);
            __grid[position.x * __width + position.y] = foodPtr;
        }
    }


    void Game::addFood(unsigned x, unsigned y)
    {
        if ((__grid[x * __width + y]) != nullptr)
        { }
        else
        {
            Position pos(x,y);
            Food *foodPtr = new Food(*this, pos, STARTING_RESOURCE_CAPACITY);
            __grid[x * __width + y] = foodPtr;
        }
    }

    void Game::addAdvantage(const Position &position)
    {

        if ((__grid[position.x * __width + position.y]) != nullptr)
                { }
                else
                {
                            Advantage *advPtr = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
                            __grid[position.x * __width + position.y] = advPtr;
                }
    }

    void Game::addAdvantage(unsigned x, unsigned y)
    {
        int place = y + (x * __width);

        if (y >= __width || x >= __height)throw OutOfBoundsEx(__width, __height, x, y);

        if (__grid[place])throw PositionNonemptyEx(x, y);

        __grid[place] = new Advantage(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
    }

    const Surroundings Game::getSurroundings(const Position &pos) const
    {
        Surroundings surs;
        unsigned int x,y;
        unsigned int i = 0;

        for (int row =-1; row <=1; ++row)
        {
            x = pos.x+row;
            for (int col =-1; col <=1; ++col)
            {
                y=pos.y+col;

                if( x>=__height || y >=__width)
                {
                    surs.array[i] = INACCESSIBLE;
                    i++;
                }
                    else if(__grid[x*__width+y]== nullptr)
                    {
                    surs.array[i] = EMPTY;
                    i++;
                    }
                        else
                        {
                    surs.array[i]=getPiece(x,y)->getType();
                    i++;
                        }
            }
        }
        surs.array[4]=SELF;
        return surs;


    }

    const ActionType Game::reachSurroundings(const Position &from, const Position &to)
    {
        ActionType Move = STAY;
        unsigned int x,y;

        x = (from.x - to.x);
        y = (from.y - to.y);

        if(x ==1 && y ==1)return Move = NW;

        else if(x ==1 && y ==0)return Move = N;

        else if(x ==1 && y ==-1)return Move = NE;

        else if(x ==0 && y ==-1)return Move = E;

        else if(x ==-1 && y ==-1)return Move = SE;

        else if(x ==-1 && y ==0)return Move = S;

        else if(x ==-1 && y ==1)return Move = SW;

        else if(x ==0 && y ==1)return Move = W;

        return Move;
    }

    bool Game::isLegal(const ActionType &ac, const Position &pos) const
    {
        unsigned int x,y;
        x=pos.x;
        y=pos.y;

        if(ac==STAY)
            return true;
        if(ac<=2)
            x--;
        else if(ac>=5)
            x++;

        if(ac<=6 &&ac>0){
            if((ac & 1)==0)
                y--;
            else
                y++;
        }

        return !( x >=__height || y >= __width);
    }


    const Position Game::move(const Position &pos, const ActionType &ACT) const
    {
        Position Pos(pos);

        if(ACT == STAY)
            return Pos;
        if(ACT <= 2)
            Pos.x--;
        else if(ACT >= 5)
            Pos.x++;

        if(ACT <= 6 && ACT> 0)
        {
            if((ACT & 1) == 0)
                Pos.y--;
            else
                Pos.y++;
        }

        return Pos;
    }
    void Game::round()
    {
        if(__round == 0 && __verbose)
        {
            __status = PLAYING;
            std::cout << std::endl << *this;
        }

        ActionType movePosition;
        Position pos;

        for(int i =0; i<__grid.size(); ++i)
        {
            if(__grid[i]!= nullptr && ! __grid[i]->getTurned())
            {
                movePosition = __grid[i]->takeTurn(getSurroundings(__grid[i]->getPosition()));
                pos = move(__grid[i]->getPosition(), movePosition);

                int place = pos.x * __width + pos.y;
                __grid[i]->setTurned(true);
                if (__grid[place] != nullptr)
                {
                    __grid[i]->operator*(*(__grid[place]));

                    if (__grid[i]->isViable())
                    {
                        delete __grid[place];
                        __grid[place] = __grid[i];
                        __grid[i] = nullptr;
                    }
                        else if (__grid[place]->isViable())
                            {
                                delete __grid[place];
                        __grid[place] = nullptr;
                             }
                            else
                            {
                        delete __grid[place];
                        delete __grid[i];
                        __grid[place]=nullptr;
                        __grid[i]=nullptr;
                             }
                }
                                else
                    __grid[i]->setPosition(pos);
            }
        }

        for (int k = 0; k < __grid.size(); ++k)
        {
            if(__grid[k] != nullptr)
            {
                if (!__grid[k]->isViable())
                {
                    delete __grid[k];__grid[k] = nullptr;
                }
                else
                {
                    __grid[k]->setTurned(false);__grid[k]->age();
                }
            }
        }

    }
    void Game::play(bool verbose)
    {
        std::cout << this;
        while (!verbose) {
            for (int i = 0; i < __height*__width; i++) {
                Piece *p = dynamic_cast<Resource*>(__grid[i]);
                if (p)
                    break;
                if (i == 8) {
                    verbose = true;
                    __status = OVER;
                }
                if (verbose == false)
                    __status = PLAYING;
            }
            round();
        }

    }
    std::ostream &operator<<(std::ostream &os, const Game &game)
    {
        os << "Round " << game.__round << std::endl;

        for (int i = 0; i < game.__height; i++) {
            for (int j = 0; j < game.__width; j++) {
                if (game.__grid[i * game.__width + j] == nullptr) {
                    os << "[" << std::setw(6) << "]";
                }
                else {
                    os << "[" << *(game.__grid[i * game.__width + j]) << "]";
                }}
            os << std::endl;
        }
        os << "Status: " << game.__status << std::endl;
        return os;

    }


}