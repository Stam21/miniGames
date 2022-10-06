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

#include "Space.h"
#include "graphics.h"
#include "config.h"
#include "iostream"
#include <windows.h>
#include <algorithm>  //std::random_shuffle


void Space::updateStrategoScreen()
{			
			graphics::getMouseState(mouse);
			
			
			if (!cards1_initialized)
			{	
				populateBench("B", cardsPlayer1, 1, textures1, card1_pos, rank1);
				cards1_initialized = true;
			}
			//NOTE: If a card is removed do not update the others on the same round.
			if (player == 1 && !removedCard) 
			{
				if (phase1 == 1) {
					controlRound(cardsPlayer1, player, card1_pos, 1);
				}
				else {
					controlRound(cardsPlayer1, player, card1_pos, 2);
				}
				
			}
			else if (player == 2 && !removedCard)
			{
				if (phase2 == 1) {
					controlRound(cardsPlayer2, player, card2_pos, 1);
				}
				else {
					controlRound(cardsPlayer2, player, card2_pos, 2);
				}
			}

			//NOTE: If player has not available moves then opponent wins.
			if (availMoves && controlCard==0) {
				availMoves = false;
			}
			else if (!availMoves && controlCard == 0)
			{
				if (player == 1)
				{
					statusPl = win2;
				}
				else
				{
					statusPl = win1;
				}
			}

			//NOTE: If phase is 2 and a battle is initialized then call removeCard to handle the battle.
			if (phase2 == 2 && phase1 == 2)
			{
				if ((cardsPlayer2[target]->getbattleMode() || cardsPlayer2[target]->getExplosion()) && (cardsPlayer1[attacker]->getbattleMode() || cardsPlayer1[attacker]->getExplosion()) && attackMode && player == 1)
				{
					controlCard = 1;
					int activeCard = 100;
					if (rank2[target] == 0)
					{
						statusPl = win1;
					}
					else if (rank1[attacker] == 3 && rank2[target] == 11)
					{
						removeCard(1, 1);

					}
					else if (rank1[attacker] == 2 && rank2[target] == 10)
					{
						
						removeCard(1, 1);
					}
					else if (rank1[attacker] > rank2[target])
					{
						removeCard(1, 1);
					}
					else if (rank1[attacker] < rank2[target])
					{
						
						removeCard(1, 2);
					}
					else
					{
						removeCard(1, 1);
						removeCard(1, 2);
					}
					
				}if ((cardsPlayer1[target]->getbattleMode() || cardsPlayer1[target]->getExplosion()) && (cardsPlayer2[attacker]->getbattleMode() || cardsPlayer2[attacker]->getExplosion()) && attackMode && player == 2)
				{
					controlCard = 1;
					int activeCard = 100;
					if (rank1[target] == 0)
					{
						statusPl = win2;
					}
					else if (rank2[attacker] == 3 && rank1[target] == 11)
					{
						removeCard(2, 2);

					}
					else if (rank2[attacker] == 2 && rank1[target] == 10)
					{
						removeCard(2, 2);
					}
					else if (rank2[attacker] > rank1[target])
					{
						removeCard(2, 2);
					}
					else if (rank2[attacker] < rank1[target])
					{
						removeCard(2, 1);
					}
					else
					{
						removeCard(2, 2);
						removeCard(2, 1);
					}
				}
			}

			processEvent();
			
			if (mouse.button_left_pressed && mouse.cur_pos_x < WINDOW_WIDTH / 2 + 560 && mouse.cur_pos_x > WINDOW_WIDTH / 2 + 440 && mouse.cur_pos_y < WINDOW_HEIGHT / 2 + 435 && mouse.cur_pos_y > WINDOW_HEIGHT / 2 + 365 && move )
			{
				if (player == 2)
				{
					changePlayer(player);
					removedCard = false;
				}
				else
				{
					changePlayer(player);
					removedCard = false;
				}
			}
			else if (mouse.button_left_pressed && mouse.cur_pos_x < WINDOW_WIDTH / 2 + 760 && mouse.cur_pos_x > WINDOW_WIDTH / 2 + 640 && mouse.cur_pos_y < WINDOW_HEIGHT / 2 + 435 && mouse.cur_pos_y > WINDOW_HEIGHT / 2 + 365)
			{
				if (player == 1)
				{
					resetChoice(cardsPlayer1, player, card1_pos);
				}
				else
				{
					resetChoice(cardsPlayer2, player, card2_pos);
				}
				
			}
			else if (mouse.button_left_pressed && mouse.cur_pos_x < WINDOW_WIDTH / 2 + 660 && mouse.cur_pos_x > WINDOW_WIDTH / 2 + 540 && mouse.cur_pos_y < WINDOW_HEIGHT / 2 + 535 && mouse.cur_pos_y > WINDOW_HEIGHT / 2 + 465 && (phase1 == 1 || phase2 == 1))
			{
				if (player == 1)
				{
					autofillBlocks(cardsPlayer1);
				}
				else
				{
					autofillBlocks(cardsPlayer2);
				}
				
			}
			if (statusPl == win1)
			{
				gameover2 = true;
			}
			else if (statusPl == win2)
			{
				gameover1 = true;
			}
}

/**
 * Parameters   : integer: containing active player, integer: containing winner
 *
 * Return Value : Nothing.
 *
 * Description:
 *
 *    This function utilizes the fadeOutEvent, animating the destruction of the losing card and generally is responsisble for anything during and after the battle.
 *
 */
