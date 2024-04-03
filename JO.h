#ifndef JORA
#define JORA

#include "DECK.h"
#include <iostream>

class Game {
private:
    std::vector<Card> deck;
    std::vector<Card> discardPile;
    std::vector<Card> player1Hand;
    std::vector<Card> player2Hand;
    sf::Texture textureCard;

    int currentPlayer;

    void InitializeDeck() {
        deck = creatingDeck(textureCard);                                               //������������� ������, ��� ������
    }

    void InitializeDescardPile() {                                                      //������������� �����
        discardPile.push_back(deck.back());
        deck.erase(deck.end() - 1);
    }

    void pushBackCard(std::vector<Card>& player) {                                      //������� ����� ������ � ������� �� ������
        if (!deck.empty()) {
            player.push_back(deck.back());
            deck.erase(deck.end() - 1);
        }
    }

    void DealHands() {                                                                  // ����� ���������� ������� ���� �������
        for (int i = 0; i < 3; ++i) {
            pushBackCard(player1Hand);
            pushBackCard(player2Hand);
        }
    }

    void SwitchPlayer() {                                                               // ����� �������� ������
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    void DrawDiscardPile(sf::RenderWindow& window) {
        // ��������� �����
        // ��������, ��������� ���� �� discardPile
        discardPile.back().draw(window);
    }

    void DrawPlayerHands(sf::RenderWindow& window) {
        // ��������� ���� �������
        // ��������, ��������� ���� �� player1Hand � player2Hand
    }

    void GameLoop() {
        sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game");

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();

            DrawDiscardPile(window);
            DrawPlayerHands(window);

            window.display();
        }
    }

public:
    Game() : currentPlayer(1) {

        if (!textureCard.loadFromFile("src//card.png")) {
            std::cerr << "load error texture card";
        }

        InitializeDeck();
        DealHands();
        GameLoop();
    }
};

#endif