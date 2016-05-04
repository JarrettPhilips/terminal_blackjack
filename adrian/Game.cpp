/*
	Jarrett Philips
	Blackjack
	Game definitions
*/

#include <iostream>
#include <string>
#include <time.h>

#include "Game.h"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

using namespace std;
Game::Game(){
}
Game::~Game(){
}
void Game::set(Player *player1, int numberOfDecksInput){
	dealer = new Player;
	dealer->playerID = 0;
	rootPlayer = player1;
	currentPlayer = player1;
	numberOfDecks = numberOfDecksInput;
	buildDeck();
}
void Game::startDealersTurn(){
	dealer->playerRootCard = NULL;
	dealer->totalValue = 0;
	Card *a = drawCard();
	Card *b = drawCard();
	a->next = b;
	dealer->playerRootCard = a;

	cout << "Dealer's Cards:" << endl;
	cout << a->rank << " of " << a->suit << " " << "****" << " of " << "****" << endl;

	Player *current = rootPlayer;
	while(current != NULL){
		current->playerRootCard = NULL;
		Card *c = drawCard();
		Card *d = drawCard();
		c->next = d;

		current->playerRootCard = c;
		current->done = false;
		current->totalValue = 0;
		current = current->next;
	}
}
void Game::finishDealersTurn(){
	cout << "Dealer's Cards:" << endl;
	Card *current = dealer->playerRootCard;
	while(current != NULL){
		cout << current->rank << " of " << current->suit << " ";
		dealer->totalValue += current->value;
		current = current->next;
	}
	cout << "\n";
	bool below17 = false;
	if(dealer->totalValue < 17){
		below17 = true;
	}
	while(below17){
		Card *c = drawCard();
		cout << "Dealer drew a " << c->rank << " of " << c->suit << endl;
 		dealer->totalValue += c->value;
 		if(dealer->totalValue >= 17){
 			below17 = false;
 		}
	}
	cout << "Total Value: " << dealer->totalValue << endl;
}
bool Game::playersTurn(Player *player){
	player->totalValue = 0;
	Card *current = player->playerRootCard;
	while(current != NULL){
		player->totalValue += current->value;
		cout << current->rank << " of " << current->suit << " ";
		current = current->next;
	}
	cout << "\n";
	cout << "Total Value: " << player->totalValue << endl;
	bool readableInput = false;
	while(readableInput == false){
		cout << "Hit or pass? (h/p) ";
		string input;
		getline(cin, input);
		if(input == "h" || input == "hit"){
			Card *c = drawCard();
			current = player->playerRootCard;
			while(current->next != NULL){
				current = current->next;
			}
			current->next = c;
			cout << "Player " << player->playerID << " drew a " << c->rank << " of " << c->suit << endl;
			player->totalValue += c->value;
			cout << "Total Value: " << player->totalValue << endl;

			readableInput = true;
		} else if(input == "p" || input == "pass"){
			cout << "Player " << player->playerID << " has passed" << endl;
			readableInput = true;
			player->done = true;
		} else {
			cout << "Unable to read players input" << endl;
		}
	}
	if(player->totalValue > 21){
		cout << RED << "Bust" << RESET << endl;
		player->done = true;
	}
	return true;
}

void Game::determineWinnings(){
	Player *p = rootPlayer;
	bool playerWon = false;
	while(p != NULL){
		if(dealer->totalValue >= 21){
			if(p->totalValue <= 21){
				playerWon = true;
			}
		} else if(dealer->totalValue < 21 && p->totalValue <= 21 && p->totalValue > dealer->totalValue){
			playerWon = true;
		}
		if(playerWon){
			cout << GREEN << "Player " << p->playerID << " Won! Adding " << p->bet << RESET << endl;
			p->money += p->bet;
		} else {
			cout << RED << "Player " << p->playerID << " Lost! Removing " << p->bet << RESET << endl;
			p->money -= p->bet;
		}
		p->bet = 0;
		p = p->next;
	}
}


void Game::buildDeck(){
	string suits[4] = {"Hearts", "Clubs", "Diamonds", "Spades"};
	string ranks[13] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eignt", "Nine", "Ten", "Jack", "Queen", "King"};
	int values[13] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
	int totalCards = numberOfDecks * 52;
	int currentDeckLength = 0;
	Card *tmpRoot = NULL;
	for(int i = 0; i < numberOfDecks; i ++){
		for(int j = 0; j < 4; j ++){
			for(int k = 0; k < 13; k ++){
				Card *tmp =  new Card;
				tmp->rank = ranks[k];
				tmp->suit = suits[j];
				tmp->value = values[k];
				tmp->next = NULL;
				if(tmpRoot == NULL){
					tmpRoot = tmp;
				} else {
					srand (time(NULL));
					int x = rand() % currentDeckLength;
					if(x == 0){
						x = 1;
					}

					Card *current = tmpRoot;

					for(int l = 0; l < x - 1; l ++){
						current = current->next;
					}
					tmp->next = current->next;
					current->next = tmp;
				}

				currentDeckLength ++;
 			}
		}
	}
	srand (time(NULL));
	int x = rand() % currentDeckLength;
	if(x == 0){
		x = 1;
	}
	Card *current = tmpRoot;
	for(int l = 0; l < x - 1; l ++){
		current = current->next;
	}
	Card *newRoot = tmpRoot->next;
	tmpRoot->next = current->next;
	current->next = tmpRoot;
	cout << "==============" << endl;
	cout << "Current Deck" << endl;
	cout << "==============" << endl;
	current = newRoot;
	while(current != NULL){
		cout << current->rank << " of " << current->suit << endl;
		current = current->next;
	}
	rootCard = newRoot;
}
Card * Game::drawCard(){
	if(rootCard == NULL){
		buildDeck();
		cout << "Reshuffling deck" << endl;
	}
	Card *c = rootCard;
	rootCard = rootCard->next;
	c->next = NULL;
	return c;
}
Player * Game::getNextPlayer(){
	Player *p = currentPlayer;
	if(currentPlayer->next == NULL){
		currentPlayer = rootPlayer;
	} else {
		currentPlayer = currentPlayer->next;
	}
	return p;
}
void Game::displayFunds(){
	cout << "==== Funds ====" << endl;
	Player *p = rootPlayer;
	while(p != NULL){
		cout << "Player " << p->playerID << ": $" << p->money << endl;
		p = p->next;
	}
}

void Game::collectBets(){
	Player *p = rootPlayer;
	while(p != NULL){
		bool acceptableBet = false;
		while(acceptableBet == false){
			cout << "Player " << p->playerID << " enter a bet: ";
			string input;
			getline(cin, input);
			int bet = stoi(input);

			if(bet <= p->money){
				acceptableBet = true;
				p->bet = bet;
			} else {
				cout << RED << "You cannot bet more money than you have" << RESET << endl;
			}
		}
		p = p->next;
	}
}

void Game::checkForBrokePlayers(){
	Player *p = rootPlayer;
	Player *previous;
	while(p != NULL){
		if(p->money <= 0){
			cout << RED << "Player " << p->playerID << " is out of money, removing from game" << RESET << endl;

			if(p == rootPlayer){
				rootPlayer = p->next;
			} else {
				previous->next = p->next;
			}
		}

		previous = p;
		p = p->next;
	}
}
bool Game::checkForGameOver(){
	if(rootPlayer == NULL){
		return false;
	} else {
		return true;
	}
}
