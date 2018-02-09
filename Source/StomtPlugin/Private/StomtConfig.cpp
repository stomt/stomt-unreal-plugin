#pragma once

#include "StomtPluginPrivatePCH.h"
#include "StomtConfig.h"
#include "StomtJsonObject.h"

#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/App.h"

UStomtConfig* UStomtConfig::ConstructStomtConfig()
{
	UStomtConfig* config = NewObject<UStomtConfig>();

	config->ConfigFolder = FPaths::EngineUserDir() + FString(TEXT("Saved/Config/stomt/"));
	UE_LOG(StomtInit, Log, TEXT("Config-Folder: %s"), *config->ConfigFolder);

	config->ConfigName = FString(TEXT("stomt.conf.json"));
	config->Accesstoken = FString(TEXT(""));
	config->LoggedInFieldName = FString(TEXT("loggedin"));
	config->SubscribedFieldName = FString(TEXT("email"));
	config->AccessTokenFieldName = FString(TEXT("accesstoken"));

	config->Load();

	return config;
}

UStomtConfig::UStomtConfig()
{
}

UStomtConfig::~UStomtConfig()
{
}

void UStomtConfig::Load()
{
	// Use this if you experience crashes at bake process.
	/*	#if UE_EDITOR
		if (!IsRunningGame())
		{
			return;
		}
		#endif
	*/

	if (FPaths::FileExists(this->ConfigFolder + this->ConfigName))
	{
		UStomtRestJsonObject* configJsonObj = ReadStomtConfAsJson();

		if (configJsonObj->HasField(this->SubscribedFieldName))
		{
			this->Accesstoken = configJsonObj->GetStringField(this->AccessTokenFieldName);
			UE_LOG(StomtInit, Log, TEXT("Accesstoken: %s"), *this->Accesstoken);
		}

		if (configJsonObj->HasField(this->SubscribedFieldName))
		{
			this->Subscribed = configJsonObj->GetBoolField(this->SubscribedFieldName);
			UE_LOG(StomtInit, Log, TEXT("Subscribed: %s"), this->Subscribed ? TEXT("true") : TEXT("false") );
		}
		else
		{
			this->Subscribed = false;
		}

		if (configJsonObj->HasField(this->LoggedInFieldName))
		{
			this->LoggedIn = configJsonObj->GetBoolField(this->LoggedInFieldName);
			UE_LOG(StomtInit, Log, TEXT("LoggedIn: %s"), this->LoggedIn ? TEXT("true") : TEXT("false") );
		}
		else
		{
			this->LoggedIn = false;
		}

		OnConfigUpdated.Broadcast(this);
	}
	else
	{
		// Force to create config file
		this->Subscribed = true;
		this->LoggedIn = true;
		this->SetSubscribed(false);
		this->SetLoggedIn(false);

		OnConfigUpdated.Broadcast(this);
	}
}

void UStomtConfig::Delete()
{
	this->DeleteStomtConf();
	this->Load();
}

FString UStomtConfig::GetAccessToken()
{
	if (this->Accesstoken.IsEmpty())
	{
		this->Accesstoken = this->ReadAccesstoken();
	}

	return this->Accesstoken;
}

void UStomtConfig::SetAccessToken(FString AccessToken)
{
	if (this->Accesstoken.Equals(AccessToken)) return;

	this->Accesstoken = AccessToken;

	this->SaveAccesstoken(this->Accesstoken);

	OnConfigUpdated.Broadcast(this);
}

bool UStomtConfig::GetSubscribed()
{	
	return this->Subscribed;
}

void UStomtConfig::SetSubscribed(bool Subscribed)
{
	if (this->Subscribed == Subscribed) return;

	this->Subscribed = Subscribed;

	this->SaveFlag(this->SubscribedFieldName, this->Subscribed);

	OnConfigUpdated.Broadcast(this);
}

bool UStomtConfig::GetLoggedIn()
{
	return this->LoggedIn;
}

void UStomtConfig::SetLoggedIn(bool LoggedIn)
{
	if (this->LoggedIn == LoggedIn) return;

	this->LoggedIn = LoggedIn;

	this->SaveFlag(this->LoggedInFieldName, this->LoggedIn);

	OnConfigUpdated.Broadcast(this);
}

bool UStomtConfig::SaveAccesstoken(FString accesstoken)
{
	return SaveValueToStomtConf(this->AccessTokenFieldName, accesstoken);
}

bool UStomtConfig::SaveValueToStomtConf(FString FieldName, FString FieldValue)
{
	UStomtRestJsonObject* jsonObj = ReadStomtConfAsJson();

	if (jsonObj->HasField(FieldName))
	{
		if (jsonObj->GetStringField(FieldName).Equals(FieldValue))
		{
			return false;
		}

		jsonObj->RemoveField(FieldName);
	}

	jsonObj->SetStringField(FieldName, FieldValue);

	return this->WriteFile(jsonObj->EncodeJson(), ConfigName, ConfigFolder, true);
}

