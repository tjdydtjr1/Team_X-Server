#include "pch.h"
#include "ObjectFactory.h"
#include "../../session/GameSession.h"
#include "../actor/Player.h"

PlayerRef ObjectFactory::CreatePlayer(const GameSessionRef& session)
{
    const int64 newId{ _idGenerator.fetch_add(1) };

    PlayerRef player{ MakeShared<Player>() };
    
    player->SetGameSession(session);
    player->SetObjectId(newId);
    player->Init();

    session->SetPlayer(player);
    session->SetObjectId(newId);

    return player;
}
