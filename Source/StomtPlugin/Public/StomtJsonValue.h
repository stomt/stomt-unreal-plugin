// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtPluginPrivatePCH.h"
#include "StomtJsonValue.generated.h"

/**
 * Represents all the types a Json Value can be.
 */
UENUM(BlueprintType)
namespace StomtEnumJson
{
	enum Type
	{
		None,
		Null,
		String,
		Number,
		Boolean,
		Array,
		Object,
	};
}

/**
 * Blueprintable FJsonValue wrapper
 */
UCLASS(BlueprintType, Blueprintable)
class UStomtJsonValue : public UObject
{
	GENERATED_BODY()

public:

	/** Create new Json Number value
	* Attn.!! float used instead of double to make the function blueprintable! */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json Number Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "Stomt|Json")
	static UStomtJsonValue* ConstructJsonValueNumber(UObject* WorldContextObject, float Number);

	/** Create new Json String value */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json String Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "Stomt|Json")
		static UStomtJsonValue* ConstructJsonValueString(UObject* WorldContextObject, const FString& StringValue);

	/** Create new Json Bool value */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json Bool Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "Stomt|Json")
		static UStomtJsonValue* ConstructJsonValueBool(UObject* WorldContextObject, bool InValue);

	/** Create new Json Array value */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json Array Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "Stomt|Json")
		static UStomtJsonValue* ConstructJsonValueArray(UObject* WorldContextObject, const TArray<UStomtJsonValue*>& InArray);

	/** Create new Json value from FJsonValue (to be used from StomtJsonObject) */
	static UStomtJsonValue* ConstructJsonValue(UObject* WorldContextObject, const TSharedPtr<FJsonValue>& InValue);

	/** Get the root Json value */
	TSharedPtr<FJsonValue>& GetRootValue();

	/** Set the root Json value */
	void SetRootValue(TSharedPtr<FJsonValue>& JsonValue);


	//////////////////////////////////////////////////////////////////////////
	// FJsonValue API

	/** Get type of Json value (Enum) */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		StomtEnumJson::Type GetType() const;

	/** Get type of Json value (String) */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		FString GetTypeString() const;

	/** Returns true if this value is a 'null' */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		bool IsNull() const;

	/** Returns this value as a double, throwing an error if this is not an Json Number
	* Attn.!! float used instead of double to make the function blueprintable! */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		float AsNumber() const;

	/** Returns this value as a number, throwing an error if this is not an Json String */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		FString AsString() const;

	/** Returns this value as a boolean, throwing an error if this is not an Json Bool */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		bool AsBool() const;

	/** Returns this value as an array, throwing an error if this is not an Json Array */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		TArray<UStomtJsonValue*> AsArray() const;

	//////////////////////////////////////////////////////////////////////////
	// Data

private:

	/** Internal JSON data */
	TSharedPtr<FJsonValue> JsonVal;


	//////////////////////////////////////////////////////////////////////////
	// Helpers

protected:
	
	/** Simple error logger */
	void ErrorMessage(const FString& InType) const;

};
