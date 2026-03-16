// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseObjectiveNode.h"

void UBaseObjectiveNode::SetParentQuest(UQuest* quest)
{
	m_ParentQuest = quest;
}

void UBaseObjectiveNode::Start()
{
}
void UBaseObjectiveNode::Stop()
{
	OnComplete.Unbind();
}

bool UBaseObjectiveNode::IsComplete() const
{
    return false;
}