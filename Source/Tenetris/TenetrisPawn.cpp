// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisPawn.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATenetrisPawn::ATenetrisPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorLocation(FVector::ZeroVector);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector::ZeroVector);
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Camera->SetOrthoWidth(1024.f);
	Camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void ATenetrisPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATenetrisPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATenetrisPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

