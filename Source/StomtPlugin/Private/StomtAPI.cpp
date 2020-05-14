// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtAPI.h"
#include "StomtPluginPrivatePCH.h"
#include "StomtJsonObject.h"

#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "Runtime/Core/Public/Misc/App.h"
#include "Runtime/Core/Public/Internationalization/Regex.h"

UStomtAPI* UStomtAPI::ConstructStomtAPI(FString NewAppId)
{
	UStomtAPI* Api = NewObject<UStomtAPI>();

	Api->SetAppId(NewAppId);

	UE_LOG(StomtInit, Log, TEXT("Construct Stomt API"));
	UE_LOG(StomtInit, Log, TEXT("AppId: %s "), *Api->GetAppId());
	UE_LOG(StomtInit, Log, TEXT("RestUrl: %s "), *Api->GetRestUrl());

	Api->LoadLanguageFile();

	UE_LOG(StomtInit, Log, TEXT("LangTest: %s"), *Api->GetLangText("SDK_STOMT_WISH_BUBBLE"));

	return Api;
}

UStomtAPI::UStomtAPI()
{
	this->bLogFileWasSend = false;
	this->bEMailFlagWasSend = false;
	this->bIsImageUploadComplete = false;
	this->bIsLogUploadComplete = false;
	this->bUseImageUpload = true;
	this->bUseDefaultLabels = true;
	this->bNetworkError = false;

	this->Config = UStomtConfig::ConstructStomtConfig();
	this->Track = UStomtTrack::ConstructStomtTrack();
	this->SetCurrentLanguage(this->GetSystemLanguage());
}

UStomtAPI::~UStomtAPI()
{
}

void UStomtAPI::SendStomt(UStomt* NewStomt)
{
	if (!this->IsConnected())
	{
		this->Config->AddStomt(NewStomt);
	}

	UStomtRestRequest* Request = this->SetupNewPostRequest();
	Request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnSendStomtRequestResponse);

	// Fields
	Request->GetRequestObject()->SetField(TEXT("target_id"), UStomtJsonValue::ConstructJsonValueString(this, NewStomt->GetTargetId()));
	Request->GetRequestObject()->SetField(TEXT("positive"), UStomtJsonValue::ConstructJsonValueBool(this, NewStomt->GetPositive()));
	Request->GetRequestObject()->SetField(TEXT("text"), UStomtJsonValue::ConstructJsonValueString(this, NewStomt->GetText()));
	Request->GetRequestObject()->SetField(TEXT("anonym"), UStomtJsonValue::ConstructJsonValueBool(this, NewStomt->GetAnonym()));
	
	//Labels
	UStomtRestJsonObject* JObjExtraData = UStomtRestJsonObject::ConstructJsonObject(this);
	TArray<UStomtJsonValue*> Labels = TArray<UStomtJsonValue*>();

	for (int i = 0; i != NewStomt->GetLabels().Num(); ++i)
	{
		if (!NewStomt->GetLabels()[i]->GetName().IsEmpty())
		{
			Labels.Add(UStomtJsonValue::ConstructJsonValueString(this, NewStomt->GetLabels()[i]->GetName()));
		}
	}

	if (this->bUseDefaultLabels)
	{
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		Labels.Add(UStomtJsonValue::ConstructJsonValueString(this, ViewportSize.ToString()));
		Labels.Add(UStomtJsonValue::ConstructJsonValueString(this, UGameplayStatics::GetPlatformName()));
	}

	if (Labels.Num() > 0)
	{
		JObjExtraData->SetArrayField(TEXT("labels"), Labels);
		Request->GetRequestObject()->SetObjectField(TEXT("extradata"), JObjExtraData);
	}

	if (this->CustomKeyValuePairs.Num() > 0)
	{
		for (int i = 0; i != this->CustomKeyValuePairs.Num(); ++i)
		{
			JObjExtraData->SetStringField(this->CustomKeyValuePairs[i][0], this->CustomKeyValuePairs[i][1]);
		}	
	}

	// Stomt Image
	if (!this->ImageUploadName.IsEmpty() && this->bUseImageUpload)
	{
		Request->GetRequestObject()->SetStringField(TEXT("img_name"), this->ImageUploadName);
		UE_LOG(StomtNetwork, Log, TEXT("Append Image"));
	}
	else
	{
		UE_LOG(StomtNetwork, Log, TEXT("Append no Image (Don't use image upload)"));
	}

	// Error Logs
	if (!this->ErrorLogFileUid.IsEmpty())
	{
		UStomtRestJsonObject* JObjFile = UStomtRestJsonObject::ConstructJsonObject(this);
		UStomtRestJsonObject* JObjFileContext = UStomtRestJsonObject::ConstructJsonObject(this);
		JObjFileContext->SetField(TEXT("file_uid"), UStomtJsonValue::ConstructJsonValueString(this, this->ErrorLogFileUid));

		JObjFile->SetObjectField(TEXT("stomt"), JObjFileContext);
		Request->GetRequestObject()->SetObjectField(TEXT("files"), JObjFile);
	}

	Request->ProcessUrl(this->GetRestUrl().Append(TEXT("/stomts")));
}

