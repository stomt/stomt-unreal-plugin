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
	UStomtConfig* Config = NewObject<UStomtConfig>();

	Config->ConfigFolder = FPaths::EngineUserDir() + FString(TEXT("Saved/Config/stomt/"));
	UE_LOG(StomtInit, Log, TEXT("Config-Folder: %s"), *Config->ConfigFolder);

	Config->ConfigName = FString(TEXT("stomt.conf.json"));
	Config->AccessToken = FString(TEXT(""));
	Config->LoggedInFieldName = FString(TEXT("loggedin"));
	Config->SubscribedFieldName = FString(TEXT("email"));
	Config->AccessTokenFieldName = FString(TEXT("accesstoken"));
	Config->StomtsFieldName = FString(TEXT("stomts"));
	Config->LogUploadFieldName = FString(TEXT("acceptLogUpload"));
	Config->ScreenshotUploadFieldName = FString(TEXT("acceptScreenshotUpload"));

	Config->Load();
	
	return Config;
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
		UStomtRestJsonObject* ConfigJsonObj = ReadStomtConfAsJson();

		if (ConfigJsonObj->HasField(this->AccessTokenFieldName))
		{
			this->AccessToken = ConfigJsonObj->GetStringField(this->AccessTokenFieldName);
			UE_LOG(StomtInit, Log, TEXT("AccessToken: %s"), *this->AccessToken);
		}

		if (ConfigJsonObj->HasField(this->SubscribedFieldName))
		{
			this->bSubscribed = ConfigJsonObj->GetBoolField(this->SubscribedFieldName);
			UE_LOG(StomtInit, Log, TEXT("Subscribed: %s"), this->bSubscribed ? TEXT("true") : TEXT("false"));
		}
		else
		{
			this->bSubscribed = false;
		}

		if (ConfigJsonObj->HasField(this->LoggedInFieldName))
		{
			this->bLoggedIn = ConfigJsonObj->GetBoolField(this->LoggedInFieldName);
			UE_LOG(StomtInit, Log, TEXT("LoggedIn: %s"), this->bLoggedIn ? TEXT("true") : TEXT("false") );
		}
		else
		{
			this->bLoggedIn = false;
		}

		if (ConfigJsonObj->HasField(this->LogUploadFieldName))
		{
			this->bAcceptLogUpload = ConfigJsonObj->GetBoolField(this->LogUploadFieldName);
			UE_LOG(StomtInit, Log, TEXT("Accept Log Upload: %s"), this->bAcceptLogUpload ? TEXT("true") : TEXT("false"));
		}
		else
		{
			this->bAcceptLogUpload = false;
		}

		if (ConfigJsonObj->HasField(this->ScreenshotUploadFieldName))
		{
			this->bAcceptScreenshotUpload = ConfigJsonObj->GetBoolField(this->ScreenshotUploadFieldName);
			UE_LOG(StomtInit, Log, TEXT("Accept Screenshot Upload: %s"), this->bAcceptScreenshotUpload ? TEXT("true") : TEXT("false"));
		}
		else
		{
			this->bAcceptScreenshotUpload = false;
		}

		if (ConfigJsonObj->HasField(this->StomtsFieldName))
		{
			this->Stomts = ConfigJsonObj->GetArrayField(this->StomtsFieldName);
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
		this->bSubscribed = true;
		this->bLoggedIn = true;
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
	if (this->AccessToken.IsEmpty())
	{
		this->AccessToken = this->ReadAccessToken();
	}

	return this->AccessToken;
}

void UStomtConfig::SetAccessToken(FString NewAccessToken)
{
	if (this->AccessToken.Equals(NewAccessToken)) return;

	this->AccessToken = NewAccessToken;

	this->SaveAccessToken(this->AccessToken);

	OnConfigUpdated.Broadcast(this);
}

bool UStomtConfig::GetSubscribed()
{	
	return this->bSubscribed;
}

void UStomtConfig::SetSubscribed(bool bNewSubscribed)
{
	if (this->bSubscribed == bNewSubscribed) return;

	this->bSubscribed = bNewSubscribed;

	this->SaveFlag(this->SubscribedFieldName, this->bSubscribed);

	OnConfigUpdated.Broadcast(this);
}

bool UStomtConfig::GetLoggedIn()
{
	return this->bLoggedIn;
}

void UStomtConfig::SetLoggedIn(bool bNewLoggedIn)
{
	if (this->bLoggedIn == bNewLoggedIn) return;

	this->bLoggedIn = bNewLoggedIn;

	this->SaveFlag(this->LoggedInFieldName, this->bLoggedIn);

	OnConfigUpdated.Broadcast(this);
}

