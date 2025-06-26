#include "pch.h"
#include "GameRedisManager.h"

void GameRedisManager::Connect(const string& host, const int port)
{
    if (GetIsConnected())
    {
        return;
    }
    
    WRITE_LOCK;

    _client.connect(host, port);

    _client.sync_commit();

    SetIsConnected(true);
}

void GameRedisManager::Connect(const wstring& host, const int port)
{
    Connect(StringUtils::WstringToString(host), port);
}

void GameRedisManager::Set(const string& key, const string& value, const int expireTime)
{
    if (GetIsConnected() == false)
    {
        return;
    }

    WRITE_LOCK;
    
    _client.set(key, value);

    if (expireTime > 0)
    {
        _client.expire(key, expireTime);
    }

    _client.commit();
}

string GameRedisManager::Get(const string& key)
{
    if (GetIsConnected() == false)
    {
        return "";
    }
    
    READ_LOCK;

    promise<string> promise;

    _client.get(key, [&promise](cpp_redis::reply& reply) 
    {
        if (reply.is_string()) 
        {
            promise.set_value(reply.as_string());
        } 
        else 
        {
            promise.set_value("");
        }
    });

    _client.sync_commit();

    return promise.get_future().get();
}

int GameRedisManager::GetTTL(const string& key)
{
    if (GetIsConnected() == false)
    {
        return 0;
    }
    
    READ_LOCK;

    future<cpp_redis::reply> futureReply{ _client.ttl(key) };
    _client.sync_commit(); 

    cpp_redis::reply reply{ futureReply.get() };

    if (reply.is_integer()) 
    {
        return reply.as_integer(); 
    }

    return 0;
}

void GameRedisManager::Delete(const string& key)
{
    if (GetIsConnected() == false)
    {
        return;
    }
    
    WRITE_LOCK;

    _client.del({key});
    _client.commit();
}
