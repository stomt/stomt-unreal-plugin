// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtRestRequest.h"
#include "StomtPluginPrivatePCH.h"

//////////////////////////////////////////////////////////////////////////
// Construction

UStomtRestRequest::UStomtRestRequest()
{
	this->bUseStaticJsonString = false;
	this->UseRequestLogging(true);
	this->ResponseJsonObj = NULL;
	this->RequestJsonObj = NULL;
	this->ResetData();
}

UStomtRestRequest::~UStomtRestRequest()
{
}


UStomtRestRequest * UStomtRestRequest::ConstructRequest()
{
	return nullptr;
}

void UStomtRestRequest::SetVerb(StomtEnumRequestVerb::Type Verb)
{
	this->RequestVerb = Verb;
}

void UStomtRestRequest::SetHeader(const FString &HeaderName, const FString &HeaderValue)
{
	this->RequestHeaders.Add(HeaderName, HeaderValue);
}

void UStomtRestRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Check we have result to process futher
	if (!bWasSuccessful)
	{
		UE_LOG(StomtNetwork, Error, TEXT("Request failed: %s ResponseCode: %d "), *Request->GetURL(), *Response->GetContentAsString());

		// Broadcast the result event
		this->OnRequestFail.Broadcast(this);
		return;
	}

	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) 
	{
		UE_LOG(StomtNetwork, Log, TEXT("EHttpResponseCodes::IsOk"));
		UE_LOG(StomtNetwork, Log, TEXT("Content: %s"), *Response->GetContentAsString() );
	}
	else
	{
		UE_LOG(StomtNetwork, Warning, TEXT( "not successful: %s"), *Response->GetContentAsString() );
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
// Helpers



///////////////////////////////////////////////////////////////////////////
// Response data access

int32 UStomtRestRequest::GetResponseCode()
{
	return this->ResponseCode;
}

FString UStomtRestRequest::GetResponseHeader(const FString HeaderName)
{
	FString Result;

	FString* Header = ResponseHeaders.Find(HeaderName);
	if (Header != NULL)
	{
		Result = *Header;
	}

	return Result;
}


TArray<FString> UStomtRestRequest::GetAllResponseHeaders()
{
	TArray<FString> Result;

	for (TMap<FString, FString>::TConstIterator It(ResponseHeaders); It; ++It)
	{
		Result.Add(It.Key() + TEXT(": ") + It.Value());
	}

	return Result;
}

//////////////////////////////////////////////////////////////////////////
// URL processing

void UStomtRestRequest::ProcessUrl(const FString& Url)
{
	// Signature changed from version 4.25 to 4.26 (ThreadSafe)
	auto HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(Url);

	// Set verb
	switch (RequestVerb)
	{
		case StomtEnumRequestVerb::GET:
			HttpRequest->SetVerb(TEXT("GET"));
			break;

		case StomtEnumRequestVerb::POST:
			HttpRequest->SetVerb(TEXT("POST"));
			break;

		case StomtEnumRequestVerb::PUT:
			HttpRequest->SetVerb(TEXT("PUT"));
			break;

		case StomtEnumRequestVerb::DEL:
			HttpRequest->SetVerb(TEXT("DELETE"));
			break;

		default:
			break;
	}

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Serialize data to json string
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(this->RequestJsonObj->GetRootObject().ToSharedRef(), Writer); 


	// Set Json content
	if ( !HttpRequest->GetVerb().Equals( TEXT("GET") ) )
	{
		if (this->bUseStaticJsonString)
		{
			if (!this->StaticJsonOutputString.IsEmpty())
			{
				OutputString = this->StaticJsonOutputString;
			}
		}

		HttpRequest->SetContentAsString(OutputString);
	}
	
	if (this->bRequestLogging)
	{
		if (OutputString.Len() > 256)
		{
			FString shortContent = OutputString.LeftChop(OutputString.Len() - 256);
			UE_LOG(StomtNetwork, Log, TEXT("Request (json): %s %s | truncated-content(256): %s"), *HttpRequest->GetVerb(), *HttpRequest->GetURL(), *shortContent);
		}
		else
		{
			UE_LOG(StomtNetwork, Log, TEXT("Request (json): %s %s %s"), *HttpRequest->GetVerb(), *HttpRequest->GetURL(), *OutputString);
		}
	}
	else
	{
		UE_LOG(StomtNetwork, Log, TEXT("Request (json): %s %s | did not log (contains user data)"), *HttpRequest->GetVerb(), *HttpRequest->GetURL());
	}

	

	// Apply additional headers
	for (TMap<FString, FString>::TConstIterator It(RequestHeaders); It; ++It)
	{
		HttpRequest->SetHeader(It.Key(), It.Value());
	}

	// Bind event
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UStomtRestRequest::OnProcessRequestComplete);

	// Execute the Request
	HttpRequest->ProcessRequest();
}


