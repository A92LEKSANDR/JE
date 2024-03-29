#ifndef JORA
#define JORA

#include "DECK.h"                                                   

class Jora{

   /* class Hand {
    public:
        std::vector<Card> cards;

        void attack() {

        }
    };*/

    class Player {
    public:
        std::vector<Card> cards;

        void attack(Card& card) {

        }

        bool isEmptyCards() {
            return cards.empty();
        }

    };

    class CPU {
    public:
        std::vector<Card> cards;

        void attack(Card& card) {

        }

        bool isEmptyCards() {
            return cards.empty();
        }
    };

    void pushCardWithdrawal() {
        if (deck.size() > 2) {
            withdrawal.push_back(deck.back());
            deck.erase(deck.end() - 1);
        }
        //reshuffle method need her
    }

    Card pushCardGamers() {                                    
        Card temp({ deck.back() });

        deck.erase(deck.end() - 1);

        return temp;
    }

     bool isStep(Card withdrawal,Player* gamer ) {                                  
        for (int i = 0; i < gamer->cards.size(); ++i) {
            if (withdrawal.getRank() == gamer->cards[i].getRank() || withdrawal.getSuit() == gamer->cards[i].getSuit()) {
                return true;
            }
            return false;
        }
    }

     void renderCard(Player player) {
         if (!player.cards.empty()) {
             for (int i = 0; i < player.cards.size(); ++i) {
                 if (i != draggedIndex) {
                     player.cards[i].draw(window);
                 }
             }
             if (draggedIndex != -1 && draggedIndex < player.cards.size()) {
                 player.cards[draggedIndex].draw(window);
             }
         }
     }

     sf::Vector2f getCoordLastCard(const Player& player) {
         if (!player.cards.empty() && player.cards.size() > 1) {
             return { player.cards[player.cards.size() - 1].coordDefaultX + 50,player.cards[player.cards.size() - 1].coordDefaultY };
         }
         return { 0.f,0.f };
     }

    Player player;
    CPU cpu;
    std::vector<Card> deck, withdrawal;
    sf::Texture cardTexture;

    int draggedIndex = -1;                                                      //for drawing up cards
    int clickedCardIndex = -1;

    const int width = 1220, height = 900;

