#pragma once

/// <summary>
/// deltaTimeやFPS管理などの時間制御部分を管理するマネージャー.
/// </summary>
class TimeManager
{
private:
	TimeManager() { SetStartTime(); }
	~TimeManager(){}
	LONGLONG startTime = 0;
	LONGLONG us = 0;
	UINT fps = 0;

	float deltaTime = 0;
	float rawDeltaTime = 0;

	bool pause = false;
public:
	//コピー禁止.
	TimeManager(const TimeManager&) = delete;
	TimeManager& operator = (const TimeManager&) = delete;

	/// <summary>
	/// Instanceの取得(初回時のみ作成).
	/// </summary>
	/// <returns>instance</returns>
	static TimeManager& GetInstance(){ 
		static TimeManager instance;
		return instance;
	}

	/// <summary>
	/// ΔTimeを取得できる.
	/// </summary>
	/// <returns>ΔTime</returns>
	float GetDeltaTime() const { return deltaTime; }

	/// <summary>
	/// ΔTimeに倍率をかけて入手できる.
	/// </summary>
	/// <param name="localTimeScale">倍率</param>
	/// <returns>倍率のかかったΔTime</returns>
	float GetScaledDeltaTime(float localTimeScale = 1.0f) const;

	/// <summary>
	/// UIなどようのpause時でも動くΔTime
	/// </summary>
	/// <returns>ΔTime</returns>
	float GetRawDeltaTime() const { return rawDeltaTime; }

	void SetPause(bool p) { pause = p; }
	bool IsPaused() const { return pause; }

	void SetStartTime();//FPS図る用の比較用最初の数値を取る.
	BOOL UpdateTiming();//取得したFPSをもとに現在は画面を更新していいかをBOOL型で返す.
	UINT GetFPS() const { return fps; }
};