//////////////////////////////////////////////////////////////////////////
// Request callbacks


void UStomtRestRequest::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Be sure that we have no data from previous response
	this->ResetResponseData();

	// Check we have result to process futher
	if (!bWasSuccessful)
	{
		UE_LOG(StomtNetwork, Error, TEXT("Request failed: %s"), *Request->GetURL());

		// Broadcast the result event
		this->OnRequestFail.Broadcast(this);
		return;
	}

	// Save response data as a string
	this->ResponseContent = Response->GetContentAsString();

	// Save response code as int32
	this->ResponseCode = Response->GetResponseCode();

	// Log response state

	if (Response->GetContentAsString().Len() > 256)
	{
		FString shortContent = Response->GetContentAsString().LeftChop(Response->GetContentAsString().Len() - 256);
		UE_LOG(StomtNetwork, Log, TEXT("Response (%d): Content(truncated 256):  %s"), Response->GetResponseCode(), *shortContent);
	}
	else
	{
		UE_LOG(StomtNetwork, Log, TEXT("Response (%d): %s"), Response->GetResponseCode(), *Response->GetContentAsString());
	}


	// Process response headers
	TArray<FString> Headers = Response->GetAllHeaders();
	for (FString Header : Headers)
	{
		FString Key;
		FString Value;
		if (Header.Split(TEXT(": "), &Key, &Value))
		{
			this->ResponseHeaders.Add(Key, Value);
		}
	}

	// Try to deserialize data to JSON
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(this->ResponseContent);
	FJsonSerializer::Deserialize(JsonReader, this->ResponseJsonObj->GetRootObject());

	// Decide whether the Request was successful
	this->bIsValidJsonResponse = bWasSuccessful && this->ResponseJsonObj->GetRootObject().IsValid();

	// Log errors
	if (!this->bIsValidJsonResponse)
	{
		if (!this->ResponseJsonObj->GetRootObject().IsValid())
		{
			// As we assume it's recommended way to use current class, but not the only one,
			// it will be the warning instead of error
			UE_LOG(StomtNetwork, Warning, TEXT("JSON could not be decoded!"));
		}
	}

	// Broadcast the result event
	this->OnRequestComplete.Broadcast(this);
	
	// Finish the latent action
	if (this->ContinueAction)
	{
		StomtLatentAction<UStomtRestJsonObject*> *K = this->ContinueAction;
		this->ContinueAction = nullptr;

		K->Call(this->ResponseJsonObj);
	}	
}

void UStomtRestRequest::UseStaticJsonString(bool bNewUseStaticJsonString)
{
	this->bUseStaticJsonString = bNewUseStaticJsonString;
}

void UStomtRestRequest::UseRequestLogging(bool bNewRequestLogging)
{
	this->bRequestLogging = bNewRequestLogging;
}

void UStomtRestRequest::SetStaticJsonString(FString JsonString)
{
	this->StaticJsonOutputString = JsonString;
}

//////////////////////////////////////////////////////////////////////////
// Destruction and reset

void UStomtRestRequest::ResetData()
{
	this->ResetRequestData();
	this->ResetResponseData();
}

void UStomtRestRequest::ResetRequestData()
{
	if (this->RequestJsonObj != NULL)
	{
		this->RequestJsonObj->Reset();
	}
	else
	{
		this->RequestJsonObj = NewObject<UStomtRestJsonObject>();
	}

	this->bUseStaticJsonString = false;
}

void UStomtRestRequest::ResetResponseData()
{
	if (this->ResponseJsonObj != NULL)
	{
		this->ResponseJsonObj->Reset();
	}
	else
	{
		this->ResponseJsonObj = NewObject<UStomtRestJsonObject>();
	}

	this->ResponseHeaders.Empty();
	this->ResponseCode = -1;

	this->bIsValidJsonResponse = false;
}


//////////////////////////////////////////////////////////////////////////
// JSON data accessors

UStomtRestJsonObject* UStomtRestRequest::GetRequestObject()
{
	return this->RequestJsonObj;
}

void UStomtRestRequest::SetRequestObject(UStomtRestJsonObject* JsonObject)
{
	this->RequestJsonObj = JsonObject;
}

UStomtRestJsonObject* UStomtRestRequest::GetResponseObject()
{
	return this->ResponseJsonObj;
}

void UStomtRestRequest::SetResponseObject(UStomtRestJsonObject* JsonObject)
{
	ResponseJsonObj = JsonObject;
}