void Space::removeCard(int player, int winner)
{
	if (player == 1 && winner == 1)
	{
		if (!cardsPlayer2[target]->getExplosion()) {
			addEvent(new fadeOutEvent(std::string("red"), cardsPlayer2[target]));
		}

		if (!cardsPlayer2[target]->getbattleMode())
		{
			cardsPlayer1[attacker]->setPos_X(cardsPlayer2[target]->getInitPos_X());
			cardsPlayer1[attacker]->setPos_Y(cardsPlayer2[target]->getInitPos_Y());
			cardsPlayer1[attacker]->setScale(92, 92);
			cardsPlayer2[target]->~Card();
			cardsPlayer2.erase(cardsPlayer2.begin() + target);
			card2_pos.erase(card2_pos.begin() + target);
			rank2.erase(rank2.begin() + target);
			textures2.erase(textures2.begin() + target);
			attackMode = false;
			music = true;
			removedCard = true;
		}
	}
	else if (player == 1 && winner == 2)
	{
		if (!cardsPlayer1[attacker]->getExplosion()) {
			addEvent(new fadeOutEvent(std::string("blue"), cardsPlayer1[attacker]));
		}

		if (!cardsPlayer1[attacker]->getbattleMode())
		{
			cardsPlayer2[target]->setPos_X(cardsPlayer2[target]->getInitPos_X());
			cardsPlayer2[target]->setPos_Y(cardsPlayer2[target]->getInitPos_Y());
			cardsPlayer2[target]->setScale(92, 92);
			cardsPlayer1[attacker]->~Card();
			cardsPlayer1.erase(cardsPlayer1.begin() + attacker);
			card1_pos.erase(card1_pos.begin() + attacker);
			rank1.erase(rank1.begin() + attacker);
			textures1.erase(textures1.begin() + attacker);
			attackMode = false;
			music = true;
			removedCard = true;
		}
	}
	else if (player == 2 && winner == 2)
	{
		if (!cardsPlayer1[target]->getExplosion()) {
			addEvent(new fadeOutEvent(std::string("blue"), cardsPlayer1[target]));
		}

		if (!cardsPlayer1[target]->getbattleMode())
		{
			cardsPlayer2[attacker]->setPos_X(cardsPlayer1[target]->getInitPos_X());
			cardsPlayer2[attacker]->setPos_Y(cardsPlayer1[target]->getInitPos_Y());
			cardsPlayer2[attacker]->setScale(92, 92);
			cardsPlayer1[target]->~Card();
			cardsPlayer1.erase(cardsPlayer1.begin() + target);
			card1_pos.erase(card1_pos.begin() + target);
			rank1.erase(rank1.begin() + target);
			textures1.erase(textures1.begin() + target);
			attackMode = false;
			music = true;
			removedCard = true;
		}
	}
	else if (player == 2 && winner == 1)
	{
		if (!cardsPlayer2[attacker]->getExplosion()) {
			addEvent(new fadeOutEvent(std::string("red"), cardsPlayer2[attacker]));
		}

		if (!cardsPlayer2[attacker]->getbattleMode())
		{
			cardsPlayer1[target]->setPos_X(cardsPlayer1[target]->getInitPos_X());
			cardsPlayer1[target]->setPos_Y(cardsPlayer1[target]->getInitPos_Y());
			cardsPlayer1[target]->setScale(92, 92);
			cardsPlayer2[attacker]->~Card();
			cardsPlayer2.erase(cardsPlayer2.begin() + attacker);
			card2_pos.erase(card2_pos.begin() + attacker);
			rank2.erase(rank2.begin() + attacker);
			textures2.erase(textures2.begin() + attacker);
			attackMode = false;
			music = true;
			removedCard = true;
		}
	}	
}

//NOTE: This function spawns red cards.
void Space::spawnPlayer2()
{
	if (!cards2_initialized)
	{
		populateBench("R", cardsPlayer2, 2, textures2, card2_pos, rank2);
		cards2_initialized = true;
	}
}

/**
 * Parameters   : integer: containing active player
 *
 * Return Value : Nothing.
 *
 * Description:
 *
 *    This function represents the OK button and changes the active player.
 *
 */
void Space::changePlayer(int t)
{
	bool changeplayer = true;
	if (t == 1)
	{
		for (int i = 0; i < cardsPlayer1.size(); i++)
		{
			if ((cardsPlayer1[i]->getPos_X() == cardsPlayer1[i]->getInitPos_X()) && (cardsPlayer1[i]->getPos_Y() == cardsPlayer1[i]->getInitPos_Y() && phase1 == 1))
			{
				changeplayer = false;
			}
		}
		if (changeplayer)
		{
			player = 2;
			if (phase1 == 1) {
				spawnPlayer2();
				phase1 = 2;
			}
			for (int i = 0; i < cardsPlayer1.size(); i++)
			{
				cardsPlayer1[i]->setGamePhase(2);
				cardsPlayer1[i]->setState(true);
				card1_pos[i].first = cardsPlayer1[i]->getPos_X();
				card1_pos[i].second = cardsPlayer1[i]->getPos_Y();
				cardsPlayer1[i]->setinitPos(cardsPlayer1[i]->getPos_X(), cardsPlayer1[i]->getPos_Y());
			}
		}
	}
	else
	{
		
		for (int i = 0; i < cardsPlayer2.size(); i++)
		{
			if ((cardsPlayer2[i]->getPos_X() == cardsPlayer2[i]->getInitPos_X()) && (cardsPlayer2[i]->getPos_Y() == cardsPlayer2[i]->getInitPos_Y() && phase2 ==1))
			{
				changeplayer = false;
			}
		}
		if (changeplayer)
		{
			for (int i = 0; i < cardsPlayer2.size(); i++)
			{
				cardsPlayer2[i]->setGamePhase(2);
				cardsPlayer2[i]->setState(true);
				card2_pos[i].first = cardsPlayer2[i]->getPos_X();
				card2_pos[i].second = cardsPlayer2[i]->getPos_Y();
				cardsPlayer2[i]->setinitPos(cardsPlayer2[i]->getPos_X(), cardsPlayer2[i]->getPos_Y());
			}
			phase2 = 2;
			player = 1;
		}
	}
}

/**
 * Parameters   : vector: containing card objects, integer: containing active player, vector: containing cards' position
 *
 * Return Value : Nothing.
 *
 * Description:
 *
 *    This function represents the reset button and returns all cards on their initial position (Initial position may be changed every round). 
 *
 */
