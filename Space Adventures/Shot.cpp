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
#include "Shot.h"
#include "config.h"

void Shot::draw()
{
	//NOTE: Different kind of bullets can be drawn here.
	graphics::Brush brush;
	if (player_bullet) {
		graphics::setOrientation(0.0f);
		brush.texture = std::string(ASSET_PATH) + "bullet.png";
		brush.outline_opacity = 0.0f;
		graphics::drawRect(shot_xx, shot_yy - 10, 15, 40, brush);
	} 
	else {
		graphics::setOrientation(direction);
		brush.texture = std::string(ASSET_PATH) + "bullet_blue.png";
		brush.outline_opacity = 0.0f;
		graphics::drawRect(shot_xx, shot_yy - 10, 35, 40, brush);
	}
}

void Shot::update()
{
	//NOTE: If bullet is shot by the player then the bullet moves with linear motion.
	if (player_bullet) {
		shot_yy -= speed * graphics::getDeltaTime();
		if (shot_xx < 0 + 30.0f) shot_xx = 0 + 30.0f;
		if (shot_xx > WINDOW_WIDTH - 30.0f) shot_xx = WINDOW_WIDTH - 30.0f;
		if (shot_yy < 0 + 30.0f) shot_yy = 0 + 30.0f;
		if (shot_yy > WINDOW_HEIGHT - 30.0f) shot_yy = WINDOW_HEIGHT - 30.0f;
	}
	//NOTE: if bullet is shot by the opponent then the bullet moves with circular motion.
	else {
		
		if (direction == 180.0f) {
			shot_yy += speed * sin(1) * (graphics::getDeltaTime());
			if (shot_xx < 0 + 30.0f) shot_xx = 0 + 30.0f;
			if (shot_xx > WINDOW_WIDTH - 30.0f) shot_xx = WINDOW_WIDTH - 30.0f;
		}
		else {
			shot_yy += speed * sin(direction/180.0f) * (graphics::getDeltaTime());
			direction += 90.0f;
			tempDirectionX = (direction * (atan(1) * 4))/180.0f;
			shot_xx += speed * cos(tempDirectionX) * (graphics::getDeltaTime());
			direction -= 90.0f;
			if (shot_xx < 0 + 30.0f) shot_xx = 0 + 30.0f;
			if (shot_xx > WINDOW_WIDTH - 30.0f) shot_xx = WINDOW_WIDTH - 30.0f;
		}
		
		
	}
}

float Shot::getPos_X()
{
	return shot_xx;
}


float Shot::getPos_Y()
{
	return shot_yy;
}

void Shot::setPos_X(float x)
{
	shot_xx = x;
}

void Shot::setPos_Y(float y)
{
	shot_yy = y;
}

void Shot::init()
{
	speed = 0.6f;	
}

void Shot::setShotInit(bool s)
{
	shot_initialized = s;
}

bool Shot::getShotInit()
{
	return shot_initialized;
}

void Shot::setbulletState(bool bul)
{
	player_bullet = bul;
}

void Shot::setbulletDirection(float dir)
{
	direction = dir;
}

Shot::Shot()
{
	init();
}


Shot::~Shot()
{

}
