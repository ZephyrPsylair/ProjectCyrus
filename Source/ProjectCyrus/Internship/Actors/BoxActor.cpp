#include "ProjectCyrus/Internship/Actors/BoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "ProjectCyrus/Internship/Widgets/ScoreWidget.h"

ABoxActor::ABoxActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(RootComponent);
}

void ABoxActor::BeginPlay()
{
	Super::BeginPlay();

	if (BoxMesh && BoxMesh->GetMaterial(0))
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BoxMesh->GetMaterial(0), this); // Create a dynamic material instance from the box mesh's material
	}
}

void ABoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoxActor::SetColor(const FLinearColor& Color)
{
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetVectorParameterValue(TEXT("BaseColor"), Color); 
		BoxMesh->SetMaterial(0, DynamicMaterialInstance); // Set the color of the dynamic material instance
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dynamic Material Instance is not initialized.")); // Error handling if the dynamic material instance is not initialized
	}
}

float ABoxActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsAlive)
	{
		return 0.0f; // If the box is already dead, no damage is applied
	}

	Health -= DamageAmount; // Reduce the health of the box by the damage amount

	if (Health <= 0) // Called when the box's health reaches zero or below
	{
		bIsAlive = false;

		if (ScoreWidget)
		{
			ScoreWidget->BP_UpdateScore(Score); // Update the score in the ScoreWidget
		}

		this->Destroy(); // Destroy the box actor when its health reaches zero
	}

	return DamageAmount;
}