void Space::resetChoice(std::vector<Card*> cardsPlayer, int t, std::vector< std::pair<float, float> > card_pos)
{
	for (int i = 0; i < cardsPlayer.size(); i++)
	{
		controlCard = 0;
		card_pos[i].first = cardsPlayer[i]->getInitPos_X();
		card_pos[i].second = cardsPlayer[i]->getInitPos_Y();
		cardsPlayer[i]->setPos_X(card_pos[i].first);
		cardsPlayer[i]->setPos_Y(card_pos[i].second);
		cardsPlayer[i]->setinitPos(cardsPlayer[i]->getPos_X(), cardsPlayer[i]->getPos_Y());
		cardsPlayer[i]->setState(true);
	}
	if (t == 1)
	{
		card1_pos = card_pos;
	}
	else
	{
		card2_pos = card_pos;
	}
}

/**
  * Parameters   : vector: containing card objects, integer: containing active player, vector: containing cards' position, integer: containing active phase
  *
  * Return Value : Nothing.
  *
  * Description:
  *
  *    This function is the main operational function that controls all movements in the game, defines valid moves and attacking states.
  *
  */
void Space::controlRound(std::vector<Card*> cardsPlayer, int t, std::vector< std::pair<float, float> > card_pos ,int phase)
{

	//NOTE: Centers that cannot be used in the game. 
	card_pos.push_back(std::make_pair(WINDOW_WIDTH / 2 - 23, WINDOW_HEIGHT / 2 + 59));
	card_pos.push_back(std::make_pair(WINDOW_WIDTH / 2 - 134, WINDOW_HEIGHT / 2 + 59));
	card_pos.push_back(std::make_pair(WINDOW_WIDTH / 2 - 467, WINDOW_HEIGHT / 2 + 59));
	card_pos.push_back(std::make_pair(WINDOW_WIDTH / 2 - 578, WINDOW_HEIGHT / 2 + 59));
	card_pos.push_back(std::make_pair(WINDOW_WIDTH / 2 - 23, WINDOW_HEIGHT / 2 - 52));
	card_pos.push_back(std::make_pair(WINDOW_WIDTH / 2 - 134, WINDOW_HEIGHT / 2 - 52));
	card_pos.push_back(std::make_pair(WINDOW_WIDTH / 2 - 467, WINDOW_HEIGHT / 2 - 52));
	card_pos.push_back(std::make_pair(WINDOW_WIDTH / 2 - 578, WINDOW_HEIGHT / 2 - 52));
	for (int i = 0; i < cardsPlayer.size(); i++) {

		//NOTE: If a player can move a card and he click on it then show available blocks and set it active.
		if (controlCard < 1)
		{
			if (cardsPlayer[i]->getavailBlocks().size() > 0)
			{
				availMoves = true;
			}
			if (mouse.button_left_released)
			{
				std::vector< std::pair<float, float> > temp = cardsPlayer[i]->getavailBlocks();
				cardsPlayer[i]->setavailBlocks(temp);
				if (t == 1)
				{
					cardsPlayer[i]->setattackBlocks(card2_pos);
				}
				else
				{
					cardsPlayer[i]->setattackBlocks(card1_pos);
				}
				
				cardsPlayer[i]->update();
				cardsPlayer[i]->setState(true);
			}
			card_pos[i].first = cardsPlayer[i]->getPos_X();
			card_pos[i].second = cardsPlayer[i]->getPos_Y();
			
			if (cardsPlayer[i]->getMoving())
			{
				controlCard += 1;
				activeCard = i;
			}
		}
		//NOTE: If active card is not moving then check whether it attacked, returned to initial position or changed to another valid position. 
		else if (!cardsPlayer[i]->getMoving() && activeCard == i)
		{
			
			if (t == 1) {

				auto it = find(card2_pos.begin(), card2_pos.end(), std::make_pair(card_pos[i].first, card_pos[i].second));
				
				// If element was found
				if (it != card2_pos.end())
				{
					cardsPlayer[i]->setState(false);
					attackMode = true;
					controlCard = 1;
					int index = it - card2_pos.begin();
					cardsPlayer2[index]->setbattleMode(true);
					cardsPlayer[i]->setbattleMode(true);
					addEvent(new battleEvent(card2_pos[index].first, card2_pos[index].second, WINDOW_WIDTH / 2 + 200, WINDOW_HEIGHT / 2,WINDOW_WIDTH / 2 + 90, WINDOW_HEIGHT / 2, cardsPlayer2[index]));
					addEvent(new battleEvent(card_pos[i].first, card_pos[i].second, WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2 - 90, WINDOW_HEIGHT / 2,cardsPlayer[i]));
					target = index;
					attacker = i;
					graphics::stopMusic();
					graphics::playSound(std::string(ASSET_PATH) + "SecondGame/fightscene.mp3", 1.0f);
					
				}
				else if ((cardsPlayer[i]->getPos_X() != cardsPlayer[i]->getInitPos_X() ||  cardsPlayer[i]->getPos_Y() != cardsPlayer[i]->getInitPos_Y()) && phase == 2) {
					controlCard = 1;
				}
				else {
					controlCard = 0;
				}
				
			}
			else {
				auto it = find(card1_pos.begin(), card1_pos.end(), std::make_pair(card_pos[i].first, card_pos[i].second));

				// If element was found
				if (it != card1_pos.end())
				{
					cardsPlayer[i]->setState(false);
					attackMode = true;

					controlCard = 1;
					int index = it - card1_pos.begin();
					cardsPlayer1[index]->setbattleMode(true);
					cardsPlayer[i]->setbattleMode(true);
					addEvent(new battleEvent(card1_pos[index].first, card1_pos[index].second, WINDOW_WIDTH / 2 + 200, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2 + 90, WINDOW_HEIGHT / 2, cardsPlayer1[index]));
					addEvent(new battleEvent(card_pos[i].first, card_pos[i].second, WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2 - 90, WINDOW_HEIGHT / 2, cardsPlayer[i]));
					target = index;
					attacker = i;
					graphics::stopMusic();
					graphics::playSound(std::string(ASSET_PATH) + "SecondGame/fightscene.mp3", 1.0f);

				}
				else if ((cardsPlayer[i]->getPos_X() != cardsPlayer[i]->getInitPos_X() || cardsPlayer[i]->getPos_Y() != cardsPlayer[i]->getInitPos_Y()) && phase == 2) {
					controlCard = 1;
				}
				else {
					controlCard = 0;
				}
			}
		}
		else if (activeCard == i)
		{
			cardsPlayer[i]->update();		
			card_pos[i].first = cardsPlayer[i]->getPos_X();
			card_pos[i].second = cardsPlayer[i]->getPos_Y();
		}
		//NOTE: When player has not moved a card in phase 2, move should be false and therefore game cannot go to the next round.
		if (phase == 2) {
			move = false;
		}		
		for (int z = 0; z < cardsPlayer.size(); z++)
		{
			if ((cardsPlayer[z]->getPos_X() != cardsPlayer[z]->getInitPos_X() || cardsPlayer[z]->getPos_Y() != cardsPlayer[z]->getInitPos_Y()) && phase == 2) {
				move = true;
			}
			if (card_pos[z].first == card_pos[i].first && card_pos[z].second == card_pos[i].second && i != z)
			{
				card_pos[z].first = cardsPlayer[z]->getInitPos_X();
				card_pos[z].second = cardsPlayer[z]->getInitPos_Y();
				cardsPlayer[z]->setPos_X(card_pos[z].first);
				cardsPlayer[z]->setPos_Y(card_pos[z].second);
				cardsPlayer[z]->setState(true);
			}
			
		}
		card_pos[i].first = cardsPlayer[i]->getPos_X();
		card_pos[i].second = cardsPlayer[i]->getPos_Y();
		if (t == 1)
		{
			card1_pos = card_pos;
		}
		else
		{
			card2_pos = card_pos;
		}
		
		cardsPlayer[i]->setunavBlocks(card_pos);
	}
	
}

