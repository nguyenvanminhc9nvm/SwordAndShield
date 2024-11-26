// Fill out your copyright notice in the Description page of Project Settings.


#include "SASGameInstance.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystem.h"
#include "struct/FOnlineSessionSearchWrapper.h"


void USASGameInstance::Init()
{
	Super::Init();
	OnlineSubsystem = IOnlineSubsystem::Get();
	Identity = OnlineSubsystem->GetIdentityInterface();
	Identity->OnLoginCompleteDelegates->AddUObject(this, &USASGameInstance::OnLoginCompleted);
	Identity->OnLogoutCompleteDelegates->AddUObject(this, &USASGameInstance::OnLogoutCompleted);

	Session = OnlineSubsystem->GetSessionInterface();
	Session->OnCreateSessionCompleteDelegates.AddUObject(this, &USASGameInstance::OnCreateSessionComplete);
	Session->OnFindSessionsCompleteDelegates.AddUObject(this, &USASGameInstance::OnFindSessionComplete);
	Session->OnJoinSessionCompleteDelegates.AddUObject(this, &USASGameInstance::OnJoinSessionComplete);
}


/**
 * This method do a task Login
 */
void USASGameInstance::Login()
{
	if (Identity)
	{
		FOnlineAccountCredentials OnlineAccountCredentials;
		OnlineAccountCredentials.Type = "accountportal";
		OnlineAccountCredentials.Id = "";
		OnlineAccountCredentials.Token = "";
		Identity->Login(0, OnlineAccountCredentials);
	}
}

bool USASGameInstance::CheckLoginStatus()
{
	if (Identity)
	{
		return Identity->GetLoginStatus(0);
	}
	return false;
}

void USASGameInstance::Logout()
{
	if (Identity)
	{
		Identity->Logout(0);
	}
}

void USASGameInstance::OnLoginCompleted(signed int NumberOfPlayer, bool IsSuccessFul, const FUniqueNetId& UniqueNetId, const FString& ErrorMessage) const
{
	OnLoginCompletedEvent.Broadcast(IsSuccessFul, ErrorMessage);
}

void USASGameInstance::OnLogoutCompleted(signed int I, bool IsSuccessFul) const
{
	OnLogoutCompletedEvent.Broadcast(IsSuccessFul);
}


/**
 * Create Session
 * @param SessionName 
 * @param NumOfPlayer 
 */
void USASGameInstance::CreateSession(const FString& SessionName, FString LobbyName, int NumOfPlayer)
{
	if (Session)
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.NumPublicConnections = NumOfPlayer;

		SessionSettings.Set(SessionNameKey, SessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		Session->CreateSession(0, *SessionName, SessionSettings);
	}
}

void USASGameInstance::OnCreateSessionComplete(FName Name, bool IsSuccessFul) const
{
	OnCreateEOSSessionCompleted.Broadcast(Name, IsSuccessFul);
	if (IsSuccessFul)
	{
		if (!MultiplayerLevel.IsValid())
		{
			MultiplayerLevel.LoadSynchronous();
		}
		if (MultiplayerLevel.IsValid())
		{
			const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(MultiplayerLevel.ToString()));
			GetWorld()->ServerTravel(LevelName.ToString() + "?listen");
		}
	}
}


void USASGameInstance::FindSession()
{
	if (Session)
	{
		SessionSearch = MakeShareable(new FOnlineSessionSearch);

		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
		Session->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void USASGameInstance::OnFindSessionComplete(bool IsSuccessFul)
{
	if (IsSuccessFul && SessionSearch->SearchResults.Num() > 0)
	{
		TArray<FOnlineSessionSearchEOSWrapper> OnlineSessionSearchEosWrappers;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			FString LobbyName = GetSessionName(SearchResult);
			FString SessionNameDefault = GetSessionName(SearchResult);
			FOnlineSessionSearchEOSWrapper OnlineSessionSearchWrapper = FOnlineSessionSearchEOSWrapper(
				SessionNameDefault,
				SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections,
				SearchResult.Session.SessionSettings.NumPublicConnections,
				SearchResult.PingInMs
				);
			SessionSearchEosWrappers.Add(OnlineSessionSearchWrapper);
		}
		OnSearchSessionEosCompleted.Broadcast(true);
	}
}


FString USASGameInstance::GetSessionName(const FOnlineSessionSearchResult& SearchResult) const
{
	FString outVal = { "Unnamed Server" };
	SearchResult.Session.SessionSettings.Get(SessionNameKey, outVal);
	return outVal;
}


void USASGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString TravelUrl;
		Session->GetResolvedConnectString("", TravelUrl);
		GetFirstLocalPlayerController(GetWorld())->ClientTravel(TravelUrl, TRAVEL_Absolute);
	}
}