void UStomtAPI::OnSendStomtRequestResponse(UStomtRestRequest * Request)
{
	if (Request->GetResponseCode() == 200)
	{
		if (Request->GetResponseObject()->HasField(TEXT("data")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("id")))
			{
				FString Id = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetStringField(TEXT("id"));
				this->Track->SetStomtId(Id);
				this->Track->SetEventCategory("stomt");
				this->Track->SetEventAction("submit");
				this->SendTrack(this->Track);
			}
		}
	}
	else
	{
		// DoTo react to this
		UE_LOG(StomtNetwork, Warning, TEXT("Send Stomt did not work | OnSendStomtRequestResponse"));
	}
}

UStomtRestRequest* UStomtAPI::SendLoginRequest(FString UserName, FString Password)
{
	UStomtRestRequest* Request = this->SetupNewPostRequest();
	Request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnLoginRequestResponse);
	Request->OnRequestFail.AddDynamic(this, &UStomtAPI::OnARequestFailed);

	Request->UseRequestLogging(false);

	Request->GetRequestObject()->SetStringField(TEXT("login_method"), TEXT("normal"));
	Request->GetRequestObject()->SetStringField(TEXT("emailusername"), UserName);
	Request->GetRequestObject()->SetStringField(TEXT("password"), Password);

	Request->ProcessUrl(this->GetRestUrl().Append(TEXT("/authentication/session")));

	Request->UseRequestLogging(true);

	return Request;
}

void UStomtAPI::OnLoginRequestResponse(UStomtRestRequest * Request)
{
	if (Request->GetResponseCode() == 200)
	{
		if (Request->GetResponseObject()->HasField(TEXT("data")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("accesstoken")))
			{
				FString Accesstoken = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetStringField(TEXT("accesstoken"));
				this->Config->SetAccessToken(Accesstoken);
				this->Config->SetSubscribed(true);
				this->Config->SetLoggedIn(true);

				this->Track->SetEventCategory("auth");
				this->Track->SetEventAction("login");
				this->SendTrack(this->Track);
			}
		}
	}
}

UStomtRestRequest* UStomtAPI::RequestSession(FString Accesstoken)
{
	if (Accesstoken.IsEmpty())
	{
		return NULL;
	}

	UStomtRestRequest* Request = NewObject<UStomtRestRequest>();
	Request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnRequestSessionResponse);
	Request->OnRequestFail.AddDynamic(this, &UStomtAPI::OnARequestFailed);

	Request->SetVerb(StomtEnumRequestVerb::GET);
	Request->SetHeader(TEXT("appid"), this->GetAppId());
	
	Request->SetHeader(TEXT("accesstoken"), this->Config->GetAccessToken());
	UE_LOG(StomtNetwork, Log, TEXT("RequestSession: AddAccesstoken: %s "), *this->Config->GetAccessToken());

	Request->ProcessUrl(this->GetRestUrl().Append("/authentication/session"));

	return Request;
}

void UStomtAPI::OnRequestSessionResponse(UStomtRestRequest * Request)
{
	if (Request->GetResponseCode() == 419)
	{
		// Forbidden: Session invalid. (Request a new access-token via login or refresh token.)

		UE_LOG(StomtNetwork, Warning, TEXT("Invalid accesstoken."));

		Config->SetAccessToken("");
	}

	if (Request->GetResponseCode() != 200) return;

	if (!Request->GetResponseObject()->HasField(TEXT("data"))) return;

	if (!Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("user"))) return;

	this->StomtsCreatedByUser = (int)Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetObjectField(TEXT("user"))->GetObjectField(TEXT("stats"))->GetNumberField(TEXT("amountStomtsCreated"));
	this->UserId = (FString)Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetObjectField(TEXT("user"))->GetStringField("id");

	OnSessionRequestComplete.Broadcast(Request);

	UE_LOG(StomtNetwork, Log, TEXT("StomtsCreatedByUser: %d | StomtsReceivedByTarget: %d"), StomtsCreatedByUser, StomtsReceivedByTarget);
}

