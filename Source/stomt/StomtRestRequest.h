// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "stomt.h"
#include "StomtJsonObject.h"
#include "StomtRestRequest.generated.h"

 /** Verb (GET, PUT, POST) */
UENUM(BlueprintType)
namespace SRequestVerb
{
	enum Type
	{
		GET,
		POST,
		PUT,
		DEL UMETA(DisplayName = "DELETE"),
	};
}

UCLASS()
class STOMT_API UStomtRestRequest : public UObject
{
	GENERATED_BODY()

public:
	//////////////////////////////////////////////////////////////////////////
	// Construction

	UStomtRestRequest();
	~UStomtRestRequest();

	void MyHttpCall();

	static UStomtRestRequest* ConstructRequest();

	/** Set verb to the request */
	void SetVerb(SRequestVerb::Type Verb);

	/** Sets header info */
	void SetHeader(const FString &HeaderName, const FString &HeaderValue);

	void OnResponseReceived(
		FHttpRequestPtr Request, 
		FHttpResponsePtr Response, 
		bool bWasSuccessful);

	//////////////////////////////////////////////////////////////////////////
	// Destruction and reset

	/** Reset all internal saved data */
	void ResetData();

	/** Reset saved request data */
	void ResetRequestData();

	/** Reset saved response data */
	void ResetResponseData();

	/** Cancel latent response waiting  */
	void Cancel();


	//////////////////////////////////////////////////////////////////////////
	// JSON data accessors
	
	/** Get the Request Json object */
	//UVaRestJsonObject* GetRequestObject();

	/** Set the Request Json object */
	//void SetRequestObject(UVaRestJsonObject* JsonObject);

	/** Get the Response Json object */
	//UVaRestJsonObject* GetResponseObject();

	/** Set the Response Json object */
	//void SetResponseObject(UVaRestJsonObject* JsonObject);


	///////////////////////////////////////////////////////////////////////////
	// Response data access

	/** Get the responce code of the last query */
	int32 GetResponseCode();

	/** Get value of desired response header */
	FString GetResponseHeader(const FString HeaderName);

	/** Get list of all response headers */
	TArray<FString> GetAllResponseHeaders();


	//////////////////////////////////////////////////////////////////////////
	// URL processing

	/** Open URL with current setup */
	virtual void ProcessURL(const FString& Url = TEXT("http://alyamkin.com"));

	/** Apply current internal setup to request and process it */
	void ProcessRequest(TSharedRef<IHttpRequest> HttpRequest);


	//////////////////////////////////////////////////////////////////////////
	// Request callbacks

private:
	/** Internal bind function for the IHTTPRequest::OnProcessRequestCompleted() event */
	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	/** Event occured when the request has been completed */
	//FOnRequestComplete OnRequestComplete;

	/** Event occured when the request wasn't successfull */
	//FOnRequestFail OnRequestFail;
	
	//////////////////////////////////////////////////////////////////////////
	// Data
public:

	/** Request response stored as a string */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StomtRest|Response")
	FString ResponseContent;

	/** Is the response valid JSON? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StomtRest|Response")
	bool bIsValidJsonResponse;




protected:

	/** Internal request data stored as JSON */
	UStomtRestJsonObject* RequestJsonObj;

	/** Response data stored as JSON */
	UStomtRestJsonObject* ResponseJsonObj;

	/** Verb for making request (GET,POST,etc) */
	SRequestVerb::Type RequestVerb;

	/** Mapping of header section to values. Used to generate final header string for request */
	TMap<FString, FString> RequestHeaders;

	/** Cached key/value header pairs. Parsed once request completes */
	TMap<FString, FString> ResponseHeaders;

	/** Http Response code */
	int32 ResponseCode;





};




