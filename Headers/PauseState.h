#ifndef PAUSESTATE_H
#define PAUSESTATE_H


#include "State.h"
#include "Player.h"

class PauseState : public State {
private:
    sf::RectangleShape* filtro; // Filtro para "apagar" el color del fondo
    sf::Text* texto; // Variable para contener el texto
    sf::RectangleShape* flecha_selector;// Flecha que indica la opcion seleccionada
    sf::RectangleShape* heart_upgrade;// Indica las mejoras de vida actuales
    sf::RectangleShape* sword_upgrade;// Indica las mejoras de espada actuales
    sf::RectangleShape* coin_purse;// Indica las monedas actuales
    sf::RectangleShape* kill_count;// Indica las kills actuales
    sf::RectangleShape* parchment;// Fondo para los datos del jugador
    sf::RectangleShape* shield_upgrade;// Indica el nivel de escudo
    sf::RectangleShape* bow_upgrade;// Indica el nivel de bow
    sf::RectangleShape* hammer_upgrade;// Indica el nivel de hammer

    sf::RectangleShape* mineKey; // The key icon that shows if you have the mine key
    sf::RectangleShape* cementeryKey; // The key icon that shows if you have the cementery key
    sf::RectangleShape* towerKey; // The key icon that shows if you have the tower key

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
    float arrowMove; //Control para animacion de la flecha
    float prevArrowMove; // Para interpolar el render
    
public:
    PauseState(std::stack<State*>* states, Player* player);
    ~PauseState();

    void loadAssets();
    void initPlayerData();

    void update();
    void render(float frameProgress);

    void drawText();
    void drawArrow(float frameProgress);
    void drawPlayerData();
    void drawParchment();

};

#endif