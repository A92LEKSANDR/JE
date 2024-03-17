#ifndef JORA
#define JORA

#include "DECK.h"                                                   

class Player : public Hand {//наследуемся виртуально, класс жора без 2 копий hand
public:
    std::vector<Card> cards;

    void attack(Card& card) override {

    }

    bool isEmptyCards() {
        return cards.empty();
    }

};

class CPU :public Hand {
public:
    std::vector<Card> cards;

    void attack(Card& card) override {

    }

    bool isEmptyCards() {
        return cards.empty();
    }
};

class Jora {
    Player player;
    CPU cpu;
	std::vector<Card> deck, withdrawal;
	sf::Texture cardTexture;
 
	int draggedIndex = -1;//for drawing up cards

	const int width = 1220, height = 900;

	sf::RenderWindow window;

    void pushCardWithdrawal() {
        if (deck.size() > 2) {
            withdrawal.push_back(deck.back());
            deck.erase(deck.end() - 1);
        }
        //reshuffle method need her
    }

    Card pushCardGamers() {                                    //отдаем карту игроку и удаляем из колоды
        Card temp({ deck.back() });

        deck.erase(deck.end() - 1);

        return temp;
    }

public:
    Jora() :window(sf::VideoMode(width, height), "Jora", sf::Style::Close) {

		// Инициализация игры, загрузка ресурсов и т.д.

		if (!cardTexture.loadFromFile("src//card.png")) {
			//std::cerr << "texture dont is open";
		}
        deck = creatingDeck(cardTexture);

        for (int i = 0; i < 3; ++i) {
            player.cards.push_back(pushCardGamers());
            cpu.cards.push_back(pushCardGamers());
        }
        pushCardWithdrawal();
        withdrawal.back().setPos(500, 400);

        for (int i = 0; i < deck.size(); ++i) {
            deck[i].setPos(0 + i * 20, 0);
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
            }
#pragma region LOGIC GAME
            //сделать рандомом. когда игра запускаеться или когда
            bool isPlayerTurn = true;

            while (!player.isEmptyCards() || cpu.isEmptyCards()) {
                
                if (isPlayerTurn) {
                    switch (withdrawal.back().getRank()) {
                    case Card::Ace:
                        break;
                    case Card::Six:
                        player.cards.push_back(pushCardGamers());
                        player.cards.push_back(pushCardGamers());
                        break;
                    case Card::Seven:
                        cpu.cards.push_back(pushCardGamers());
                        break;
                    default:  player.attack(withdrawal.back()); break;
                    }
                }
                else {
                    switch (withdrawal.back().getRank()) {
                    case Card::Ace:
                        break;
                    case Card::Six:
                        player.cards.push_back(pushCardGamers());
                        player.cards.push_back(pushCardGamers());
                        break;
                    case Card::Seven:
                        cpu.cards.push_back(pushCardGamers());
                        break;
                    default:  player.attack(withdrawal.back());
                    }
                }
                isPlayerTurn = !isPlayerTurn;
            }
#pragma endregion

#pragma region RENDER()


            for (int i = 0; i < deck.size(); ++i) {
                deck[i].draw(window);
            }

            withdrawal.back().draw(window);
            window.display();
        }

#pragma endregion
    }

    void gameOver() {

        //std::cout << "game over\n";
    }

};


#endif