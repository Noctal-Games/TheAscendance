// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SpellCasterComponent.generated.h"

class ISpell;
class ISpellCaster;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API USpellCasterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpellCasterComponent();

	void SetSpells(const TArray<FGameplayTag>& spellTags);
	bool CastSpell(int slot) const;

	// Called every frame
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	static constexpr int32 MaxSpells = 4;

private:
	UPROPERTY()
	TArray<TScriptInterface<ISpell>> m_Spells;
	UPROPERTY()
	TScriptInterface<ISpellCaster> m_SpellOwner = nullptr;
};
