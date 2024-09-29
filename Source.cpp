#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <ctime>  

using namespace std;

enum Suit {
	NONE_S = 0, SPADES, DIAMONDS, CLUBS, HEARTS
};
enum Power {
	NONE_P = 0, P6 = 6, P7, P8, P9, P10, JACK, QUEEN, KING, ACE
};


class Card {
private:
	Power power;
	Suit suit;
public:
	Card(Suit, Power);
	void printInLine();
	void printEndl();
	bool isComparable(const Card*, const Card*);
	bool isOfTheSamePower(const Card*);
	bool isGreaterThan(const Card*, const Card*);
	Suit getSuit();
};

Card::Card(Suit newSuit, Power newPower) {
	suit = newSuit;
	power = newPower;
}

void Card::printInLine() {
	char currentChar;
	if (suit == SPADES) {
		currentChar = 6;
		cout << "|" << currentChar; //♠️
	}
	else if (suit == DIAMONDS) {
		currentChar = 4;
		cout << "|" << currentChar; //♦️ 
	}
	else if (suit == CLUBS) {
		currentChar = 5;
		cout << "|" << currentChar; //♣️
	}
	else {
		currentChar = 3;
		cout << "|" << currentChar; //♥️
	}
	if (power == P6) {
		cout << "6|";
	}
	else if (power == P7) {
		cout << "7|";
	}
	else if (power == P8) {
		cout << "8|";
	}
	else if (power == P9) {
		cout << "9|";
	}
	else if (power == P10) {
		cout << "10|";
	}
	else if (power == JACK) {
		cout << "JACK|";
	}
	else if (power == QUEEN) {
		cout << "QUEEN|";
	}
	else if (power == KING) {
		cout << "KING|";
	}
	else {
		cout << "ACE|";
	}
}

void Card::printEndl() {
	printInLine();
	cout << endl;
}

bool Card::isComparable(const Card* cardForComparison, const Card* trump) {
	if (suit == trump->suit) {
		return true;
	}
	else if (cardForComparison->suit == trump->suit) {
		return true;
	}
	else if (suit == cardForComparison->suit) {
		return true;
	}
	return false;
}

bool Card::isOfTheSamePower(const Card* cardForComparison) {
	int selfPower, comparisonPower;
	if (power > ACE) {
		selfPower = power - ACE + P6;
	}
	else {
		selfPower = power;
	}
	if (cardForComparison->power > ACE) {
		comparisonPower = power - ACE + P6;
	}
	else {
		comparisonPower = cardForComparison->power;
	}
	if (selfPower == comparisonPower) {
		return true;
	}
	return false;
}

bool Card::isGreaterThan(const Card* cardForComparison, const Card* trump) {
	int power1, power2;
	if (suit == trump->suit) {
		power1 = power + P9;
	}
	else {
		power1 = power;
	}
	if (cardForComparison->suit == trump->suit) {
		power2 = cardForComparison->power + P9;
	}
	else {
		power2 = cardForComparison->power;
	}
	if (power1 > power2) {
		return true;
	}
	return false;
}

Suit Card::getSuit() {
	return suit;
}

class Deck {
private:
	Card* trump;
	list<Card*> cards;
public:
	Deck();
	void print();
	void deal(int, list<Card*>&);
	Card* getTrump();
	int cardsRemaining();
	~Deck();
};

Deck::Deck() {
	Card* trumpSample;
	for (int suit = SPADES; suit <= HEARTS; suit++) {
		for (int power = P6; power <= ACE; power++) {
			Card* newCard = new Card((Suit)suit, (Power)power);
			cards.push_back(newCard);
		}
	}
	srand(time(0));
	vector<Card*> temp(cards.begin(), cards.end());
	random_shuffle(temp.begin(), temp.end());
	copy(temp.begin(), temp.end(), cards.begin());
	trump = new Card(*cards.front());
	trumpSample = cards.front();
	cards.pop_front();
	cards.push_back(trumpSample);
}

void Deck::print() {
	cout << "Cards remaining:" << cards.size() << endl;
	cout << "Trump:";
	trump->printEndl();
}

