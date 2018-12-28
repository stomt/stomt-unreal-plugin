// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtJsonObject.h"
#include "StomtPluginPrivatePCH.h"

typedef TJsonWriterFactory< TCHAR, TCondensedJsonPrintPolicy<TCHAR> > FCondensedJsonStringWriterFactory;
typedef TJsonWriter< TCHAR, TCondensedJsonPrintPolicy<TCHAR> > FCondensedJsonStringWriter;

UStomtRestJsonObject::UStomtRestJsonObject()
{
	Reset();
}

UStomtRestJsonObject* UStomtRestJsonObject::ConstructJsonObject(UObject* WorldContextObject)
{
	return NewObject<UStomtRestJsonObject>();
}

void UStomtRestJsonObject::Reset()
{
	if (JsonObj.IsValid())
	{
		JsonObj.Reset();
	}

	JsonObj = MakeShareable(new FJsonObject());
}

TSharedPtr<FJsonObject>& UStomtRestJsonObject::GetRootObject()
{
	return JsonObj;
}

void UStomtRestJsonObject::SetRootObject(TSharedPtr<FJsonObject>& JsonObject)
{
	JsonObj = JsonObject;
}


////////////////////////////////////////////////////////////////////////////
//// Serialization

FString UStomtRestJsonObject::EncodeJson() const
{
	if (!JsonObj.IsValid())
	{
		return TEXT("");
	}

	FString OutputString;
	TSharedRef< FCondensedJsonStringWriter > Writer = FCondensedJsonStringWriterFactory::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);

	return OutputString;
}

FString UStomtRestJsonObject::EncodeJsonToSingleString() const
{
	FString OutputString = EncodeJson();

	// Remove line terminators
	OutputString.Replace(LINE_TERMINATOR, TEXT(""));
	
	// Remove tabs
	OutputString.Replace(LINE_TERMINATOR, TEXT("\t"));

	return OutputString;
}

bool UStomtRestJsonObject::DecodeJson(const FString& JsonString)
{
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(*JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonObj) && JsonObj.IsValid())
	{
		return true;
	}

	// If we've failed to deserialize the string, we should clear our internal data
	Reset();

	UE_LOG(Stomt, Error, TEXT("Json decoding failed for: %s"), *JsonString);

	return false;
}


////////////////////////////////////////////////////////////////////////////
//// FJsonObject API
//
TArray<FString> UStomtRestJsonObject::GetFieldNames()
{
	TArray<FString> Result;
	
	if (!JsonObj.IsValid())
	{
		return Result;
	}
	
	JsonObj->Values.GetKeys(Result);
	
	return Result;
}

bool UStomtRestJsonObject::HasField(const FString& FieldName) const
{
	if (!JsonObj.IsValid())
	{
		return false;
	}

	return JsonObj->HasField(FieldName);
}

void UStomtRestJsonObject::RemoveField(const FString& FieldName)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	JsonObj->RemoveField(FieldName);
}

UStomtJsonValue* UStomtRestJsonObject::GetField(const FString& FieldName) const
{
	if (!JsonObj.IsValid())
	{
		return NULL;
	}

	TSharedPtr<FJsonValue> NewVal = JsonObj->TryGetField(FieldName);

	UStomtJsonValue* NewValue = NewObject<UStomtJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

void UStomtRestJsonObject::SetField(const FString& FieldName, UStomtJsonValue* JsonValue)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	JsonObj->SetField(FieldName, JsonValue->GetRootValue());
}


////////////////////////////////////////////////////////////////////////////
//// FJsonObject API Helpers (easy to use with simple Json objects)

float UStomtRestJsonObject::GetNumberField(const FString& FieldName) const
{
	if (!JsonObj.IsValid())
	{
		return 0.0f;
	}

	return JsonObj->GetNumberField(FieldName);
}

void UStomtRestJsonObject::SetNumberField(const FString& FieldName, float Number)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	JsonObj->SetNumberField(FieldName, Number);
}

FString UStomtRestJsonObject::GetStringField(const FString& FieldName) const
{
	if (!JsonObj.IsValid())
	{
		return TEXT("");
	}
	
	return JsonObj->GetStringField(FieldName);
}

void UStomtRestJsonObject::SetStringField(const FString& FieldName, const FString& StringValue)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	JsonObj->SetStringField(FieldName, StringValue);
}

bool UStomtRestJsonObject::GetBoolField(const FString& FieldName) const
{
	if (!JsonObj.IsValid())
	{
		return false;
	}

	return JsonObj->GetBoolField(FieldName);
}

void UStomtRestJsonObject::SetBoolField(const FString& FieldName, bool InValue)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	JsonObj->SetBoolField(FieldName, InValue);
}

TArray<UStomtJsonValue*> UStomtRestJsonObject::GetArrayField(const FString& FieldName)
{
	TArray<UStomtJsonValue*> OutArray;
	if (!JsonObj.IsValid())
	{
		return OutArray;
	}


	TArray< TSharedPtr<FJsonValue> > ValArray = JsonObj->GetArrayField(FieldName);
	for (auto Value : ValArray)
	{
		UStomtJsonValue* NewValue = NewObject<UStomtJsonValue>();
		NewValue->SetRootValue(Value);

		OutArray.Add(NewValue);
	}

	return OutArray;
}

