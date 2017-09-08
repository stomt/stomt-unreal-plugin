// Copyright 2016 Daniel Schukies. All Rights Reserved.

#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtAPI.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapperModule.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "Runtime/Core/Public/Misc/App.h"
#include "StomtJsonObject.h"

UStomtAPI* UStomtAPI::ConstructStomtAPI(FString TargetID, FString RestURL, FString AppID)
{
	UStomtAPI* api = NewObject<UStomtAPI>();
	api->SetAppID(AppID);
	api->SetTargetID(TargetID);
	api->SetRestURL(RestURL);

	return api;
}


UStomtAPI::UStomtAPI()
{
	this->ConfigFolder = FPaths::EngineUserDir() + FString(TEXT("Saved/Config/stomt/"));
	//UE_LOG(LogTemp, Warning, TEXT("configfolder: %s"), *this->ConfigFolder);
	this->ConfigName = FString(TEXT("stomt.conf.json"));
	this->Accesstoken = FString(TEXT(""));
	ReadStomtConf(TEXT("accesstoken"));

	this->Request = NewObject<UStomtRestRequest>();

	this->Request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnReceiving);

	LogFileWasSend = false;
	EMailFlagWasSend = false;
}

UStomtAPI::~UStomtAPI()
{
}

void UStomtAPI::SendStomt(UStomt* stomt)
{
	UStomtRestRequest* request = this->SetupNewPostRequest();
	request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnSendStomtRequestResponse);

	// Fields
	request->GetRequestObject()->SetField(TEXT("target_id"),	UStomtJsonValue::ConstructJsonValueString(	this, stomt->GetTargetID()	));
	request->GetRequestObject()->SetField(TEXT("positive"),	UStomtJsonValue::ConstructJsonValueBool(	this, stomt->GetPositive()	));
	request->GetRequestObject()->SetField(TEXT("text"),		UStomtJsonValue::ConstructJsonValueString(	this, stomt->GetText()		));
	request->GetRequestObject()->SetField(TEXT("anonym"),		UStomtJsonValue::ConstructJsonValueBool(	this, stomt->GetAnonym()	));
	
	//Labels
	UStomtRestJsonObject* jObjExtraData = UStomtRestJsonObject::ConstructJsonObject(this);
	TArray<UStomtJsonValue*> labels = TArray<UStomtJsonValue*>();

	for (int i = 0; i != stomt->GetLabels().Num(); ++i)
	{
		labels.Add(UStomtJsonValue::ConstructJsonValueString(this, stomt->GetLabels()[i]->GetName() ));
	}

	jObjExtraData->SetArrayField(TEXT("labels"), labels);
	request->GetRequestObject()->SetObjectField(TEXT("extradata"), jObjExtraData);

	// Error Logs
	if (!this->errorLog_file_uid.IsEmpty())
	{
		UStomtRestJsonObject* jObjFile = UStomtRestJsonObject::ConstructJsonObject(this);
		UStomtRestJsonObject* jObjFileContext = UStomtRestJsonObject::ConstructJsonObject(this);
		jObjFileContext->SetField(TEXT("file_uid"), UStomtJsonValue::ConstructJsonValueString(this, this->errorLog_file_uid));

		jObjFile->SetObjectField(TEXT("stomt"), jObjFileContext);
		request->GetRequestObject()->SetObjectField(TEXT("files"), jObjFile);
	}
	
	request->ProcessURL( this->GetRestURL().Append(TEXT("/stomts")) );
}

void UStomtAPI::OnSendStomtRequestResponse(UStomtRestRequest * Request)
{
	// DoTo react to this
}

UStomtRestRequest* UStomtAPI::SendLoginRequest(FString UserName, FString Password)
{
	UStomtRestRequest* request = this->SetupNewPostRequest();
	request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnLoginRequestResponse);

	request->UseRequestLogging(false);

	request->GetRequestObject()->SetStringField(TEXT("login_method"), TEXT("normal"));
	request->GetRequestObject()->SetStringField(TEXT("emailusername"), UserName);
	request->GetRequestObject()->SetStringField(TEXT("password"), Password);

	request->ProcessURL(this->GetRestURL().Append(TEXT("/authentication/session")));

	request->UseRequestLogging(true);

	return request;
}