/**
 * Parameters   : vector: containing card objects
 *
 * Return Value : Nothing.
 *
 * Description:
 *
 *    This function represents the autofill button and gives a random position on the board to every card.
 *
 */
void Space::autofillBlocks(std::vector<Card*> cardsPlayer)
{
	int tempcountX = 0;
	int tempcountY = 0;
	std::vector<Card*> cards;
	std::vector< std::pair<float, float> > unavBlocks;
	std::random_shuffle(cardsPlayer.begin(), cardsPlayer.end());
	//NOTE: Check which cards already are on the board.
	for (auto card: cardsPlayer)
	{
		if (card->getPos_X() <= WINDOW_WIDTH / 2 + 243 && card->getPos_X() >= WINDOW_WIDTH / 2 - 880 && card->getPos_Y() >= WINDOW_HEIGHT / 2 - 540 && card->getPos_Y() <= WINDOW_HEIGHT / 2 + 547)
		{
			unavBlocks.push_back(std::make_pair(card->getPos_X(), card->getPos_Y()));
			
		}
		else {
			cards.push_back(cardsPlayer[tempcountX]);	
		}
		tempcountX += 1;
	}
	//NOTE: Give all the cards that are not on the board a random position.
	while (cards.size() > 0)
	{
			tempcountX = 0;
			cards[tempcountY]->setunavBlocks(unavBlocks);
			while (cards[tempcountY]->getPos_X() == cards[tempcountY]->getInitPos_X() && cards[tempcountY]->getPos_Y() == cards[tempcountY]->getInitPos_Y())
			{
				cards[tempcountY]->autofillCenters(tempcountX);
				tempcountX += 1;
			}
			
			unavBlocks.push_back(std::make_pair(cards[tempcountY]->getPos_X(), cards[tempcountY]->getPos_Y()));
			cards.erase(cards.begin()); 
	}
}

/**
 * Parameters   : string: containing letter for blue or red , vector: containing card objects, vector: containing strings of paths, vector: containing cards' position, vector: containing cards' rank 
 *
 * Return Value : Nothing.
 *
 * Description:
 *
 *    This function populate vectors with all necessary information to display and update the cards.
 *
 */