void UStomtRestJsonObject::SetArrayField(const FString& FieldName, const TArray<UStomtJsonValue*>& InArray)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	TArray< TSharedPtr<FJsonValue> > ValArray;

	// Process input array and COPY original values
	for (auto InVal : InArray)
	{
		TSharedPtr<FJsonValue> JsonVal = InVal->GetRootValue();

		switch (InVal->GetType())
		{
		case StomtEnumJson::None:
			break;

		case StomtEnumJson::Null:
			ValArray.Add(MakeShareable(new FJsonValueNull()));
			break;

		case StomtEnumJson::String:
			ValArray.Add(MakeShareable(new FJsonValueString(JsonVal->AsString())));
			break;

		case StomtEnumJson::Number:
			ValArray.Add(MakeShareable(new FJsonValueNumber(JsonVal->AsNumber())));
			break;

		case StomtEnumJson::Boolean:
			ValArray.Add(MakeShareable(new FJsonValueBoolean(JsonVal->AsBool())));
			break;

		case StomtEnumJson::Array:
			ValArray.Add(MakeShareable(new FJsonValueArray(JsonVal->AsArray())));
			break;

		case StomtEnumJson::Object:
			ValArray.Add(MakeShareable(new FJsonValueObject(JsonVal->AsObject())));
			break;

		default:
			break;
		}
	}

	JsonObj->SetArrayField(FieldName, ValArray);
}

void UStomtRestJsonObject::MergeJsonObject(UStomtRestJsonObject* InJsonObject, bool Overwrite)
{
	TArray<FString> Keys = InJsonObject->GetFieldNames();
	
	for (auto Key : Keys)
	{
		if (Overwrite == false && HasField(Key))
		{
			continue;
		}
		
		SetField(Key, InJsonObject->GetField(Key));
	}
}

UStomtRestJsonObject* UStomtRestJsonObject::GetObjectField(const FString& FieldName) const
{
	if (!JsonObj.IsValid())
	{
		return NULL;
	}

	if ( !JsonObj->HasField(FieldName) )
	{
		return NULL;
	}

	TSharedPtr<FJsonObject> JsonObjField = JsonObj->GetObjectField(FieldName);

	UStomtRestJsonObject* OutRestJsonObj = NewObject<UStomtRestJsonObject>();
	OutRestJsonObj->SetRootObject(JsonObjField);

	return OutRestJsonObj;
}

void UStomtRestJsonObject::SetObjectField(const FString& FieldName, UStomtRestJsonObject* JsonObject)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	JsonObj->SetObjectField(FieldName, JsonObject->GetRootObject());
}


////////////////////////////////////////////////////////////////////////////
//// Array fields helpers (uniform arrays)

TArray<float> UStomtRestJsonObject::GetNumberArrayField(const FString& FieldName)
{
	TArray<float> NumberArray;

	if (!JsonObj.IsValid())
	{
		return NumberArray;
	}

	TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
	for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
	{
		NumberArray.Add((*It)->AsNumber());
	}

	return NumberArray;
}

void UStomtRestJsonObject::SetNumberArrayField(const FString& FieldName, const TArray<float>& NumberArray)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	TArray< TSharedPtr<FJsonValue> > EntriesArray;

	for (auto Number : NumberArray)
	{
		EntriesArray.Add(MakeShareable(new FJsonValueNumber(Number)));
	}

	JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<FString> UStomtRestJsonObject::GetStringArrayField(const FString& FieldName)
{
	TArray<FString> StringArray;

	if (!JsonObj.IsValid())
	{
		return StringArray;
	}

	TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
	for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
	{
		StringArray.Add((*It)->AsString());
	}

	return StringArray;
}

void UStomtRestJsonObject::SetStringArrayField(const FString& FieldName, const TArray<FString>& StringArray)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	TArray< TSharedPtr<FJsonValue> > EntriesArray;

	for (auto String : StringArray)
	{
		EntriesArray.Add(MakeShareable(new FJsonValueString(String)));
	}

	JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<bool> UStomtRestJsonObject::GetBoolArrayField(const FString& FieldName)
{
	TArray<bool> BoolArray;

	if (!JsonObj.IsValid())
	{
		return BoolArray;
	}

	TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
	for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
	{
		BoolArray.Add((*It)->AsBool());
	}

	return BoolArray;
}

void UStomtRestJsonObject::SetBoolArrayField(const FString& FieldName, const TArray<bool>& BoolArray)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	TArray< TSharedPtr<FJsonValue> > EntriesArray;

	for (auto Boolean : BoolArray)
	{
		EntriesArray.Add(MakeShareable(new FJsonValueBoolean(Boolean)));
	}

	JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<UStomtRestJsonObject*> UStomtRestJsonObject::GetObjectArrayField(const FString& FieldName)
{
	TArray<UStomtRestJsonObject*> OutArray;

	if (!JsonObj.IsValid())
	{
		return OutArray;
	}

	TArray< TSharedPtr<FJsonValue> > ValArray = JsonObj->GetArrayField(FieldName);
	for (auto Value : ValArray)
	{
		TSharedPtr<FJsonObject> NewObj = Value->AsObject();

		UStomtRestJsonObject* NewJson = NewObject<UStomtRestJsonObject>();
		NewJson->SetRootObject(NewObj);

		OutArray.Add(NewJson);
	}

	return OutArray;
}

void UStomtRestJsonObject::SetObjectArrayField(const FString& FieldName, const TArray<UStomtRestJsonObject*>& ObjectArray)
{
	if (!JsonObj.IsValid())
	{
		return;
	}

	TArray< TSharedPtr<FJsonValue> > EntriesArray;

	for (auto Value : ObjectArray)
	{
		EntriesArray.Add(MakeShareable(new FJsonValueObject(Value->GetRootObject())));
	}

	JsonObj->SetArrayField(FieldName, EntriesArray);
}
