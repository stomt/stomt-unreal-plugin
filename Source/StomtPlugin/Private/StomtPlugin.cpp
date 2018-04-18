// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once
#include "StomtPlugin.h"
#include "StomtPluginPrivatePCH.h"

class FStomtPlugin : public StomtPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		//Force classes to be compiled on shipping build
	}

	virtual void ShutdownModule() override
	{

	}
};

IMPLEMENT_MODULE(FStomtPlugin, StomtPlugin);

DEFINE_LOG_CATEGORY(Stomt);
DEFINE_LOG_CATEGORY(StomtNetwork);
DEFINE_LOG_CATEGORY(StomtInit);
DEFINE_LOG_CATEGORY(StomtFileAccess);
