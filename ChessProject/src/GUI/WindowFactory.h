#ifndef WINDOWFACTORY_H_
#define WINDOWFACTORY_H_

#include "../services/ErrorHandling.h"
#include "GUI.h"
#include "GUIUtils.h"
#include "GUIDefinitions.h"
#include "StartFunctions.h"
#include "TranslateFunctions.h"
#include "HandleFunctions.h"
#include "StopFunctions.h"

Window initWindow(WindowId id, SDL_Surface* screen);



#endif /* WINDOWFACTORY_H_ */
