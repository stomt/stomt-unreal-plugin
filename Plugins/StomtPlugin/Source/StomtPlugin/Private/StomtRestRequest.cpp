// Copyright 2016 Daniel Schukies. All Rights Reserved.

#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"

//////////////////////////////////////////////////////////////////////////
// Construction

UStomtRestRequest::UStomtRestRequest()
{
	ResponseJsonObj = NULL;
	RequestJsonObj = NULL;

	ResetData();
}

UStomtRestRequest::~UStomtRestRequest()
{
}

void UStomtRestRequest::MyHttpCall() 
{
	// Testing V3: Write Stomt
	///////////////////////////////////////////////////////////

	this->SetVerb(ERequestVerb::POST);
	this->SetHeader(TEXT("appid"), TEXT("R18OFQXmb6QzXwzP1lWdiZ7Y9"));

	//TSharedPtr<FJsonValue> NewVal = UStomtJsonValue::ConstructJsonValueString(this, TEXT("unreal");// MakeShareable(new FJsonObject());

	//UStomtJsonValue* NewValue = UStomtJsonValue::ConstructJsonValueString(this, TEXT("unreal") );// MakeShareable(new FJsonObject());

	this->GetRequestObject()->SetField(TEXT("target_id"), UStomtJsonValue::ConstructJsonValueString(this, TEXT("unreal")) );

	this->GetRequestObject()->SetField(TEXT("positive"), UStomtJsonValue::ConstructJsonValueBool(this, true) );

	this->GetRequestObject()->SetField(TEXT("text"), UStomtJsonValue::ConstructJsonValueString(this, TEXT("A Text")));

	this->GetRequestObject()->SetField(TEXT("anonym"), UStomtJsonValue::ConstructJsonValueBool(this, true));

	this->ProcessURL("https://test.rest.stomt.com/stomts");



	// Testing V2: Read Target 
	///////////////////////////////////////////////////////////

	/*
	this->SetVerb(ERequestVerb::GET);
	this->SetHeader(TEXT("appid"), TEXT("R18OFQXmb6QzXwzP1lWdiZ7Y9"));
	
	//TSharedPtr<FJsonValue> NewVal = UStomtJsonValue::ConstructJsonValueString(this, TEXT("unreal");// MakeShareable(new FJsonObject());

	//UStomtJsonValue* NewValue = UStomtJsonValue::ConstructJsonValueString(this, TEXT("unreal") );// MakeShareable(new FJsonObject());
	//NewValue->SetRootValue(NewVal);
	//this->GetRequestObject()->SetField(TEXT("target_id"), NewValue);

	this->ProcessURL("https://test.rest.stomt.com/targets/unreal?target_id=unreal");
	*/
	
	// Testing V1: Manually
	///////////////////////////////////////////////////////////

	/*
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); // Gets an singelton and creates request.

	//Request->SetURL("https://test.rest.stomt.com/stomts/java-sdk-test-33956");
	Request->SetURL("https://test.rest.stomt.com/targets/unreal");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("appid"), "R18OFQXmb6QzXwzP1lWdiZ7Y9");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString(TEXT("{\"target_id\": \"unreal\"}"));


	Request->OnProcessRequestComplete().BindUObject(this, &UStomtRestRequest::OnResponseReceived);

	if (!Request->ProcessRequest()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Internal error: ProcessRequest failed"));
	}
	*/
}

UStomtRestRequest * UStomtRestRequest::ConstructRequest()
{
	return nullptr;
}

void UStomtRestRequest::SetVerb(ERequestVerb::Type Verb)
{
	this->RequestVerb = Verb;
}


void UStomtRestRequest::SetHeader(const FString &HeaderName, const FString &HeaderValue)
{
	this->RequestHeaders.Add( HeaderName, HeaderValue);
}

void UStomtRestRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Check we have result to process futher
	if (!bWasSuccessful)
	{
		//UE_LOG(LogTemp, Error, TEXT("Request failed: %s ResponseCode: %d "), *Request->GetURL(), Response->GetContentAsString());
		
		
		
		// Broadcast the result event
		OnRequestFail.Broadcast(this);
		return;
	}


	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) 
	{
		UE_LOG(LogTemp, Warning, TEXT("EHttpResponseCodes::IsOk"));
		UE_LOG(LogTemp, Warning, TEXT("Content: %s"), *Response->GetContentAsString() );
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT( "not successful: %s"), *Response->GetContentAsString() );
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

void UStomtRestRequest::ProcessURL(const FString& Url)
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(Url);

	ProcessRequest(HttpRequest);
}