void Deck::deal(int amount, list<Card*>& cardsGiven) {
	if (amount < cards.size()) {
		for (int i = 0; i < amount; i++) {
			cardsGiven.push_back(cards.front());
			cards.pop_front();
		}
	}
	else {
		for (int i = 0; i < cards.size(); i++) {
			cardsGiven.push_back(cards.front());
			cards.pop_front();
		}
	}
}

Card* Deck::getTrump() {
	return trump;
}

int Deck::cardsRemaining() {
	return cards.size();
}

Deck::~Deck() {
	for (list<Card*>::iterator currentElement = cards.begin(); currentElement != cards.end(); currentElement++) {
		delete* currentElement;
	}
	delete trump;
}

class Pair {
private:
	Card* p1, * p2;
public:
	Pair(Card*);
	void addSecond(Card*);
	void print();
	bool isUncomplete();
	Card* getFirstCard();
	Card* getSecondCard();
};

Pair::Pair(Card* newElement) {
	p1 = newElement;
	p2 = NULL;
}

void Pair::addSecond(Card* newElement) {
	if (!p2) {
		p2 = newElement;
		return;
	}
	throw invalid_argument("Пара перепонена");
}

void Pair::print() {
	if (!p2) {
		p1->printEndl();
	}
	else {
		p1->printInLine();
		p2->printInLine();
		cout << "(beaten)" << endl;
	}
}

bool Pair::isUncomplete() {
	if (!p2) {
		return true;
	}
	return false;
}

Card* Pair::getFirstCard() {
	return p1;
}

Card* Pair::getSecondCard() {
	return p2;
}

class Table {
private:
	list<Pair*> playedCards;
public:
	void print();
	void putCard(Card*);
	Pair* getUnbeatenCard();
	void beat(Card*);
	bool hasUnbeatenCards();
	void clear(bool);
	int getAmountOfCards();
	list<Card*> giveAllCards();
	~Table();
};

void Table::print() {
	if (playedCards.size() == 0) {
		cout << "Table: empty" << endl;
	}
	else {
		cout << "Table:" << endl;
		for (list<Pair*>::iterator currentElement = playedCards.begin(); currentElement != playedCards.end(); currentElement++) {
			(*currentElement)->print();
		}
	}
}

void Table::putCard(Card* newCard) {
	Pair* newPair = new Pair(newCard);
	playedCards.push_back(newPair);
}

Pair* Table::getUnbeatenCard() {
	for (list<Pair*>::iterator currentElement = playedCards.begin(); currentElement != playedCards.end(); currentElement++) {
		if ((*currentElement)->isUncomplete()) {
			return *currentElement;
		}
	}
	return NULL;
}

void Table::beat(Card* newCard) {
	getUnbeatenCard()->addSecond(newCard);
}

bool Table::hasUnbeatenCards() {
	if (getUnbeatenCard() == NULL) {
		return false;
	}
	return true;
}

void Table::clear(bool cardDelete) {
	if (!cardDelete) {
		for (list<Pair*>::iterator currentElement = playedCards.begin(); currentElement != playedCards.end(); currentElement++) {
			delete* currentElement;
		}
	}
	else {
		for (list<Pair*>::iterator currentElement = playedCards.begin(); currentElement != playedCards.end(); currentElement++) {
			delete (*currentElement)->getFirstCard();
			delete (*currentElement)->getSecondCard();
			delete* currentElement;
		}
	}
	playedCards.clear();
}

int Table::getAmountOfCards() {
	return playedCards.size();
}

list<Card*> Table::giveAllCards() {
	list<Card*> cards;
	for (list<Pair*>::iterator currentElement = playedCards.begin(); currentElement != playedCards.end(); currentElement++) {
		cards.push_back((*currentElement)->getFirstCard());
		if (!(*currentElement)->isUncomplete()) {
			cards.push_back((*currentElement)->getSecondCard());
		}
	}
	return cards;
}

Table::~Table() {
	clear(true);
}

