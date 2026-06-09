// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitForEQSTask.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/TAAIController.h"
#include "TheAscendance/AI/AIEventGameplayTags.h"

#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "StateTreeAsyncExecutionContext.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FWaitForEQSTask::EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const
{
	FInstanceDataType& data = context.GetInstanceData(*this);

	if (data.Duration > 0.0f)
	{
		return EStateTreeRunStatus::Running;
	}

	if (data.QueryOwner == nullptr)
	{
		LOG_ERROR("[WAIT FOR EQS TASK] QueryOwner is invalid");
		return EStateTreeRunStatus::Failed;
	}

	if (data.QueryTemplate == nullptr)
	{
		LOG_ERROR("[WAIT FOR EQS TASK] QueryTemplate is invalid");
		return EStateTreeRunStatus::Failed;
	}

	FEnvQueryRequest request(data.QueryTemplate, data.QueryOwner);

	for (FAIDynamicParam& dynamicParam : data.QueryConfig)
	{
		request.SetDynamicParam(dynamicParam, nullptr);
	}

	data.RequestId = request.Execute(data.RunMode, FQueryFinishedSignature::CreateLambda([weakContext = context.MakeWeakExecutionContext()](TSharedPtr<FEnvQueryResult> queryResult) mutable
			{
				const FStateTreeStrongExecutionContext strongContext = weakContext.MakeStrongExecutionContext();

				if (FInstanceDataType* instanceDataPtr = strongContext.GetInstanceDataPtr<FInstanceDataType>())
				{
					instanceDataPtr->RequestId = INDEX_NONE;
					instanceDataPtr->Duration = 5.0f;

					bool bSuccess = false;

					if (queryResult && queryResult->IsSuccessful() && queryResult->Items.Num() > 0)
					{
						LOG_ONSCREEN(-1, 1.0f, FColor::Red, "%s", *instanceDataPtr->ResultLocation.ToString())
						instanceDataPtr->ResultLocation = queryResult->GetItemAsLocation(0);
						LOG_ONSCREEN(-1, 1.0f, FColor::Red, "%s", *instanceDataPtr->ResultLocation.ToString())
						bSuccess = true;
					}
					
					if (bSuccess == false)
					{
						strongContext.FinishTask(EStateTreeFinishTaskType::Failed);
					}
				}
			}));

	return data.RequestId != INDEX_NONE ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FWaitForEQSTask::Tick(FStateTreeExecutionContext& context, const float deltaTime) const
{
	FInstanceDataType& data = context.GetInstanceData(*this);
	
	if (data.Duration > 0.0f)
	{
		data.Duration -= deltaTime;
		return EStateTreeRunStatus::Running;
	}

	if (data.Controller.IsValid() == false)
	{
		LOG_ERROR("[STRAFE TASK] Controller is invalid");
		return EStateTreeRunStatus::Failed;
	}

	ATAAIController* controller = data.Controller.Get();

	FStateTreeEvent event;
	event.Tag = EVENT_STRAFE_COMPLETE;

	controller->SendStateTreeEvent(event);
	return EStateTreeRunStatus::Succeeded;
}

void FWaitForEQSTask::ExitState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const
{
	FInstanceDataType& instanceData = context.GetInstanceData(*this);

	if (instanceData.RequestId != INDEX_NONE)
	{
		if (UEnvQueryManager* queryManager = UEnvQueryManager::GetCurrent(context.GetOwner()))
		{
			queryManager->AbortQuery(instanceData.RequestId);
		}

		instanceData.RequestId = INDEX_NONE;
	}
}
