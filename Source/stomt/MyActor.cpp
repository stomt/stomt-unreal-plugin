// Fill out your copyright notice in the Description page of Project Settings.

#include "stomt.h"
#include "MyActor.h"
//#include "StomtRestRequest.h"
#include "StomtPlugin.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();


	//UStomtRestRequest *request = NewObject<UStomtRestRequest>();
	//request->MyHttpCall();
	
}

// Called every frame
void AMyActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

