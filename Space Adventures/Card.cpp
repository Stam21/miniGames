/**
 * File:    Card.cpp
 *
 * Author: George Stamatopoulos
 * Date:    30/01/2022
 * Partner:  No
 * Course:   Programming with C++
 *
 * Summary of File:
 *
 *   This file contains code that represent the cards and their interaction with the stratego game.
 */
#include "Card.h"
#include "config.h"

void Card::draw()
{

	if (!highlighted) SETCOLOR(brush.fill_color, 0.2f, 0.2f, 0.8f);
	brush.outline_opacity = 0.0f;
	brush.fill_opacity = opacity;
	graphics::drawRect(card_x, card_y, 100, 100, brush);
	SETCOLOR(brush.fill_color, 1.0f, 1.0f, 1.0f);
	brush.outline_opacity = 0.0f;
	brush.texture = std::string(ASSET_PATH) + texture;
	graphics::drawRect(card_x, card_y, size_x, size_y, brush);
	//NOTE: Hightlight available blocks for player 1 in phase 1.
	if (player == 1 && phase == 1 && moving == true)
	{
		graphics::Brush brush2;
		brush2.fill_opacity = 0.5f;
		for (int z = 0; z < 40; z++)
		{
			if (!std::count(unavBlocks.begin(), unavBlocks.end(), centers1[z]))
			{
					SETCOLOR(brush2.fill_color, 0.0f, 0.6f, 0.2f);
					brush2.outline_opacity = 0.6f;
					graphics::drawRect(centers1[z].first, centers1[z].second, 100, 100, brush2);
			}
		}
	}
	//NOTE: Hightlight available blocks for player 2 in phase 1.
	else if (player == 2 && phase == 1 && moving == true)
	{
		graphics::Brush brush2;
		brush2.fill_opacity = 0.5f;
		for (int z = 0; z < 40; z++)
		{
			if (!std::count(unavBlocks.begin(), unavBlocks.end(), centers2[z]))
			{
				SETCOLOR(brush2.fill_color, 0.0f, 0.6f, 0.2f);
				brush2.outline_opacity = 0.6f;
				graphics::drawRect(centers2[z].first, centers2[z].second, 100, 100, brush2);
			}
		}
	}
	//NOTE: Hightlight available blocks for player 1/2 in phase 2.
	else if (phase == 2 && mouse.cur_pos_x<= card_x + 55 && mouse.cur_pos_x >= card_x - 55 && mouse.cur_pos_y <= card_y + 56 && mouse.cur_pos_y >= card_y - 56 && highlighted)
	{
		graphics::Brush brush2;
		for (int z = 0; z < availBlocks.size(); z++)
		{
			
			brush2.outline_opacity = 0.6f;
			brush2.fill_opacity = 0.5f;
			if (std::count(attackBlocks.begin(), attackBlocks.end(), availBlocks[z]))
			{
				brush2.fill_opacity = 0.3f;
				SETCOLOR(brush2.fill_color, 1.0f, 0.0f, 0.0f);
			}
			else {
				SETCOLOR(brush2.fill_color, 0.0f, 0.6f, 0.2f);
			}
			graphics::drawRect(availBlocks[z].first, availBlocks[z].second, 100, 100, brush2);
		}
	}
	//NOTE: Draw explosion
	if (opacity < 1.0f)
	{
		graphics::Brush brush2;
		brush.fill_opacity = opacity + 0.5;
		brush2.outline_opacity = 0.0f;
		brush2.texture = explodeimg;
		graphics::drawRect(card_x, card_y, 500,500, brush2);
	}
}

