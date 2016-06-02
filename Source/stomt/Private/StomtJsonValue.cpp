// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.
// Copyright 2016 Stomt. All Rights Reserved.

#include "stomt.h"
#include "StomtJsonValue.h"
#include "CoreMisc.h"

//UStomtJsonValue::UStomtJsonValue(const class FObjectInitializer& PCIP)
//{
//
//}
//
//UStomtJsonValue::UStomtJsonValue(FString& StringValue)
//{
//	this->JsonVal = MakeShareable(new FJsonValueString(StringValue));
//}

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

//UStomtJsonValue* UStomtJsonValue::ConstructJsonValueObject(UObject* WorldContextObject, UStomtJsonObject *JsonObject)
//{
//	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueObject(JsonObject->GetRootObject()));
//
//	UStomtJsonValue* NewValue = NewObject<UStomtJsonValue>();
//	NewValue->SetRootValue(NewVal);
//
//	return NewValue;
//}

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

EVaJson::Type UStomtJsonValue::GetType() const
{
	if (!(JsonVal.IsValid()))
	{
		return EVaJson::None;
	}

	switch (JsonVal->Type)
	{
	case EJson::None:
		return EVaJson::None;

	case EJson::Null:
		return EVaJson::Null;

	case EJson::String:
		return EVaJson::String;

	case EJson::Number:
		return EVaJson::Number;

	case EJson::Boolean:
		return EVaJson::Boolean;

	case EJson::Array:
		return EVaJson::Array;

	case EJson::Object:
		return EVaJson::Object;

	default:
		return EVaJson::None;
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

//UStomtJsonObject* UStomtJsonValue::AsObject()
//{
//	if (!JsonVal.IsValid())
//	{
//		ErrorMessage(TEXT("Object"));
//		return NULL;
//	}
//
//	TSharedPtr<FJsonObject> NewObj = JsonVal->AsObject();
//
//	UStomtJsonObject* JsonObj = NewObject<UStomtJsonObject>();
//	JsonObj->SetRootObject(NewObj);
//
//	return JsonObj;
//}

//
////////////////////////////////////////////////////////////////////////////
//// Helpers
//
void UStomtJsonValue::ErrorMessage(const FString& InType) const
{
	UE_LOG(LogTemp, Error, TEXT("Json Value of type '%s' used as a '%s'."), *GetTypeString(), *InType);
}