class Player {
public:
	list<Card*> hand;
	string name;
	Player(list<Card*>, string);
	void printHand();
	virtual Card* play(Table*, Card*) = 0;
	int getHandLength();
	void takeCards(list<Card*>);
	bool canAddCards(Table*);
	virtual Card* addCard(Table*, Card*) = 0;
	void printName();
	virtual bool NeedsFullInformation() = 0;
	Card* getLowestTrump(Card*);
	void printHandLength();
	virtual void printCantAddCard() = 0;
	~Player();
};

Player::Player(list<Card*> newHand, string newName) {
	hand = newHand;
	name = newName;
}

void Player::printHand() {
	cout << "Your hand:";
	if (hand.size() == 0) {
		cout << "empty" << endl;
	}
	else {
		for (list<Card*>::iterator currentElement = hand.begin(); currentElement != hand.end(); currentElement++) {
			(*currentElement)->printInLine();
		}
	}
	cout << endl;
}

int Player::getHandLength() {
	return hand.size();
}

void Player::takeCards(list<Card*> cards) {
	hand.splice(hand.end(), cards);
}

bool Player::canAddCards(Table* table) {
	list<Card*> cardsOnTable = table->giveAllCards();
	for (list<Card*>::iterator currentTableCard = cardsOnTable.begin(); currentTableCard != cardsOnTable.end(); currentTableCard++) {
		for (list<Card*>::iterator currentHandCard = hand.begin(); currentHandCard != hand.end(); currentHandCard++) {
			if ((*currentHandCard)->isOfTheSamePower((*currentTableCard))) {
				return true;
			}
		}
	}
	return false;
}

void Player::printName() {
	cout << name << endl;
}

Card* Player::getLowestTrump(Card* trump) {
	Card* min = NULL;
	for (list<Card*>::iterator currentElement = hand.begin(); currentElement != hand.end(); currentElement++) {
		if ((*currentElement)->getSuit() == trump->getSuit()) {
			if (min == NULL || min->isGreaterThan(*currentElement, trump)) {
				min = *currentElement;
			}
		}
	}
	return min;
}

void Player::printHandLength() {
	cout << name << " has " << hand.size() << " cards." << endl;
}

Player::~Player() {
	for (list<Card*>::iterator currentElement = hand.begin(); currentElement != hand.end(); currentElement++) {
		delete* currentElement;
	}
}

class Bot : public Player {
public:
	Bot(list<Card*>, string);
	virtual Card* play(Table*, Card*);
	virtual Card* addCard(Table*, Card*);
	virtual bool NeedsFullInformation();
	virtual void printCantAddCard();
};

Bot::Bot(list<Card*> newHand, string newName) :
	Player(newHand, newName) {
}

Card* Bot::play(Table* table, Card* trump) {
	Card* min = NULL;
	list<Card*>::iterator minIter;
	Pair* unbeatenCard = table->getUnbeatenCard();
	if (!table->hasUnbeatenCards()) {
		for (list<Card*>::iterator currentElement = hand.begin(); currentElement != hand.end(); currentElement++) {
			if (min == NULL || min->isGreaterThan(*currentElement, trump)) {
				min = *currentElement;
				minIter = currentElement;
			}
		}
		if (min != NULL) {
			cout << name << " plays ";
			min->printEndl();
		}
	}
	else {
		for (list<Card*>::iterator currentElement = hand.begin(); currentElement != hand.end(); currentElement++) {
			if ((*currentElement)->isComparable(unbeatenCard->getFirstCard(), trump) && (*currentElement)->isGreaterThan(unbeatenCard->getFirstCard(), trump)) {
				if (min == NULL || min->isGreaterThan(*currentElement, trump)) {
					min = (*currentElement);
					minIter = currentElement;
				}
			}
		}
		if (min != NULL) {
			cout << name << " beats ";
			unbeatenCard->getFirstCard()->printInLine();
			cout << " with ";
			min->printEndl();
		}
	}
	if (min == NULL) {
		cout << name << " takes." << endl;
		return NULL;
	}
	else {
		hand.remove(*minIter);
	}
	return min;
}

