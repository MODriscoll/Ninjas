#include "ProjectileBase.h"
#include "NinjasGameSettings.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBase::AProjectileBase()
{	
	PrimaryActorTick.bCanEverTick = false;

	bReportOverlaps = false;
	bReportHits = true;
	bIgnoreInstigator = true;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	MovementComponent->InitialSpeed = 2000.f;
	MovementComponent->MaxSpeed = 2000.f;

	// Limit movement (this is a 2D game)
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintAxisSetting(GetDefault<UNinjasGameSettings>()->DefaultPlaneConstraintAxis);
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (bIgnoreInstigator && Instigator)
	{
		UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent());
		if (RootPrimitive)
		{
			RootPrimitive->IgnoreActorWhenMoving(Instigator, true);
		}

		Instigator->MoveIgnoreActorAdd(this);
	}
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// Overlap response
	if (bReportOverlaps)
	{
		OnActorBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);
	}

	// Hit response
	if (bReportHits)
	{
		OnActorHit.AddDynamic(this, &AProjectileBase::OnHit);
	}
}

bool AProjectileBase::IsComponentRelevantForNavigation(UActorComponent* Component) const
{
	// If we use navigation, we don't want to affect it
	return false;
}

bool AProjectileBase::ShouldIgnoreCollisionWith(AActor* OtherActor) const
{
	if (OtherActor == this)
	{
		return true;
	}

	// Possible that another primitive component has collsion enabled
	// and has collided with our instigator. We need to check for that here again
	if (bIgnoreInstigator && OtherActor == Instigator)
	{
		return true;
	}

	return false;
}

void AProjectileBase::OnOverlap(AActor* SelfActor, AActor* OtherActor)
{
	if (!ShouldIgnoreCollisionWith(OtherActor))
	{
		BP_OnActorOverlapped(OtherActor);
	}
}

void AProjectileBase::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!ShouldIgnoreCollisionWith(OtherActor))
	{
		BP_OnActorHit(OtherActor, NormalImpulse, Hit);
	}
}

void AProjectileBase::Reflect(bool bResetLifespan)
{
	// Quick method for now, in future we might want to use a normal
	// TODO: Look into UProjectileMovementComponent::HandleDeflection
	MovementComponent->Velocity = -MovementComponent->Velocity;

	if (bResetLifespan)
	{
		SetLifeSpan(InitialLifeSpan);
	}
}