void UStomtAPI::OnLoginRequestResponse(UStomtRestRequest * Request)
{
	if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 404 || Request->GetResponseCode() == 413)
	{
		if (StomtToSend != NULL)
		{
			this->SendStomt(StomtToSend);
		}
	}
	else
	{
		if (Request->GetResponseObject()->HasField(TEXT("data")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("accesstoken")))
			{
				this->Accesstoken = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetStringField(TEXT("accesstoken"));
				this->SaveAccesstoken(this->Accesstoken);
				this->SaveFlag(TEXT("email"), true);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Login did not work"));
			}
		}
	}

	OnLoginRequestComplete.Broadcast(Request);
}

void UStomtAPI::SendStomtLabels(UStomt * stomt)
{
	return;
	//ToDo: Finish this some day. Works only with target owner login.

	/*
	if (!stomt->GetServersideID().IsEmpty() && stomt->GetLabels().Max() > 0)
	{
		//Reset Request
		this->Request->ResetResponseData();
		this->Request->ResetRequestData();

		UE_LOG(LogTemp, Warning, TEXT("nice1"));
		this->Request->SetVerb(ERequestVerb::POST);
		this->Request->SetHeader(TEXT("appid"), this->GetAppID() );

		this->Request->GetRequestObject()->SetField(TEXT("name"),	UStomtJsonValue::ConstructJsonValueString(	this, TEXT("newlabeltest")	));
		this->Request->GetRequestObject()->SetField(TEXT("as_target_owner"), UStomtJsonValue::ConstructJsonValueString(this, TEXT("true")));

		this->Request->ProcessURL( this->GetRestURL().Append(TEXT("/stomts/")).Append(stomt->GetServersideID()).Append(TEXT("/labels") ) );
	}
	*/
}

UStomtRestRequest* UStomtAPI::RequestTarget(FString TargetID)
{
	UStomtRestRequest* request = NewObject<UStomtRestRequest>();
	request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnRequestTargetResponse);

	request->SetVerb(ERequestVerb::GET);
	request->SetHeader(TEXT("appid"), this->GetAppID() );

	request->ProcessURL( this->GetRestURL().Append("/targets/").Append(TargetID) );

	return request;
}

void UStomtAPI::OnRequestTargetResponse(UStomtRestRequest * Request)
{
	if (Request->GetResponseCode() != 200) return;

	if (!Request->GetResponseObject()->HasField(TEXT("data"))) return;
	
	if (!Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("displayname"))) return;
	
	this->TargetName = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetStringField(TEXT("displayname"));
	this->SetImageURL(Request->GetResponseObject()
		->GetObjectField(TEXT("data"))
		->GetObjectField(TEXT("images"))
		->GetObjectField(TEXT("profile"))
		->GetStringField(TEXT("url")));
}

void UStomtAPI::SetRestURL(FString URL)
{
	this->RestURL = URL;
}

FString UStomtAPI::GetRestURL()
{
	return this->RestURL;
}

void UStomtAPI::SetAppID(FString appID)
{
	this->AppID = appID;
}

FString UStomtAPI::GetAppID()
{
	return this->AppID;
}

FString UStomtAPI::GetTargetName()
{
	return this->TargetName;
}

void UStomtAPI::SetTargetID(FString targetID)
{
	this->TargetID = targetID;
}

FString UStomtAPI::GetTargetID()
{
	return this->TargetID;
}

void UStomtAPI::SetImageURL(FString URL)
{
	this->ImageURL = URL;
}

FString UStomtAPI::GetImageURL()
{
	return this->ImageURL;
}

void UStomtAPI::SetStomtToSend(UStomt * stomt)
{
	this->StomtToSend = stomt;
}

UStomtRestRequest * UStomtAPI::GetRequest()
{
	return this->Request;
}

bool UStomtAPI::SaveAccesstoken(FString accesstoken)
{
	return SaveValueToStomtConf(TEXT("accesstoken"), accesstoken);
}

bool UStomtAPI::SaveValueToStomtConf(FString FieldName, FString FieldValue)
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