Card* Bot::addCard(Table* table, Card* trump) {
	if (!canAddCards(table)) {
		return NULL;
	}
	Card* min = NULL;
	list<Card*> cardsOnTable = table->giveAllCards();
	for (list<Card*>::iterator currentTableCard = cardsOnTable.begin(); currentTableCard != cardsOnTable.end(); currentTableCard++) {
		for (list<Card*>::iterator currentHandCard = hand.begin(); currentHandCard != hand.end(); currentHandCard++) {
			if ((*currentHandCard)->isOfTheSamePower((*currentTableCard))) {
				if (min == NULL || min->isGreaterThan(*currentHandCard, trump)) {
					min = *currentHandCard;
				}
			}
		}
	}
	if (min != NULL) {
		cout << name << " adds ";
		min->printEndl();
		hand.remove(min);
	}
	return min;
}

bool Bot::NeedsFullInformation() {
	return false;
}

void Bot::printCantAddCard() {
	return;
}

class Human : public Player {
public:
	Human(list<Card*>, string);
	virtual Card* play(Table*, Card*);
	virtual Card* addCard(Table*, Card*);
	virtual bool NeedsFullInformation();
	virtual void printCantAddCard();
};

Human::Human(list<Card*> newHand, string newName) :
	Player(newHand, newName) {
}

Card* Human::play(Table* table, Card* trump) {
	int choice;
	Card* min = NULL;
	Pair* unbeatenCard = table->getUnbeatenCard();
	list<Card*>::iterator currentCard;
	if (!table->hasUnbeatenCards()) {
		while (true) {
			cout << "Which card do you want to play?" << endl;
			cin >> choice;
			choice--;
			if (choice < 0 || choice >= hand.size()) {
				cout << "That is not a valid number, try another." << endl;
			}
			else {
				currentCard = hand.begin();
				advance(currentCard, choice);
				Card* ans = *currentCard;
				hand.remove(*currentCard);
				return ans;
			}
		}
	}
	else {
		cout << "Which card do you want to beat with? If you want to take enter 0." << endl;
		while (true) {
			cin >> choice;
			if (choice == 0) {
				return NULL;
			}
			choice--;
			if (choice < 0 || choice >= hand.size()) {
				cout << "That is not a valid number, try another." << endl;
			}
			else {
				currentCard = hand.begin();
				advance(currentCard, choice);
				if ((*currentCard)->isComparable((*unbeatenCard).getFirstCard(), trump) && (*currentCard)->isGreaterThan((*unbeatenCard).getFirstCard(), trump)) {
					Card* ans = *currentCard;
					hand.remove(*currentCard);
					return ans;
				}
				else {
					cout << "You can't beat that card like this" << endl;
				}
			}
		}
	}
}

Card* Human::addCard(Table* table, Card* trump) {
	if (!canAddCards(table)) {
		return NULL;
	}
	int choice;
	list<Card*>::iterator currentHandCard;
	cout << "Which card do you want to add? If you do not want to add any cards, type 0" << endl;
	while (true) {
		cin >> choice;
		if (choice < 0 || choice > hand.size()) {
			cout << "That is not a valid number, try another." << endl;
		}
		else {
			if (choice == 0) {
				return NULL;
			}
			choice--;
			list<Card*> cardsOnTable = table->giveAllCards();
			while (true) {
				currentHandCard = hand.begin();
				advance(currentHandCard, choice);
				for (list<Card*>::iterator currentTableCard = cardsOnTable.begin(); currentTableCard != cardsOnTable.end(); currentTableCard++) {
					if ((*currentHandCard)->isOfTheSamePower((*currentTableCard))) {
						Card* ans = *currentHandCard;
						hand.remove(*currentHandCard);
						return ans;
					}
				}
				cout << "Choose another card. If you do not want to add type 0." << endl;
				while (true) {
					cin >> choice;
					if (choice < 0 || choice > hand.size()) {
						cout << "That is not a valid number, try another." << endl;
					}
					else {
						break;
					}
				}
				if (choice == 0) {
					return NULL;
				}
				choice--;
			}
		}
	}
}

bool Human::NeedsFullInformation() {
	return true;
}

void Human::printCantAddCard() {
	cout << name << " can not add any cards" << endl;
}

