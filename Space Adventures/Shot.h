/**
 * File:    Shot.cpp
 *
 * Author: George Stamatopoulos
 * Date:    30/01/2022
 * Partner:  No
 * Course:   Programming with C++
 *
 * Summary of File:
 *
 *   This file contains code that that represent the bullets and their interaction with the space game.
 *
 */
#pragma once
#include "games.h"
#include "graphics.h"
#include <vector>

class Shot : public Games
{
	float speed;
	float shot_xx, shot_yy;
	bool player_bullet;
	bool shot_initialized = false;
	float changeSpeed = 0.0f;
	float direction = 180.0f;
	float tempDirectionX = 270.0f;
public:

	void draw() override;
	void update() override;
	void init() override;
	void setShotInit(bool s);
	void setbulletState(bool bul);
	void setbulletDirection(float dir);
	bool getShotInit();
	float getPos_X();
	float getPos_Y();
	void setPos_X(float x);
	void setPos_Y(float y);
	Shot();
	~Shot();
}; 
