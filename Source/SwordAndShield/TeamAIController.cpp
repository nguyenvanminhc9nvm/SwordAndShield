// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamAIController.h"

ATeamAIController::ATeamAIController()
{
	SetGenericTeamId(GetGenericTeamId());
}

ETeamAttitude::Type ATeamAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (APawn const* OtherPawn = Cast<APawn>(&Other))
	{
		if (auto const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if (TeamAgent->GetGenericTeamId() == GetGenericTeamId())
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}
	}
	return ETeamAttitude::Neutral;
}

FGenericTeamId ATeamAIController::GetGenericTeamId() const
{
	return TeamId;
}