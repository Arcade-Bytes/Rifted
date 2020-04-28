#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Level.h"

class GameState : public State {
private:
    Level* level;
    sf::RectangleShape PocionesShape;
    sf::RectangleShape EfectoShape;

public:
    GameState(std::stack<State*>* states, Player* player);
    ~GameState();

    void initGame();
    void changeDamageEffects(); //cambia el efecto en pantalla al recibir danyo
    void changePotionShape(); //cambia la textura de las pociones segun las que te queden
    void update();
    void render(float frameProgress);
};

#endif
