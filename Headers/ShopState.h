#ifndef ShopSTATE_H
#define ShopSTATE_H


#include "State.h"

class ShopState : public State {
private:
    sf::RectangleShape* parchment;// Fondo para los datos del jugador
    sf::Text* texto; // Variable para contener el texto
    sf::RectangleShape* flecha_selector;// Flecha que indica la opcion seleccionada
    sf::RectangleShape* heart_upgrade;// Indica las mejoras de vida actuales
    sf::RectangleShape* sword_upgrade;// Indica las mejoras de espada actuales
    sf::RectangleShape* coin_purse;// Indica las monedas actuales
    sf::RectangleShape* hammer_upgrade;// Indica las mejoras de hammer actuales
    sf::RectangleShape* bow_upgrade;// Indica las mejoras de bow actuales
    sf::RectangleShape* shield_upgrade;// Indica las mejoras de shield actuales
    sf::RectangleShape* npc_cleric;// Indica el sprite del tendero actuales
    sf::RectangleShape* text_box;// Indica el sprite de la caja de texto
    sf::Clock* clock;

    std::vector<sf::Text> textoTienda_v1; //Opciones de bienvenida
    std::vector<sf::Text> textoTienda_v2; //Opciones: Comprado
    std::vector<sf::Text> textoTienda_v3; //Opciones: Te falta dinero
    std::vector<sf::Text> textoTienda_v4; //Opciones: Eso esta al maximo
    std::vector<sf::Text> textoTienda_v5; //Opciones de despedida
    sf::Text texto_render;
    sf::Text pointer;
    
    int i_damage; //Para montar las estadísticas del jugador en la pantalla de pausa
    int i_life;
    int i_money;
    int i_hammer;
    int i_shield;
    int i_bow;
    bool dir; //Direccion de la animacion de la flecha
    float arrowMove; //Control para animacion de la flecha
    float prevArrowMove; //Control para animacion de la flecha interpolado
    int max_seleccion;
    
public:
    ShopState(std::stack<State*>* states, Player* player);
    ~ShopState();

    void update();
    void render(float frameProgress);

    void initPlayerData();

    bool getIsUnlocked(int index);
    void selectionDown();
    void selectionUp();

    void drawText();
    void drawArrow(float frameProgress);
    void drawPlayerData();
    void drawParchment();
    void drawPrices();

    void loadDialogue();
    void loadAssets();
};

#endif