#ifndef DECK
#define DECK

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <set>

class Card {
public:
    enum Suit { Clubs, Diamonds, Hearts, Spades };
    enum Rank { Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };

    bool isDragging = false;
    bool isClicked = false;
    sf::Vector2f offset; // Смещение для корректного отображения спрайта при перетаскивании
    float coordDefaultX = 500, coordDefaultY = 600;

    Card() = default;

    Card(Rank rank, Suit suit, sf::Texture& t) :m_rank(rank), m_suit(suit) {

        sprite.setTexture(t);
        //						hod debug int to float
        sprite.setTextureRect(sf::IntRect(147.8 * static_cast<int>(rank), static_cast<int>(suit) * 214, 147, 214));
        sprite.scale(1.0f, 1.0f);
    }

    sf::Sprite& getSprite() {
        return sprite;
    }

    Suit getSuit()const {
        return m_suit;
    }

    Rank getRank()const {
        return m_rank;
    }

    void setPos(float x, float y) {
        sprite.setPosition(x, y);
    }

    void setPos(sf::Vector2f vec) {
        sprite.setPosition(vec.x, vec.y);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);

    }
    bool checkCollision(const Card& other) {
        sf::FloatRect thisCard = sprite.getGlobalBounds();
        sf::FloatRect otherCard = other.sprite.getGlobalBounds();

        return thisCard.intersects(otherCard);
    }

    // Перегрузка оператора ==
    bool operator==(const Card& other) const {
        return (m_suit == other.m_suit) && (m_rank == other.m_rank);
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;

    Suit m_suit;
    Rank m_rank;
};

std::vector<Card> creatingDeck(sf::Texture& cardTexture) {  
    std::vector<Card> deck; deck.reserve(52);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Card::Rank> ranks = { Card::Ace, Card::Two, Card::Three, Card::Four, Card::Five, Card::Six,
                                      Card::Seven, Card::Eight, Card::Nine, Card::Ten, Card::Jack, Card::Queen, Card::King };
    std::vector<Card::Suit> suits = { Card::Clubs, Card::Diamonds, Card::Hearts, Card::Spades };

    std::set<std::pair<Card::Rank, Card::Suit>> addedCards;

    while (deck.size() < 52) {
        std::shuffle(ranks.begin(), ranks.end(), rd);
        std::shuffle(suits.begin(), suits.end(), rd);

        auto newCard = std::make_pair(ranks[0], suits[0]);

        if (addedCards.find(newCard) == addedCards.end()) {
            deck.push_back(Card(ranks[0], suits[0], cardTexture));

            addedCards.insert(newCard);
        }
    }
    
    return deck;                                               
}


#endif