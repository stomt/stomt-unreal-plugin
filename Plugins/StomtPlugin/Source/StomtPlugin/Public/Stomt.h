#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "Stomt.generated.h"

UCLASS()
class UStomt : public UObject
{
	GENERATED_BODY()

public:
	//////////////////////////////////////////////////////////////////////////
	// Construction

	UStomt();

	/** Create new Stomt object */
	static UStomt* ConstructStomt(FString target_id, bool positive, FString text);

	//////////////////////////////////////////////////////////////////////////
	// Destruction and reset

	//////////////////////////////////////////////////////////////////////////
	// Data accessors

	void SetTargetID(FString target_id);
	void SetPoitive(bool positive);
	void SetText(FString text);
	void SetAnonym();

	//////////////////////////////////////////////////////////////////////////
	// Data

private:

	FString		target_id;
	bool		positive;
	FString		text;
	FString		url;
	bool		anonym;
	FString		img_name;
	FString		lonlat;
};

