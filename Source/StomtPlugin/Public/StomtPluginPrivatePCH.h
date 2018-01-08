#pragma once

#include "CoreUObject.h"
#include "Engine.h"

#include "Delegate.h"
#include "Http.h"
#include "Map.h"
#include "Json.h"

#include "LatentActions.h"
#include "Core.h"
#include "Engine.h"
#include "SharedPointer.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(Stomt, Log, All);

//Logging during plugin startup
DECLARE_LOG_CATEGORY_EXTERN(StomtInit, Log, All);

//Logging during network communication
DECLARE_LOG_CATEGORY_EXTERN(StomtNetwork, Log, All);

//Logging during file access
DECLARE_LOG_CATEGORY_EXTERN(StomtFileAccess, Log, All);
#include "StomtPlugin.h"