void Card::update()
{
	
	graphics::getMouseState(mouse);

	if (mouse.cur_pos_x <= card_x + 44 && mouse.cur_pos_x >= card_x - 44 && mouse.cur_pos_y >= card_y - 44 && mouse.cur_pos_y <= card_y + 44 && (mouse.button_left_down || mouse.button_left_pressed) && moving ) {
		if (mouse.button_left_down && card_x <= WINDOW_WIDTH / 2 + 243 && card_x >= WINDOW_WIDTH / 2 - 880 && card_y >= WINDOW_HEIGHT / 2 + 126 && card_y <= WINDOW_HEIGHT / 2 + 547 && player == 1 && phase == 1)
		{
			placeCard(centers1);
		}
		else if (mouse.button_left_down && card_x <= WINDOW_WIDTH / 2 + 243 && card_x >= WINDOW_WIDTH / 2 - 880 && card_y >= WINDOW_HEIGHT / 2 - 540 && card_y <= WINDOW_HEIGHT / 2 - 119 && player == 2 && phase == 1)
		{
			placeCard(centers2);
		}
		else if (mouse.button_left_down && phase == 2 && card_x <= WINDOW_WIDTH / 2 + 243 && card_x >= WINDOW_WIDTH / 2 - 880 && card_y >= WINDOW_HEIGHT / 2 - 540 && card_y <= WINDOW_HEIGHT / 2 + 547)
		{
			placeCard(extraCenters);
		}
		else if (mouse.button_left_down)
		{
			card_x = initcard_x;
			card_y = initcard_y;
			control = false;
			moving = false;
			highlighted = false;

		}
	}
	//NOTE: When card is selected and has to follow the mouse.
	else if ((mouse.cur_pos_x <= card_x + 44 && mouse.cur_pos_x >= card_x - 44 && mouse.cur_pos_y >= card_y - 44 && mouse.cur_pos_y <= card_y + 44 && mouse.button_left_released || moving) && control){
			card_x = (float)mouse.cur_pos_x;
			card_y = (float)mouse.cur_pos_y;
			moving = true;
			highlighted = true;
		
	}
}

void Card::init()
{
}

float Card::getPos_X()
{
	return card_x;
}

float Card::getPos_Y()
{
	return card_y;
}

float Card::getInitPos_X()
{
	return initcard_x;
}

float Card::getInitPos_Y()
{
	return initcard_y;
}


/**
 * Return Value : vector: containing available blocks.
 *
 * Description:
 *
 *    This function finds all available blocks for a card.
 *
 */
