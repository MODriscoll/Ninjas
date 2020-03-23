#include "BaseNinjaCharacter.h"
#include "HealthComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseNinjaCharacter::ABaseNinjaCharacter()
{
	// Configure capsule
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->bOrientRotationToMovement = true; 
	MovementComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MovementComp->GravityScale = 2.f;
	MovementComp->AirControl = 0.80f;
	MovementComp->JumpZVelocity = 1000.f;
	MovementComp->GroundFriction = 3.f;
	MovementComp->MaxWalkSpeed = 600.f;
	MovementComp->MaxFlySpeed = 600.f;

	// Create health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeath.AddDynamic(this, &ABaseNinjaCharacter::OnDeath);
}

void ABaseNinjaCharacter::SetRagdollEnabled(bool bEnable)
{
	if (bEnable)
	{
		EnterRagdoll();
	}
	else
	{
		ExitRagdoll();
	}
}

void ABaseNinjaCharacter::EnterRagdoll()
{
	if (bIsRagdoll)
	{
		return;
	}

	// Stop any animations that might be playing
	StopAnimMontage();

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->StopActiveMovement();
	MovementComp->DisableMovement();

	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent->SetAllBodiesSimulatePhysics(true);
	MeshComponent->SetAllBodiesPhysicsBlendWeight(1.f);
	MeshComponent->SetAnimationMode(EAnimationMode::AnimationCustomMode);
	MeshComponent->SetCollisionProfileName(TEXT("Ragdoll"));

	bIsRagdoll = true;
}

void ABaseNinjaCharacter::ExitRagdoll()
{
	// This is unlikely to be called (except for testing the ragdolls)
	if (!bIsRagdoll)
	{
		return;
	}

	ABaseNinjaCharacter* DefaultCharacter = GetClass()->GetDefaultObject<ABaseNinjaCharacter>();

	// We grab the defaults so we properly restore the collision profiles (as we never no what changes could be made in blueprints)
	const UCapsuleComponent* DefaultCapsuleComp = CastChecked<UCapsuleComponent>(DefaultCharacter->GetDefaultSubobjectByName(ACharacter::CapsuleComponentName));
	const USkeletalMeshComponent* DefaultMeshComp = CastChecked<USkeletalMeshComponent>(DefaultCharacter->GetDefaultSubobjectByName(ACharacter::MeshComponentName));

	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent->SetCollisionProfileName(DefaultMeshComp->GetCollisionProfileName());
	MeshComponent->SetAnimationMode(DefaultMeshComp->GetAnimationMode());
	MeshComponent->SetAllBodiesPhysicsBlendWeight(0.f);
	MeshComponent->SetAllBodiesSimulatePhysics(false);

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(DefaultCapsuleComp->GetCollisionEnabled());

	bIsRagdoll = false;
}

bool ABaseNinjaCharacter::IsRagdolling() const
{
	return bIsRagdoll;
}

void ABaseNinjaCharacter::OnDeath(UHealthComponent* HealthComp)
{
	EnterRagdoll();
}
