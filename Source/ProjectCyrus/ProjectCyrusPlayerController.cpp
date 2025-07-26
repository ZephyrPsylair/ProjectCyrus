// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectCyrusPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "ProjectCyrusCameraManager.h"

AProjectCyrusPlayerController::AProjectCyrusPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AProjectCyrusCameraManager::StaticClass();
}

void AProjectCyrusPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
