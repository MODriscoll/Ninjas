#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	// At this moment we do not need tick
	PrimaryComponentTick.bCanEverTick = false;

	Health = 100.f;
	MaxHealth = 100.f;
	bCanBeDamaged = true;
}

float UHealthComponent::ApplyDamage(float Damage)
{
	return FMath::Abs(ApplyDelta(-Damage));
}

float UHealthComponent::Heal(float Amount)
{
	return ApplyDelta(Amount);
}

bool UHealthComponent::IsDead() const
{
	return Health <= 0.f;
}

bool UHealthComponent::CanBeDamaged() const
{
	return bCanBeDamaged;
}

void UHealthComponent::Kill()
{
	ApplyDamage(-Health);
}

void UHealthComponent::PostInitProperties()
{
	Super::PostInitProperties();

	MaxHealth = FMath::Max(0.f, MaxHealth);
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
}

#if WITH_EDITOR
void UHealthComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHealthComponent, Health))
	{
		Health = FMath::Clamp(Health, 0.f, MaxHealth);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UHealthComponent, MaxHealth))
	{
		MaxHealth = FMath::Max(0.f, MaxHealth);
	}
}
#endif

float UHealthComponent::ApplyDelta(float Delta)
{
	// No point in applying change if already dead
	if (IsDead() || FMath::IsNearlyZero(Delta))
	{
		return 0.f;
	}

	// Stop here if applying damage and invincible
	if (Delta < 0.f && !CanBeDamaged())
	{
		return 0.f;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(OldHealth + Delta, 0.f, MaxHealth);
	
	// Update delta based on how much health has actually changed
	// (Healing will be positive, damage will be negative)
	Delta = NewHealth - Health;
	Health = NewHealth;

	OnHealthChanged.Broadcast(this, OldHealth, NewHealth);

	// Call this after HealthChanged event
	if (IsDead())
	{
		OnDeath.Broadcast(this);
	}

	return Delta;
}


