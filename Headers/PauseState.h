#ifndef PAUSESTATE_H
#define PAUSESTATE_H


#include "State.h"
#include "Player.h"

class PauseState : public State {
private:
    sf::RectangleShape* filtro; // Filtro para "apagar" el color del fondo
    sf::Font fuente; // Fuente para escribir
    sf::Text* texto; // Variable para contener el texto
    sf::Sprite* flecha_selector;// Flecha que indica la opcion seleccionada
    sf::Texture arrow; //Contiene la textura para construir la flecha
    sf::Sprite* heart_upgrade;// Indica las mejoras de vida actuales
    sf::Texture heart; //Contiene la textura para construir heart_upgrade
    sf::Sprite* sword_upgrade;// Indica las mejoras de espada actuales
    sf::Texture sword; //Contiene la textura para construir sword_upgrade
    sf::Sprite* coin_purse;// Indica las monedas actuales
    sf::Texture coin; //Contiene la textura para construir coin_purse
    sf::Sprite* kill_count;// Indica las kills actuales
    sf::Texture skull; //Contiene la textura para construir kill_count
    sf::Sprite* parchment;// Fondo para los datos del jugador
    sf::Texture pergamino; //Contiene la textura para construir parchment
    sf::Sprite* armor_upgrade;// Indica el nivel de armadura
    sf::Texture armor; //Contiene la textura para construir armor_upgrade
    sf::Clock* clock;

    bool b_reInit;
    int i_damage; //Para montar las estadísticas del jugador en la pantalla de pausa
    int i_life;
    int i_kills;
    int i_money;
    int i_armor;
    int i_score;
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