UStomtRestRequest* UStomtAPI::RequestTargetByAppId()
{
	UStomtRestRequest* Request = NewObject<UStomtRestRequest>();
	Request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnRequestTargetResponse);
	Request->OnRequestFail.AddDynamic(this, &UStomtAPI::OnARequestFailed);

	Request->SetVerb(StomtEnumRequestVerb::GET);
	Request->SetHeader(TEXT("appid"), this->GetAppId());

	Request->ProcessUrl(this->GetRestUrl().Append("/targets/"));

	return Request;
}

UStomtRestRequest* UStomtAPI::RequestTarget(FString TargetId)
{
	UStomtRestRequest* Request = NewObject<UStomtRestRequest>();
	Request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnRequestTargetResponse);
	Request->OnRequestFail.AddDynamic(this, &UStomtAPI::OnARequestFailed);

	Request->SetVerb(StomtEnumRequestVerb::GET);
	Request->SetHeader(TEXT("appid"), this->GetAppId());

	Request->ProcessUrl(this->GetRestUrl().Append("/targets/").Append(TargetId));

	return Request;
}

void UStomtAPI::OnRequestTargetResponse(UStomtRestRequest * Request)
{
	if (Request->GetResponseCode() != 200) return;

	if (!Request->GetResponseObject()->HasField(TEXT("data"))) return;
	
	if (!Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("displayname"))) return;
	
	this->TargetName = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetStringField(TEXT("displayname"));
	this->TargetId = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetStringField(TEXT("id"));
	this->SetImageUrl(Request->GetResponseObject()
		->GetObjectField(TEXT("data"))
		->GetObjectField(TEXT("images"))
		->GetObjectField(TEXT("profile"))
		->GetStringField(TEXT("url")));

	this->StomtsReceivedByTarget = (int)Request->GetResponseObject()
		->GetObjectField(TEXT("data"))
		->GetObjectField(TEXT("stats"))
		->GetNumberField(TEXT("amountStomtsReceived"));

	if (!this->Config->GetAccessToken().IsEmpty())
	{
		this->RequestSession(this->Config->GetAccessToken());
	}

	this->bNetworkError = false;
}

void UStomtAPI::SetRestUrl(FString NewRestUrl)
{
	this->RestUrl = NewRestUrl;
}

FString UStomtAPI::GetRestUrl()
{
	return this->RestUrl;
}

void UStomtAPI::SetStomtUrl(FString NewStomtUrl)
{
	this->StomtUrl = NewStomtUrl;
}

FString UStomtAPI::GetStomtUrl()
{
	return this->StomtUrl;
}

void UStomtAPI::SetAppId(FString NewAppId)
{
	if (NewAppId.Equals("Copy_your_AppId_here") || NewAppId.Equals("AKN5M7Ob0MqxKXYdE9i3IhQtF") || NewAppId.Equals(""))
	{
		this->AppId = "AKN5M7Ob0MqxKXYdE9i3IhQtF";
		this->SetRestUrl("https://test.rest.stomt.com");
		this->SetStomtUrl("https://test.stomt.com/");
	}
	else
	{
		this->AppId = NewAppId;
		this->SetRestUrl("https://rest.stomt.com");
		this->SetStomtUrl("https://www.stomt.com/");
	}	
}

FString UStomtAPI::GetAppId()
{
	return this->AppId;
}

FString UStomtAPI::GetTargetName()
{
	return this->TargetName;
}

void UStomtAPI::SetTargetId(FString NewTargetId)
{
	this->TargetId = NewTargetId;
}

FString UStomtAPI::GetTargetId()
{
	return this->TargetId;
}

void UStomtAPI::SetImageUrl(FString NewImageUrl)
{
	this->ImageUrl = NewImageUrl;
}

FString UStomtAPI::GetImageUrl()
{
	return this->ImageUrl;
}

void UStomtAPI::SetStomtToSend(UStomt * NewStomt)
{
	this->StomtToSend = NewStomt;
}

FString UStomtAPI::ReadLogFile(FString LogFileName)
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
		return TEXT("");
	}

	// Read LogFileCopy from Disk
	if (!this->ReadFile(ErrorLog, LogFileCopyName, FPaths::ProjectLogDir() ))
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

void UStomtAPI::SendLogFile(FString LogFileData, FString LogFileName)
{
	if (LogFileData.IsEmpty())
	{
		this->bIsLogUploadComplete = true;
		return;
	}

	UStomtRestRequest* Request = this->SetupNewPostRequest();
	Request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnSendLogFileResponse);

	FString LogJsonString = FString(TEXT("{ \"files\": { \"stomt\": [ { \"data\":\"") + FBase64::Encode(LogFileData) + TEXT("\", \"filename\" : \"") + LogFileName + TEXT("\" } ] } }"));

	Request->UseStaticJsonString(true);
	Request->SetStaticJsonString(LogJsonString);

	Request->ProcessUrl(this->GetRestUrl().Append(TEXT("/files")));

	this->bLogFileWasSend = true;
}

