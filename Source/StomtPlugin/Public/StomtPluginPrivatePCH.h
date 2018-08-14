// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "CoreUObject.h"
#include "Engine.h"

#include "Http.h"
#include "Json.h"

#include "LatentActions.h"
#include "Core.h"
#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(Stomt, Log, All);

//Logging during plugin startup
DECLARE_LOG_CATEGORY_EXTERN(StomtInit, Log, All);

//Logging during network communication
DECLARE_LOG_CATEGORY_EXTERN(StomtNetwork, Log, All);

//Logging during file access
DECLARE_LOG_CATEGORY_EXTERN(StomtFileAccess, Log, All);
#include "StomtPlugin.h"