    sf::RenderWindow window;

public:
    Jora() :window(sf::VideoMode(width, height), "Jora", sf::Style::Close) {

                                                                                // Инициализация игры, загрузка ресурсов и т.д.

        if (!cardTexture.loadFromFile("src//card.png")) {
                                                                                 //std::cerr << "texture dont is open";
        }
        deck = creatingDeck(cardTexture);                                       

        for (int i = 0; i < 3; ++i) {
            player.cards.push_back(pushCardGamers());

                                
            player.cards[i].coordDefaultX = 490 + (i * 20);                      //запись в переменную координат карт игрока по X
            player.cards[i].coordDefaultY = 650;                                 //                                             Y

            
            player.cards[i].setPos(player.cards[i].coordDefaultX, 
                player.cards[i].coordDefaultY);                                   //Установка координат карт игрока

            cpu.cards.push_back(pushCardGamers());
            cpu.cards[i].setPos(490 + i * 20, 50);                                //Установка координат карт сpu
        }
                                                                                  //отбой карта параметры
        pushCardWithdrawal();
        withdrawal.back().coordDefaultX = 500;
        withdrawal.back().coordDefaultY = 380;
        withdrawal.back().setPos(500, 380);                                      //Установка координат отбоя(500, 380);


        for (int i = 0; i < deck.size(); ++i) {                                  //Установка координат колоды
            deck[i].setPos(800, 400);
        }
    }

void run() {
        window.setFramerateLimit(60);
        sf::Event event;

<<<<<<< HEAD
        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                //BUTTON EVENT CLOSE
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                //MOUSE EVENT BUTTON ON
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    //**************************наши лок координаты
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(pos);
                    //**************************************************
                    
                    ///************** координаты мышки на карте колоды верхней back()
                    if (deck.back().getSprite().getGlobalBounds().contains(worldPos)) {
                        deck.back().isDragging = true;
                        deck.back().offset =deck.back().getSprite().getPosition() - worldPos;
                    }
                    //**********************************************************************

                    //*********проверяем каждую карту на которой мышь
                    for (int index = 0; index < player.cards.size(); ++index) {
                        if (player.cards[index].getSprite().getGlobalBounds().contains(worldPos)) {
                            
                            player.cards[index].isDragging = true;
                            draggedIndex = index;
=======
        while (window.isOpen()) {                                               //пока окно не закроем, программа работает
            
                while (window.pollEvent(event)) {                                     //еще один цикл срабатывает если есть события
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }

                    if (event.type == sf::Event::MouseButtonPressed &&
                        event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i pos = sf::Mouse::getPosition(window);
                        sf::Vector2f worldPos = window.mapPixelToCoords(pos);

                        //нужны условия для активации карты колоды
#pragma region DECK //activity deck.back()
                        if (deck.back().getSprite().getGlobalBounds().contains(worldPos)) {
                            deck.back().isDragging = true;
                            deck.back().offset = deck.back().getSprite().getPosition() - worldPos;
                        }
#pragma endregion
#pragma region PLAYER 
                        for (int index = 0; index < player.cards.size(); ++index) {
                            if (player.cards[index].getSprite().getGlobalBounds().contains(worldPos)) {
>>>>>>> 39eefc213176268906f1f16ce3782a711b3739b7

                                player.cards[index].isDragging = true;
                                draggedIndex = index;

                                player.cards[index].offset = player.cards[index].getSprite().getPosition() - worldPos;

                                for (int i = 0; i < player.cards.size(); ++i) {
                                    if (i != index) {
                                        player.cards[i].isDragging = false;
                                    }
                                }
                            }
                        }
<<<<<<< HEAD
                    }
                    //****************************************************************

                }//MOUSE EVENT BUTTON ON

                //MOUSE EVENT BUTTON OFF
                else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    std::vector<Card> deleteCard;

                    //*********проверяем каждую карту на которой мышь
                    for (int i = 0; i < player.cards.size(); ++i) {
                            player.cards[i].isDragging = false;
          
                            if (player.cards[i].checkCollision(withdrawal.back())) {
=======
#pragma endregion
                    }                                                                   //********************************
                    /*******************************************************************/

                    else if (event.type == sf::Event::MouseButtonReleased &&
                        event.mouseButton.button == sf::Mouse::Left) {                //событие когда отпускаем кнопку мыши
                        std::vector<Card> deleteCard;
#pragma region PLAYER 
                        for (int i = 0; i < player.cards.size(); ++i) {
                            player.cards[i].isDragging = false;

                            if (player.cards[i].checkCollision(withdrawal.back())) {//если было пересечение с обоем
>>>>>>> 39eefc213176268906f1f16ce3782a711b3739b7
                                if (player.cards[i].getRank() == withdrawal.back().getRank() ||
                                    player.cards[i].getSuit() == withdrawal.back().getSuit()) {

                                    withdrawal.push_back(player.cards[i]);
                                    deleteCard.push_back(player.cards[i]);

<<<<<<< HEAD
                                    withdrawal.back().setPos(500, 380);                                 
 
=======
                                    withdrawal.back().setPos(500, 380);          //расположение добавленой карты


>>>>>>> 39eefc213176268906f1f16ce3782a711b3739b7
                                }
                                else {
                                    player.cards[i].setPos(player.cards[i].coordDefaultX, player.cards[i].coordDefaultY);
                                }

                            }
<<<<<<< HEAD
                            else {
                                player.cards[i].setPos(player.cards[i].coordDefaultX, player.cards[i].coordDefaultY);
                            }
                    }
                    //********************************************************

                    //удаляем карту из колоды
                    for (const auto& card : deleteCard) {
                        player.cards.erase(std::remove(player.cards.begin(), player.cards.end(), card), player.cards.end());
                    }
                    //*************************************
                     
                    //*********проверяем карту колоды на пересечение
                    if (deck.back().checkCollision(player.cards.back())) {
                        deck.back().isDragging = false;
=======
                        }
                        for (const auto& card : deleteCard) {
                            player.cards.erase(std::remove(player.cards.begin(), player.cards.end(), card), player.cards.end());
                        }
#pragma endregion
#pragma region DECK
                        //если было пересечение с картой.back() игрока
                        if (deck.back().checkCollision(player.cards.back())) {
                            deck.back().isDragging = false;
>>>>>>> 39eefc213176268906f1f16ce3782a711b3739b7

                            player.cards.push_back(deck.back());
                            deck.erase(deck.end() - 1);

<<<<<<< HEAD
                        player.cards.back().setPos(player.cards.back().coordDefaultX + 50, 650);
                       
                     }
                    else {
                        deck.back().setPos(800, 400);
                        deck.back().isDragging = false;
                    }
                    //*************************************
                    
                   
                }//EVENT MOUSE OFF
=======
                            //позиция карты в колоде игрока
                            /*нужен алгоритм для коректной добавки карт и их смещения в случаее переполнения*/
                            //наверное нужен большой спрайт
                            player.cards.back().setPos(player.cards.back().getSprite().getPosition().x - 50, 650);
                        }
                        else {
                            deck.back().setPos(800, 400);
                            deck.back().isDragging = false;
                        }
#pragma endregion
                    }
                    /*******************************************************************/
>>>>>>> 39eefc213176268906f1f16ce3782a711b3739b7

                    for (int i = 0; i < player.cards.size(); ++i) {
                        if (player.cards[i].isDragging) {

                            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                            sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
                            sf::Vector2f newPosition = worldPosition + player.cards[i].offset;

                            player.cards[i].setPos(newPosition);

                        }
                    }
                    if (deck.back().isDragging) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                        sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
                        sf::Vector2f newPosition = worldPosition + deck.back().offset;

                        deck.back().setPos(newPosition);
                    }

<<<<<<< HEAD
            }//inner while()//

#pragma region RENDER BLOCK