void UStomtAPI::OnSendLogFileResponse(UStomtRestRequest * Request)
{
	// Request access token
	if (this->Config->GetAccessToken().IsEmpty())
	{
		if (Request->GetResponseObject()->HasField(TEXT("meta")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("meta"))->HasField(TEXT("accesstoken")))
			{
				FString Accesstoken = Request->GetResponseObject()->GetObjectField(TEXT("meta"))->GetStringField(TEXT("accesstoken"));
				this->Config->SetAccessToken(Accesstoken);
			}
		}
	}

	// Get File uid of the error log and send stomt
	if (Request->GetResponseObject()->HasField(TEXT("data")))
	{
		if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("files")))
		{
			this->ErrorLogFileUid = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetObjectField(TEXT("files"))->GetObjectField(TEXT("stomt"))->GetStringField("file_uid");
			this->bLogFileWasSend = false;

			if (this->bIsImageUploadComplete)
			{
				this->SendStomt(this->StomtToSend);
				UE_LOG(StomtNetwork, Log, TEXT("Sent Stomt after sending log files"));
			}
			else
			{
				if (!this->bUseImageUpload)
				{
					this->SendStomt(this->StomtToSend);
					UE_LOG(StomtNetwork, Log, TEXT("Sent Stomt after sending log files | ImageUpload disabled"));
				}
				else
				{
					UE_LOG(StomtNetwork, Log, TEXT("Did not send stomt | image upload not complete"));
				}		
			}
		}
	}

	if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 419 || Request->GetResponseCode() == 413)
	{
		if (this->StomtToSend != NULL)
		{
			this->SendStomt(this->StomtToSend);
			this->bLogFileWasSend = false;
		}
	}

	this->bIsLogUploadComplete = true;
}

void UStomtAPI::SendImageFile(FString ImageFileDataBase64)
{
	if (ImageFileDataBase64.IsEmpty())
	{
		UE_LOG(StomtNetwork, Warning, TEXT("Could not send stomt image | ImageFileDataBase64 was empty"));
		return;
	}

	UStomtRestRequest* Request = this->SetupNewPostRequest();
	Request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnSendImageFileResponse);

	FString ImageJson = FString(TEXT("{ \"images\": { \"stomt\": [ { \"data\":\"") + ImageFileDataBase64 + TEXT("\" } ] } }"));

	Request->UseStaticJsonString(true);
	Request->SetStaticJsonString(ImageJson);

	Request->ProcessUrl(this->GetRestUrl().Append(TEXT("/images")));

}

void UStomtAPI::OnSendImageFileResponse(UStomtRestRequest * Request)
{
	// Request access token
	if (this->Config->GetAccessToken().IsEmpty())
	{
		if (Request->GetResponseObject()->HasField(TEXT("meta")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("meta"))->HasField(TEXT("accesstoken")))
			{
				FString Accesstoken = Request->GetResponseObject()->GetObjectField(TEXT("meta"))->GetStringField(TEXT("accesstoken"));
				this->Config->SetAccessToken(Accesstoken);
			}
		}
	}

	// Get File uid of the error log and send stomt
	if (Request->GetResponseObject()->HasField(TEXT("data")))
	{
		if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("images")))
		{
			this->ImageUploadName = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetObjectField(TEXT("images"))->GetObjectField(TEXT("stomt"))->GetStringField("name");
			UE_LOG(StomtFileAccess, Log, TEXT("Image Upload complete %s"), *this->ImageUploadName);
		}
	}

	if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 419 || Request->GetResponseCode() == 413)
	{
		if (this->StomtToSend != NULL)
		{
			//this->SendStomt(this->StomtToSend);
			//this->bLogFileWasSend = false;
		}
	}

	if (this->bIsLogUploadComplete)
	{
		this->SendStomt(this->StomtToSend);
		UE_LOG(StomtNetwork, Log, TEXT("Sent Stomt after sending screenshot file"));
	}
	else
	{
		UE_LOG(StomtNetwork, Log, TEXT("Did not send stomt | log upload not complete"));
	}

	this->bIsImageUploadComplete = true;
}

void UStomtAPI::SendSubscription(FString EMail)
{
	SendSubscription(EMail, false);
}