void Space::populateBench(std::string cluster, std::vector<Card*> cardsPlayer , int t, std::vector<std::string> textures, std::vector< std::pair<float, float> > card_pos, std::vector<int> rank)
{
	float initPosX = WINDOW_WIDTH / 2 + 360;
	float initPosY = WINDOW_HEIGHT / 2 - 240;
	for (int i = 0; i < 40; i++) {
		if (i == 0) {
			card = new Card();
			cardsPlayer.push_back(card);
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "0.png");
			rank.push_back(0);
			card->setRank(0);
			card->setPos_X(initPosX);
			card->setPos_Y(initPosY);
			card->setinitPos(initPosX, initPosY);
			card_pos.push_back(std::make_pair(initPosX, initPosY));
		}
		else if (i == 1) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "1.png");
			rank.push_back(1);
			card->setRank(1);
			cardsPlayer.push_back(card);
			card->setPos_X(initPosX + 120.f);
			card->setPos_Y(initPosY);
			card->setinitPos(initPosX + 120.f, initPosY);
			card_pos.push_back(std::make_pair(initPosX + 120.f, initPosY));
		}
		else if (i < 10) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "2.png");
			rank.push_back(2);
			card->setRank(2);
			cardsPlayer.push_back(card);
			card->setPos_X(initPosX + 240.f);
			card->setPos_Y(initPosY);
			card->setinitPos(initPosX + 240.f, initPosY);
			card_pos.push_back(std::make_pair(initPosX + 240.f, initPosY));
		}
		else if (i < 15) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			cardsPlayer.push_back(card);
			textures.push_back("SecondGame/" + cluster + "3.png");
			rank.push_back(3);
			card->setRank(3);
			card->setPos_X(initPosX + 360.f);
			card->setPos_Y(initPosY);
			card->setinitPos(initPosX + 360.f, initPosY);
			card_pos.push_back(std::make_pair(initPosX + 360.f, initPosY));
		}
		else if (i < 19) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "4.png");
			rank.push_back(4);
			card->setRank(4);
			cardsPlayer.push_back(card);
			card->setPos_X(initPosX + 480.f);
			card->setPos_Y(initPosY);
			card->setinitPos(initPosX + 480.f, initPosY);
			card_pos.push_back(std::make_pair(initPosX + 480.f, initPosY));
		}
		else if (i < 23) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "5.png");
			rank.push_back(5);
			card->setRank(5);
			cardsPlayer.push_back(card);
			card->setPos_X(initPosX);
			card->setPos_Y(initPosY + 120.f); 
			card->setinitPos(initPosX, initPosY + 120.f);
			card_pos.push_back(std::make_pair(initPosX, initPosY + 120.f));
		}
		else if (i < 27) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "6.png");
			rank.push_back(6);
			card->setRank(6);
			cardsPlayer.push_back(card);
			card->setPos_X(initPosX + 120.f);
			card->setPos_Y(initPosY + 120.f);
			card->setinitPos(initPosX + 120.f, initPosY + 120.f);
			card_pos.push_back(std::make_pair(initPosX + 120.f, initPosY + 120.f));
		}
		else if (i < 30) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "7.png");
			rank.push_back(7);
			card->setRank(7);
			cardsPlayer.push_back(card);
			card->setPos_X(initPosX + 240.f);
			card->setPos_Y(initPosY + 120.f);
			card->setinitPos(initPosX + 240.f, initPosY + 120.f);
			card_pos.push_back(std::make_pair(initPosX + 240.f, initPosY + 120.f));
		}
		else if (i < 32) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "8.png");
			rank.push_back(8);
			card->setRank(8);
			cardsPlayer.push_back(card);
			card->setPos_X(initPosX + 360.f);
			card->setPos_Y(initPosY + 120.f);
			card->setinitPos(initPosX + 360.f, initPosY + 120.f);
			card_pos.push_back(std::make_pair(initPosX + 360.f, initPosY + 120.f));
		}
		else if (i < 33) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "9.png");
			rank.push_back(9);
			card->setRank(9);
			cardsPlayer.push_back(card);
			card->setPos_X(initPosX + 480.f);
			card->setPos_Y(initPosY + 120.f);
			card->setinitPos(initPosX + 480.f, initPosY + 120.f);
			card_pos.push_back(std::make_pair(initPosX + 480.f, initPosY + 120.f));
		}
		else if (i < 34) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "10.png");
			rank.push_back(10);
			card->setRank(10);
			cardsPlayer.push_back(card);
			card->setPos_X(initPosX);
			card->setPos_Y(initPosY + 240.f);
			card->setinitPos(initPosX, initPosY + 240.f);
			card_pos.push_back(std::make_pair(initPosX, initPosY + 240.f));
		}
		else if (i < 40) {
			card = new Card();
			card->setState(true);
			card->setPlayer(t);
			textures.push_back("SecondGame/" + cluster + "11.png");
			rank.push_back(11);
			card->setRank(11);
			cardsPlayer.push_back(card);
			card->setPos_X(initPosX + 120.f);
			card->setPos_Y(initPosY + 240.f);
			card->setinitPos(initPosX + 120.f, initPosY + 240.f);
			card_pos.push_back(std::make_pair(initPosX + 120.f, initPosY + 240.f));
		}
		cardsPlayer[i]->populateCenters();
	}
	if (t == 1)
	{
		textures1 = textures;
		cardsPlayer1 = cardsPlayer;
		card1_pos = card_pos;
		rank1 = rank;
	}
	else
	{
		textures2 = textures;
		cardsPlayer2 = cardsPlayer;
		card2_pos = card_pos;
		rank2 = rank;
	}
}

/**
 * Parameters   : list: containing Event objects.
 *
 * Return Value : Nothing.
 *
 * Description:
 *
 *    This function updates the events list with new events.
 *
 */
void Space::addEvent(Events * evt)
{
	events.push_front(evt);
}


/**
 * Return Value : Nothing.
 *
 * Description:
 *
 *    This function updates all active events and keeps track of finished events.
 *
 */
void Space::processEvent()
{
	for (auto e : events)
	{
		e->update();
	}
	events.remove_if([](Events* ev) {return !ev->active(); });
}

