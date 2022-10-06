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
#include "player.h"
#include "graphics.h"
#include "Shot.h"


//NOTE: The second update function is not necessary. It is better practice to use only the overriden function.
// The same thing applies to the rest of the functions, both players can be represented by the same functions.
void Player::update()
{
	shot_initialized = false;
	if (graphics::getKeyState(graphics::SCANCODE_A))
	{
		pos_x -= speed * graphics::getDeltaTime() / 30.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		pos_x += speed * graphics::getDeltaTime() / 30.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_W))
	{
		pos_y -= speed * graphics::getDeltaTime() / 30.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_S))
	{
		pos_y += speed * graphics::getDeltaTime() / 30.0f;
	}
	
	
	if (pos_x < 0 + 30.0f) pos_x = 0 + 30.0f;
	if (pos_x > WINDOW_WIDTH - 30.0f) pos_x = WINDOW_WIDTH - 30.0f;
	if (pos_y < 0 + 30.0f) pos_y = 0 + 30.0f;
	if (pos_y > WINDOW_HEIGHT - 30.0f) pos_y = WINDOW_HEIGHT - 30.0f;
}

void Player::updateSecond()
{
	if (graphics::getKeyState(graphics::SCANCODE_LEFT))
	{
		pos_x1 -= speed * graphics::getDeltaTime() / 30.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_RIGHT))
	{
		pos_x1 += speed * graphics::getDeltaTime() / 30.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_UP))
	{
		pos_y1 -= speed * graphics::getDeltaTime() / 30.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_DOWN))
	{
		pos_y1 += speed * graphics::getDeltaTime() / 30.0f;
	}

	if (pos_x1 < 0 + 30.0f) pos_x1 = 0 + 30.0f;
	if (pos_x1 > WINDOW_WIDTH - 30.0f) pos_x1 = WINDOW_WIDTH - 30.0f;
	if (pos_y1 < 0 + 30.0f) pos_y1 = 0 + 30.0f;
	if (pos_y1 > WINDOW_HEIGHT - 30.0f) pos_y1 = WINDOW_HEIGHT - 30.0f;
}


void Player::draw()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "player.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y, 85, 85, br);
}


void Player::drawFirst()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "player.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x - 100, pos_y, 85, 85, br);
}


void Player::drawSecond()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "player.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x1 + 100, pos_y1, 85, 85, br);
}

void Player::init()
{
}

float Player::getPos_X()
{
	return pos_x;
}

float Player::getPos_Y()
{
	return pos_y;
}

bool Player::getShot()
{
	return shot_initialized;
}