void UStomtAPI::SendSubscription(FString EMailOrNumber, bool bUseEmail)
{
	UStomtRestRequest* Request = this->SetupNewPostRequest();
	Request->OnRequestComplete.AddDynamic(this, &UStomtAPI::UStomtAPI::OnSendEMailResponse);

	if (bUseEmail)
	{
		Request->GetRequestObject()->SetStringField(TEXT("email"), EMailOrNumber);
	}
	else
	{
		Request->GetRequestObject()->SetStringField(TEXT("phone"), EMailOrNumber);
	}

	Request->GetRequestObject()->SetStringField(TEXT("message"), this->GetLangText("SDK_SUBSCRIBE_GET_NOTIFIED"));

	Request->ProcessUrl(this->GetRestUrl().Append(TEXT("/authentication/subscribe")));
}

void UStomtAPI::OnSendEMailResponse(UStomtRestRequest * Request)
{
	if (Request->GetResponseCode() == 200)
	{
		if (Request->GetResponseObject()->HasField(TEXT("data")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("success")))
			{
				if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetBoolField("success"))
				{
					this->Config->SetSubscribed(true);
					this->Track->SetEventCategory("auth");
					this->Track->SetEventAction("subscribed");
					this->SendTrack(this->Track);
				}
			}
		}
	}
	else
	{
		this->Config->SetSubscribed(false);
	}
}

void UStomtAPI::SendLogoutRequest()
{
	UStomtRestRequest* Request = this->SetupNewDeleteRequest();
	Request->OnRequestComplete.AddDynamic(this, &UStomtAPI::UStomtAPI::OnSendLogoutResponse);

	Request->ProcessUrl(this->GetRestUrl().Append(TEXT("/authentication/session")));

	this->Config->SetAccessToken(TEXT(""));
	this->Config->SetLoggedIn(false);
	this->Config->SetSubscribed(false);
}

void UStomtAPI::OnSendLogoutResponse(UStomtRestRequest * Request)
{
	if (Request->GetResponseCode() == 200)
	{
		if (Request->GetResponseObject()->HasField(TEXT("data")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("success")))
			{
				if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetBoolField("success"))
				{
					return; // logout was successful
				}
			}
		}
	}

	UE_LOG(StomtNetwork, Warning, TEXT("Logout failed"));
}

void UStomtAPI::SendTrack(UStomtTrack * NewTrack)
{
	UStomtRestRequest* Request = this->SetupNewPostRequest();

	// Add target id
	NewTrack->SetTargetId(this->GetTargetId());
	
	UStomtRestJsonObject* JObjTrack = NewTrack->GetAsJsonObject();

	if (JObjTrack != NULL)
	{
		if (!JObjTrack->IsValidLowLevel())
		{
			UE_LOG(StomtNetwork, Warning, TEXT("SendTrack: track not valid"));
			return;
		}
	}
	else
	{
		UE_LOG(StomtNetwork, Warning, TEXT("SendTrack: track was null"));
		return;
	}

	Request->SetRequestObject(JObjTrack);

	Request->ProcessUrl(this->GetRestUrl().Append(TEXT("/tracks")));
}

FString UStomtAPI::ReadScreenshotAsBase64()
{
	FString ScreenDir = FPaths::ScreenShotDir();
	FString FilePath = ScreenDir + this->DefaultScreenshotName;

	UE_LOG(StomtLog, Log, TEXT("TakeScreenshot | FilePath: %s"), *FilePath);
	UE_LOG(StomtLog, Log, TEXT("Screenshot | AllocatedSize: %d"), this->ReadBinaryFile(FilePath).GetAllocatedSize());

	TArray<uint8> File = this->ReadBinaryFile(FilePath);

	//Delete Screenshot
	FString AbsoluteFilePath = ScreenDir + this->DefaultScreenshotName;
	if (!FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*AbsoluteFilePath))
	{
		UE_LOG(StomtFileAccess, Warning, TEXT("Could not delete old screenshot File (Could Not Find Screenshot File)") );
	}

	return FBase64::Encode(File);
}

void UStomtAPI::OnARequestFailed(UStomtRestRequest * Request)
{
	this->bNetworkError = true;
	this->OnRequestFailed.Broadcast(Request);
}

bool UStomtAPI::IsConnected()
{
	return !bNetworkError;
}

void UStomtAPI::ConnectionTest()
{
	this->RequestTargetByAppId();
}


UStomtRestJsonObject* UStomtAPI::LoadLanguageFile()
{
	FString JsonString = this->LoadLanguageFileContent();
	UStomtRestJsonObject* JsonObject = UStomtRestJsonObject::ConstructJsonObject(this);
	if (JsonObject->DecodeJson(JsonString))
	{
		this->Languages = JsonObject;
	}
	else
	{
		UE_LOG(StomtInit, Error, TEXT("Could not decode Language File StomtPlugin(Sub)/Resources/languages.json"));
	}

	return JsonObject;
}