bool UStomtAPI::SaveFlag(FString FlagName, bool FlagState)
{
	return SaveValueToStomtConf(FlagName, FlagState ? TEXT("true") : TEXT("false"));
}

FString UStomtAPI::ReadStomtConf(FString FieldName)
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

bool UStomtAPI::ReadFlag(FString FlagName)
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

UStomtRestJsonObject* UStomtAPI::ReadStomtConfAsJson()
{
	FString result;
	UStomtRestJsonObject* jsonObj = UStomtRestJsonObject::ConstructJsonObject(this);

	if (this->ReadFile(result, ConfigName, ConfigFolder))
	{
		jsonObj->DecodeJson(result);
	}

	return jsonObj;
}

bool UStomtAPI::WriteStomtConfAsJson(UStomtRestJsonObject * StomtConf)
{
	return this->WriteFile(StomtConf->EncodeJson(), ConfigName, ConfigFolder, true);
}

void UStomtAPI::DeleteStomtConf()
{
	FString file = this->ConfigFolder + this->ConfigName;
	if (!FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*file))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not delete stomt.conf.json: %s"), *file);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Deleted stomt.conf.json because of wrong access token Path: %s"), *file);
	}
}


FString UStomtAPI::ReadLogFile(FString LogFileName)
{
	FString errorLog;

	FString LogFilePath = FPaths::GameLogDir() + LogFileName;
	FString LogFileCopyPath = FPaths::GameLogDir() + LogFileName + TEXT("Copy.log");
	FString LogFileCopyName = LogFileName + TEXT("Copy.log");

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.BypassSecurity(true);

	// Copy LogFileData
	if (!PlatformFile.CopyFile(*LogFileCopyPath, *LogFilePath, EPlatformFileRead::AllowWrite, EPlatformFileWrite::AllowRead))
	{
		UE_LOG(LogTemp, Warning, TEXT("LogFile Copy did not work FromFile: %s | ToFile %s"), *LogFilePath, *LogFileCopyPath);
	}

	// Read LogFileCopy from Disk
	if (!this->ReadFile(errorLog, LogFileCopyName, FPaths::GameLogDir() ))
	{
		if (FPaths::FileExists(FPaths::GameLogDir() + LogFileCopyName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not read LogFile %s, but it exists"), *LogFileCopyName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not read LogFile %s, because it does not exist"), *LogFileCopyName);
		}
	}

	// Delete LogFileCopy
	if (!PlatformFile.DeleteFile(*LogFileCopyPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not delete LogFileCopy %s"), *LogFileCopyPath);
	}

	return errorLog;
}

void UStomtAPI::SendLogFile(FString LogFileData, FString LogFileName)
{
	UStomtRestRequest* request = this->SetupNewPostRequest();
	request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnSendLogFileResponse);

	FString logJson = FString(TEXT("{ \"files\": { \"stomt\": [ { \"data\":\"") + FBase64::Encode(LogFileData) + TEXT("\", \"filename\" : \"") + LogFileName + TEXT("\" } ] } }"));

	request->UseStaticJsonString(true);
	request->SetStaticJsonString(logJson);

	request->ProcessURL(this->GetRestURL().Append(TEXT("/files")));

	LogFileWasSend = true;
}

void UStomtAPI::OnSendLogFileResponse(UStomtRestRequest * Request)
{
	// Request access token
	if (this->Accesstoken.IsEmpty())
	{
		if (Request->GetResponseObject()->HasField(TEXT("meta")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("meta"))->HasField(TEXT("accesstoken")))
			{
				this->Accesstoken = Request->GetResponseObject()->GetObjectField(TEXT("meta"))->GetStringField(TEXT("accesstoken"));
				this->SaveAccesstoken(this->Accesstoken);
			}
		}
	}

	// Get File uid of the error log and send stomt
	if (Request->GetResponseObject()->HasField(TEXT("data")))
	{
		if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("files")))
		{
			this->errorLog_file_uid = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetObjectField(TEXT("files"))->GetObjectField(TEXT("stomt"))->GetStringField("file_uid");
			this->LogFileWasSend = false;
			this->SendStomt(StomtToSend);
		}
	}

	if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 419 || Request->GetResponseCode() == 413)
	{
		if (StomtToSend != NULL)
		{
			this->SendStomt(StomtToSend);
			LogFileWasSend = false;
		}
	}
}

