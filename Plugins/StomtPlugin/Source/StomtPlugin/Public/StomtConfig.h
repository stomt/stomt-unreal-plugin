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


	//////////////////////////////////////////////////////////////////////////
	// Read Config


	/**
	* Loads the access token from disk.
	*/
	FString ReadStomtConf(FString FieldName);

	/**
	* Loads the flag from disk.
	*/
	bool ReadFlag(FString FlagName);

	/**
	* Loads the access token from disk.
	*/
	UStomtRestJsonObject* ReadStomtConfAsJson();



	//////////////////////////////////////////////////////////////////////////
	// Write Config


	/**
	* Saves the access token in /stomt/stomt.conf.json
	*/
	bool SaveAccesstoken(FString accesstoken);

	/**
	* Saves the a value in /stomt/stomt.conf.json
	*/
	bool SaveValueToStomtConf(FString FieldName, FString FieldValue);

	/**
	* Saves the flag in /stomt/stomt.conf.json
	*/
	bool SaveFlag(FString FlagName, bool FlagState);

	/**
	* Loads the access token from disk.
	*/
	bool WriteStomtConfAsJson(UStomtRestJsonObject* StomtConf);

	/**
	* Delete stomt.conf.json
	*/
	void DeleteStomtConf();

	/**
	* Loads an Log file from disk.
	* @param LogFileName - Log File Name.
	*/
	FString ReadLogFile(FString LogFileName);

private:
	bool WriteFile(FString TextToSave, FString FileName, FString SaveDirectory, bool AllowOverwriting);
	bool ReadFile(FString& Result, FString FileName, FString SaveDirectory);

	//////////////////////////////////////////////////////////////////////////
	// Data


	FString				Accesstoken;
	FString				ConfigFolder;
	FString				ConfigName;

};