FString UStomtAPI::LoadLanguageFileContent()
{
	TArray<FString> PluginFolders;
	PluginFolders.Add(FPaths::EnginePluginsDir() + "Marketplace/");
	PluginFolders.Add(FPaths::ProjectPluginsDir());
	PluginFolders.Add(FPaths::ProjectPluginsDir()); 
	PluginFolders.Add(FPaths::EnterprisePluginsDir());

	TArray<FString> PluginNames;
	PluginNames.Add("StomtPlugin/");
	PluginNames.Add("StomtPluginSub/");
	PluginNames.Add("stomt-unreal-plugin/");

	FString LocalFolder = "Resources/";

	FString FileName = "languages.json";

	FString WorkingPath = "";

	for (auto& StrFolder : PluginFolders)
	{
		for (auto& StrPluginName : PluginNames)
		{
			if (FPaths::FileExists(StrFolder + StrPluginName + LocalFolder + FileName))
			{
				WorkingPath = StrFolder + StrPluginName + LocalFolder;
				UE_LOG(StomtInit, Log, TEXT("Using language file in path: %s"), *WorkingPath);
			}
		}
	}

	FString JsonString = "";

	if( this->ReadFile(JsonString, FileName, WorkingPath) )
	{
		return JsonString;
	}

	if (WorkingPath.IsEmpty())
	{
		UE_LOG(StomtInit, Warning, TEXT("Language file not found"));

		JsonString = "{\"data\":{\"de\": {\"SDK_STOMT_WISH_BUBBLE\": \"Ich wünschte\",\"SDK_STOMT_LIKE_BUBBLE\": \"Ich mag\",\"SDK_STOMT_DEFAULT_TEXT_WISH\": \"würde\",\"SDK_STOMT_DEFAULT_TEXT_LIKE\": \"weil\",\"SDK_STOMT_PLACEHOLDER\": \"...bitte beende diesen Satz\",\"SDK_STOMT_ERROR_MORE_TEXT\": \"Bitte schreibe etwas mehr.\",\"SDK_STOMT_ERROR_LESS_TEXT\": \"Nutze nur {0} Zeichen.\", \"SDK_STOMT_SCREENSHOT\": \"Screenshot\",\"SDK_HEADER_TARGET_STOMTS\": \"STOMTS\",\"SDK_HEADER_YOUR_STOMTS\": \"DEINE\",\"SDK_SUBSCRIBE_GET_NOTIFIED\": \"Werde benachrichtigt, wenn jemand reagiert.\",\"SDK_SUBSCRIBE_TOGGLE_EMAIL\": \"E-Mail\",\"SDK_SUBSCRIBE_TOGGLE_PHONE\": \"SMS\",\"SDK_SUBSCRIBE_EMAIL_QUESTION\": \"Wie lautet deine E-Mail-Adresse?\",\"SDK_SUBSCRIBE_PHONE_QUESTION\": \"Wie lautet deine Telefonnummer?\",\"SDK_SUBSCRIBE_PHONE_PLACEHOLDER\" : \"+49 152 03959902\",\"SDK_SUBSCRIBE_EMAIL_PLACEHOLDER\" : \"deine@email.com\",\"SDK_SUBSCRIBE_DEFAULT_PLACEHOLDER\" : \"Wir senden dir keine Spam Nachrichten\",\"SDK_SUBSCRIBE_VALID_EMAIL\" : \"E-Mail-Adresse ist gültig, fantastisch!\",\"SDK_SUBSCRIBE_NO_VALID_EMAIL\" : \"Bitte gebe eine gültige Adresse ein.\",\"SDK_SUBSCRIBE_SKIP\": \"Überspringen\",\"SDK_SUCCESS_THANK_YOU\": \"DANKE!\",\"SDK_SUCCESS_FIND_ALL_STOMTS\": \"Super, finde mehr Wünsche auf\",\"SDK_SUCCESS_FIND_YOUR_STOMTS\": \"Finde deine stomts\",\"SDK_SUCCESS_CREATE_NEW_WISH\": \"Wünsch dir noch was\",\"SDK_NETWORK_NOT_CONNECTED\": \"Verbindung zu STOMT unterbrochen\",\"SDK_NETWORK_NO_INTERNET\": \"Keine Internetverbindung\",\"SDK_NETWORK_RECONNECT\": \"Wiederverbinden\",\"SDK_LOGIN_ACCOUNT_WRONG\": \"Account Name stimmt nicht.\",\"SDK_LOGIN_PASSWORD_WRONG\": \"Passwort stimmt nicht.\",\"SDK_LOGIN_PASSWORD\": \"Passwort\",\"SDK_LOGIN\": \"Login\",\"SDK_LOGIN_SIGNUP\": \"Anmelden\",\"SDK_LOGIN_LOGOUT\": \"Ausloggen\",\"SDK_LOGIN_FORGOT_PW\": \"Passwort vergessen?\",\"SDK_LOGIN_SUCCESS\": \"Du bist eingeloggt!\",\"SDK_LOGIN_WENT_WRONG\": \"Ups, da lief was schief.\"},\"en\": {\"SDK_STOMT_WISH_BUBBLE\": \"I wish\",\"SDK_STOMT_LIKE_BUBBLE\": \"I like\",\"SDK_STOMT_DEFAULT_TEXT_WISH\": \"would\",\"SDK_STOMT_DEFAULT_TEXT_LIKE\": \"because\",\"SDK_STOMT_PLACEHOLDER\": \"...please finish the sentence\",\"SDK_STOMT_ERROR_MORE_TEXT\": \"Please write a bit more.\",\"SDK_STOMT_ERROR_LESS_TEXT\": \"Use only {0} characters.\",   \"SDK_STOMT_SCREENSHOT\": \"Screenshot\",\"SDK_HEADER_TARGET_STOMTS\": \"STOMTS\",\"SDK_HEADER_YOUR_STOMTS\": \"YOURS\",\"SDK_SUBSCRIBE_GET_NOTIFIED\": \"Get notified when someone reacts.\",\"SDK_SUBSCRIBE_TOGGLE_EMAIL\": \"E-Mail\",\"SDK_SUBSCRIBE_TOGGLE_PHONE\": \"SMS\",\"SDK_SUBSCRIBE_EMAIL_QUESTION\": \"What's your email address?\",\"SDK_SUBSCRIBE_PHONE_QUESTION\": \"What's your phone number?\",\"SDK_SUBSCRIBE_PHONE_PLACEHOLDER\" : \"+1-541-754-3010\",\"SDK_SUBSCRIBE_EMAIL_PLACEHOLDER\" : \"your@gmail.com\",\"SDK_SUBSCRIBE_DEFAULT_PLACEHOLDER\" : \"We won’t share your contact nor spam you.\",\"SDK_SUBSCRIBE_VALID_EMAIL\" : \"Email address is valid, fantastic!\",\"SDK_SUBSCRIBE_NO_VALID_EMAIL\" : \"Please type in a valid address.\",\"SDK_SUBSCRIBE_SKIP\": \"Skip\",\"SDK_SUCCESS_THANK_YOU\": \"THANK YOU!\",\"SDK_SUCCESS_FIND_ALL_STOMTS\": \"Amazing, find more wishes on\",\"SDK_SUCCESS_FIND_YOUR_STOMTS\": \"click to find your stomts here\",\"SDK_SUCCESS_CREATE_NEW_WISH\": \"Create another wish\",\"SDK_NETWORK_NOT_CONNECTED\": \"Could not connect to STOMT\",\"SDK_NETWORK_NO_INTERNET\": \"No internet connection\",\"SDK_NETWORK_RECONNECT\": \"Reconnect\",\"SDK_LOGIN_ACCOUNT_WRONG\": \"Account was incorrect.\",\"SDK_LOGIN_PASSWORD_WRONG\": \"Password was incorrect.\",\"SDK_LOGIN_PASSWORD\": \"Password\",\"SDK_LOGIN\": \"Login\",\"SDK_LOGIN_SIGNUP\": \"Signup\",\"SDK_LOGIN_LOGOUT\": \"Logout\",\"SDK_LOGIN_FORGOT_PW\": \"Forgot password?\",\"SDK_LOGIN_SUCCESS\": \"You're now logged in!\",\"SDK_LOGIN_WENT_WRONG\": \"Ups something went wrong.\"}}}";
		return JsonString;
	}

	return JsonString;
}

