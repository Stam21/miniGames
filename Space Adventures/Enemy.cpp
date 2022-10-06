/**
 * File:    Enemy.cpp
 *
 * Author: George Stamatopoulos
 * Date:    30/01/2022
 * Partner:  No
 * Course:   Programming with C++
 *
 * Summary of File:
 *
 *   This file contains code that that represent the enemy and his interaction with the space game.
 *
 */
#include "Enemy.h"
#include "config.h"
#include <cmath>


//NOTE: A setTexture function can be added in which I can specify what kind of enemy it will draw and update.
void Enemy::draw()
{
	graphics::Brush brush;
	graphics::setOrientation(direction);
	brush.texture = std::string(ASSET_PATH) + "alienship_blue5.png";
	brush.outline_opacity = 0.0f;
	graphics::drawRect(pos_xx, pos_yy, 100, 100, brush);
	
	
}

//NOTE: Change position with specific frequency based on the motion I want to achieve. 
void Enemy::update()
{
	if (direction == 0) {
		pos_yy += speedy * graphics::getDeltaTime();
		pos_xx += speedx * graphics::getDeltaTime();
	}
	else if (direction<=35){
		speedx += 0.0006f;
		speedy += 0.0001f;
		pos_yy += speedy * graphics::getDeltaTime();
		pos_xx += speedx * graphics::getDeltaTime();
	}
	else if (direction <= 180) {
		speedx += 0.0025f;
		speedy -= 0.003f;
		pos_yy += speedy * graphics::getDeltaTime();
		pos_xx += speedx * graphics::getDeltaTime();
	}
}

void Enemy::setShotInit(bool s)
{
	shot_initialized = s;
}

bool Enemy::getShotInit()
{
	return shot_initialized;
}

float Enemy::getPos_X()
{
	return pos_xx;
}

float Enemy::getDirection()
{
	return direction;
}

void Enemy::setDirection(float direct)
{
	 direction = direct;
}

float Enemy::getPos_Y()
{
	return pos_yy;
}

void Enemy::setPos_X(float x)
{
	pos_xx = x;
}

void Enemy::setPos_Y(float y)
{
	pos_yy = y;
}

void Enemy::init()
{
	speedy = 0.30f;
	speedx = 0.0f;
	direction = 0.0f;
	pos_yy = -10.0f;
	pos_xx = 500.0f;
}

Enemy::Enemy()
{
	init();
}

Enemy::~Enemy()
{
}
