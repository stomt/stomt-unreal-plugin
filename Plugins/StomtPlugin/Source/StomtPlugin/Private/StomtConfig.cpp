// Copyright 2017 Daniel Schukies. All Rights Reserved.

#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtConfig.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/App.h"
#include "StomtJsonObject.h"

UStomtConfig* UStomtConfig::ConstructStomtConfig()
{
	UStomtConfig* config = NewObject<UStomtConfig>();


	return config;
}


UStomtConfig::UStomtConfig()
{

}

UStomtConfig::~UStomtConfig()
{
}
