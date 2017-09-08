// Copyright 2017 Daniel Schukies. All Rights Reserved.


#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "StomtConfig.generated.h"


/**
 * 
 */
UCLASS()
class UStomtConfig : public UObject
{
	GENERATED_BODY()
public:

	static UStomtConfig* ConstructStomtConfig();

	UStomtConfig();

	~UStomtConfig();


};
