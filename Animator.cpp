#include "stdafx.h"
#include "Animator.h"

using namespace MyStd;

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

void Animator::Draw(Vec2f pos, float scale, float angle, BOOL trans_flag)
{
    if (!currentAnim) return;

    const auto& frame = currentAnim->GetFrames()[currentFrame];
    if(frame.graphicHandle != -1)
    {
        DrawRotaGraphFastF(
            pos.x,
            pos.y,
            scale,
            angle,
            frame.graphicHandle,
            trans_flag
        );
    }

    for (const auto& shape : frame.shapes)
    {
        std::visit([&](auto&& s) {
            using S = std::decay_t<decltype(s)>;

            if constexpr (std::is_same_v<S, AnimCircle>) {
                // 円の描画（x, y からのオフセットを足す）
                DrawCircle(
                    Cast<int>(pos.x + s.offset.x),
                    Cast<int>(pos.y + s.offset.y),
                    Cast<int>(s.radius),
                    s.color, s.fill
                );
            }
            else if constexpr (std::is_same_v<S, AnimRect>) {
                // 矩形の描画
                DrawBox(
                    Cast<int>(pos.x + s.offset.x),
                    Cast<int>(pos.y + s.offset.y),
                    Cast<int>(pos.x + s.offset.y + s.width),
                    Cast<int>(pos.y + s.offset.x + s.height),
                    s.color, s.fill
                );
            }
            }, shape);
    }
}
