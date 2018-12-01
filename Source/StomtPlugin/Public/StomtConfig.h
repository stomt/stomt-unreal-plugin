// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once
#include "StomtJsonObject.h"
#include "Stomt.h"
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "StomtConfig.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfigUpdated, class UStomtConfig*, Config);

UCLASS()
class UStomtConfig : public UObject
{
	GENERATED_BODY()

public:

	static UStomtConfig* ConstructStomtConfig();

	UStomtConfig();

	~UStomtConfig();
	//////////////////////////////////////////////////////////////////////////
	// Public

	/**
	*	checks if a config file exists, creates one or reads it
	**/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void Load();

	/**
	*	deletes to current config file
	**/
	void Delete();

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	FString GetAccessToken();
	void	SetAccessToken(FString AccessToken);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool	GetSubscribed();
	void	SetSubscribed(bool Subscribed);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool	GetLoggedIn();
	void	SetLoggedIn(bool LoggedIn);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool	GetAcceptScreenshotUpload();
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void	SetAcceptScreenshotUpload(bool acceptScreenshotUpload);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool	GetAcceptLogUpload();
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void	SetAcceptLogUpload(bool acceptLogUpload);

	UPROPERTY(BlueprintAssignable, Category = "Stomt|Event")
	FOnConfigUpdated OnConfigUpdated;

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	TArray<UStomtJsonValue*> GetStomtsAsJson();
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	TArray<UStomt*> GetStomts();
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool	AddStomt(UStomt* stomt);

	/**
	* Clears the stomt array in /stomt/stomt.conf.json
	*/
	bool ClearStomts();


	//////////////////////////////////////////////////////////////////////////
	// Read Config

private:

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

	/**
	* Loads the access token from disk.
	*/
	FString ReadAccesstoken();


	//////////////////////////////////////////////////////////////////////////
	// Write Config

private:

	/**
	* Saves the access token in /stomt/stomt.conf.json
	*/
	bool SaveAccesstoken(FString accesstoken);

	/**
	* Saves the a value in /stomt/stomt.conf.json
	*/
	bool SaveValueToStomtConf(FString FieldName, FString FieldValue);

	/**
	* Saves the a stomt in /stomt/stomt.conf.json
	*/
	bool SaveStomtToConf(UStomt& stomt);

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

	bool				Subscribed;
	bool				LoggedIn;
	bool				AcceptLogUpload;
	bool				AcceptScreenshotUpload;

	FString				SubscribedFieldName;
	FString				LoggedInFieldName;
	FString				AccessTokenFieldName;
	FString				StomtsFieldName;
	FString				LogUploadFieldName;
	FString				ScreenshotUploadFieldName;

	FString				Accesstoken;
	FString				ConfigFolder;
	FString				ConfigName;

	UPROPERTY()
	TArray<UStomtJsonValue*> Stomts;

};
