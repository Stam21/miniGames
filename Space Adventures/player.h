/**
 * File:    player.cpp
 *
 * Author: George Stamatopoulos
 * Date:    30/01/2022
 * Partner:  No
 * Course:   Programming with C++
 *
 * Summary of File:
 *
 *   This file contains code that that represent the player and his interaction with the space game.
 *
 */
#pragma once
#include "Games.h"
#include "config.h"
#include "Shot.h"

class Player : public Games
{
	float speed = 10.0f;
	float pos_x = WINDOW_WIDTH/2, pos_y = WINDOW_HEIGHT/2 + 300;
	float pos_x1 = WINDOW_WIDTH / 2, pos_y1 = WINDOW_HEIGHT / 2 + 300;
	bool shot_initialized= false;
	
public:
	void draw() override;	
	void update() override;
	void init() override;
	void drawSecond();
	void updateSecond();
	void drawFirst();
	float getPos_X();
	float getPos_Y();
	bool getShot();
	
};