#ifndef STATE_H
#define STATE_H

#include <stack>
#include <map>
#include "Engine.h"
#include "Player.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

enum StateType {MENU_STATE = 1, GAME_STATE, PAUSE_STATE, SHOP_STATE, MAINMENU_STATE, TEXT_STATE, ANIMATION_STATE, SUMMARY_STATE, CREDITS_STATE};

class State {
private:

protected:
    StateType Iam; //Necesario para poder condicionar los cambios de estados
    bool b_reInit;
    int seleccion;  //Necesario para que los botones del menu puedan cambiar estados

    Player* player;
    std::stack<State*>* states; //Para poder meter tienda y dialogo en estado juego
    
public:
    State(std::stack<State*>* statesRef, Player* playerRef);
    ~State();

    StateType getIam();
    int getSeleccion();

    void setPlayer(Player* player);

    virtual void update() = 0;
    virtual void render(float frameProgress) = 0;
    void changeState(StateType target, bool reInit);
};

#endif
