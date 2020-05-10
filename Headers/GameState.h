#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Level.h"

class GameState : public State {
private:
    Level* level;
    sf::RectangleShape PocionesShape;
    sf::RectangleShape EfectoShape;
    sf::RectangleShape healingEffectShape;
    AnimationComponent* healingEffectManager;

    std::string previousZone = "aaa";

    // Level transition helpers
    sf::RectangleShape fadeOutPanel;
    bool b_isTransitioning;
    bool b_isLeaving;
    bool b_gameBeaten;
    float f_transitionCounter;
    float f_transitionTime;
public:
    GameState(std::stack<State*>* states, Player* player);
    ~GameState();

    // Initializes the game state. Called when state re init is needed
    void initGame();
    // Initializes the new level using player's data (level name and door)
    void initLevel();

    //Cambia el efecto en pantalla al recibir danyo
    void changeDamageEffects(); 
    //Cambia la textura de las pociones segun las que te queden
    void changePotionShape(); 

    // Gets level exit data from Level and performs the level change, deleting and reinitialazing the level
    void changeLevel();
    // Activates what needs to be activated when the game detects that the player beat the game
    // Used only when the final boss is defeated and the game can be considered beaten
    void beatTheGame();
    // Checks end of game (final boss beaten), and starts the transition towards the SummaryState
    void checkBeatTheGame();

    // Updates the fade out/in animation when the player leave/enter a level
    void transitionUpdate();

    void update();
    void render(float frameProgress);
};

#endif
