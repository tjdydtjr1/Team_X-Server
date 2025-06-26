#pragma once

/*==========================
		WorldTimer
 =========================*/

struct GameTime
{
	int32 day{};
	int32 hour{};
	int32 minute{};
	int32 second{};
};

class WorldTimer
{
	SINGLETON_BASE(WorldTimer);

public:
	/**
	* @brief : WorldTimer 초기화
	* @param : fps : 게임 프레임 설정
	* @param : ratio : 게임 시간 비율
	* @param : maxDeltaTime : 최대 델타 타임
	*/
	void Init(const int32 fps = 60, const float ratio = 1.0f, const float maxDeltaTime = 0.1f);

	/**
	* @brief : WorldTimer Tick
	* @details : 게임 시간 업데이트, FPS 계산
	*/
	void Tick();

	/**
	* @brief : FPS 유지
	* @details : 현재 프레임 시간에 따라 Sleep
	*/
	void MaintainFrameRate() const;

public:
	/**Getter, Setter*/
	float GetTimeScale() const noexcept { return _scale; }
	void SetTimeScale(const float scale) noexcept { _scale = scale; }

	float GetDeltaTime() const noexcept { return _scaleDeltaTime; }
	float GetRealDeltaTime() const noexcept { return _realDeltaTime; }
	double GetTotalGameTime() const noexcept { return _totalGameTime; }
	int32 GetFps() const noexcept { return _fps; }

	/**
	* @brief : 현재 게임 월드 시간 반환
	* @details : 게임 월드 시간은 게임 시간 비율에 따라 조정됨
	* @return : GameTime 구조체
	*/
	GameTime GetCurrentWorldTime() const;	

private:
	using TimePoint = std::chrono::high_resolution_clock::time_point;

	inline static constexpr double WORLD_TIMER_MINUTE{ 60.0 };
	inline static constexpr double WORLD_TIMER_HOUR{ 60.0 * WORLD_TIMER_MINUTE };
	inline static constexpr double WORLD_TIMER_DAY{ 24.0 * WORLD_TIMER_HOUR };

	TimePoint _lastTick{};
	float _gameMSTick{};
	float _maxDeltaTime{};
	float _realDeltaTime{};
	float _scaleDeltaTime{};
	float _scale{ 1.0f };
	double _totalGameTime{};
	float _gameTimeRatio{};
	uint64 _frameCount{};
	float _frameTimeAccumulator{};
	int32 _fps{};
};

#define WorldTimeManager WorldTimer::GetInstance()