#include "pch.h"
#include "WorldTimer.h"
#include <thread>
#include <cmath> 
#include <algorithm>

#undef min

void WorldTimer::Init(const int32 targetFps, const float gameTimeRatio, const float maxDeltaTime)
{
	_gameMSTick = 1000.0f / targetFps;
	_gameTimeRatio = gameTimeRatio;
	_maxDeltaTime = maxDeltaTime;

	_lastTick = std::chrono::high_resolution_clock::now();
	_fps = targetFps;
}

void WorldTimer::Tick()
{
	const TimePoint currentTick{ std::chrono::high_resolution_clock::now() };
	const std::chrono::duration<float> elapsed{ currentTick - _lastTick };
	_lastTick = currentTick;

	_realDeltaTime = elapsed.count();
	_realDeltaTime = std::min(_realDeltaTime, _maxDeltaTime);
	_scaleDeltaTime = _realDeltaTime * _scale;
	_totalGameTime += static_cast<double>(_scaleDeltaTime);

	++_frameCount;
	_frameTimeAccumulator += _realDeltaTime;

#ifdef _DEBUG
	if (_frameTimeAccumulator >= 1.0f)
	{
		_fps = static_cast<int32>(static_cast<float>(_frameCount) / _frameTimeAccumulator);
		_frameCount = 0;
		_frameTimeAccumulator = 0.0f;

		const auto worldTime{ GetCurrentWorldTime() };

		LOG_INFO("World Time {}:{}:{}:{} / FPS: {}", worldTime.day, worldTime.hour, worldTime.minute, worldTime.second, _fps); 
	}
#endif
}

void WorldTimer::MaintainFrameRate() const
{
	const auto elapsedMs{ _realDeltaTime * 1000.0f };

	if (elapsedMs < _gameMSTick)
	{
		const auto sleepMs{ static_cast<int64>(_gameMSTick - elapsedMs) };
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
	}
}

GameTime WorldTimer::GetCurrentWorldTime() const
{
	const double totalGameSeconds{ _totalGameTime * _gameTimeRatio };

	const int32 day{ static_cast<int32>(std::floor(totalGameSeconds / WORLD_TIMER_DAY)) };
	const double secondToDay{ std::fmod(totalGameSeconds, WORLD_TIMER_DAY) };

	const int32 hour{ static_cast<int32>(std::floor(secondToDay / WORLD_TIMER_HOUR)) };
	const double secondToHour{ std::fmod(secondToDay, WORLD_TIMER_HOUR) };

	const int32 minute{ static_cast<int32>(std::floor(secondToHour / WORLD_TIMER_MINUTE)) };
	const double secondToMin{ std::fmod(secondToHour, WORLD_TIMER_MINUTE) };

	return GameTime{ day, hour, minute, static_cast<int32>(secondToMin) };
}
