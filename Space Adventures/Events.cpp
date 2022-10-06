/**
 * File:    Events.cpp
 *
 * Author: George Stamatopoulos
 * Date:    30/01/2022
 * Partner:  No
 * Course:   Programming with C++
 *
 * Summary of File:
 *
 *   This file contains code that handles all events. When an event has finished it returns feedback. 
 *   Every event runs for a given duration.
 */
#include "Events.h"

void Events::update()
{
	if (!activeEV)
	{
		return;
	}

	if (waiting())
	{
		elapsed_delay += graphics::getDeltaTime() / 1000.0f;
		return;
	}

	elapsed += graphics::getDeltaTime() / 1000.0f;
	if (elapsed > duration)
	{
		activeEV = false;
	}
}

Events::Events(float x, float y, float dur, float del)
	:pos_x(x),pos_y(y),duration(dur),delay(del)
{

}

bool Events::waiting()
{
	return elapsed_delay < delay;
}

void battleEvent::update()
{
	Events::update();
	if (!player)
	{
		activeEV = false;
		return;
	}
	//NOTE: Divide the whole motion into 3 different movements based on elapsed time.
	if (duration == 1.5f && elapsed <1.4f)
	{
		
		float s = elapsed / duration;
		float x = start_x * (1.0f - s) + mid_x * s;
		float y = start_y * (1.0f - s) + mid_y * s;
		player->setScale(92 + s * 100, 92 + s * 100);
		player->setPos_X(x);
		player->setPos_Y(y);
	}
	else if ((duration == 1.5f && elapsed < 1.5f) || (duration == 0.3f && elapsed <0.2f))
	{
		if (duration == 1.5f)
		{
			elapsed = 0.0f;
			duration = 0.3f;
		}
		float s = elapsed / duration;
		float x = mid_x * (1.0f - s) + stop_x * s;
		float y = mid_y * (1.0f - s) + stop_y * s;
		player->setPos_X(x);
		player->setPos_Y(y);
	}
	else if ((duration == 0.3f && elapsed < 0.3f) || (duration == 0.2f && elapsed < 0.195f))
	{
		if (duration == 0.3f)
		{
			elapsed = 0.0f;
			duration = 0.2f;
		}
		float s = elapsed / duration;
		float x = stop_x * (1.0f - s) + mid_x * s;
		float y = stop_y * (1.0f - s) + mid_y * s;
		player->setPos_X(x);
		player->setPos_Y(y);
	}
}

battleEvent::battleEvent(float tstart_x, float tstart_y,float tmid_x, float tmid_y,float tstop_x, float tstop_y, Card* crd)
	: Events(0.0f,0.0f,1.5f,0.0f), start_x(tstart_x), start_y(tstart_y),mid_x(tmid_x), mid_y(tmid_y),
	stop_x(tstop_x), stop_y(tstop_y), player(crd)
{

}
void fadeOutEvent::update()
{
	Events::update();
	if (!player)
	{
		activeEV = false;
		return;
	}
	float s = elapsed / duration;
	
	//NOTE: After 3 sec start fade out card and create the explosion.
	if (elapsed > 3.0f)
	{
		player->setOpacity(1.0f - s);
		if (color == "red") {
			std::string texture = std::string(ASSET_PATH) + "explosion/1_" + std::to_string(imgcounter) + ".png";
			if (imgcounter < 12 && controlExplosion > 3) {
				imgcounter += 1;
				controlExplosion = 0;
			}

			player->setExplosion(texture);
		}
		else
		{
			std::string texture = std::string(ASSET_PATH) + "blue_explosion/1_" + std::to_string(imgcounter) + ".png";
			if (imgcounter < 14 && controlExplosion > 3) {
				imgcounter += 1;
				controlExplosion = 0;
			}

			player->setExplosion(texture);
		}
		controlExplosion += 1;
		
	}
	if ((1.0f - s) < 0.1)
	{
		player->setbattleMode(false); //Completely delete everything that has to do with the card faded out.
	}
	else if ((1.0f - s) < 0.5)
	{
		graphics::playSound(std::string(ASSET_PATH) + "SecondGame/explosion.wav", 0.4f); //Explosion sound.
	}
	
}

fadeOutEvent::fadeOutEvent(std::string tcolor, Card* crd)
	: Events(0.0f, 0.0f, 5.0f, 0.0f), color(tcolor), player(crd)
{

}