#pragma once

/*=========================
		  Player
=========================*/

#include "Actor.h"

class Player : public Actor
{
public:
	virtual ~Player() = default;

public:
	void Init();

public:
	/**Getter Setter*/ 
	void SetGameSession(GameSessionRef session) { _session = session; }
	GameSessionRef GetGameSession() const { return _session.lock(); }

private:
	weak_ptr<GameSession> _session;
	
};

