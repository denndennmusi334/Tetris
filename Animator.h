#pragma once
/*
	Animator.h
	アニメーションを管理するクラスを定義するヘッダーファイル.
*/
#pragma region AnimationData

struct AnimCircle {
	Vec2f offset;       // アニメーション内での位置オフセット.
    float radius;       // 半径.
    unsigned int color; // 色.
	bool fill;          // 塗りつぶすかどうか.
};

struct AnimRect {
    Vec2f offset;       // アニメーション内での位置オフセット.
	float width, height;// 幅と高さ.
	unsigned int color; // 色.
	bool fill;          // 塗りつぶすかどうか.
};

using AnimShape = std::variant<AnimCircle, AnimRect>;// 形状の種類を表す型（円または矩形）.


/// <summary>
/// 一フレームにおいて描画する情報をまとめた構造体.
/// </summary>
struct Frame {
    int graphicHandle = -1;       // 画像がない場合は -1.
    std::vector<AnimShape> shapes; // 複数の図形をサポート.
};
#pragma endregion

/// <summary>
/// アニメーションの情報をまとめたクラス.
/// 実際に触るのはこのクラスが多いと思われる.
/// Animation流したいオブジェクトがこのクラス情報を持ち、Animatorがそれを再生する.
/// </summary>
class Animation
{
private:
	std::vector<Frame> frames;  // アニメーションのフレーム情報.
    int frameTimeMs;            // 1フレームの表示時間 (ms).

public:
    Animation()
        : frameTimeMs(100){}

    // 必須情報をコンストラクタで強制
    Animation(const std::vector<Frame>& frames, int frameTimeMs)
        : frames(frames), frameTimeMs(frameTimeMs){}


    /// <summary>
	/// Animationのフレーム情報を設定する.
    /// 一括ですべて入れる.
    /// </summary>
    /// <param name="frames">すべてのフレーム情報</param>
    /// <param name="frame_time_ms">一フレームの間隔</param>
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

	std::chrono::steady_clock::time_point lastUpdate;// 最後にフレームを更新した時間.

public:
    Animator();

    /// <summary>
	/// Animationを切り替える.
    /// </summary>
    /// <param name="anim">切り替えるアニメーション情報</param>
    /// <param name="loop">loopするかどうか</param>
    void ChangeAnimation(const Animation& anim, bool loop);

    void Stop();

    void Update();


	//TO:DO: 図形描画にも拡大、回転を適用できるようにする.

    /// <summary>
	/// ゲームオブジェクトのDraw関数内で呼び出す.
    /// </summary>
    /// <param name="pos">描画位置</param>
    /// <param name="scale">拡大したい場合など(画像のみ)</param>
    /// <param name="angle">回転させたい場合(画像のみ)</param>
    /// <param name="trans_flag"></param>
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

/*
    実際の使い方.
	Animation animation;


    
	animation.SetFrames({
		MakeFrame(-1, { Circle({ 0.0f, 0.0f }, 10.0f, GetColor(255, 0, 0)) }),
		MakeFrame(-1, { Circle({ 0.0f, 0.0f }, 20.0f, GetColor(255, 255, 0)) }),
		MakeFrame(-1, { Circle({ 0.0f, 0.0f }, 30.0f, GetColor(255, 255, 255)) })
	}, 100);

    もしくは.

    std::vector<Frame> idleShapes;
    for (int i = 0; i < 15; i++)
    {
        idleShapes.push_back(MakeFrame(-1, { Circle({ 0.0f, 0.0f }, 3.0f, GetColor(255 - i * 10, 255 - i * 10, 255 - i * 10)) }));
    }
    for (int i = 15; i >= 0; i--)
    {
        idleShapes.push_back(MakeFrame(-1, { Circle({ 0.0f, 0.0f }, 3.0f, GetColor(255 - i * 10, 255 - i * 10, 255 - i * 10)) }));
    }
    starIdleAnim->SetFrames(idleShapes, 100);

    のように使います.

    画像のみ描画の場合は
	MakeFrame(画像ハンドル, {}) 
    のように空の図形ベクターを渡すことで、図形描画を行わずに画像のみ描画することができます.

*/