void Space::drawStrategoScreen()
{
	graphics::Brush br;
	graphics::Brush bru;
	graphics::Brush brus;
	graphics::Brush brush;
	graphics::Brush brush1;
	graphics::Brush brush2;
	graphics::Brush brush3;
	graphics::Brush brush4;

	brus.texture = std::string(ASSET_PATH) + "SecondGame/soundbtn.png";
	bru.texture = std::string(ASSET_PATH) + "SecondGame/goback_btn.png";
	br.texture = std::string(ASSET_PATH) + "SecondGame/background3.png";
	brush.texture = std::string(ASSET_PATH) + "SecondGame/bench.png";
	brush1.texture = std::string(ASSET_PATH) + "SecondGame/OkBTN1.png";
	brush2.texture = std::string(ASSET_PATH) + "SecondGame/ResetBTN1.png";
	brush3.texture = std::string(ASSET_PATH) + "SecondGame/ResetBTN2.png";
	graphics::setFont(std::string(ASSET_PATH) + "Mitr-Bold.ttf");
	SETCOLOR(brush3.fill_color, 1.0f, 1.0f, 0.0f);
	brus.outline_opacity = 0.0f;
	bru.outline_opacity = 0.0f;
	br.outline_opacity = 0.0f;
	brush.outline_opacity = 1.0f;
	brush1.outline_opacity = 0.0f;
	brush2.outline_opacity = 0.0f;
	brush3.outline_opacity = 0.0f;
	brush3.fill_opacity = 0.8;
	//draw background
	graphics::drawRect(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1920, 1080, br);
	graphics::drawRect(WINDOW_WIDTH / 2 + 600, WINDOW_HEIGHT / 2, 600, 600, brush);
	graphics::drawRect(WINDOW_WIDTH / 2 + 500, WINDOW_HEIGHT / 2 + 400, 120, 70, brush1);
	graphics::drawRect(WINDOW_WIDTH / 2 + 700, WINDOW_HEIGHT / 2 + 400, 120, 70, brush2);
	graphics::drawRect(WINDOW_WIDTH / 2 + 600, WINDOW_HEIGHT / 2 + 500, 120, 70, brush3); 
	graphics::drawText(WINDOW_WIDTH / 2 + 550, WINDOW_HEIGHT / 2 + 505, 25, std::string("Autofill"), brush3);
	graphics::drawRect(WINDOW_WIDTH / 2 - 900, WINDOW_HEIGHT / 2 - 500, 60, 70, bru);
	graphics::drawRect(WINDOW_WIDTH / 2 + 900, WINDOW_HEIGHT / 2 + 500, 56, 35, brus);

	if (cards1_initialized && cards2_initialized)
	{
			//NOTE: Draw Player's cards and hide the opponent's. Exception is the card in a battle.
			if (player == 2)
			{
				for (int x = 0; x < cardsPlayer1.size(); x++) {
					if (x == target && attackMode)
					{
						cardsPlayer1[x]->setTexture(textures1[x]);
						cardsPlayer1[x]->draw();
					}
					else {
						cardsPlayer1[x]->setTexture("SecondGame/BBG.png");
						cardsPlayer1[x]->draw();
					}
				}
				for (int x = 0; x < cardsPlayer2.size(); x++)
				{
					cardsPlayer2[x]->setTexture(textures2[x]);
					cardsPlayer2[x]->draw();
				}
			}
			else {
				for (int x = 0; x < cardsPlayer2.size(); x++) {
					if (x == target && attackMode)
					{
						cardsPlayer2[x]->setTexture(textures2[x]);
						cardsPlayer2[x]->draw();
					}
					else {
						cardsPlayer2[x]->setTexture("SecondGame/RBG.png");
						cardsPlayer2[x]->draw();
					}
				}
				for (int x = 0; x < cardsPlayer1.size(); x++)
				{
					cardsPlayer1[x]->setTexture(textures1[x]);
					cardsPlayer1[x]->draw();
				}
			}
	}
	else
	{
		for (int i = 0; i < cardsPlayer1.size(); i++) {
				cardsPlayer1[i]->setTexture(textures1[i]);
				cardsPlayer1[i]->draw();
		}	
	}
	
	if (gameover1) {
		graphics::Brush brush5;
		brush5.texture = std::string(ASSET_PATH) + "SecondGame/gameover.png";
		brush5.outline_opacity = 0.0f;
		SETCOLOR(brush.fill_color, 0.0f, 0.0f, 0.0f);
		graphics::drawRect(WINDOW_WIDTH / 2 + 600, WINDOW_HEIGHT / 2, 600, 600, brush);
		graphics::drawRect(WINDOW_WIDTH / 2 + 600, WINDOW_HEIGHT / 2, 780, 635, brush5);
		SETCOLOR(brush5.fill_color, 1.0f, 0.0f, 0.0f);
		graphics::drawText(WINDOW_WIDTH / 2 + 450, WINDOW_HEIGHT / 2 - 100, 60, std::string("RED WINS"), brush5);
		
	}
	else if (gameover2) {
		graphics::Brush brush5;
		brush5.texture = std::string(ASSET_PATH) + "SecondGame/gameover.png";
		brush5.outline_opacity = 0.0f;
		SETCOLOR(brush.fill_color, 0.0f, 0.0f, 0.0f);
		graphics::drawRect(WINDOW_WIDTH / 2 + 600, WINDOW_HEIGHT / 2, 600, 600, brush);
		graphics::drawRect(WINDOW_WIDTH / 2 + 600, WINDOW_HEIGHT / 2, 780, 635, brush5);
		SETCOLOR(brush5.fill_color, 0.0f, 0.0f, 1.0f);
		graphics::drawText(WINDOW_WIDTH / 2 + 450, WINDOW_HEIGHT / 2 - 100, 60, std::string("BLUE WINS"), brush5);
		
	}
	
}

/*
-----------------------------------------------------------------------------------------------------------------------------
*/
void Space::updateSpaceScreen()
{
	//NOTE: The same functionality works with 2 players as well.
	if (statusp == ONE_PLAYER)
	{
		if (!player1_initialized && graphics::getGlobalTime() > 1000)
		{
			player1 = new Player();
			player1_initialized = true;
			//NOTE: A vector of blue enemies can be added in every level.
			enemyBlue = new Enemy();
			enemyShot = true;

		}
		enemyBlue->update();

		player1->update();

		
		if (graphics::getKeyState(graphics::SCANCODE_SPACE) && reduceFireRate <= 1)
		{
			//NOTE: Check if the bullet pool is empty.
			if (shots.size() > 30) {
				shots.clear();
				shot_pos.clear();
			}
			bullet = new Shot();
			bullet->setbulletState(true);
			bullet->setPos_X(player1->getPos_X());
			bullet->setPos_Y(player1->getPos_Y() - 30);
			bullet->setShotInit(true);
			shots.push_back(bullet);
			shot_pos.push_back(std::make_pair(bullet->getPos_X(), bullet->getPos_Y()));
			reduceFireRate = reduceFireRate + 1;

		}
		//NOTE: Update position of all active bullets.
		for (int i = 0; i < shots.size(); i++) {

			shots[i]->update(); 
			shot_pos[i].first = shots[i]->getPos_X();
			shot_pos[i].second = shots[i]->getPos_Y();
		}
		for (int i = 0; i < shots_enemy.size(); i++) {

			shots_enemy[i]->update();
		}
		//NOTE: Change direction of enemies' bullets based enemies' position.
		if (enemyBlue->getPos_Y() > WINDOW_HEIGHT / 2 - 300) {
			if (enemyBlue->getDirection() >= 170) {
				enemyBlue->setDirection(enemyBlue->getDirection());
			}
			else if (enemyBlue->getDirection() >= 70) {
				enemyBlue->setDirection(enemyBlue->getDirection() + 0.4f);
				enemyShot = false;
			}
			else {
				enemyBlue->setDirection(enemyBlue->getDirection() + 0.4f);
			}
		}
		//NOTE: Control enemies' spray frequency.
		if ((controlEnemyFireRate <= 1) && (enemyShot)) {
			bullet = new Shot();
			bullet->setbulletState(false);
			bullet->setbulletDirection(enemyBlue->getDirection() + 180.0f);
			bullet->setPos_X(enemyBlue->getPos_X());
			bullet->setPos_Y(enemyBlue->getPos_Y() + 50);
			bullet->setShotInit(true);
			shots_enemy.push_back(bullet);
			controlEnemyFireRate = controlEnemyFireRate + 1;
		}
		controlEnemyFireRate += 1;
		reduceFireRate = reduceFireRate + 1;
		if (reduceFireRate > 14) {
			reduceFireRate = 0;
		}
		if (controlEnemyFireRate > 45) {
			controlEnemyFireRate = 0;
		}
	}
	else if (statusp == TWO_PLAYER)
	{
		if (!player1_initialized && !player2_initialized && graphics::getGlobalTime() > 1000)
		{
			player1 = new Player();
			player2 = new Player();
			player1_initialized = true;
			player2_initialized = true;
		}
		player1->update();
		player2->updateSecond();
	}
}