class Game {
private:
	Deck* deck;
	Table* table;
	list<Player*> players;
	list<Player*>::iterator currentAdder, attacker, defender;
	list<Player*>::iterator getNext(list<Player*>::iterator);
	list<Player*>::iterator getPrevious(list<Player*>::iterator);
	bool turnEndHandle(bool);
	int turnCnt;
	void printTurn(list<Player*>::iterator);
	enum State {
		turnStart, defence, adding, defenceTakes
	};
	State currentState;
public:
	Game();
	void start();
	~Game();
};

Game::Game() {
	deck = new Deck();
	table = new Table();
	turnCnt = 0;
	currentState = turnStart;
}

list<Player*>::iterator Game::getNext(list<Player*>::iterator currentElement) {
	advance(currentElement, 1);
	if (currentElement == players.end()) {
		currentElement = players.begin();
	}
	return currentElement;
}

list<Player*>::iterator Game::getPrevious(list<Player*>::iterator currentElement) {
	if (currentElement == players.begin()) {
		currentElement = players.end();
	}
	return --currentElement;
}

void Game::start() {
	list<Card*> newHand;
	bool cardAdded, gameEnd;
	int numberOfPlayers, playerDecision;
	string newNickname;
	Card* currentCard;
	cout << "Enter number of players:" << endl;
	cin >> numberOfPlayers;
	if (numberOfPlayers < 2 || numberOfPlayers > 6) {
		return;
	}
	for (int i = 0; i < numberOfPlayers; i++) {
		while (true) {
			cout << "If you want player number " << i + 1 << " to be a human player, enter 1, else, if you want him to be a bot, enter 2." << endl;
			cin >> playerDecision;
			if (playerDecision != 1 && playerDecision != 2) {
				cout << "That is not a valid number." << endl;
			}
			else {
				break;
			}
		}
		if (playerDecision == 1) {
			cout << "Enter player " << i + 1 << " nickname" << endl;
			cin >> newNickname;
			deck->deal(6, newHand);
			Human* newHuman = new Human(newHand, newNickname);
			players.push_back(newHuman);
		}
		else {
			cout << "Enter bot " << i + 1 << " nickname" << endl;
			cin >> newNickname;
			deck->deal(6, newHand);
			Bot* newBot = new Bot(newHand, newNickname);
			players.push_back(newBot);
		}
		newHand.clear();
	}
	cout << endl;
	Card* min = NULL;
	for (list<Player*>::iterator currentPlayer = players.begin(); currentPlayer != players.end(); currentPlayer++) {
		Card* currentTrump = (*currentPlayer)->getLowestTrump(deck->getTrump());
		if (currentTrump != NULL) {
			if (min == NULL || min->isGreaterThan(currentTrump, deck->getTrump()), deck->getTrump()) {
				min = currentTrump;
				attacker = currentPlayer;
			}
		}
	}
	if (min == NULL) {
		attacker = players.begin();
	}
	defender = getNext(attacker);
	currentState = turnStart;
	turnCnt = 0;
	while (true) {
		switch (currentState) {
		case turnStart:
			turnCnt++;
			printTurn(attacker);
			currentCard = (*attacker)->play(table, deck->getTrump());
			table->putCard(currentCard);
			currentState = defence;
			break;
		case defence:
			printTurn(defender);
			currentCard = (*defender)->play(table, deck->getTrump());
			if (currentCard == NULL) {
				currentState = defenceTakes;
				currentAdder = attacker;
				break;
			}
			table->beat(currentCard);
			if ((*defender)->getHandLength() == 0 || (table->getAmountOfCards() == 5 && turnCnt == 1) || table->getAmountOfCards() > 5) {
				currentState = turnStart;
				table->clear(true);
				cout << endl << "All cards dropped" << endl << endl;
				gameEnd = turnEndHandle(true);
				if (gameEnd) {
					return;
				}
				break;
			}
			currentState = adding;
			currentAdder = attacker;
			break;
		case adding:
			cardAdded = false;
			if ((*currentAdder)->canAddCards(table)) {
				printTurn(currentAdder);
				currentCard = (*currentAdder)->addCard(table, deck->getTrump());
				if (currentCard != NULL) {
					table->putCard(currentCard);
					currentState = defence;
					cardAdded = true;
				}
			}
			else {
				(*currentAdder)->printCantAddCard();
			}
			if (!cardAdded) {
				currentAdder = getPrevious(currentAdder);
				if (currentAdder == defender) {
					currentState = turnStart;
					table->clear(true);
					cout << endl << "All cards dropped" << endl << endl;
					gameEnd = turnEndHandle(true);
					if (gameEnd) {
						return;
					}
					break;
				}
			}
			break;
		case defenceTakes:
			if ((*currentAdder)->canAddCards(table)) {
				printTurn(currentAdder);
				currentCard = (*currentAdder)->addCard(table, deck->getTrump());
				if (currentCard != NULL) {
					table->putCard(currentCard);
					if ((*defender)->getHandLength() == 0 || (table->getAmountOfCards() == 5 && turnCnt == 1) || table->getAmountOfCards() > 5) {
						currentState = turnStart;
						newHand = table->giveAllCards();
						(*defender)->takeCards(newHand);
						newHand.clear();
						table->clear(false);
						gameEnd = turnEndHandle(false);
						if (gameEnd) {
							return;
						}
						break;
					}
				}
				else {
					currentAdder = getPrevious(currentAdder);
				}
			}
			else {
				(*currentAdder)->printCantAddCard();
				currentAdder = getPrevious(currentAdder);
				if (currentAdder == defender) {
					currentState = turnStart;
					newHand = table->giveAllCards();
					(*defender)->takeCards(newHand);
					newHand.clear();
					table->clear(false);
					gameEnd = turnEndHandle(false);
					if (gameEnd) {
						return;
					}
					break;
				}
			}
		}
	}
}

