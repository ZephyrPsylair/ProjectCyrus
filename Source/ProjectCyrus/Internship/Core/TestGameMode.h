#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "ProjectCyrus/Internship/Actors/BoxActor.h"
#include "ProjectCyrus/Internship/Widgets/ScoreWidget.h"
#include "TestGameMode.generated.h"

USTRUCT()
struct FBoxTypeData
{
    GENERATED_BODY()

    FString Name;
    FLinearColor Color;
    int32 Health;
    int32 Score;
};

USTRUCT()
struct FBoxSpawnData
{
    GENERATED_BODY()

    FString Type;
    FVector Location;
    FRotator Rotation;
    FVector Scale;
};

UCLASS()

class PROJECTCYRUS_API ATestGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UScoreWidget> ScoreWidgetClass; // Class reference for the ScoreWidget

	UPROPERTY()
	TObjectPtr<UScoreWidget> ScoreWidget; // Pointer of the ScoreWidget

    UPROPERTY()
	TMap<FString, FBoxTypeData> BoxTypes; // Map to store box types with their data

    UPROPERTY(EditAnywhere, Category = "Box")
	TSubclassOf<ABoxActor> BoxActorClass;  // Class reference for the BoxActor

	void FetchJson(); // Function to fetch JSON data from a URL

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); // Callback function for when the HTTP request is completed

    UFUNCTION()
	void SpawnActor(const FBoxTypeData& TypeData, const FBoxSpawnData& SpawnData); //Function to spawn an actor based on the box type data and spawn data
};
