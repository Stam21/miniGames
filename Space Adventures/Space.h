/**
 * File:    Space.cpp
 *
 * Author: George Stamatopoulos
 * Date:    30/01/2022
 * Partner:  No
 * Course:   Programming with C++
 *
 * Summary of File:
 *
 *   This file contains code that handles both games.
 *
 */
#pragma once
#include "player.h"
#include "graphics.h"
#include "Events.h"
#include "games.h"
#include "Card.h"
#include <stdlib.h>
#include "Shot.h"
#include "Enemy.h"
#include <vector>
#include <list>

class Space
{
	//General declarations
	graphics::MouseState mouse;
	typedef enum {STATUS_START , STATUS_GAMES , STATUS_PLAYERS , STATUS_PLAYING, STATUS_LOADING } status_t; 
	typedef enum {STRATEGO, SPACE } status_g; //NOTE: Draw could also be added 
	status_t status = STATUS_LOADING;
	status_g statusg = STRATEGO;

	void updateStartScreen();
	void updateGamesScreen();
	void updatePlayersScreen();
	void updatePlayingScreen();
	void drawStartScreen();
	void drawGamesScreen();
	void drawPlayersScreen();
	void drawPlayingScreen();

	//Space-invaders declarations
	typedef enum { ONE_PLAYER, TWO_PLAYER } status_p;
	status_p statusp = ONE_PLAYER;
	Player * player1 = nullptr;
	Player * player2 = nullptr;
	bool player1_initialized = false;
	bool player2_initialized = false;
	std::vector<bool> active;
	Shot * bullet = nullptr;
	//NOTE: I could create only one Enemy object and decide which to draw(blue or red) inside its draw function.
	Enemy* enemyBlue = nullptr; 
	Enemy* enemyRed = nullptr;
	std::vector< std::pair<float,float> > shot_pos;
	//Player's bullet objects 
	std::vector<Shot *> shots; 
	// Enemies' bullet objects
	std::vector<Shot*> shots_enemy;
	std::vector<bool> shot_init; 
	bool shot_initialized = false;
	//Control frequency of spray
	int reduceFireRate = 0;
	int controlEnemyFireRate = 0; 
	bool enemyShot = true; 
	void updateSpaceScreen();
	void drawSpaceScreen();

	//Stratego declarations
	typedef enum { play, win1, win2 } status_play; 
	status_play statusPl = play;
	Card* card = nullptr;
	std::vector<Card*> cardsPlayer1; //Blue cards.
	std::vector<Card*> cardsPlayer2; //Red cards.
	std::list<Events*> events; //Active events.
	std::vector<std::string> textures1; 
	std::vector<std::string> textures2;
	std::vector<int> rank1;
	std::vector<int> rank2;
	std::vector< std::pair<float, float> > card1_pos;
	std::vector< std::pair<float, float> > card2_pos;
	//NOTE: Available blocks are the blocks that do not exist in unavBlocks but with attackBlocks added.
	std::vector< std::pair<float, float> > unavBlocks;
	std::vector< std::pair<float, float> > attackBlocks;	
	bool cards1_initialized = false;
	bool cards2_initialized = false;
	int activeCard = 100;
	//Blocks' centers for both players in phase 2.
	float filledCentersX[40] = {}; 
	float filledCentersY[40] = {};
	//Indicates whether a player can move another card.
	int controlCard = 0; 
	//phase becomes 2 when all cards are positioned in the board.
	int phase1 = 1;
	int phase2 = 1; 
	//Player currently playing.
	int player = 1; 
	int target = 0 ;
	int attacker = 0 ;
	float volume = 0.7 ;
	bool music = true; 
	bool musicActiv = true;
	//Forbids playing again on the same round when a card is removed.
	bool removedCard = false;
	//Checks if player has available moves otherwise he loses.
	bool availMoves = false; 
	bool move = true;
	bool attackMode = false;
	//NOTE: gameover indicates the player that finally lost.
	bool gameover1 = false;
	bool gameover2 = false;
	
	void changePlayer(int t);
	void resetChoice(std::vector<Card*> cardsPlayer, int t, std::vector< std::pair<float, float> > card_pos);
	void controlRound(std::vector<Card*> cardsPlayer, int t, std::vector< std::pair<float, float> > card_pos,int phase);
	void autofillBlocks(std::vector<Card*> cardsPlayer);
	void populateBench(std::string cluster, std::vector<Card*> cardsPlayer, int t, std::vector<std::string> textures, std::vector< std::pair<float, float> > card_pos, std::vector<int> rank);
	void addEvent(Events* evt);
	void processEvent();
	void removeCard(int player, int winner);
	void spawnPlayer2();
	void updateStrategoScreen();
	void drawStrategoScreen();
	
public:
	void update();
	void draw();
	void init();
	Space();
	~Space();
};