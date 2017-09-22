// Copyright 2017 Daniel Schukies. All Rights Reserved.

#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtTrack.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/App.h"
#include "StomtJsonObject.h"

UStomtTrack* UStomtTrack::ConstructStomtConfig()
{
	UStomtTrack* track = NewObject<UStomtTrack>();

	return track;
}


UStomtTrack::UStomtTrack()
{

}

UStomtTrack::~UStomtTrack()
{

}
