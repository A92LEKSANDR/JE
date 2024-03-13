#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include <string>
#include <set>

class Card {
public:
	enum Suit { Clubs, Diamonds, Hearts, Spades };
	enum Rank { Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };

	bool isDragging = false;
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

//если я пошел то тру, и пока у меня не false
bool logicGame(std::vector<Card>& player, const std::vector<Card>& outDeck, std::vector<Card>& deck, bool& check) {

	bool playersStep = false;

	switch (outDeck.back().getRank()) {
	case Card::Ace:
		playersStep = false;
		break;
	case Card::Six:
		for (int i = 0; i < 2; ++i) {
			player.push_back(deck[i]);
			player[i].coordDefaultX += i * 50;
			player[i].setPos(player[i].coordDefaultX, player[i].coordDefaultY);

			deck.erase(deck.begin() + i);
		}
		playersStep = false;
		break;
	case Card::Seven:
		player.push_back(deck.back());
		player.back().coordDefaultX += 50;
		player.back().setPos(player.back().coordDefaultX, player.back().coordDefaultY);

		deck.erase(deck.begin() + 1);
		playersStep = false;
		break;
	default:
		for (auto card : player) {
			if (card.getRank() == outDeck.back().getRank()) {
				playersStep = true;
			}
			else if (card.getSuit() == outDeck.back().getSuit()) {
				playersStep = true;
			}
			playersStep = false;
		}
		break;
	}
	return playersStep;
}

bool isGoingAboard(const int& width, const int& height, const sf::RenderWindow& window, Card& card, sf::Vector2f& newPos) {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
	sf::Vector2f newPosition = worldPosition + card.offset;

	if (mousePosition.x >= 0 && mousePosition.y >= 0 &&
		mousePosition.x <= width && mousePosition.y <= height) {

		if (newPosition.x + card.getSprite().getLocalBounds().width <= width &&
			newPosition.y + card.getSprite().getLocalBounds().height <= height) {
			return false;
			newPos = newPosition;
		}
	}
	return true;
}

class Game {
public:
	Game();
	void run();

	sf::Texture& getTexture() {
		return cardTexture;
	}

	bool gameCheck = false;
private:
	void render();
	void processEvents();
	void update();

	int draggedIndex = -1;//for drawing up cards

	const int width = 1220, height = 900;

	sf::RenderWindow window;

	std::vector<Card> deck, outDeck, player, cpu;

	sf::Texture cardTexture;
};

Game::Game() : window(sf::VideoMode(width, height), "Jora", sf::Style::Close) {
	// Инициализация игры, загрузка ресурсов и т.д.

	if (!cardTexture.loadFromFile("src//card.png")) {
		std::cerr << "texture dont is open";
	}

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

	//init player
	for (int i = 0; i < 3; ++i) {
		player.push_back(deck[i]);
		cpu.push_back(deck[i]);

		player[i].coordDefaultX += i * 50;
		cpu[i].coordDefaultX += i * 50;

		player[i].setPos(player[i].coordDefaultX, player[i].coordDefaultY);

		deck.erase(deck.begin() + i);
	}

	outDeck.push_back(deck.back());
	outDeck.back().setPos(550, 300);
	deck.erase(deck.begin() + deck.size() - 1);

	for (int i = 0; i < deck.size(); ++i) {
		deck[i].setPos(800, 300);
	}
}

void Game::run() {
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		processEvents();
		update();
		render();
	}
}

void Game::processEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
#pragma region MOUSE on
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

			sf::Vector2i pos = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(pos);

			for (int index = 0; index < player.size(); ++index) {
				if (player[index].getSprite().getGlobalBounds().contains(worldPos)) {
					player[index].isDragging = true;
					draggedIndex = index;
					player[index].offset = player[index].getSprite().getPosition() - worldPos;

					for (int i = 0; i < player.size(); ++i) {
						if (i != index) {
							player[i].isDragging = false;
						}
					}
				}
			}
		}
#pragma endregion
#pragma region MOUSE off
		else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			//check all card player

			std::vector<Card> remove;
				for (int index = 0; index < player.size(); ++index) {
					player[index].isDragging = false;
					if (player[index].checkCollision(outDeck.back())) {
						player[index].setPos(outDeck.back().getSprite().getPosition());

						outDeck.push_back(player[index]);
						//push in deck out and this card stan back()
						remove.push_back(player[index]);
					}
					else {
						player[index].setPos(player[index].coordDefaultX, player[index].coordDefaultY);
					}
				}

				for (const auto& card : remove) {
					player.erase(std::remove(player.begin(), player.end(), card), player.end());
				}
				
		}
	}
#pragma endregion
}//processEvents();

void Game::update() {
	// Логика обновления игры, обработка ввода, изменение состояний объектов и т.д.	
	for (int i = 0; i < player.size(); ++i) {
		sf::Vector2f Position;
			if (player[i].isDragging) {
				
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
				sf::Vector2f newPosition = worldPosition + player[i].offset;

				if (mousePosition.x >= 0 && mousePosition.y >= 0 &&
					mousePosition.x <= width && mousePosition.y <= height) {

					if (newPosition.x + player[i].getSprite().getLocalBounds().width <= width &&
						newPosition.y + player[i].getSprite().getLocalBounds().height <= height) {
						
						player[i].setPos(newPosition);
					}
				}

			}
			
	}
}

void Game::render() {
	window.clear();

	outDeck.back().draw(window);

#pragma region DECK
	for (auto card : deck) {
		card.draw(window);
	}
#pragma endregion


#pragma region PLAYER

	if (!player.empty()) {
			for (int i = 0; i < player.size(); ++i) {
				if (i != draggedIndex) {
					player[i].draw(window);
				}
			}
			if (draggedIndex != -1) {
				player[draggedIndex].draw(window);
			}
	}
	std::cout << player.size() << '\n';
	

	//здесь проблема ошибка переполнения
	
#pragma endregion



	// Отрисовка объектов игры
	window.display();
}


int main() {

	Game jora;
	jora.run();

	return 0;
}