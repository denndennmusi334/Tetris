#include "stdafx.h"
#include "Animator.h"

Animator::Animator()
    : currentAnim(nullptr),
    currentFrame(0),
    loop(false),
    isPlaying(false)
{
    lastUpdate = std::chrono::steady_clock::now();
}

void Animator::ChangeAnimation(const Animation& anim, bool isLoop)
{
    // 同じアニメを再生中なら何もしない
    if (currentAnim == &anim && isPlaying)
        return;

    currentAnim = &anim;
    loop = isLoop;
    currentFrame = 0;
    isPlaying = true;
    lastUpdate = std::chrono::steady_clock::now();
}

void Animator::Stop()
{
    isPlaying = false;
}

void Animator::Update()
{
    if (!isPlaying || !currentAnim) return;
    if (currentAnim->GetFrameCount() == 0) return;

    auto now = std::chrono::steady_clock::now();
    int elapsed =
        (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count();

    // まだフレーム切り替え時間に達していない
    if (elapsed < currentAnim->GetFrameTime())
        return;

    lastUpdate = now;
    currentFrame++;

    // 最終フレームに到達
    if (currentFrame >= currentAnim->GetFrameCount())
    {
        if (loop)
        {
            currentFrame = 0;
        }
        else
        {
            currentFrame = currentAnim->GetFrameCount() - 1;
            isPlaying = false;
        }
    }
}

void Animator::Draw(float x, float y, float scale, float angle, BOOL trans_flag)
{
    if (!currentAnim) return;

    const auto& frames = currentAnim->GetFrames();
    if (frames.empty()) return;

    DrawRotaGraphFastF(
        x,
        y,
        scale,
        angle,
        frames[currentFrame],
        trans_flag
    );
}