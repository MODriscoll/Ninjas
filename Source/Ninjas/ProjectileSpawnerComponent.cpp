#include "ProjectileSpawnerComponent.h"
#include "ProjectileBase.h"

#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UProjectileSpawnerComponent::UProjectileSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UProjectileSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

}

AProjectileBase * UProjectileSpawnerComponent::SpawnProjectile()
{
	if (!ProjectileClass)
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	// We need a location to spawn the projectile
	USceneComponent* SpawnSource = GetSpawnSourceComponent();
	if (!SpawnSource)
	{
		return nullptr;
	}

	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	SpawnSource->GetSocketWorldLocationAndRotation(SpawnSourceSocket, SpawnLocation, SpawnRotation);

	// Finish spawning the new projectile
	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.Instigator = Cast<APawn>(GetOwner());
	return World->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, Params);	
}

USceneComponent* UProjectileSpawnerComponent::GetSpawnSourceComponent()
{
	if (CachedSpawnSource.IsValid())
	{
		return CachedSpawnSource.Get();
	}

	// Try finding the component from our owner
	USceneComponent* SpawnSource = GetSpawnSourceComponentImpl();
	if (SpawnSource)
	{
		CachedSpawnSource = SpawnSource;
		return SpawnSource;
	}

	return nullptr;
}

USceneComponent* UProjectileSpawnerComponent::GetSpawnSourceComponent() const
{
	if (CachedSpawnSource.IsValid())
	{
		return CachedSpawnSource.Get();
	}

	return nullptr;
}

USceneComponent* UProjectileSpawnerComponent::ResetSpawnSourceComponent()
{
	// Nulling our cached reference will force another search for the component
	CachedSpawnSource = nullptr;
	return GetSpawnSourceComponent();
}

USceneComponent* UProjectileSpawnerComponent::GetSpawnSourceComponentImpl() const
{
	UActorComponent* Component = SpawnSourceComponent.GetComponent(GetOwner());
	if (Component)
	{
		USceneComponent* SceneComponent = Cast<USceneComponent>(Component);
		if (!SceneComponent)
		{
			UE_LOG(LogNinjas, Warning, TEXT("Projectile Spawn Source component %s is "
				"invalid as its not a scene component!"), *Component->GetName());
		}

		return SceneComponent;
	}

	return nullptr;
}

