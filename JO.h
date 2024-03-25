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

    Player player;
    CPU cpu;
    std::vector<Card> deck, withdrawal;
    sf::Texture cardTexture;

    int draggedIndex = -1;//for drawing up cards
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

            player.cards[i].coordDefaultX = 490 + (i*20);
            player.cards[i].coordDefaultY = 650;

            player.cards[i].setPos(player.cards[i].coordDefaultX, player.cards[i].coordDefaultY);

            cpu.cards.push_back(pushCardGamers());
            cpu.cards[i].setPos(490 + i * 20, 50);
        }
        ////////////////////withdrawal
        pushCardWithdrawal();
        withdrawal.back().coordDefaultX = 500;
        withdrawal.back().coordDefaultY = 380;

        withdrawal.back().setPos(500, 380);//(500, 380);
        ///////////////////////////////////////////////

        for (int i = 0; i < deck.size(); ++i) {
            deck[i].setPos(800, 400);
        }
    }

void run() {
        window.setFramerateLimit(60);
        sf::Event event;

        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(pos);

#pragma region DECK //activity deck.back()
                    if (deck.back().getSprite().getGlobalBounds().contains(worldPos)) {
                        deck.back().isDragging = true;
                        deck.back().offset =deck.back().getSprite().getPosition() - worldPos;
                    }
#pragma endregion
#pragma region PLAYER //помечаем карту для перемещения если координаты на ней
                    for (int index = 0; index < player.cards.size(); ++index) {
                        if (player.cards[index].getSprite().getGlobalBounds().contains(worldPos)) {
                            
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
#pragma endregion
                }

                /*блок когда мышь опущена, здесь перемещаем нашу карту с условием, если подходит иначе назад*/
                else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    std::vector<Card> deleteCard;
#pragma region PLAYER 
                    for (int i = 0; i < player.cards.size(); ++i) {
                            player.cards[i].isDragging = false;
                            //если было пересечение с обоем
          
                            if (player.cards[i].checkCollision(withdrawal.back())) {
                                //есть условия
                                if (player.cards[i].getRank() == withdrawal.back().getRank() ||
                                    player.cards[i].getSuit() == withdrawal.back().getSuit()) {

                                    withdrawal.push_back(player.cards[i]);
                                    deleteCard.push_back(player.cards[i]);

                                    withdrawal.back().setPos(500, 380);                                 //расположение добавленой карты
                                    //player.cards[i].setPos(withdrawal.back().getSprite().getPosition());
 
                                }
                                //позиция по умолчанию
                                else {
                                    player.cards[i].setPos(player.cards[i].coordDefaultX, player.cards[i].coordDefaultY);
                                }

                            }
                    }
                    for (const auto& card : deleteCard) {
                        player.cards.erase(std::remove(player.cards.begin(), player.cards.end(), card), player.cards.end());
                    }
#pragma endregion
#pragma region DECK
                    if (deck.back().checkCollision(player.cards.back())) {
                        deck.back().isDragging = false;

                        player.cards.push_back(deck.back());
                        deck.erase(deck.end() - 1);

                        player.cards.back().setPos(player.cards[player.cards.size()-1].coordDefaultX+20, 650);
                        //        
                        //        //deck.back().setPos(600, 600);

                                
                        
                        //        //player.cards.back().isDragging = false;
                        //}
                        //else {
                        //    deck.back().setPos(deck.back().coordDefaultX, deck.back().coordDefaultY);
                     }
                    else {
                        deck.back().setPos(800, 400);
                        deck.back().isDragging = false;
                    }
#pragma endregion
                    
                   
                }//else if

                /*здесь блок для перемещения карты, если она отметилась в событии мыши*/
                    for (int i = 0; i < player.cards.size(); ++i) {
                        if (player.cards[i].isDragging) {

                            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                            sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
                            sf::Vector2f newPosition = worldPosition + player.cards[i].offset;

                            player.cards[i].setPos(newPosition);

                            /*if (!player.cards[i].checkCollision(withdrawal.back())) {
                                player.cards[i].setPos(player.cards[i].coordDefaultX, player.cards[i].coordDefaultX);
                            }*/
                        }
                    }
                    if (deck.back().isDragging) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                        sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
                        sf::Vector2f newPosition = worldPosition + deck.back().offset;

                        deck.back().setPos(newPosition);
                    }

            }//inner while()//
                
             // isStep(withdrawal, player);
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
        }//outter while
}
    

    void gameOver() {

    }

};


#endif