void Space::drawSpaceScreen()
{
	graphics::Brush br;
	graphics::Brush bru;
	graphics::Brush brus;
	br.texture = std::string(ASSET_PATH) + "background.png";
	brus.texture = std::string(ASSET_PATH) + "SecondGame/soundbtn.png";
	bru.texture = std::string(ASSET_PATH) + "SecondGame/goback_btn.png";
	br.outline_opacity = 0.0f;
	brus.outline_opacity = 0.0f;
	bru.outline_opacity = 0.0f;
	//draw background
	graphics::drawRect(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT, br);
	graphics::drawRect(WINDOW_WIDTH / 2 - 900, WINDOW_HEIGHT / 2 - 500, 60, 70, bru);
	graphics::drawRect(WINDOW_WIDTH / 2 + 900, WINDOW_HEIGHT / 2 + 500, 56, 35, brus);
	//draw player
	if (statusp == ONE_PLAYER)
	{
		player1->draw();
		enemyBlue->draw();
	}
	else if (statusp == TWO_PLAYER && player2)
	{
		player1->drawFirst();
		player2->drawSecond();
	}
	for (int i = 0; i < shots.size(); i++) {
		shots[i]->setPos_X(shot_pos[i].first);
		shots[i]->setPos_Y(shot_pos[i].second);
		shots[i]->draw();
	}
	for (int i = 0; i < shots_enemy.size(); i++) {
		shots_enemy[i]->draw();
	}

}
/*
--------------------------------------------------------------------------------------------------------
*/
void Space::drawStartScreen()
{
	graphics::Brush br;
	if (status == STATUS_LOADING)
	{
		graphics::setFont(std::string(ASSET_PATH) + "Amatic-Bold.ttf");
		br.fill_color[0] = 1.0f;
		graphics::drawText(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2, 60, std::string("LOADING..."), br);

	}
	else {

		graphics::setFont(std::string(ASSET_PATH) + "Amatic-Bold.ttf");
		br.fill_color[0] = 1.0f;
		graphics::drawText(WINDOW_WIDTH / 2 - 350, WINDOW_HEIGHT / 2, 60, std::string("Press ENTER to navigate on MINI-GAMES "), br);
	}
}

void Space::drawGamesScreen()
{
	graphics::Brush br;
	graphics::Brush bru;
	graphics::Brush brus;
	graphics::Brush brush;
	brus.texture = std::string(ASSET_PATH) + "SecondGame/soundbtn.png";
	bru.texture = std::string(ASSET_PATH) + "SecondGame/goback_btn.png";
	brush.texture = std::string(ASSET_PATH) + "SecondGame/starwarsss.png";
	graphics::setFont(std::string(ASSET_PATH) + "Amatic-Bold.ttf");
	brus.outline_opacity = 0.0f;
	bru.outline_opacity = 0.0f;
	graphics::drawText(WINDOW_WIDTH / 2 + 350, WINDOW_HEIGHT/2,100, std::string("SPACE-ODYSSEY"), br);
	graphics::drawRect(WINDOW_WIDTH / 2 - 500, WINDOW_HEIGHT / 2, 1000, 1080, brush);
	graphics::drawRect(WINDOW_WIDTH / 2 - 900, WINDOW_HEIGHT/2 - 500, 60, 70, bru);
	graphics::drawRect(WINDOW_WIDTH / 2 + 900, WINDOW_HEIGHT / 2 + 500, 56, 35, brus);

}

void Space::drawPlayersScreen()
{
	graphics::Brush br;
	graphics::Brush bru;
	graphics::Brush brus;
	graphics::setFont(std::string(ASSET_PATH) + "Amatic-Bold.ttf");
	brus.texture = std::string(ASSET_PATH) + "SecondGame/soundbtn.png";
	bru.texture = std::string(ASSET_PATH) + "SecondGame/goback_btn.png";
	br.fill_color[0] = 1.0f;
	brus.outline_opacity = 0.0f;
	bru.outline_opacity = 0.0f;
	if (statusg == STRATEGO)
	{
		br.texture = std::string(ASSET_PATH) + "SecondGame/instructions.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(WINDOW_WIDTH / 2 , WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT, br);
		graphics::drawText(WINDOW_WIDTH / 2 - 100, 100, 30, std::string("Instructions"), br);
	}
	else {
		graphics::drawLine(WINDOW_WIDTH / 2, WINDOW_HEIGHT, WINDOW_WIDTH / 2, 0, br);
		graphics::drawText(WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT / 2, 30, std::string("1 Player"), br);
		graphics::drawText(WINDOW_WIDTH / 2 + 300, WINDOW_HEIGHT / 2, 30, std::string("2 Players"), br);
	}
	graphics::drawRect(WINDOW_WIDTH / 2 - 900, WINDOW_HEIGHT / 2 - 500, 60, 70, bru);
	graphics::drawRect(WINDOW_WIDTH / 2 + 900, WINDOW_HEIGHT / 2 + 500, 56, 35, brus);
}

void Space::drawPlayingScreen()
{
	if (player1)
	{
		drawSpaceScreen();
	}
	if (cards1_initialized)
	{
		drawStrategoScreen();
	}

}

/*
------------------------------------------------------------------------------------------------------------------------------
*/

void Space::updateStartScreen()
{
	if (status == STATUS_LOADING)
	{
		init();
		status = STATUS_START;
	}
	if (graphics::getKeyState(graphics::SCANCODE_RETURN) && status==STATUS_START)
	{
		status = STATUS_GAMES;
	}
}

