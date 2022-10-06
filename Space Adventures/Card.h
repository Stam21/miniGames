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
#pragma once
#include "games.h"
#include "graphics.h"
#include "config.h"
#include <vector>
#include <algorithm>

class Card : public Games
{
	std::string texture;
	graphics::MouseState mouse;
	graphics::Brush brush;
	float card_x;
	float card_y;
	float size_x = 92;
	float size_y = 92;
	float initcard_x;
	float initcard_y;
	float opacity = 1.0;
	bool moving = false;
	bool control = true;
	bool placed = false;
	bool attackMode = false;
	bool explosion = false;
	int player;
	int phase = 1;
	int rank;
	bool highlighted = false;
	std::string explodeimg;
	std::vector< std::pair<float, float> > unavBlocks;
	std::vector< std::pair<float, float> > blocks;
	std::vector< std::pair<float, float> > availBlocks;
	std::vector< std::pair<float, float> > attackBlocks;
	std::vector< std::pair<float, float> > centers1;
	std::vector< std::pair<float, float> > centers2;
	std::vector< std::pair<float, float> > extraCenters;
public:
	void draw() override;
	void update() override;
	void init() override;
	float getPos_X();
	float getPos_Y();
	float getInitPos_X();
	std::vector< std::pair<float, float> > getavailBlocks();
	float getInitPos_Y();
	bool getState();
	int getRank();
	bool getMoving();
	bool getExplosion();
	bool getbattleMode();
	void placeCard(std::vector< std::pair<float, float> > centers);
	void setState(bool state);
	//void setRuleSet(pieceType type);
	void autofillCenters(int x);
	void setPlayer(int x);
	void setRank(int x);
	void setPos_X(float x);
	void setTexture(std::string path);
	void setPos_Y(float y);
	void setScale(float x, float y);
	void setinitPos(float x, float y);
	void setMoving(bool mov);
	void setOpacity(float opc);
	void setExplosion(std::string explodeimg);
	void setbattleMode(bool atkMode);
	void setGamePhase(int phaset);
	void populateCenters();
	void setunavBlocks(std::vector< std::pair<float, float> > unavBlockst);
	void setavailBlocks(std::vector< std::pair<float, float> > blocks);
	void setattackBlocks(std::vector< std::pair<float, float> > ablocks);
	Card();
	~Card();
};
