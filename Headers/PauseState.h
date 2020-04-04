#ifndef PAUSESTATE_H
#define PAUSESTATE_H


#include "State.h"
#include "Player.h"

class PauseState : public State {
private:
    sf::RectangleShape* filtro; // Filtro para "apagar" el color del fondo
    sf::Text* texto; // Variable para contener el texto
    sf::Sprite* flecha_selector;// Flecha que indica la opcion seleccionada
    sf::Sprite* heart_upgrade;// Indica las mejoras de vida actuales
    sf::Sprite* sword_upgrade;// Indica las mejoras de espada actuales
    sf::Sprite* coin_purse;// Indica las monedas actuales
    sf::Sprite* kill_count;// Indica las kills actuales
    sf::Sprite* parchment;// Fondo para los datos del jugador
    sf::Sprite* shield_upgrade;// Indica el nivel de escudo
    sf::Sprite* bow_upgrade;// Indica el nivel de bow
    sf::Sprite* hammer_upgrade;// Indica el nivel de hammer

    sf::Clock* clock;

    int i_damage; //Para montar las estadísticas del jugador en la pantalla de pausa
    int i_life;
    int i_money;
    int i_hammer;
    int i_shield;
    int i_bow;
    int i_score;
    int i_kills;
    bool dir; //Direccion de la animacion de la flecha
    int i; //COntrol para animacion de la flecha
    
public:
    PauseState(std::stack<State*>* states, Player* player);
    ~PauseState();

    void update();
    void render();

    void initPlayerData();

    void drawText();
    void drawArrow();
    void drawPlayerData();
    void drawParchment();

    void loadAssets();
};

#endif