// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisPawn.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATNTenetrisPawn::ATNTenetrisPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector::ZeroVector);
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Camera->SetOrthoWidth(1024.f);
	Camera->bUsePawnControlRotation = true;

	RootComponent = Camera;
}

// Called when the game starts or when spawned
void ATNTenetrisPawn::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(FVector::ZeroVector);
}

// Called every frame
void ATNTenetrisPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATNTenetrisPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

