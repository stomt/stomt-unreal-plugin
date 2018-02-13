// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtJsonValue.h"
#include "StomtJsonObject.generated.h"

/**
 * Blueprintable FJsonObject wrapper
 */
UCLASS()
class UStomtRestJsonObject : public UObject
{
	GENERATED_BODY()

public:

	UStomtRestJsonObject();

	/** Create new Json object */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json Object", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "Stomt|Json")
	static UStomtRestJsonObject* ConstructJsonObject(UObject* WorldContextObject);

	/** Reset all internal data */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
	void Reset();

	/** Get the root Json object */
	TSharedPtr<FJsonObject>& GetRootObject();

	/** Set the root Json object */
	void SetRootObject(TSharedPtr<FJsonObject>& JsonObject);


//	//////////////////////////////////////////////////////////////////////////
//	// Serialization

	/** Serialize Json to string (formatted with line breaks) */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		FString EncodeJson() const;

	/** Serialize Json to string (signel string without line breaks) */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		FString EncodeJsonToSingleString() const;

	/** Construct Json object from string */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		bool DecodeJson(const FString& JsonString);


//	//////////////////////////////////////////////////////////////////////////
//	// FJsonObject API
//
	/** Returns a list of field names that exist in the object */
	UFUNCTION(BlueprintPure, Category = "Stomt|Json")
		TArray<FString> GetFieldNames();

	/** Checks to see if the FieldName exists in the object */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		bool HasField(const FString& FieldName) const;

	/** Remove field named FieldName */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void RemoveField(const FString& FieldName);

	/** Get the field named FieldName as a JsonValue */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		UStomtJsonValue* GetField(const FString& FieldName) const;

	/** Add a field named FieldName with a Value */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void SetField(const FString& FieldName, UStomtJsonValue* JsonValue);

	/** Get the field named FieldName as a Json Array */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		TArray<UStomtJsonValue*> GetArrayField(const FString& FieldName);

	/** Set an ObjectField named FieldName and value of Json Array */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void SetArrayField(const FString& FieldName, const TArray<UStomtJsonValue*>& InArray);

	/** Adds all of the fields from one json object to this one */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void MergeJsonObject(UStomtRestJsonObject* InJsonObject, bool Overwrite);


//	//////////////////////////////////////////////////////////////////////////
//	// FJsonObject API Helpers (easy to use with simple Json objects)

	/** Get the field named FieldName as a number. Ensures that the field is present and is of type Json number.
	* Attn.!! float used instead of double to make the function blueprintable! */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		float GetNumberField(const FString& FieldName) const;

	/** Add a field named FieldName with Number as value
	* Attn.!! float used instead of double to make the function blueprintable! */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void SetNumberField(const FString& FieldName, float Number);

	/** Get the field named FieldName as a string. */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		FString GetStringField(const FString& FieldName) const;

	/** add a field named fieldname with value of stringvalue */
	UFUNCTION(BlueprintCallable, category = "Stomt|Json")
		void SetStringField(const FString& Fieldname, const FString& stringvalue);

	/** Get the field named FieldName as a boolean. */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		bool GetBoolField(const FString& FieldName) const;

	/** Set a boolean field named FieldName and value of InValue */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void SetBoolField(const FString& FieldName, bool InValue);

	/** Get the field named FieldName as a Json object. */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		UStomtRestJsonObject* GetObjectField(const FString& FieldName) const;

	/** Set an ObjectField named FieldName and value of JsonObject */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void SetObjectField(const FString& FieldName, UStomtRestJsonObject* JsonObject);

//
//	//////////////////////////////////////////////////////////////////////////
//	// Array fields helpers (uniform arrays)

	/** Get the field named FieldName as a Number Array. Use it only if you're sure that array is uniform!
	* Attn.!! float used instead of double to make the function blueprintable! */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		TArray<float> GetNumberArrayField(const FString& FieldName);

	/** Set an ObjectField named FieldName and value of Number Array
	* Attn.!! float used instead of double to make the function blueprintable! */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void SetNumberArrayField(const FString& FieldName, const TArray<float>& NumberArray);

	/** Get the field named FieldName as a String Array. Use it only if you're sure that array is uniform! */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		TArray<FString> GetStringArrayField(const FString& FieldName);

	/** Set an ObjectField named FieldName and value of String Array */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void SetStringArrayField(const FString& FieldName, const TArray<FString>& StringArray);

	/** Get the field named FieldName as a Bool Array. Use it only if you're sure that array is uniform! */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		TArray<bool> GetBoolArrayField(const FString& FieldName);

	/** Set an ObjectField named FieldName and value of Bool Array */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void SetBoolArrayField(const FString& FieldName, const TArray<bool>& BoolArray);

	/** Get the field named FieldName as an Object Array. Use it only if you're sure that array is uniform! */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		TArray<UStomtRestJsonObject*> GetObjectArrayField(const FString& FieldName);

	/** Set an ObjectField named FieldName and value of Ob Array */
	UFUNCTION(BlueprintCallable, Category = "Stomt|Json")
		void SetObjectArrayField(const FString& FieldName, const TArray<UStomtRestJsonObject*>& ObjectArray);


//	//////////////////////////////////////////////////////////////////////////
//	// Data

private:
	/** Internal JSON data */
	TSharedPtr<FJsonObject> JsonObj;

};
