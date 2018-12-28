// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtConfig.h"
#include "StomtPluginPrivatePCH.h"
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
	config->StomtsFieldName = FString(TEXT("stomts"));
	config->LogUploadFieldName = FString(TEXT("acceptLogUpload"));
	config->ScreenshotUploadFieldName = FString(TEXT("acceptScreenshotUpload"));

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

		if (configJsonObj->HasField(this->AccessTokenFieldName))
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

		if (configJsonObj->HasField(this->LogUploadFieldName))
		{
			this->AcceptLogUpload = configJsonObj->GetBoolField(this->LogUploadFieldName);
			UE_LOG(StomtInit, Log, TEXT("Accept Log Upload: %s"), this->AcceptLogUpload ? TEXT("true") : TEXT("false"));
		}
		else
		{
			this->AcceptLogUpload = false;
		}

		if (configJsonObj->HasField(this->ScreenshotUploadFieldName))
		{
			this->AcceptScreenshotUpload = configJsonObj->GetBoolField(this->ScreenshotUploadFieldName);
			UE_LOG(StomtInit, Log, TEXT("Accept Screenshot Upload: %s"), this->AcceptScreenshotUpload ? TEXT("true") : TEXT("false"));
		}
		else
		{
			this->AcceptScreenshotUpload = false;
		}

		if (configJsonObj->HasField(this->StomtsFieldName))
		{
			this->Stomts = configJsonObj->GetArrayField(this->StomtsFieldName);
			UE_LOG(StomtInit, Log, TEXT("Saved Offline Stomts: %d"), this->Stomts.Num());
		}
		else
		{
			UE_LOG(StomtInit, Log, TEXT("Saved Offline Stomts: 0"));
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

bool UStomtConfig::GetAcceptScreenshotUpload()
{
	return this->AcceptScreenshotUpload;
}

void UStomtConfig::SetAcceptScreenshotUpload(bool acceptScreenshotUpload)
{
	if (this->AcceptScreenshotUpload == acceptScreenshotUpload) return;

	this->AcceptScreenshotUpload = acceptScreenshotUpload;

	this->SaveFlag(this->ScreenshotUploadFieldName, this->AcceptScreenshotUpload);

	OnConfigUpdated.Broadcast(this);
}

bool UStomtConfig::GetAcceptLogUpload()
{
	return this->AcceptLogUpload;
}

void UStomtConfig::SetAcceptLogUpload(bool acceptLogUpload)
{
	if (this->AcceptLogUpload == acceptLogUpload) return;

	this->AcceptLogUpload = acceptLogUpload;

	this->SaveFlag(this->LogUploadFieldName, this->AcceptLogUpload);

	OnConfigUpdated.Broadcast(this);
}

TArray<UStomtJsonValue*> UStomtConfig::GetStomtsAsJson()
{
	UStomtRestJsonObject* jsonObj = ReadStomtConfAsJson();

	if (jsonObj->HasField(this->StomtsFieldName))
	{
		return jsonObj->GetArrayField(this->StomtsFieldName);
	}
	else
	{
		return TArray<UStomtJsonValue*>();
	}
}

TArray<UStomt*> UStomtConfig::GetStomts()
{
	TArray<UStomt*> stomtObjects;
	UStomtRestJsonObject* jsonObj = ReadStomtConfAsJson();

	if (jsonObj->HasField(this->StomtsFieldName))
	{

		for (UStomtRestJsonObject* stomt : jsonObj->GetObjectArrayField(this->StomtsFieldName))
		{
			UStomt* stomtObject = UStomt::ConstructStomt(
				stomt->GetStringField("target_id"), 
				stomt->GetBoolField("positive"),
				stomt->GetStringField("text")	
			);

			stomtObjects.Add(stomtObject);

			/*
			jObj->SetField(TEXT("target_id"), UStomtJsonValue::ConstructJsonValueString(this, stomt.GetTargetID()));
			jObj->SetField(TEXT("positive"), UStomtJsonValue::ConstructJsonValueBool(this, stomt.GetPositive()));
			jObj->SetField(TEXT("text"), UStomtJsonValue::ConstructJsonValueString(this, stomt.GetText()));
			jObj->SetField(TEXT("anonym"), UStomtJsonValue::ConstructJsonValueBool(this, stomt.GetAnonym()));
			jObj->SetArrayField(TEXT("labels"), labels);

			for (int i = 0; i != stomt.GetLabels().Num(); ++i)
			{
				if (!stomt.GetLabels()[i]->GetName().IsEmpty())
				{
					labels.Add(UStomtJsonValue::ConstructJsonValueString(this, stomt.GetLabels()[i]->GetName()));
				}
			}
			*/
		}
	}
	else
	{
		return TArray<UStomt*>();
	}

	return stomtObjects;
}

bool UStomtConfig::AddStomt(UStomt* stomt)
{
	return SaveStomtToConf(*stomt);
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

bool UStomtConfig::SaveStomtToConf(UStomt& stomt)
{
	UStomtRestJsonObject* jsonObj = ReadStomtConfAsJson();

	UStomtRestJsonObject* jObj = UStomtRestJsonObject::ConstructJsonObject(this);
	TArray<UStomtJsonValue*> labels = TArray<UStomtJsonValue*>();

	for (int i = 0; i != stomt.GetLabels().Num(); ++i)
	{
		if (!stomt.GetLabels()[i]->GetName().IsEmpty())
		{
			labels.Add(UStomtJsonValue::ConstructJsonValueString(this, stomt.GetLabels()[i]->GetName()));
		}
	}

	jObj->SetField(TEXT("target_id"), UStomtJsonValue::ConstructJsonValueString(this, stomt.GetTargetID()));
	jObj->SetField(TEXT("positive"), UStomtJsonValue::ConstructJsonValueBool(this, stomt.GetPositive()));
	jObj->SetField(TEXT("text"), UStomtJsonValue::ConstructJsonValueString(this, stomt.GetText()));
	jObj->SetField(TEXT("anonym"), UStomtJsonValue::ConstructJsonValueBool(this, stomt.GetAnonym()));
	jObj->SetArrayField(TEXT("labels"), labels);

	if (!jsonObj->HasField(StomtsFieldName))
	{
		TArray<UStomtRestJsonObject*> objArray;
		objArray.Add(jObj);
		jsonObj->SetObjectArrayField(StomtsFieldName, objArray);

		UE_LOG(StomtNetwork, Log, TEXT("Add Offline Stomt: To new field"));
	}
	else
	{
		TArray<UStomtRestJsonObject*> objArray2;
		objArray2 = jsonObj->GetObjectArrayField(StomtsFieldName);
		objArray2.Add(jObj);
		jsonObj->SetObjectArrayField(StomtsFieldName, objArray2);
		UE_LOG(StomtNetwork, Log, TEXT("Add Offline Stomt: To existing field"));
	}

	UE_LOG(StomtNetwork, Log, TEXT("Add offline stomt: %s"), *jsonObj->EncodeJson());

	return this->WriteFile(jsonObj->EncodeJson(), ConfigName, ConfigFolder, true);
}

bool UStomtConfig::ClearStomts()
{
	UStomtRestJsonObject* jsonObj = ReadStomtConfAsJson();

	if (jsonObj->HasField(StomtsFieldName))
	{
		UE_LOG(StomtNetwork, Log, TEXT("Clear offline stomts"));
		// void RemoveField(const FString& FieldName);
		jsonObj->GetArrayField(StomtsFieldName).Empty();
		jsonObj->RemoveField(StomtsFieldName);
	}
	else
	{
		return true;
	}

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