bool UStomtConfig::SaveFlag(FString FlagName, bool FlagState)
{
	return SaveValueToStomtConf(FlagName, FlagState ? TEXT("true") : TEXT("false"));
}

FString UStomtConfig::ReadStomtConf(FString FieldName)
{
	FString result;

	if (this->ReadFile(result, ConfigName, ConfigFolder))
	{
		UStomtRestJsonObject* jsonObj = UStomtRestJsonObject::ConstructJsonObject(this);
		jsonObj->DecodeJson(result);
		this->Accesstoken = jsonObj->GetField(FieldName)->AsString();
	}

	return result;
}

bool UStomtConfig::ReadFlag(FString FlagName)
{
	FString result;
	bool FlagState = false;

	if (this->ReadFile(result, ConfigName, ConfigFolder))
	{
		UStomtRestJsonObject* jsonObj = UStomtRestJsonObject::ConstructJsonObject(this);
		jsonObj->DecodeJson(result);
		FlagState = jsonObj->GetField(FlagName)->AsBool();
	}

	return FlagState;
}

UStomtRestJsonObject* UStomtConfig::ReadStomtConfAsJson()
{
	FString result;
	UStomtRestJsonObject* jsonObj = UStomtRestJsonObject::ConstructJsonObject(this);

	if (this->ReadFile(result, ConfigName, ConfigFolder))
	{
		jsonObj->DecodeJson(result);
	}

	return jsonObj;
}

FString UStomtConfig::ReadAccesstoken()
{
	UStomtRestJsonObject* StomtConfigJson = this->ReadStomtConfAsJson();

	if (StomtConfigJson->HasField(this->AccessTokenFieldName))
	{
		return StomtConfigJson->GetStringField(this->AccessTokenFieldName);
	}

	return FString(TEXT(""));
}

bool UStomtConfig::WriteStomtConfAsJson(UStomtRestJsonObject * StomtConf)
{
	return this->WriteFile(StomtConf->EncodeJson(), ConfigName, ConfigFolder, true);
}

void UStomtConfig::DeleteStomtConf()
{
	FString file = this->ConfigFolder + this->ConfigName;
	if (!FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*file))
	{
		UE_LOG(StomtFileAccess, Warning, TEXT("Could not delete stomt.conf.json: %s"), *file);
	}
	else
	{
		UE_LOG(StomtFileAccess, Warning, TEXT("Deleted stomt.conf.json because of wrong access token Path: %s"), *file);
	}
}

FString UStomtConfig::ReadLogFile(FString LogFileName)
{
	FString errorLog;

	FString LogFilePath = FPaths::ProjectLogDir() + LogFileName;
	FString LogFileCopyPath = FPaths::ProjectLogDir() + LogFileName + TEXT("Copy.log");
	FString LogFileCopyName = LogFileName + TEXT("Copy.log");

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.BypassSecurity(true);

	// Copy LogFileData
	if (!PlatformFile.CopyFile(*LogFileCopyPath, *LogFilePath, EPlatformFileRead::AllowWrite, EPlatformFileWrite::AllowRead))
	{
		UE_LOG(StomtFileAccess, Warning, TEXT("LogFile Copy did not work FromFile: %s | ToFile %s"), *LogFilePath, *LogFileCopyPath);
	}

	// Read LogFileCopy from Disk
	if (!this->ReadFile(errorLog, LogFileCopyName, FPaths::ProjectLogDir()))
	{
		if (FPaths::FileExists(FPaths::ProjectLogDir() + LogFileCopyName))
		{
			UE_LOG(StomtFileAccess, Warning, TEXT("Could not read LogFile %s, but it exists"), *LogFileCopyName);
		}
		else
		{
			UE_LOG(StomtFileAccess, Warning, TEXT("Could not read LogFile %s, because it does not exist"), *LogFileCopyName);
		}
	}

	// Delete LogFileCopy
	if (!PlatformFile.DeleteFile(*LogFileCopyPath))
	{
		UE_LOG(StomtFileAccess, Warning, TEXT("Could not delete LogFileCopy %s"), *LogFileCopyPath);
	}

	return errorLog;
}

bool UStomtConfig::WriteFile(FString TextToSave, FString FileName, FString SaveDirectory, bool AllowOverwriting)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		// Get absolute file path
		FString AbsoluteFilePath = SaveDirectory + "/" + FileName;

		// Allow overwriting or file doesn't already exist
		if (AllowOverwriting || !FPaths::FileExists(*AbsoluteFilePath))
		{
			// Use " FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);" for append
			return FFileHelper::SaveStringToFile(TextToSave, *AbsoluteFilePath);
		}
	}
	
	return false;
}

bool UStomtConfig::ReadFile(FString& Result, FString FileName, FString SaveDirectory)
{
	FString path = SaveDirectory + FileName;

	if (!FPaths::FileExists(path))
	{
		UE_LOG(StomtFileAccess, Warning, TEXT("File with this path does not exist: %s "), *path);
	}

	return FFileHelper::LoadFileToString(Result, *path);
}