FString UStomtAPI::GetLangText(FString Text)
{
	if (this->CurrentLanguage.IsEmpty())
	{
		this->CurrentLanguage = "en";
	}

	if (this->Languages != NULL)
	{
		if (!this->Languages->HasField("data"))
		{
			return "";
		}

		if (this->Languages->GetObjectField("data") != NULL)
		{
			if (!this->Languages->GetObjectField("data")->HasField(this->CurrentLanguage))
			{
				UE_LOG(StomtNetwork, Warning, TEXT("Language %s not supported (does not exist in language file) falling back to english."), *this->CurrentLanguage);
				this->CurrentLanguage = "en";
			}

			if (!this->Languages->GetObjectField("data")->GetObjectField(this->CurrentLanguage)->HasField(Text))
			{
				UE_LOG(StomtNetwork, Warning, TEXT("Translation for '%s' not found in language: '%s'."), *Text, *this->CurrentLanguage);
				return "No Transl.";
			}

			return this->Languages->GetObjectField("data")->GetObjectField(this->CurrentLanguage)->GetStringField(Text);
		}
	}

	return FString();
}

FString UStomtAPI::GetCurrentLanguage()
{
	return this->CurrentLanguage;
}

bool UStomtAPI::SetCurrentLanguage(FString Language)
{
	if (Language.IsEmpty())
		return false;

	this->CurrentLanguage = Language.Left(2);

	return true;
}

