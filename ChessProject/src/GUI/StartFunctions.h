/*
 * StartFunctions.h
 *
 *  Created on: Aug 29, 2015
 *      Author: nogalavi1
 */

#ifndef STARTFUNCTIONS_H_
#define STARTFUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_video.h>
#include <SDL/SDL_image.h>
#include "GUIUtils.h"
#include "GUIDefinitions.h"

int welcomeWindowStart (Window* window, void* initData);
void clipWelcomeOrPlayerSelection( SDL_Rect* clip );
int playerSelectionWindowStart (Window* window, void* initData);

#endif /* STARTFUNCTIONS_H_ */
