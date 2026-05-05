// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyData.h"
#include "TheAscendance/Core/CoreMacros.h"

void UEnemyData::GetAssetRegistryTags(FAssetRegistryTagsContext context) const
{
    context.AddTag(FAssetRegistryTag(
        TEXT("EnemyTag"),
        EnemyTag.ToString(),
        FAssetRegistryTag::TT_Hidden
    ));

    Super::GetAssetRegistryTags(context);
}

#if WITH_EDITOR
void UEnemyData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (BehaviourSettings.ReactionTime.Max < BehaviourSettings.ReactionTime.Min)
    {
        BehaviourSettings.ReactionTime.Max = BehaviourSettings.ReactionTime.Min;
    }

    for (const EAbilityGoal& goal : ClassData->ExpectedAbilityGoals)
    {
        bool goalFound = false;

        for (const FEnemyAbilityData& Ability : CombatSettings.Abilities)
        {
            if (Ability.Goals.Contains(goal))
            {
                goalFound = true;
                break;
            }
        }

        if (goalFound == false)
        {
            LOG_WARNING("[ENEMY DATA] Missing ability for goal: %s", *UEnum::GetValueAsString(goal));
        }
    }
}
#endif
