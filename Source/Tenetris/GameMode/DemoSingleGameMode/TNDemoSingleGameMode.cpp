// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DemoSingleGameMode/TNDemoSingleGameMode.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TestModel/TNTestModel.h"
#include "Tenetris/Module/MVC/View/TestView/TNTestView.h"
#include "Tenetris/Module/MVC/Controller/TestController/TNTestController.h"
#include "Tenetris/Module/MVC/Model/Field/TNFieldModel.h"
#include "Tenetris/Actors/Field/TNFieldBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ATNDemoSingleGameMode::ATNDemoSingleGameMode()
{
  PrimaryActorTick.bStartWithTickEnabled = true;
  PrimaryActorTick.bCanEverTick = true;
}

void ATNDemoSingleGameMode::StartPlay()
{
  Super::StartPlay();

  FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

  holder.SetModel(MakeShareable(new FTNTestModel()));
  holder.SetView(MakeShareable(new FTNTestView()));
  holder.SetController(MakeShareable(new FTNTestController()));

  TSharedPtr<ITNController> tnController = holder.GetController();
  if (!tnController.IsValid())
  {
    return;
  }

  tnController->Init();

  TArray<AActor*> playerFieldStartActors;
  UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PlayerFieldStart"), playerFieldStartActors);
  if (playerFieldStartActors.Num() > 0 && IsValid(playerFieldStartActors[0]))
  {
    AActor* positionActor = playerFieldStartActors[0];
    positionActor->SetActorHiddenInGame(true);
    PlayerField = spawnField(positionActor->GetActorLocation(), FVector(1.f, 1.f, 1.f));
    FTNFieldContext fieldContext(E_TNFieldType::Player);
    tnController->CreateField(fieldContext, RowMax, ColumnMax, PlayerField);
  }

  // Bind the single player board's game-rule delegates (score / level / game over).
  TSharedPtr<ITNModel> tnModel = holder.GetModel();
  if (tnModel.IsValid())
  {
    const TSharedPtr<FTNFieldModel> playerFieldModel = tnModel->GetPlayerFieldModel();
    if (playerFieldModel.IsValid())
    {
      playerFieldModel->GetOnLinesClearedDelegate().BindUObject(this, &ATNDemoSingleGameMode::handleLinesCleared);
      playerFieldModel->GetOnGameOverDelegate().BindUObject(this, &ATNDemoSingleGameMode::handleGameOver);
    }
  }

  tnController->StartPlay();

  if (HUDWidgetClass)
  {
    if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
    {
      if (UUserWidget* hudWidget = CreateWidget<UUserWidget>(playerController, HUDWidgetClass))
      {
        hudWidget->AddToViewport();
      }
    }
  }
}

void ATNDemoSingleGameMode::Tick(float deltaSeconds)
{
  // Once the board tops out, stop driving the simulation.
  if (bGameOver)
  {
    return;
  }

  FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

  TSharedPtr<ITNController> tnController = holder.GetController();
  if (tnController.IsValid())
  {
    tnController->Tick(deltaSeconds);
  }
}

void ATNDemoSingleGameMode::handleLinesCleared(const int32 lineCount)
{
  if (lineCount <= 0)
  {
    return;
  }

  TotalLines += lineCount;
  Score += scoreForLines(lineCount) * Level;

  // Level up every LinesPerLevel cleared lines.
  Level = (TotalLines / LinesPerLevel) + 1;

  // TODO: Feed Level into the board's fall speed (getFallingSpeed) once a setter/hook exists.

  OnScoreChanged.Broadcast(Score, Level, TotalLines);
}

void ATNDemoSingleGameMode::handleGameOver()
{
  bGameOver = true;

  OnGameOverUI.Broadcast();
}

void ATNDemoSingleGameMode::RestartMatch()
{
  UWorld* world = GetWorld();
  if (!IsValid(world))
  {
    return;
  }

  // Reload the current level to replay from scratch.
  const FName currentLevelName(*UGameplayStatics::GetCurrentLevelName(world, true));
  UGameplayStatics::OpenLevel(world, currentLevelName);
}

void ATNDemoSingleGameMode::ReturnToTitle()
{
  if (TitleLevelName.IsNone())
  {
    return;
  }

  UGameplayStatics::OpenLevel(this, TitleLevelName);
}

int32 ATNDemoSingleGameMode::scoreForLines(const int32 lineCount) const
{
  // Standard-guideline-like base points (before level scaling).
  switch (lineCount)
  {
  case 1:  return 100;
  case 2:  return 300;
  case 3:  return 500;
  case 4:  return 800;
  default: return 0;
  }
}

ATNFieldBase* ATNDemoSingleGameMode::spawnField(const FVector& location, const FVector& scale)
{
  UWorld* world = GetWorld();

  if (!IsValid(world))
  {
    return nullptr;
  }

  ATNFieldBase* field = world->SpawnActorDeferred<ATNFieldBase>(FieldClass, FTransform(location), this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
  field->SetActorScale3D(scale);
  UGameplayStatics::FinishSpawningActor(field, FTransform(location));

  return field;
}