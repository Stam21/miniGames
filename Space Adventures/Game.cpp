/**
 * File:    Game.cpp
 *
 * Author: George Stamatopoulos  
 * Date:    30/01/2022
 * Partner:  No
 * Course:   Programming with C++
 *
 * Summary of File:
 *
 *   This file contains code that initializes the application.
 *
 */

#include "graphics.h"
#include "Space.h"
#include "config.h"
// The custom callback function that the library calls 
// to check for and set the current application state.
void update(float ms)
{
    Space* spaceadv = reinterpret_cast<Space *>(graphics::getUserData());
    spaceadv->update();
}

// The window content drawing function.
void draw()
{
    Space* spaceadv = reinterpret_cast<Space *>(graphics::getUserData());
    spaceadv->draw();

}

int main()
{ 
    Space spaceadv;
    
    graphics::createWindow(WINDOW_WIDTH , WINDOW_HEIGHT, "MINI-GAMES");
    graphics::setUserData(&spaceadv);
   
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);
    
    graphics::setCanvasSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    graphics::setFullScreen(true);
    graphics::startMessageLoop();

    return 0;
}