FString UStomtAPI::GetSystemLanguage()
{
	return FInternationalization::Get().GetCurrentCulture()->GetName().Left(2);
}

bool UStomtAPI::IsEmailCorrect(FString Email)
{
	const FRegexPattern Pattern(TEXT("^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$"));
	FRegexMatcher matcher(Pattern, Email);

	return matcher.FindNext();
}

bool UStomtAPI::DoesScreenshotFileExist()
{
	return FPaths::FileExists(FPaths::ScreenShotDir() + this->DefaultScreenshotName);
}

void UStomtAPI::UseScreenshotUpload(bool bUseUpload)
{
	this->bUseImageUpload = bUseUpload;
}

void UStomtAPI::AddCustomKeyValuePair(FString Key, FString Value)
{
	TArray<FString> Pair = TArray<FString>();
	Pair.Add(Key);
	Pair.Add(Value);

	CustomKeyValuePairs.Add(Pair);
}

void UStomtAPI::HandleOfflineStomts()
{
	if (this->IsConnected())
	{
		UE_LOG(StomtNetwork, Log, TEXT("Stomt API is connected."));
		this->SendOfflineStomts();
	}
	else
	{
		this->Config->AddStomt(this->StomtToSend);
	}
}

void UStomtAPI::SendOfflineStomts()
{
	TArray<UStomt*> Stomts = this->Config->GetStomts();
	if (Stomts.Num() > 0)
	{
		UE_LOG(StomtNetwork, Log, TEXT("Start sending offline stomts: %d"), Stomts.Num());
		for (UStomt* StomtToSend : Stomts)
		{
			UE_LOG(StomtNetwork, Log, TEXT("Sending Offline Stomt"));
			this->SendStomt(StomtToSend);
		}

		this->Config->ClearStomts();
	}
}

bool UStomtAPI::WriteFile(FString TextToSave, FString FileName, FString SaveDirectory, bool bAllowOverwriting)
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
			//Use " FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);" for append
			return FFileHelper::SaveStringToFile(TextToSave, *AbsoluteFilePath);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UStomtAPI::ReadFile(FString& Result, FString FileName, FString SaveDirectory)
{	
	FString Path = SaveDirectory + FileName;

	if (!FPaths::FileExists(Path))
	{
		UE_LOG(StomtFileAccess, Warning, TEXT("File with this path does not exist: %s "), *Path);

		return false;
	}

	return FFileHelper::LoadFileToString( Result, *Path);
}

TArray<uint8> UStomtAPI::ReadBinaryFile(FString FilePath)
{
	TArray<uint8> BufferArray;
	FFileHelper::LoadFileToArray(BufferArray, *FilePath);
	
	return BufferArray;
}

UStomtRestRequest* UStomtAPI::SetupNewPostRequest()
{
	UStomtRestRequest* Request = NewObject<UStomtRestRequest>();
	Request->OnRequestFail.AddDynamic(this, &UStomtAPI::OnARequestFailed);

	Request->SetVerb(StomtEnumRequestVerb::POST);
	Request->SetHeader(TEXT("appid"), this->GetAppId());

	this->AddAccesstokenToRequest(Request);

	return Request;
}

UStomtRestRequest * UStomtAPI::SetupNewDeleteRequest()
{
	UStomtRestRequest* Request = NewObject<UStomtRestRequest>();
	Request->OnRequestFail.AddDynamic(this, &UStomtAPI::OnARequestFailed);

	Request->SetVerb(StomtEnumRequestVerb::DEL);
	Request->SetHeader(TEXT("appid"), this->GetAppId());

	this->AddAccesstokenToRequest(Request);

	return Request;
}

void UStomtAPI::AddAccesstokenToRequest(UStomtRestRequest * Request)
{
	if (!this->Config->GetAccessToken().IsEmpty())
	{
		Request->SetHeader(TEXT("accesstoken"), this->Config->GetAccessToken());
		UE_LOG(StomtNetwork, Log , TEXT("AddAccesstoken: %s "), *this->Config->GetAccessToken());
	}
}