std::vector< std::pair<float, float> > Card::getavailBlocks()
{
	availBlocks.clear();
	float counter = 111; //111 pixels up-down-left-right to the next block.
	//NOTE: flags indicate whether there are available blocks up-down-left-right of the card.
	bool flagup = false;
	bool flagdown = false;
	bool flagleft = false;
	bool flagright = false;
	if (rank == 2)
	{
		
		for (int i = 0; i < 10; i++)
		{
			
				if (!std::count(unavBlocks.begin(), unavBlocks.end(), std::make_pair(initcard_x + counter, initcard_y)) && !flagright && !(initcard_x + counter > WINDOW_WIDTH / 2 + 199))
				{
					if (std::count(attackBlocks.begin(), attackBlocks.end(), std::make_pair(initcard_x + counter, initcard_y)))
					{
						flagright = true;
						availBlocks.push_back(std::make_pair(initcard_x + counter, initcard_y));
					}

					availBlocks.push_back(std::make_pair(initcard_x + counter, initcard_y));
				}
				else {
					flagright = true;
				}
				if (!std::count(unavBlocks.begin(), unavBlocks.end(), std::make_pair(initcard_x, initcard_y + counter)) && !flagdown && !(initcard_y + counter > WINDOW_HEIGHT / 2 + 503))
				{
					if (std::count(attackBlocks.begin(), attackBlocks.end(), std::make_pair(initcard_x, initcard_y + counter)))
					{
						flagdown = true;
						availBlocks.push_back(std::make_pair(initcard_x, initcard_y + counter));
					}

					availBlocks.push_back(std::make_pair(initcard_x, initcard_y + counter));
				}
				else {
					flagdown = true;
				}
				if (!std::count(unavBlocks.begin(), unavBlocks.end(), std::make_pair(initcard_x, initcard_y - counter)) && !flagup && !(initcard_y - counter < WINDOW_HEIGHT / 2 - 496))
				{
					if (std::count(attackBlocks.begin(), attackBlocks.end(), std::make_pair(initcard_x, initcard_y - counter)))
					{
						flagup = true;
						availBlocks.push_back(std::make_pair(initcard_x, initcard_y - counter));
					}

					availBlocks.push_back(std::make_pair(initcard_x, initcard_y - counter));
				}
				else {
					flagup = true;
				}
				if (!std::count(unavBlocks.begin(), unavBlocks.end(), std::make_pair(initcard_x - counter, initcard_y)) && !flagleft && !(initcard_x - counter < WINDOW_WIDTH / 2 - 800))
				{
					if (std::count(attackBlocks.begin(), attackBlocks.end(), std::make_pair(initcard_x - counter, initcard_y)))
					{
						flagleft = true;
						availBlocks.push_back(std::make_pair(initcard_x - counter, initcard_y));
					}

					availBlocks.push_back(std::make_pair(initcard_x - counter, initcard_y));
				}
				else {
					flagleft = true;
				}
			
				counter += 111;
		}
	}
	else if (!(rank == 0 || rank == 11))
	{
		
			if (!std::count(unavBlocks.begin(), unavBlocks.end(), std::make_pair(initcard_x + counter, initcard_y)) && !flagright && !(initcard_x + counter > WINDOW_WIDTH / 2 + 199))
			{
				if (std::count(attackBlocks.begin(), attackBlocks.end(), std::make_pair(initcard_x + counter, initcard_y)))
				{
					flagright = true;
					availBlocks.push_back(std::make_pair(initcard_x + counter, initcard_y));
				}

				availBlocks.push_back(std::make_pair(initcard_x + counter, initcard_y));
			}
			else {
				flagright = true;
			}
			if (!std::count(unavBlocks.begin(), unavBlocks.end(), std::make_pair(initcard_x, initcard_y + counter)) && !flagdown && !(initcard_y + counter > WINDOW_HEIGHT / 2 + 503))
			{
				if (std::count(attackBlocks.begin(), attackBlocks.end(), std::make_pair(initcard_x, initcard_y + counter)))
				{
					flagdown = true;
					availBlocks.push_back(std::make_pair(initcard_x, initcard_y + counter));
				}

				availBlocks.push_back(std::make_pair(initcard_x, initcard_y + counter));
			}
			else {
				flagdown = true;
			}
			if (!std::count(unavBlocks.begin(), unavBlocks.end(), std::make_pair(initcard_x, initcard_y - counter)) && !flagup && !(initcard_y - counter < WINDOW_HEIGHT / 2 - 496))
			{
				if (std::count(attackBlocks.begin(), attackBlocks.end(), std::make_pair(initcard_x, initcard_y - counter)))
				{
					flagup = true;
					availBlocks.push_back(std::make_pair(initcard_x, initcard_y - counter));
				}

				availBlocks.push_back(std::make_pair(initcard_x, initcard_y - counter));
			}
			else {
				flagup = true;
			}
			if (!std::count(unavBlocks.begin(), unavBlocks.end(), std::make_pair(initcard_x - counter, initcard_y)) && !flagleft && !(initcard_x - counter < WINDOW_WIDTH / 2 - 800))
			{
				if (std::count(attackBlocks.begin(), attackBlocks.end(), std::make_pair(initcard_x - counter, initcard_y)))
				{
					flagleft = true;
					availBlocks.push_back(std::make_pair(initcard_x - counter, initcard_y));
				}

				availBlocks.push_back(std::make_pair(initcard_x - counter, initcard_y));
			}
			else {
				flagleft = true;
			}
	}
	else if ((rank == 0 || rank == 11) && phase == 2) {
			control = false;
	}
	return availBlocks;
}

bool Card::getState()
{
	return control;
}

int Card::getRank()
{
	return rank;
}

bool Card::getMoving()
{
	return moving;
}

bool Card::getExplosion()
{
	return explosion;
}

bool Card::getbattleMode()
{
	return attackMode;
}


/**
 * Parameters   : vector: containing all centers
 *
 * Return Value : Nothing.
 *
 * Description:
 *
 *    This function checks whether an available block is selected to place the card or not and finally places the card.
 *
 */
void Card::placeCard(std::vector< std::pair<float, float> > centers)
{
	float min_x = 1100;
	float min_y = 1110;
	int selectedCard_x = 0;
	int selectedCard_y = 0;
	//NOTE: Find the closest center in the place the card was released.
	for (int i = 0; i < centers.size(); i++)
	{
		float tempX = std::abs(card_x - centers[i].first);
		float tempY = std::abs(card_y - centers[i].second);
		if (tempX < min_x) {
			min_x = tempX;
			selectedCard_x = i;
		}
		if (tempY < min_y) {
			min_y = tempY;
			selectedCard_y = i;
		}
	}

	//NOTE: If closest center is available block then change position.
	if ((!std::count(availBlocks.begin(), availBlocks.end(), std::make_pair(centers[selectedCard_x].first, centers[selectedCard_y].second)) && phase == 2))
	{
		card_x = initcard_x;
		card_y = initcard_y;
		control = false;
		moving = false;
		highlighted = false;
	}
	else
	{
		card_x = centers[selectedCard_x].first;
		card_y = centers[selectedCard_y].second;
		highlighted = false;
		moving = false;
		control = false;
	}
}

