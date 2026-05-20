#pragma once
#pragma region AnimationData

struct AnimCircle {
	Vec2f offset; // アニメーション内での位置オフセット
    float radius;
    unsigned int color;
    bool fill;
};

struct AnimRect {
    Vec2f offset; // アニメーション内での位置オフセット
    float width, height;
    unsigned int color;
    bool fill;
};

using AnimShape = std::variant<AnimCircle, AnimRect>;

struct Frame {
    int graphicHandle = -1;       // 画像がない場合は -1
    std::vector<AnimShape> shapes; // 複数の図形をサポート
};
#pragma endregion

class Animation
{
private:
    std::vector<Frame> frames; // DxLib グラフィックハンドル
    int frameTimeMs;         // 1フレームの表示時間（ms）

public:
    Animation()
        : frameTimeMs(100){}
    // 必須情報をコンストラクタで強制
    Animation(const std::vector<Frame>& frames, int frameTimeMs)
        : frames(frames), frameTimeMs(frameTimeMs){}

    void SetFrames(const std::vector<Frame>& frames, int frame_time_ms)
    {
        this->frames = frames;
        frameTimeMs = frame_time_ms;
    }
    // 読み取り専用アクセサ
    const std::vector<Frame>& GetFrames() const { return frames; }
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
    void Draw(Vec2f pos, float scale = 1.0f, float angle = 0.0f, BOOL trans_flag = TRUE);

    bool IsPlaying() const { return isPlaying; }
};

// 記述を劇的に短くするためのヘルパー関数
inline Frame MakeFrame(int handle, std::vector<AnimShape> shapes) {
    return Frame{ handle, std::move(shapes) };
}

inline AnimShape Circle(Vec2f offset, float radius, unsigned int color, bool fill = true) {
    return AnimShape(AnimCircle{ offset, radius, color, fill });
}

inline AnimShape Rect(Vec2f offset, float width, float height, unsigned int color, bool fill = true) {
    return AnimShape(AnimRect{ offset, width, height, color, fill });
}