#include "ProjectileSpawnerComponent.h"
#include "ProjectileBase.h"

#include "TimerManager.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UProjectileSpawnerComponent::UProjectileSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SpawnDelay = 0.2f;
	bStartWithAutomaticSpawning = false;
}

#if WITH_EDITOR
void UProjectileSpawnerComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UProjectileSpawnerComponent, SpawnDelay))
	{
		// We only need to update ourselves if the game world
		UWorld* World = GetWorld();
		if (!World || !World->IsGameWorld())
		{
			return;
		}

		// Need to update the tick interval of automatic spawning if active
		if (IsAutomaticallySpawning())
		{
			FTimerManager& TimerManager = World->GetTimerManager();
			TimerManager.ClearTimer(Handle_AutomaticSpawning);

			float Interval = SpawnDelay <= 0.f ? 0.1f : SpawnDelay;

			// Re-caclulate current delay, we set this as initial delay for timer
			float WaitTillTime = LastSpawnTime + SpawnDelay;
			float InitialDelay = WaitTillTime - World->GetTimeSeconds();

			// If we can already spawn a projectile, do so
			if (InitialDelay <= 0.f)
			{
				SpawnProjectile();
				InitialDelay = -1.f;
			}

			TimerManager.SetTimer(Handle_AutomaticSpawning, this, &UProjectileSpawnerComponent::OnAutomaticSpawnTick,
				Interval, true, InitialDelay);
		}
	}
}
#endif

void UProjectileSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	LastSpawnTime = -SpawnDelay;
	SetAutomaticSpawningImpl(bStartWithAutomaticSpawning);
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

	// We might be waiting for projectiles to restock
	if (!CanSpawnProjectile())
	{
		return nullptr;
	}

	LastSpawnTime = World->GetTimeSeconds();

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

bool UProjectileSpawnerComponent::CanSpawnProjectile() const
{
	UWorld* World = GetWorld();
	if (World)
	{
		float WaitTillTime = LastSpawnTime + SpawnDelay;
		return World->GetTimeSeconds() >= WaitTillTime;
	}

	return false;
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

void UProjectileSpawnerComponent::SetAutomaticSpawning(bool bEnable)
{
	if (bEnable != IsAutomaticallySpawning())
	{
		SetAutomaticSpawningImpl(bEnable);
	}
}

bool UProjectileSpawnerComponent::IsAutomaticallySpawning() const
{
	return Handle_AutomaticSpawning.IsValid();
}

void UProjectileSpawnerComponent::SetAutomaticSpawningImpl(bool bEnable)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		if (bEnable)
		{
			UE_LOG(LogNinjas, Error, TEXT("Unable to set projectile spawner "
				"to auto spawn as GetWorld() returned null"));
		}

		return;
	}

	if (bEnable)
	{
		if (Handle_AutomaticSpawning.IsValid())
		{
			return;
		}

		float Interval = SpawnDelay <= 0.f ? 0.1f : SpawnDelay;

		// The initial delay, if we are already delaying another projectile to spawn,
		// we want to pick up from the earliest possible time we can spawn another
		float WaitTillTime = LastSpawnTime + SpawnDelay;
		float InitialDelay = WaitTillTime - World->GetTimeSeconds();
		if (InitialDelay <= 0.f)
		{
			InitialDelay = -1.f;
		}

		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.SetTimer(Handle_AutomaticSpawning, this, &UProjectileSpawnerComponent::OnAutomaticSpawnTick,
			Interval, true, InitialDelay);
	}
	else
	{
		if (!Handle_AutomaticSpawning.IsValid())
		{
			return;
		}

		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(Handle_AutomaticSpawning);
	}
}

void UProjectileSpawnerComponent::OnAutomaticSpawnTick()
{
	SpawnProjectile();
}