             // isStep(withdrawal, player);
            window.clear();
=======
                }                                                                     //inner while()
                 /*******************************************************************/
>>>>>>> 39eefc213176268906f1f16ce3782a711b3739b7

                /*где здесь нужна логика хода игроков, то что вверху будет работать бесконечно*/

<<<<<<< HEAD
            renderCard(player);
            
            
            for (int i = 0; i < cpu.cards.size(); ++i) {
                cpu.cards[i].draw(window);
            }

            if (!deck.empty()) {
                for (int i = 0; i < deck.size(); ++i) {
                    deck[i].draw(window);
                }
            }


            window.display();
#pragma endregion
        }//outter while
}
=======
                window.clear();

                withdrawal.back().draw(window);

                if (!player.cards.empty()) {
                    for (int i = 0; i < player.cards.size(); ++i) {
                        if (i != draggedIndex) {
                            player.cards[i].draw(window);
                        }
                    }
                    if (draggedIndex != -1 && draggedIndex < player.cards.size()) {
                        player.cards[draggedIndex].draw(window);
                    }
                }



                for (int i = 0; i < cpu.cards.size(); ++i) {
                    cpu.cards[i].draw(window);
                }

                if (!deck.empty()) {
                    for (int i = 0; i < deck.size(); ++i) {
                        deck[i].draw(window);
                    }
                }


                window.display();
            

        }//***********************************************************outter while
}//*******************************************************************this is exit method run()                                           
>>>>>>> 39eefc213176268906f1f16ce3782a711b3739b7
    

    void gameOver() {

    }

};//******************************************************************JORA CLASS


#endif