bool UStomtConfig::GetAcceptScreenshotUpload()
{
	return this->bAcceptScreenshotUpload;
}

void UStomtConfig::SetAcceptScreenshotUpload(bool bNewAcceptScreenshotUpload)
{
	if (this->bAcceptScreenshotUpload == bNewAcceptScreenshotUpload) return;

	this->bAcceptScreenshotUpload = bNewAcceptScreenshotUpload;

	this->SaveFlag(this->ScreenshotUploadFieldName, this->bAcceptScreenshotUpload);

	OnConfigUpdated.Broadcast(this);
}

bool UStomtConfig::GetAcceptLogUpload()
{
	return this->bAcceptLogUpload;
}

void UStomtConfig::SetAcceptLogUpload(bool bNewAcceptLogUpload)
{
	if (this->bAcceptLogUpload == bNewAcceptLogUpload) return;

	this->bAcceptLogUpload = bNewAcceptLogUpload;

	this->SaveFlag(this->LogUploadFieldName, this->bAcceptLogUpload);

	OnConfigUpdated.Broadcast(this);
}

TArray<UStomtJsonValue*> UStomtConfig::GetStomtsAsJson()
{
	UStomtRestJsonObject* JsonObj = ReadStomtConfAsJson();

	if (JsonObj->HasField(this->StomtsFieldName))
	{
		return JsonObj->GetArrayField(this->StomtsFieldName);
	}
	else
	{
		return TArray<UStomtJsonValue*>();
	}
}

TArray<UStomt*> UStomtConfig::GetStomts()
{
	TArray<UStomt*> StomtObjects;
	UStomtRestJsonObject* JsonObj = ReadStomtConfAsJson();

	if (JsonObj->HasField(this->StomtsFieldName))
	{
		for (UStomtRestJsonObject* JObjStomt : JsonObj->GetObjectArrayField(this->StomtsFieldName))
		{
			UStomt* StomtObject = UStomt::ConstructStomt(
				JObjStomt->GetStringField("target_id"), 
				JObjStomt->GetBoolField("positive"),
				JObjStomt->GetStringField("text")	
			);

			StomtObjects.Add(StomtObject);
		}
	}
	else
	{
		return TArray<UStomt*>();
	}

	return StomtObjects;
}

bool UStomtConfig::AddStomt(UStomt* NewStomt)
{
	return SaveStomtToConf(*NewStomt);
}

bool UStomtConfig::SaveAccessToken(FString NewAccessToken)
{
	return SaveValueToStomtConf(this->AccessTokenFieldName, NewAccessToken);
}

bool UStomtConfig::SaveValueToStomtConf(FString FieldName, FString FieldValue)
{
	UStomtRestJsonObject* JsonObj = ReadStomtConfAsJson();

	if (JsonObj->HasField(FieldName))
	{
		if (JsonObj->GetStringField(FieldName).Equals(FieldValue))
		{
			return false;
		}

		JsonObj->RemoveField(FieldName);
	}

	JsonObj->SetStringField(FieldName, FieldValue);

	return this->WriteFile(JsonObj->EncodeJson(), this->ConfigName, this->ConfigFolder, true);
}

bool UStomtConfig::SaveStomtToConf(UStomt& NewStomt)
{
	UStomtRestJsonObject* JsonObj = ReadStomtConfAsJson();

	UStomtRestJsonObject* JObjStomt = UStomtRestJsonObject::ConstructJsonObject(this);
	TArray<UStomtJsonValue*> Labels = TArray<UStomtJsonValue*>();

	for (int i = 0; i != NewStomt.GetLabels().Num(); ++i)
	{
		if (!NewStomt.GetLabels()[i]->GetName().IsEmpty())
		{
			Labels.Add(UStomtJsonValue::ConstructJsonValueString(this, NewStomt.GetLabels()[i]->GetName()));
		}
	}

	JObjStomt->SetField(TEXT("target_id"), UStomtJsonValue::ConstructJsonValueString(this, NewStomt.GetTargetId()));
	JObjStomt->SetField(TEXT("positive"), UStomtJsonValue::ConstructJsonValueBool(this, NewStomt.GetPositive()));
	JObjStomt->SetField(TEXT("text"), UStomtJsonValue::ConstructJsonValueString(this, NewStomt.GetText()));
	JObjStomt->SetField(TEXT("anonym"), UStomtJsonValue::ConstructJsonValueBool(this, NewStomt.GetAnonym()));
	JObjStomt->SetArrayField(TEXT("labels"), Labels);

	if (!JsonObj->HasField(StomtsFieldName))
	{
		TArray<UStomtRestJsonObject*> JObjArray1;
		JObjArray1.Add(JObjStomt);
		JsonObj->SetObjectArrayField(StomtsFieldName, JObjArray1);

		UE_LOG(StomtNetwork, Log, TEXT("Add Offline Stomt: To new field"));
	}
	else
	{
		TArray<UStomtRestJsonObject*> JObjArray2;
		JObjArray2 = JsonObj->GetObjectArrayField(StomtsFieldName);
		JObjArray2.Add(JObjStomt);
		JsonObj->SetObjectArrayField(StomtsFieldName, JObjArray2);
		UE_LOG(StomtNetwork, Log, TEXT("Add Offline Stomt: To existing field"));
	}

	UE_LOG(StomtNetwork, Log, TEXT("Add offline stomt: %s"), *JsonObj->EncodeJson());

	return this->WriteFile(JsonObj->EncodeJson(), this->ConfigName, this->ConfigFolder, true);
}