void UStomtRestRequest::ProcessRequest(TSharedRef<IHttpRequest> HttpRequest)
{	
	// Set verb
	switch (RequestVerb)
	{
		case ERequestVerb::GET:
			HttpRequest->SetVerb(TEXT("GET"));
			break;

		case ERequestVerb::POST:
			HttpRequest->SetVerb(TEXT("POST"));
			break;

		case ERequestVerb::PUT:
			HttpRequest->SetVerb(TEXT("PUT"));
			break;

		case ERequestVerb::DEL:
			HttpRequest->SetVerb(TEXT("DELETE"));
			break;

		default:
			break;
	}

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Serialize data to json string
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(RequestJsonObj->GetRootObject().ToSharedRef(), Writer); 


	// Set Json content
	if ( !HttpRequest->GetVerb().Equals( TEXT("GET") ) )
	{
		HttpRequest->SetContentAsString(OutputString);
	}
	

	UE_LOG(LogTemp, Log, TEXT("Request (json): %s %s %s"), *HttpRequest->GetVerb(), *HttpRequest->GetURL(), *OutputString);

	// Apply additional headers
	for (TMap<FString, FString>::TConstIterator It(RequestHeaders); It; ++It)
	{
		HttpRequest->SetHeader(It.Key(), It.Value());
	}

	// Bind event
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UStomtRestRequest::OnProcessRequestComplete);

	// Execute the request
	HttpRequest->ProcessRequest();
}

//////////////////////////////////////////////////////////////////////////
// Request callbacks


void UStomtRestRequest::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Be sure that we have no data from previous response
	ResetResponseData();

	// Check we have result to process futher
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("Request failed: %s"), *Request->GetURL());

		// Broadcast the result event
		OnRequestFail.Broadcast(this);
		return;
	}

	// Save response data as a string
	ResponseContent = Response->GetContentAsString();

	// Save response code as int32
	ResponseCode = Response->GetResponseCode();

	// Log response state
	UE_LOG(LogTemp, Log, TEXT("Response (%d): %s"), Response->GetResponseCode(), *Response->GetContentAsString());

	// Process response headers
	TArray<FString> Headers = Response->GetAllHeaders();
	for (FString Header : Headers)
	{
		FString Key;
		FString Value;
		if (Header.Split(TEXT(": "), &Key, &Value))
		{
			ResponseHeaders.Add(Key, Value);
		}
	}

	// Try to deserialize data to JSON
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);
	FJsonSerializer::Deserialize(JsonReader, ResponseJsonObj->GetRootObject());

	// Decide whether the request was successful
	bIsValidJsonResponse = bWasSuccessful && ResponseJsonObj->GetRootObject().IsValid();

	// Log errors
	if (!bIsValidJsonResponse)
	{
		if (!ResponseJsonObj->GetRootObject().IsValid())
		{
			// As we assume it's recommended way to use current class, but not the only one,
			// it will be the warning instead of error
			UE_LOG(LogTemp, Warning, TEXT("JSON could not be decoded!"));
		}
	}

	
	//UE_LOG(LogTemp, Log, TEXT("Response (json): %s "), this->ResponseJsonObj->GetRootObject()-> );
	// Broadcast the result event
	OnRequestComplete.Broadcast(this);

	
	// Finish the latent action
	if (ContinueAction)
	{
		FVaRestLatentAction<UStomtRestJsonObject*> *K = ContinueAction;
		ContinueAction = nullptr;

		K->Call(ResponseJsonObj);
	}
	
}

//////////////////////////////////////////////////////////////////////////
// Destruction and reset

void UStomtRestRequest::ResetData()
{
	ResetRequestData();
	ResetResponseData();
}

void UStomtRestRequest::ResetRequestData()
{
	if (RequestJsonObj != NULL)
	{
		RequestJsonObj->Reset();
	}
	else
	{
		RequestJsonObj = NewObject<UStomtRestJsonObject>();
	}
}

void UStomtRestRequest::ResetResponseData()
{
	if (ResponseJsonObj != NULL)
	{
		ResponseJsonObj->Reset();
	}
	else
	{
		ResponseJsonObj = NewObject<UStomtRestJsonObject>();
	}

	ResponseHeaders.Empty();
	ResponseCode = -1;

	bIsValidJsonResponse = false;
}


//////////////////////////////////////////////////////////////////////////
// JSON data accessors

UStomtRestJsonObject* UStomtRestRequest::GetRequestObject()
{
	return RequestJsonObj;
}

void UStomtRestRequest::SetRequestObject(UStomtRestJsonObject* JsonObject)
{
	RequestJsonObj = JsonObject;
}

UStomtRestJsonObject* UStomtRestRequest::GetResponseObject()
{
	return ResponseJsonObj;
}

void UStomtRestRequest::SetResponseObject(UStomtRestJsonObject* JsonObject)
{
	ResponseJsonObj = JsonObject;
}