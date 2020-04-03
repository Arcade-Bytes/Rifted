#ifndef ShopSTATE_H
#define ShopSTATE_H


#include "State.h"

class ShopState : public State {
private:
    sf::Sprite* parchment;// Fondo para los datos del jugador
    sf::Texture pergamino; //Contiene la textura para construir parchment
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
    sf::Sprite* armor_upgrade;// Indica las mejoras de armadura actuales
    sf::Texture armor; //Contiene la textura para construir armor_upgrade
    sf::Sprite* npc_cleric;// Indica el sprite del tendero actuales
    sf::Texture cleric; //Contiene la textura para construir npc_cleric
    sf::Sprite* text_box;// Indica el sprite de la caja de texto
    sf::Texture box; //Contiene la textura para construir text_box
    sf::Clock* clock;

    std::vector<sf::Text> textoTienda_v1; //Opciones de bienvenida
    std::vector<sf::Text> textoTienda_v2; //Opciones: Comprado
    std::vector<sf::Text> textoTienda_v3; //Opciones: Te falta dinero
    std::vector<sf::Text> textoTienda_v4; //Opciones: Eso esta al maximo
    std::vector<sf::Text> textoTienda_v5; //Opciones de despedida
    sf::Text texto_render;
    sf::Text pointer;

    bool b_reInit;
    int i_damage; //Para montar las estadísticas del jugador en la pantalla de pausa
    int i_life;
    int i_money;
    int i_armor;
    bool dir; //Direccion de la animacion de la flecha
    int i; //COntrol para animacion de la flecha
    
public:
    ShopState(std::stack<State*>* states, Player* player);
    ~ShopState();

    void update();
    void render();

    void initPlayerData();

    void drawText();
    void drawArrow();
    void drawPlayerData();
    void drawParchment();
    void drawPrices();

    void loadDialogue();
    void loadAssets();
};

#endif