bool UStomtConfig::ClearStomts()
{
	UStomtRestJsonObject* JsonObj = ReadStomtConfAsJson();

	if (JsonObj->HasField(StomtsFieldName))
	{
		UE_LOG(StomtNetwork, Log, TEXT("Clear offline stomts"));
		JsonObj->GetArrayField(StomtsFieldName).Empty();
		JsonObj->RemoveField(StomtsFieldName);
	}
	else
	{
		return true;
	}

	return this->WriteFile(JsonObj->EncodeJson(), this->ConfigName, this->ConfigFolder, true);
}

bool UStomtConfig::SaveFlag(FString FlagName, bool bFlagState)
{
	return SaveValueToStomtConf(FlagName, bFlagState ? TEXT("true") : TEXT("false"));
}

FString UStomtConfig::ReadStomtConf(FString FieldName)
{
	FString Result;

	if (this->ReadFile(Result, this->ConfigName, this->ConfigFolder))
	{
		UStomtRestJsonObject* JsonObj = UStomtRestJsonObject::ConstructJsonObject(this);
		JsonObj->DecodeJson(Result);
		this->AccessToken = JsonObj->GetField(FieldName)->AsString();
	}

	return Result;
}

bool UStomtConfig::ReadFlag(FString FlagName)
{
	FString Result;
	bool bFlagState = false;

	if (this->ReadFile(Result, this->ConfigName, this->ConfigFolder))
	{
		UStomtRestJsonObject* JsonObj = UStomtRestJsonObject::ConstructJsonObject(this);
		JsonObj->DecodeJson(Result);
		bFlagState = JsonObj->GetField(FlagName)->AsBool();
	}

	return bFlagState;
}

UStomtRestJsonObject* UStomtConfig::ReadStomtConfAsJson()
{
	FString Result;
	UStomtRestJsonObject* JsonObj = UStomtRestJsonObject::ConstructJsonObject(this);

	if (this->ReadFile(Result, this->ConfigName, this->ConfigFolder))
	{
		JsonObj->DecodeJson(Result);
	}

	return JsonObj;
}

FString UStomtConfig::ReadAccessToken()
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
	return this->WriteFile(StomtConf->EncodeJson(), this->ConfigName, this->ConfigFolder, true);
}

void UStomtConfig::DeleteStomtConf()
{
	FString File = this->ConfigFolder + this->ConfigName;
	if (!FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*File))
	{
		UE_LOG(StomtFileAccess, Warning, TEXT("Could not delete stomt.conf.json: %s"), *File);
	}
	else
	{
		UE_LOG(StomtFileAccess, Warning, TEXT("Deleted stomt.conf.json because of wrong access token Path: %s"), *File);
	}
}

FString UStomtConfig::ReadLogFile(FString LogFileName)
{
	FString ErrorLog;

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
	if (!this->ReadFile(ErrorLog, LogFileCopyName, FPaths::ProjectLogDir()))
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

	return ErrorLog;
}

bool UStomtConfig::WriteFile(FString TextToSave, FString FileName, FString SaveDirectory, bool bAllowOverwriting)
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
		if (bAllowOverwriting || !FPaths::FileExists(*AbsoluteFilePath))
		{
			// Use " FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);" for append
			return FFileHelper::SaveStringToFile(TextToSave, *AbsoluteFilePath);
		}
	}
	
	return false;
}

bool UStomtConfig::ReadFile(FString& Result, FString FileName, FString SaveDirectory)
{
	FString Path = SaveDirectory + FileName;

	if (!FPaths::FileExists(Path))
	{
		UE_LOG(StomtFileAccess, Warning, TEXT("File with this path does not exist: %s "), *Path);
	}

	return FFileHelper::LoadFileToString(Result, *Path);
}