void Card::setState(bool state)
{
	control = state;
}

void Card::setScale(float x, float y)
{
	size_x=  x;
	size_y = y;
}

void Card::setMoving(bool mov)
{
	moving = mov;
}

void Card::setOpacity(float opc)
{
	opacity = opc;
}

void Card::setExplosion(std::string expimg)
{
	explodeimg = expimg;
	explosion = true;
}

void Card::setbattleMode(bool atkMode)
{
	attackMode = atkMode;
}

void Card::autofillCenters(int x)
{
	if (player == 1 && (!std::count(unavBlocks.begin(), unavBlocks.end(), std::make_pair(centers1[x].first, centers1[x].second))))
	{
		card_x = centers1[x].first;
		card_y = centers1[x].second;
		moving = false;
		control = false;
	}
	else if (player == 2 && (!std::count(unavBlocks.begin(), unavBlocks.end(), std::make_pair(centers2[x].first, centers2[x].second))))
	{
		card_x = centers2[x].first;
		card_y = centers2[x].second;
		moving = false;
		control = false;
	}
}

void Card::setPlayer(int x)
{
	player = x;
}

void Card::setPos_X(float x)
{
	card_x = x;
}

void Card::setinitPos(float x, float y)
{
	initcard_x = x;
	initcard_y = y;
}

void Card::setTexture(std::string path)
{
	texture = path;
}

void Card::setPos_Y(float y)
{
	card_y = y;
}

void Card::setavailBlocks(std::vector< std::pair<float, float> > blocks)
{
	availBlocks = blocks;
}

void Card::setGamePhase(int phaset)
{
	phase = phaset;
}

void Card::setRank(int x)
{
	rank = x;
}

void Card::populateCenters()
{

	float centers_x[10] = { WINDOW_WIDTH / 2 + 199,WINDOW_WIDTH / 2 + 88,WINDOW_WIDTH / 2 - 23,WINDOW_WIDTH / 2 - 134,WINDOW_WIDTH / 2 - 245,WINDOW_WIDTH / 2 - 356,WINDOW_WIDTH / 2 - 467,WINDOW_WIDTH / 2 - 578,WINDOW_WIDTH / 2 - 689,WINDOW_WIDTH / 2 - 800 };
	float centers_y[10] = { WINDOW_HEIGHT / 2 + 503, WINDOW_HEIGHT / 2 + 392, WINDOW_HEIGHT / 2 + 281, WINDOW_HEIGHT / 2 + 170 ,  WINDOW_HEIGHT / 2 + 59 ,  WINDOW_HEIGHT / 2 - 52 , WINDOW_HEIGHT / 2 - 163, WINDOW_HEIGHT / 2 - 274, WINDOW_HEIGHT / 2 - 385, WINDOW_HEIGHT / 2 - 496 };
	if (player == 1)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int z = 0; z < 4; z++)
			{
				centers1.push_back(std::make_pair(centers_x[i], centers_y[z]));
				extraCenters.push_back(std::make_pair(centers_x[i], centers_y[z]));
			}
			for (int z = 4; z < 10; z++)
			{
				extraCenters.push_back(std::make_pair(centers_x[i], centers_y[z]));
			}

		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			for (int z = 6; z < 10; z++)
			{
				centers2.push_back(std::make_pair(centers_x[i], centers_y[z]));
				extraCenters.push_back(std::make_pair(centers_x[i], centers_y[z]));
			}

			for (int z = 0; z < 6; z++)
			{
				extraCenters.push_back(std::make_pair(centers_x[i], centers_y[z]));
			}
		}
	}
}

void Card::setunavBlocks(std::vector<std::pair<float, float>> unavBlockst)
{
	unavBlocks = unavBlockst;
}

void Card::setattackBlocks(std::vector<std::pair<float, float>> aBlocks)
{
	attackBlocks = aBlocks;
}

Card::Card()
{
	init();
}

Card::~Card()
{
}
