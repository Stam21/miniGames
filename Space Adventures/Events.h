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
#pragma once
#include "graphics.h"
#include <vector>
#include "config.h"
#include "Card.h"

class Events
{
protected:
	float pos_x;
	float pos_y;
	float duration = 2.0f;
	float delay = 0.0f;
	float elapsed = 0.0f;
	float elapsed_delay = 0.0f;
	bool activeEV = true;

public:
	virtual void draw() {};
	virtual void update();
	Events(float x = 0.0f, float y = 0.0f, float dur = 2.0f, float del = 0.0f);
	virtual ~Events() {};
	bool active() { return activeEV; }
	void disable() { activeEV = false; }
	bool waiting();
};

class battleEvent : public Events
{
	class Card* player;
	float start_x;
	float start_y;
	float mid_x;
	float mid_y;
	float stop_x;
	float stop_y;
	float scale;
public:
	void update() override;
	battleEvent(float tstart_x, float tstart_y, float tmid_x, float tmid_y, float tstop_x, float tstop_y, class Card* crd);
};

class fadeOutEvent : public Events
{
	class Card* player;
	std::string color;
	int imgcounter = 0;
	int controlExplosion = 0;

public:
	void update() override;
	fadeOutEvent(std::string tcolor, class Card* crd);
};