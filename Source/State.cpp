#include "State.h"

State::State(std::stack<State*>* statesRef, Player* playerRef)
{
    this->states = statesRef;
    this->player = playerRef;
}

State::~State()
{

}

void State::setPlayer(Player* player)
{
    this->player = player;
}

StateType State::getIam()
{
    return this->Iam;
}

int State::getSeleccion()
{
    return this->seleccion;
}

void State::changeState(StateType target)
{
    std::vector<State*> helper;
    int originalSize = states->size();
    for(int i=0; i<originalSize;i++)
    {
        helper.push_back(states->top());
        states->pop();

        if((helper[helper.size()-1]->getIam()) == target)
        {
            for(auto item : helper)
            {
                states->push(item);
            }
            break;
        }
    }
    helper.clear();
}