bool Game::turnEndHandle(bool defenderPlays) {
	bool gameEnd = false;
	list<Player*>::iterator currentPlayer = attacker;
	do {
		int currentHandLength = (*currentPlayer)->getHandLength();
		if (currentHandLength < 6) {
			list<Card*> newHand;
			deck->deal(6 - currentHandLength, newHand);
			(*currentPlayer)->takeCards(newHand);
			newHand.clear();
			if ((*currentPlayer)->getHandLength() == 0) {
				cout << "Player ";
				(*currentPlayer)->printName();
				cout << " won!" << endl;
				if (players.size() <= 2) {
					gameEnd = true;
					break;
				}
				if (currentPlayer == defender) {
					delete* defender;
					players.erase(defender);
					attacker = getNext(attacker);
					defender = getNext(attacker);
					return false;
				}
				if (currentPlayer == attacker) {
					delete* attacker;
					players.erase(attacker);
					if (defenderPlays) {
						attacker = defender;
					}
					else {
						attacker = getNext(defender);
					}
					defender = getNext(attacker);
					return false;
				}
				else {
					delete* currentPlayer;
					players.erase(currentPlayer);
				}
			}
			newHand.clear();

		}
		currentPlayer = getPrevious(currentPlayer);
	} while (currentPlayer != attacker);
	if (defenderPlays) {
		attacker = defender;
	}
	else {
		attacker = getNext(defender);
	}
	defender = getNext(attacker);
	return gameEnd;
}

void Game::printTurn(list<Player*>::iterator currentPlayer) {
	if ((*currentPlayer)->NeedsFullInformation()) {
		deck->print();
		for (list<Player*>::iterator currentIterator = players.begin(); currentIterator != players.end(); currentIterator++) {
			if (currentIterator != currentPlayer) {
				(*currentIterator)->printHandLength();
			}
		}
		table->print();
		(*currentPlayer)->printHand();
		cout << endl;
	}
}

Game::~Game() {
	delete deck;
	delete table;
	for (list<Player*>::iterator currentPlayer = players.begin(); currentPlayer != players.end(); currentPlayer++) {
		delete* currentPlayer;
	}
}

int main() {
	Game* game = new Game();
	game->start();
	delete game;
	return 0;
}