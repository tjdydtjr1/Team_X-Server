#pragma once

/*==============================
		  ObjectFactory
==============================*/

class Player;
class GameSession;

class ObjectFactory
{
public:
	static PlayerRef CreatePlayer(const GameSessionRef& session);

private:
	static inline atomic<int64> _idGenerator{};
};

