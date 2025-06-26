#include "pch.h"
#include "ClientPacketHandler.h"
#include "../../game/map/GameMap.h"
#include "../../game/object/actor/Player.h"
#include "../../game/map/GameMapManager.h"
#include "../../game/session/GameSession.h"
#include "../../game/utils/EnumConverter.h"
#include "../../game/type/GameObjectType.h"
#include "../../game/object/factory/ObjectFactory.h"
#include "../../game/component/move/MoveComponent.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
    // TODO: 로그 남기고 종료

    LOG_WARN("INVALID");

    session->Disconnect(L"INVALID");

    return true;
}


bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
    // TOOD: 인증 확인
    const auto& ip{ session->GetAddress().GetIpAddress() };
	LOG_DEBUG("LOGIN IP = {}", StringUtils::WstringToString(ip));

    Protocol::S_LOGIN loginPkt;

    loginPkt.set_success(true);

    SEND_PACKET(loginPkt);

    return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
    LOG_DEBUG("ENTER_GAME");

    const auto& gameSession{ static_pointer_cast<GameSession>( session ) };

    PlayerRef player{ ObjectFactory::CreatePlayer(static_pointer_cast<GameSession>(session)) };

    gameSession->SetObjectId(player->GetObjectId());
    gameSession->SetMapId(static_cast<int64>( GameMapType::TOWN ));

    const auto& move{ player->GetComponent<MoveComponent>() };

    move->SetPosition(Vector3<float>{100.f, 100.f, 100.f});
    move->SetYaw(100.f);
    move->SetSpeed(100.f);

    Protocol::S_ENTER_GAME enterGamePkt;
    auto playerId{ player->GetObjectId() };
    enterGamePkt.set_object_id(playerId);

    SendBufferRef sendBuffer{ ClientPacketHandler::MakeSendBuffer(enterGamePkt) };
    session->Send(sendBuffer);

    const float x{ MathUtils::GetRandom<float>(100.f, 200.f) };
    const float y{ MathUtils::GetRandom<float>(100.f, 200.f) };
    const float z{ MathUtils::GetRandom<float>(100.f, 200.f) };

    player->SetPosition({ x, y, z });
    player->SetGameObjectType(EnumConverter::ConvertGameObjectType(Protocol::OBJECT_TYPE_PLAYER));

    MapManager.AddPlayer(static_cast<int64>(GameMapType::TOWN), player);

    return true;
}

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
    auto gameSession{ static_pointer_cast<GameSession>( session ) };

    PlayerRef player{ gameSession->GetPlayer() };

    if (player == nullptr)
    {
        return false;
    }

    auto map{ player->GetCurrentMap() };

    if (map == nullptr)
    {
        return false;
    }

    if (player->GetObjectId() != pkt.info().object_id())
    {
        return false;
    }

    LOG_DEBUG("전 player x{} y{} z{}", player->GetX(), player->GetY(), player->GetZ());

    map->PushJob([player, pkt, map]()
    {
        player->SetPosition({ pkt.info().position().x(),
            pkt.info().position().y(),
            pkt.info().position().z() });
        player->SetYaw(pkt.info().yaw());

        LOG_DEBUG("후 player x{} y{} z{}", player->GetX(), player->GetY(), player->GetZ());

        Protocol::S_MOVE movePkt{};
        Protocol::MoveInfo* move = movePkt.add_info();

        move->set_object_id(player->GetObjectId());
        move->mutable_position()->set_x(player->GetX());
        move->mutable_position()->set_y(player->GetY());
        move->mutable_position()->set_z(player->GetZ());
        move->set_yaw(player->GetYaw());

        SendBufferRef sendBuffer{ ClientPacketHandler::MakeSendBuffer(movePkt) };
        map->Broadcast(sendBuffer, player->GetObjectId());
    });

    return true;
}

