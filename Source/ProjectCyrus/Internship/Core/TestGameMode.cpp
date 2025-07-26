#include "ProjectCyrus/Internship/Core/TestGameMode.h"
#include "Http.h"
#include "ProjectCyrus/Internship/Actors/BoxActor.h"
#include "Kismet/GameplayStatics.h"


void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	FetchJson(); // Fetch JSON data from the specified URL

	ScoreWidget = CreateWidget<UScoreWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ScoreWidgetClass);
	ScoreWidget->AddToViewport(0); //Create and add the ScoreWidget to the viewport
}

void ATestGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestGameMode::FetchJson()
{
	FString Url = TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json"); // URL to fetch JSON data from

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest(); // Create a new HTTP request
	Request->SetURL(Url); // Set the URL for the request
	Request->SetVerb(TEXT("GET")); // Set the HTTP method to GET
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // Set the content type header to application/json
	Request->OnProcessRequestComplete().BindUObject(this, &ATestGameMode::OnResponseReceived); // Bind the callback function to handle the response
	Request->ProcessRequest(); // Send the request
}

void ATestGameMode::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to fetch JSON data.")); //Error handling if the request was not successful or the response is invalid
		return;
	}
	
	TSharedPtr<FJsonObject> JsonObject; // Shared pointer to hold the parsed JSON object
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString()); // JSON reader to read the response content

	if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		const TArray<TSharedPtr<FJsonValue>>* TypesArray; // Pointer to hold the array of box types from the JSON object

		if (JsonObject->TryGetArrayField(TEXT("types"), TypesArray)) // Check if the "types" field exists
		{
			for (auto& TypeValue : *TypesArray) // Loop for each type in the array 
			{
				auto TypeObject = TypeValue->AsObject(); // Convert the JSON value to a JSON object

				FBoxTypeData BoxTypeData; //Structure to hold the box type data
				BoxTypeData.Name = TypeObject->GetStringField(TEXT("name")); // Stores the name of the box type

				const TArray<TSharedPtr<FJsonValue>>* ColorArray; //Array to hold the color values
				if (TypeObject->TryGetArrayField(TEXT("color"), ColorArray))
				{
					BoxTypeData.Color = FLinearColor(
						(*ColorArray)[0]->AsNumber() / 255.f,
						(*ColorArray)[1]->AsNumber() / 255.f,
						(*ColorArray)[2]->AsNumber() / 255.f
					);
				}

				BoxTypeData.Health = TypeObject->GetIntegerField(TEXT("health")); // Stores the health of the box type
				BoxTypeData.Score = TypeObject->GetIntegerField(TEXT("score")); // Stores the score of the box type

				BoxTypes.Add(BoxTypeData.Name, BoxTypeData); // Adds the box type data to the map

				UE_LOG(LogTemp, Log, TEXT("Parsed Box Type Data"));
			}
		}

		const TArray<TSharedPtr<FJsonValue>>* ObjectsArray; // Pointer to hold the array of objects from the JSON object

		if (JsonObject->TryGetArrayField(TEXT("objects"), ObjectsArray)) // Check if the "objects" field exists
		{
			for (auto& ObjValue : *ObjectsArray) // Loop for each object in the array
			{
				auto Obj = ObjValue->AsObject(); // Convert the JSON value to a JSON object
				FBoxSpawnData SpawnData;
				SpawnData.Type = Obj->GetStringField(TEXT("type")); // Stores the type of the box to spawn

				const TSharedPtr<FJsonObject>* ObjTransform; // Pointer to hold the transform data of the object

				if (Obj->TryGetObjectField(TEXT("transform"), ObjTransform)) // Check if the "transform" field exists
				{
					auto GetVec3 = [](const TArray<TSharedPtr<FJsonValue>>& TransformArray) {
						return FVector(
							TransformArray[0]->AsNumber(),
							TransformArray[1]->AsNumber(),
							TransformArray[2]->AsNumber()
						);
						};

					const TArray<TSharedPtr<FJsonValue>>* LocationArray; // Array to hold the location values
					const TArray<TSharedPtr<FJsonValue>>* RotationArray; // Array to hold the rotation values
					const TArray<TSharedPtr<FJsonValue>>* ScaleArray; // Array to hold the scale values

					if ((*ObjTransform)->TryGetArrayField(TEXT("location"), LocationArray)) // Calls and stores the value if the "location" field exists
						SpawnData.Location = GetVec3(*LocationArray);
						
					if ((*ObjTransform)->TryGetArrayField(TEXT("rotation"), RotationArray)) // Calls and stores the value if the "rotation" field exists
						SpawnData.Rotation = FRotator::MakeFromEuler(GetVec3(*RotationArray));

					if ((*ObjTransform)->TryGetArrayField(TEXT("scale"), ScaleArray)) // Calls and stores the value if the "scale" field exists
						SpawnData.Scale = GetVec3(*ScaleArray);
				}

				if (BoxTypes.Contains(SpawnData.Type))
				{
					SpawnActor(BoxTypes[SpawnData.Type], SpawnData); // Spawns the actor using the box type data and spawn data
				}
			}
		}
	}

	else if(!JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON data.")); //Error handling if the JSON object is not valid
		return;
	}
}

void ATestGameMode::SpawnActor(const FBoxTypeData& TypeData, const FBoxSpawnData& SpawnData)
{
	AActor* BoxActor = GetWorld()->SpawnActor<ABoxActor>(BoxActorClass, SpawnData.Location, SpawnData.Rotation); //Spawns a new BoxActor

	if (BoxActor)
	{
		BoxActor->SetActorScale3D(SpawnData.Scale); // Sets the scale of the spawned actor

		#if WITH_EDITOR
			BoxActor->SetActorLabel(TypeData.Name); // Works only in editor builds
		#endif

		ABoxActor* Box = Cast<ABoxActor>(BoxActor); // Casts the spawned actor to ABoxActor to access its properties
		if (Box)
		{
			Box->SetColor(TypeData.Color); // Sets the color of the box using the dynamic material instance
			Box->Health = TypeData.Health; // Sets the health of the box
			Box->Score = TypeData.Score; // Sets the score of the box
			Box->ScoreWidget = ScoreWidget; // Assigns the ScoreWidget to the box actor
		}

		UE_LOG(LogTemp, Log, TEXT("Spawned Box Actor: %s"), *TypeData.Name); // Logs the successful spawning of the box actor
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Box Actor: %s"), *TypeData.Name); // Logs an error if the box actor could not be spawned
	}
}