void UStomtAPI::SendEMail(FString EMail)
{
	UStomtRestRequest* request = this->SetupNewPostRequest();
	request->OnRequestComplete.AddDynamic(this, &UStomtAPI::UStomtAPI::OnSendEMailResponse);

	request->GetRequestObject()->SetStringField(TEXT("email"), EMail);

	request->ProcessURL(this->GetRestURL().Append(TEXT("/authentication/subscribe")));
}


void UStomtAPI::OnSendEMailResponse(UStomtRestRequest * Request)
{
	if (Request->GetResponseCode() != 400)
	{
		if (Request->GetResponseObject()->HasField(TEXT("data")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("success")))
			{
				this->SaveFlag(TEXT("email"), true);
			}
			else
			{
				this->SaveFlag(TEXT("email"), false);
			}
		}
	}
	else
	{
		this->SaveFlag(TEXT("email"), true);
	}
}



void UStomtAPI::OnReceiving(UStomtRestRequest * Request)
{
	UE_LOG(LogTemp, Warning, TEXT("Should not be called!"));
	// Wrong access token
	if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 419)
	{
		LoginRequestWasSend = false;

		UStomtRestJsonObject* stomtconf = this->ReadStomtConfAsJson();
		stomtconf->RemoveField(Accesstoken);
		WriteStomtConfAsJson(stomtconf);

		this->Accesstoken.Empty();
	}


	if (LoginRequestWasSend)
	{
		if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 404 || Request->GetResponseCode() == 413)
		{
			LoginRequestWasSend = false;

			if (StomtToSend != NULL)
			{
				this->SendStomt(StomtToSend);
			}
		}
		else
		{
			if (Request->GetResponseObject()->HasField(TEXT("data")))
			{
				if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("accesstoken")))
				{
					this->Accesstoken = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetStringField(TEXT("accesstoken"));
					this->SaveAccesstoken(this->Accesstoken);
					this->SaveFlag(TEXT("email"), true);
					LoginRequestWasSend = false;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Login did not work"));
				}
			}
		}
	}

	if (LogFileWasSend)
	{
		// Request access token
		if (this->Accesstoken.IsEmpty())
		{
			if (Request->GetResponseObject()->HasField(TEXT("meta")))
			{
				if (Request->GetResponseObject()->GetObjectField(TEXT("meta"))->HasField(TEXT("accesstoken")))
				{
					this->Accesstoken = Request->GetResponseObject()->GetObjectField(TEXT("meta"))->GetStringField(TEXT("accesstoken"));
					this->SaveAccesstoken(this->Accesstoken);
				}
			}
		}

		// Get File uid of the error log and send stomt
		if (Request->GetResponseObject()->HasField(TEXT("data")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("files")))
			{
				this->errorLog_file_uid = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetObjectField(TEXT("files"))->GetObjectField(TEXT("stomt"))->GetStringField("file_uid");
				this->LogFileWasSend = false;
				this->SendStomt(StomtToSend);
			}
		}

		if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 419 || Request->GetResponseCode() == 413)
		{
			if (StomtToSend != NULL)
			{
				this->SendStomt(StomtToSend);
				LogFileWasSend = false;
			}
		}
	}

	if (EMailFlagWasSend)
	{
		if (! (Request->GetResponseCode() == 400))
		{
			if (Request->GetResponseObject()->HasField(TEXT("data")))
			{
				if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("success")))
				{
					this->SaveFlag(TEXT("email"), true);
				}
				else
				{
					this->SaveFlag(TEXT("email"), false);
				}
			}
		}
		else
		{
			this->SaveFlag(TEXT("email"), true);
		}

		EMailFlagWasSend = false;
	}
}

