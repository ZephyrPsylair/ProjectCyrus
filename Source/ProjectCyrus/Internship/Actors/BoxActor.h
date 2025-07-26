#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxActor.generated.h"

UCLASS()

class PROJECTCYRUS_API ABoxActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ABoxActor(); //Constructor

public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 Health = 0; // Variable to store the health of the box

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 Score = 0; // Variable to store the score associated with the box

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bIsAlive = true; // Variable to check if the box is alive

	UPROPERTY()
	class UScoreWidget* ScoreWidget; // Pointer to the ScoreWidget instance

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box")
	class UStaticMeshComponent* BoxMesh; // Static mesh component for the box

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box")
	class USceneComponent* SceneRoot; // Scene root component for the box actor

	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicMaterialInstance; // Dynamic material instance for changing the color of the box

	virtual void BeginPlay() override; // Function called when the actor is spawned or begins play

	virtual void Tick(float DeltaTime) override; // Function called every frame

	UFUNCTION()
	void SetColor(const FLinearColor& Color); // Function to set the color of the box using a dynamic material instance

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override; // Function to handle damage taken by the box actor

};
