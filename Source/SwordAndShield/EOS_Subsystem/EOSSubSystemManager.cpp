// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSSubSystemManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "IOnlineSubsystemEOS.h"
#include "Interfaces/OnlineSessionInterface.h"


void UEOSSubSystemManager::CreateEOSSession(FName SessionName, int32 maxPlayer)
{
	OnlineSubsystem = IOnlineSubsystem::Get();
	
	if (OnlineSubsystem)
	{
		SessionPtr = OnlineSubsystem->GetSessionInterface();
		if (SessionPtr.IsValid())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.NumPublicConnections = maxPlayer;
			SessionSettings.bIsLANMatch = false;
			SessionSettings.bAllowInvites = true;
			SessionSettings.bUsesPresence = false;
			SessionSettings.bIsDedicated = false;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bUseLobbiesIfAvailable = true;
			SessionSettings.Set(FName(TEXT("MAPNAME")), FString("Multiplayer"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			SessionSettings.Set(FName(TEXT("NoLoginRequired")), true, EOnlineDataAdvertisementType::ViaOnlineService);

			
			SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSSubSystemManager::OnCreateSessionComplete);
			SessionPtr->CreateSession(0, SessionName, SessionSettings);
		}
			else
		{
			UE_LOG(LogTemp, Warning, TEXT("Session is not valid"));
				OnCreateSessionCompleted.Broadcast(SessionName, false); 
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is null"));
		OnCreateSessionCompleted.Broadcast(SessionName, false);
	}
}


void UEOSSubSystemManager::OnCreateSessionComplete(FName SessionName, bool bWasSuccessFul)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete"));
	OnCreateSessionCompleted.Broadcast(SessionName, bWasSuccessFul);
}
