// Copyright 2016 Daniel Schukies. All Rights Reserved.

#pragma once
#include "stomt.h"
#include "StomtRestRequest.h"


UStomtRestRequest::UStomtRestRequest()
{
	ResponseJsonObj = NULL;
	RequestJsonObj = NULL;
}

UStomtRestRequest::~UStomtRestRequest()
{
}

void UStomtRestRequest::MyHttpCall() 
{
	//TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); // Gets an singelton and creates request.
	
	this->SetVerb(SRequestVerb::GET);
	this->SetHeader(FString(TEXT("appid")), FString(TEXT("R18OFQXmb6QzXwzP1lWdiZ7Y9")));
	this->ProcessURL("https://test.rest.stomt.com/stomts/java-sdk-test-33956");

	/*
	Request->SetURL("https://test.rest.stomt.com/stomts/java-sdk-test-33956");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("appid"), "R18OFQXmb6QzXwzP1lWdiZ7Y9");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	Request->OnProcessRequestComplete().BindUObject(this, &UStomtRestRequest::OnResponseReceived);

	if (!Request->ProcessRequest()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Internal error: ProcessRequest failed"));
	}
	*/
	return;
}

UStomtRestRequest * UStomtRestRequest::ConstructRequest()
{
	return nullptr;
}

void UStomtRestRequest::SetVerb(SRequestVerb::Type Verb)
{
	this->RequestVerb = Verb;
}


void UStomtRestRequest::SetHeader(const FString &HeaderName, const FString &HeaderValue)
{
	this->RequestHeaders.Add( HeaderName, HeaderValue);
}

void UStomtRestRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) 
	{
		UE_LOG(LogTemp, Warning, TEXT("EHttpResponseCodes::IsOk"));
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT( "not successful: %s"), *Response->GetContentAsString() );
	}

	return;
}

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
{/*
	// Set verb
	switch (RequestVerb)
	{
	case SRequestVerb::GET:
		HttpRequest->SetVerb(TEXT("GET"));
		break;

	case SRequestVerb::POST:
		HttpRequest->SetVerb(TEXT("POST"));
		break;

	case SRequestVerb::PUT:
		HttpRequest->SetVerb(TEXT("PUT"));
		break;

	case SRequestVerb::DEL:
		HttpRequest->SetVerb(TEXT("DELETE"));
		break;

	default:
		break;
	}

	// Set content-type
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	// Serialize data to json string
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(RequestJsonObj.ToSharedRef(), Writer);

	// Set Json content
	HttpRequest->SetContentAsString(OutputString);
	

	// Apply additional headers
	for (TMap<FString, FString>::TConstIterator It(RequestHeaders); It; ++It)
	{
		HttpRequest->SetHeader(It.Key(), It.Value());
	}

	// Bind event
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UStomtRestRequest::OnProcessRequestComplete);

	// Execute the request
	HttpRequest->ProcessRequest();
	*/

	
	// Set verb
	switch (RequestVerb)
	{
	case SRequestVerb::GET:
		HttpRequest->SetVerb(TEXT("GET"));
		break;

	case SRequestVerb::POST:
		HttpRequest->SetVerb(TEXT("POST"));
		break;

	case SRequestVerb::PUT:
		HttpRequest->SetVerb(TEXT("PUT"));
		break;

	case SRequestVerb::DEL:
		HttpRequest->SetVerb(TEXT("DELETE"));
		break;

	default:
		break;
	}

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Serialize data to json string
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(ResponseJsonObj->GetRootObject().ToSharedRef(), Writer); // kaputt


	// Set Json content
	HttpRequest->SetContentAsString(OutputString);

		
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
		//OnRequestFail.Broadcast(this);

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

	//// Try to deserialize data to JSON
	//TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);
	//FJsonSerializer::Deserialize(JsonReader, *ResponseJsonObj);

	//// Decide whether the request was successful
	//bIsValidJsonResponse = bWasSuccessful && ResponseJsonObj->IsValid();

	//// Log errors
	//if (!bIsValidJsonResponse)
	//{
	//	if (!ResponseJsonObj->IsValid())
	//	{
	//		// As we assume it's recommended way to use current class, but not the only one,
	//		// it will be the warning instead of error
	//		UE_LOG(LogTemp, Warning, TEXT("JSON could not be decoded!"));
	//	}
	//}

	// Broadcast the result event
	//OnRequestComplete.Broadcast(this);

	/*
	// Finish the latent action
	if (ContinueAction)
	{
		FVaRestLatentAction<UVaRestJsonObject*> *K = ContinueAction;
		ContinueAction = nullptr;

		K->Call(ResponseJsonObj);
	}
	*/
}

//////////////////////////////////////////////////////////////////////////
// Destruction and reset


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
