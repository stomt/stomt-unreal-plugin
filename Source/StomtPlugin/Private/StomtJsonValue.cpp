// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtJsonValue.h"
#include "StomtPluginPrivatePCH.h"

UStomtJsonValue* UStomtJsonValue::ConstructJsonValueNumber(UObject* WorldContextObject, float Number)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueNumber(Number));

	UStomtJsonValue* NewValue = NewObject<UStomtJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UStomtJsonValue* UStomtJsonValue::ConstructJsonValueString(UObject* WorldContextObject, const FString& StringValue)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueString(StringValue));

	UStomtJsonValue* NewValue = NewObject<UStomtJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UStomtJsonValue* UStomtJsonValue::ConstructJsonValueBool(UObject* WorldContextObject, bool InValue)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueBoolean(InValue));

	UStomtJsonValue* NewValue = NewObject<UStomtJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UStomtJsonValue* UStomtJsonValue::ConstructJsonValueArray(UObject* WorldContextObject, const TArray<UStomtJsonValue*>& InArray)
{
	// Prepare data array to create new value
	TArray< TSharedPtr<FJsonValue> > ValueArray;
	for (auto InVal : InArray)
	{
		ValueArray.Add(InVal->GetRootValue());
	}

	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueArray(ValueArray));

	UStomtJsonValue* NewValue = NewObject<UStomtJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UStomtJsonValue* ConstructJsonValue(UObject* WorldContextObject, const TSharedPtr<FJsonValue>& InValue)
{
	TSharedPtr<FJsonValue> NewVal = InValue;

	UStomtJsonValue* NewValue = NewObject<UStomtJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

TSharedPtr<FJsonValue>& UStomtJsonValue::GetRootValue()
{
	return JsonVal;
}

void UStomtJsonValue::SetRootValue(TSharedPtr<FJsonValue>& JsonValue)
{
	JsonVal = JsonValue;
}


////////////////////////////////////////////////////////////////////////////
//// FJsonValue API

StomtEnumJson::Type UStomtJsonValue::GetType() const
{
	if (!(JsonVal.IsValid()))
	{
		return StomtEnumJson::None;
	}

	switch (JsonVal->Type)
	{
	case EJson::None:
		return StomtEnumJson::None;

	case EJson::Null:
		return StomtEnumJson::Null;

	case EJson::String:
		return StomtEnumJson::String;

	case EJson::Number:
		return StomtEnumJson::Number;

	case EJson::Boolean:
		return StomtEnumJson::Boolean;

	case EJson::Array:
		return StomtEnumJson::Array;

	case EJson::Object:
		return StomtEnumJson::Object;

	default:
		return StomtEnumJson::None;
	}
}

FString UStomtJsonValue::GetTypeString() const
{
	if (!JsonVal.IsValid())
	{
		return "None";
	}

	switch (JsonVal->Type)
	{
	case EJson::None:
		return TEXT("None");

	case EJson::Null:
		return TEXT("Null");

	case EJson::String:
		return TEXT("String");

	case EJson::Number:
		return TEXT("Number");

	case EJson::Boolean:
		return TEXT("Boolean");

	case EJson::Array:
		return TEXT("Array");

	case EJson::Object:
		return TEXT("Object");

	default:
		return TEXT("None");
	}
}

bool UStomtJsonValue::IsNull() const 
{
	if (!JsonVal.IsValid())
	{
		return true;
	}

	return JsonVal->IsNull();
}

float UStomtJsonValue::AsNumber() const
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Number"));
		return 0.f;
	}

	return JsonVal->AsNumber();
}

FString UStomtJsonValue::AsString() const
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("String"));
		return FString();
	}

	return JsonVal->AsString();
}

bool UStomtJsonValue::AsBool() const
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Boolean"));
		return false;
	}

	return JsonVal->AsBool();
}

TArray<UStomtJsonValue*> UStomtJsonValue::AsArray() const
{
	TArray<UStomtJsonValue*> OutArray;

	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Array"));
		return OutArray;
	}

	TArray< TSharedPtr<FJsonValue> > ValArray = JsonVal->AsArray();
	for (auto Value : ValArray)
	{
		UStomtJsonValue* NewValue = NewObject<UStomtJsonValue>();
		NewValue->SetRootValue(Value);

		OutArray.Add(NewValue);
	}

	return OutArray;
}


//
////////////////////////////////////////////////////////////////////////////
//// Helpers
//
void UStomtJsonValue::ErrorMessage(const FString& InType) const
{
	UE_LOG(Stomt, Error, TEXT("Json Value of type '%s' used as a '%s'."), *GetTypeString(), *InType);
}