void Space::updateGamesScreen()
{
	graphics::getMouseState(mouse);
	if (mouse.button_left_pressed)
	{
		if (mouse.cur_pos_x > (WINDOW_WIDTH / 2 + 10) && mouse.cur_pos_x < 1520 && mouse.cur_pos_y < ((WINDOW_HEIGHT / 2)) + 100 && mouse.cur_pos_y >((WINDOW_HEIGHT / 2) - 200))
		{
			statusg = SPACE;
			status = STATUS_PLAYERS;
		}
		else if (mouse.cur_pos_x < ((WINDOW_WIDTH / 2) - 10) && mouse.cur_pos_x > 20 && mouse.cur_pos_y < ((WINDOW_HEIGHT / 2) + 100) && mouse.cur_pos_y >((WINDOW_HEIGHT / 2) - 200))
		{
			statusg = STRATEGO;
			status = STATUS_PLAYERS;
		}
		else if ((mouse.cur_pos_x >=1464)  && (mouse.cur_pos_x <= 1510) && (mouse.cur_pos_y <= 850)  && (mouse.cur_pos_y >= 810) )
		{
			status = STATUS_PLAYERS;
		}
		else if ((mouse.cur_pos_x > 20) && (mouse.cur_pos_x < 70) && (mouse.cur_pos_y <= 60) && (mouse.cur_pos_y > 0) )
		{
			status = STATUS_START;
		}
	}

}

void Space::updatePlayersScreen()
{
	graphics::getMouseState(mouse);
	
	if (mouse.button_left_pressed)
	{
		
		if (mouse.cur_pos_x > (WINDOW_WIDTH / 2 + 10) && mouse.cur_pos_x < 1520 && mouse.cur_pos_y < ((WINDOW_HEIGHT / 2)) + 100 && mouse.cur_pos_y >((WINDOW_HEIGHT / 2) - 200) && statusg == SPACE)
		{
			statusp = TWO_PLAYER;
			status = STATUS_PLAYING;
		}
		else if (mouse.cur_pos_x < ((WINDOW_WIDTH / 2) - 10) && mouse.cur_pos_x > 20 && mouse.cur_pos_y < ((WINDOW_HEIGHT / 2) + 100) && mouse.cur_pos_y >((WINDOW_HEIGHT / 2) - 200) && statusg == SPACE)
		{
			statusp = ONE_PLAYER;
			status = STATUS_PLAYING;
		}
		else if ((mouse.cur_pos_x >= 1830) && (mouse.cur_pos_x <= 1890) && (mouse.cur_pos_y <= 1060) && (mouse.cur_pos_y >= 1020))
		{
			status = STATUS_PLAYERS;
		}
		else if ((mouse.cur_pos_x > 20) && (mouse.cur_pos_x < 70) && (mouse.cur_pos_y <= 60) && (mouse.cur_pos_y > 0))
		{
			status = STATUS_GAMES;
		}
	}
	else if (graphics::getKeyState(graphics::SCANCODE_RETURN) && statusg == STRATEGO)
	{
		status = STATUS_PLAYING;
	}
}

void Space::updatePlayingScreen()
{
	graphics::getMouseState(mouse);
	if (statusg == SPACE)
	{
		updateSpaceScreen();
	}
	else if (statusg == STRATEGO)
	{
		if (music == true)
		{
			graphics::playMusic(std::string(ASSET_PATH) + "SecondGame/starwarstheme.mp3", volume);
			music = false;
			if (musicActiv) {
				musicActiv = false;
			}
			else {
				musicActiv = true;
			}
		}
		updateStrategoScreen();
	}
	if (mouse.button_left_pressed)
	{
		if ((mouse.cur_pos_x >= 1830) && (mouse.cur_pos_x <= 1890) && (mouse.cur_pos_y <= 1060) && (mouse.cur_pos_y >= 1020))
		{
			if (musicActiv == false)
			{
				volume = 0;
				music = true;
			}
			else
			{
				volume = 0.7;
				music = true;
				musicActiv = true;
			}
		}
		else if ((mouse.cur_pos_x > 20) && (mouse.cur_pos_x < 70) && (mouse.cur_pos_y <= 60) && (mouse.cur_pos_y > 0))
		{
			//NOTE: Reset all states and destroy assets.
			graphics::stopMusic();
			status = STATUS_PLAYERS;
			shots.clear();
			enemyBlue = nullptr;
			player1 = nullptr;
			player2 = nullptr;
			player1_initialized = false;
			player2_initialized = false;
			for (auto card : cardsPlayer1) {
				card->~Card();
			}
			card1_pos.clear();
			cardsPlayer1.clear();
			textures1.clear();
			rank1.clear();
			card2_pos.clear();
			textures2.clear();
			rank2.clear();

			for (auto card : cardsPlayer2) {
				card->~Card();
			}
			cardsPlayer2.clear();
			move = true;
			music = true;
			cards1_initialized = false;
			cards2_initialized = false;
			phase1 = 1;
			phase2 = 1;
			player = 1;
				


		}
	}
}

/*
---------------------------------------------------------------------------------------------------------
*/

void Space::update()
{
	if (status == STATUS_START || status == STATUS_LOADING)
	{
		updateStartScreen();
	}
	else if (status == STATUS_GAMES)
	{
		updateGamesScreen();
	}
	else if (status == STATUS_PLAYERS)
	{
		updatePlayersScreen();
	}
	else
	{
		updatePlayingScreen();
	}
	    
}

void Space::draw()
{
	if (status == STATUS_START || status == STATUS_LOADING)
	{
		drawStartScreen();
	}
	else if (status == STATUS_GAMES)
	{
		drawGamesScreen();
	}
	else if (status == STATUS_PLAYERS)
	{
		drawPlayersScreen();
	}
	else
	{
		drawPlayingScreen();
	}
}

void Space::init()
{
	graphics::preloadBitmaps(std::string(ASSET_PATH) + "SecondGame");
}

Space::Space()
{
}

Space::~Space()
{
	if (player1)
	{
		delete player1;
	}
	if (player2)
	{
		delete player2;
	}
	if (enemyBlue)
	{
		delete enemyBlue;
	}
	if (enemyRed)
	{
		delete enemyRed;
	}
	if (card)
	{
		delete card;
	}
}