bool UStomtAPI::CaptureComponent2D_SaveImage(USceneCaptureComponent2D * Target, const FString ImagePath, const FLinearColor ClearColour)
{
	// Bad scene capture component! No render target! Stay! Stay! Ok, feed!... wait, where was I?
	if ((Target == nullptr) || (Target->TextureTarget == nullptr))
	{
		return false;
	}

	FRenderTarget* RenderTarget = Target->TextureTarget->GameThread_GetRenderTargetResource();
	if (RenderTarget == nullptr)
	{
		return false;
	}

	TArray<FColor> RawPixels;

	// Format not supported - use PF_B8G8R8A8.
	if (Target->TextureTarget->GetFormat() != PF_B8G8R8A8)
	{
		// TRACEWARN("Format not supported - use PF_B8G8R8A8.");
		return false;
	}

	if (!RenderTarget->ReadPixels(RawPixels))
	{
		return false;
	}

	// Convert to FColor.
	FColor ClearFColour = ClearColour.ToFColor(false); // FIXME - want sRGB or not?

	for (auto& Pixel : RawPixels)
	{
		// Switch Red/Blue changes.
		const uint8 PR = Pixel.R;
		const uint8 PB = Pixel.B;
		Pixel.R = PB;
		Pixel.B = PR;

		// Set alpha based on RGB values of ClearColour.
		Pixel.A = ((Pixel.R == ClearFColour.R) && (Pixel.G == ClearFColour.G) && (Pixel.B == ClearFColour.B)) ? 0 : 255;
	}

	//IImageWrapperPtr ImageWrapper = GetImageWrapperByExtention(ImagePath);
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	const int32 Width = Target->TextureTarget->SizeX;
	const int32 Height = Target->TextureTarget->SizeY;

	if (ImageWrapper.IsValid() && ImageWrapper->SetRaw(&RawPixels[0], RawPixels.Num() * sizeof(FColor), Width, Height, ERGBFormat::RGBA, 8))
	{
		FFileHelper::SaveArrayToFile(ImageWrapper->GetCompressed(), *ImagePath);
		return true;
	}

	return false;
}

void UStomtAPI::SaveRenderTargetToDisk(UTextureRenderTarget2D* InRenderTarget, FString Filename)
{
	FTextureRenderTargetResource* RTResource = InRenderTarget->GameThread_GetRenderTargetResource();

	FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
	ReadPixelFlags.SetLinearToGamma(true);

	TArray<FColor> OutBMP;
	RTResource->ReadPixels(OutBMP, ReadPixelFlags);

	for (FColor& color : OutBMP)
	{
		color.A = 255;
	}


	FIntRect SourceRect;

	FIntPoint DestSize(InRenderTarget->GetSurfaceWidth(), InRenderTarget->GetSurfaceHeight());


	FString ResultPath;
	FHighResScreenshotConfig& HighResScreenshotConfig = GetHighResScreenshotConfig();
	HighResScreenshotConfig.SaveImage(Filename, OutBMP, DestSize, &ResultPath);
}

bool UStomtAPI::WriteFile(FString TextToSave, FString FileName, FString SaveDirectory, bool AllowOverwriting)
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

	FString path = SaveDirectory + FileName;

/*
	if (SaveDirectory.GetCharArray()[SaveDirectory.Len() - 2] == '/')
	{
		UE_LOG(LogTemp, Warning, TEXT(" / am enden: %s "), *path);
	}*/

	if (!FPaths::FileExists(path))
	{
		UE_LOG(LogTemp, Warning, TEXT("File with this path does not exist: %s "), *path);
	}

	return FFileHelper::LoadFileToString( Result, *path);

}

inline void UStomtAPI::SetupAndResetRequest()
{
	//Reset Request
	this->Request->ResetResponseData();
	this->Request->ResetRequestData();

	this->Request->SetVerb(ERequestVerb::POST);
	this->Request->SetHeader(TEXT("appid"), this->GetAppID());

	if (!this->Accesstoken.IsEmpty())
	{
		this->Request->SetHeader(TEXT("accesstoken"), this->Accesstoken);
	}
}

UStomtRestRequest* UStomtAPI::SetupNewPostRequest()
{
	UStomtRestRequest* request = NewObject<UStomtRestRequest>();

	request->SetVerb(ERequestVerb::POST);
	request->SetHeader(TEXT("appid"), this->GetAppID());

	if (!this->Accesstoken.IsEmpty())
	{
		request->SetHeader(TEXT("accesstoken"), this->Accesstoken);
	}

	return request;
}


