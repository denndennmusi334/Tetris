#pragma once
#include <chrono>

class Animation
{
private:
    std::vector<int> frames; // DxLib グラフィックハンドル
    int frameTimeMs;         // 1フレームの表示時間（ms）

public:
    Animation()
        : frameTimeMs(100)
    {
    }
    // 必須情報をコンストラクタで強制
    Animation(const std::vector<int>& frames, int frameTimeMs)
        : frames(frames), frameTimeMs(frameTimeMs)
    {
    }

    void SetFrames(const std::vector<int>& frames, int frame_time_ms)
    {
        this->frames = frames;
        frameTimeMs = frame_time_ms;
    }
    // 読み取り専用アクセサ
    const std::vector<int>& GetFrames() const { return frames; }
    int GetFrameTime() const { return frameTimeMs; }
    int GetFrameCount() const { return (int)frames.size(); }
};

class Animator
{
private:
    const Animation* currentAnim;  // 今再生中のアニメ

    int currentFrame;              // 現在のフレーム番号
    bool loop;                     // ループするか
    bool isPlaying;                // 再生中か

    std::chrono::steady_clock::time_point lastUpdate;

public:
    Animator();

    // アニメ切り替え（再生開始）
    void ChangeAnimation(const Animation& anim, bool loop);

    void Stop();

    void Update();
    void Draw(float x, float y, float scale = 1.0f, float angle = 0.0f, BOOL trans_flag = TRUE);

    bool IsPlaying() const { return isPlaying; }
};