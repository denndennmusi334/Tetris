#include "stdafx.h"
#include "BackGround.h"
#include "GameObjectManager.h"
#include "TimeManager.h"

using namespace MyStd;

BackGround::BackGround()
{
}

BackGround::~BackGround()
{

}

void BackGround::Initialize()
{
	SetDrawLayer(-1);
	CreateAnimation();
}

void BackGround::Update()
{
    spawnTimer += TimeManager::GetInstance().GetRawDeltaTime();

    if (spawnTimer >= SPAWN_INTERVAL)
    {
        for (int i = 0; i < Config::STAR_COUNT; i++)
        {
            if (stars[i] == nullptr)
            {
                Vec2f pos{ 0.0f, 0.0f };
                int side = IRange(0, 3);
                float margin = 30.0f;

                switch (side)
                {
                case 0: pos.x = Cast<float>(IRange(0, Config::SCREEN_WIDTH));  pos.y = -margin; break;
                case 1: pos.x = Cast<float>(IRange(0, Config::SCREEN_WIDTH));  pos.y = Cast<float>(Config::SCREEN_HEIGHT) + margin; break;
                case 2: pos.x = -margin;                                       pos.y = Cast<float>(IRange(0, Config::SCREEN_HEIGHT)); break;
                case 3: pos.x = Cast<float>(Config::SCREEN_WIDTH) + margin;    pos.y = Cast<float>(IRange(0, Config::SCREEN_HEIGHT)); break;
                }

                Vec2f targetPos = Vec2f{
                    Cast<float>(IRange(Cast<int>(Config::SCREEN_WIDTH * 0.3f), Cast<int>(Config::SCREEN_WIDTH * 0.7f))),
                    Cast<float>(IRange(Cast<int>(Config::SCREEN_HEIGHT * 0.3f), Cast<int>(Config::SCREEN_HEIGHT * 0.7f)))
                };

                Vec2f vec = targetPos - pos;

                stars[i] = GameObjectManager::GetInstance().Create<BackGroundStar>(pos, vec, 50.0f);
                stars[i]->SetAnimation(starIdleAnim, starDestroyAnim);

                spawnTimer = 0.0f;
                break;
            }
        }
    }
}

void BackGround::Draw(const Camera& camera)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 155);
	DrawBox(Config::FIELD_X, 0, Config::FIELD_X + Config::FIELD_PIXEL_W, Config::FIELD_Y + Config::FIELD_PIXEL_H + 24, GetColor(50, 50, 150), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void BackGround::CreateAnimation() 
{
	starIdleAnim = new Animation();
	starDestroyAnim = new Animation();

    starIdleAnim->SetFrames({
        MakeFrame(-1, { Circle({0.0f, 0.0f}, 3.0f, GetColor(255, 255, 255)) })
        }, 100);

    std::vector<Frame> destroyFrames;
    float size = 3.0f;

    for (int i = 1; i <= 100; ++i)
    {
        float d = Cast<float>(i * 2.0f); 

        std::vector<AnimShape> shapes = {
            Circle({ 0.0f,    d }, size, GetColor(255, 255, 255)), // ã
            Circle({    d,    d }, size, GetColor(255, 255, 255)), // ‰Eã
            Circle({   -d,    d }, size, GetColor(255, 255, 255)), // ¶ã
            Circle({ 0.0f,   -d }, size, GetColor(255, 255, 255)), // ‰º
            Circle({    d,   -d }, size, GetColor(255, 255, 255)), // ‰E‰º
            Circle({   -d,   -d }, size, GetColor(255, 255, 255)), // ¶‰º
            Circle({    d, 0.0f }, size, GetColor(255, 255, 255)), // ‰E
            Circle({   -d, 0.0f }, size, GetColor(255, 255, 255))  // ¶
        };

        destroyFrames.push_back(MakeFrame(-1, std::move(shapes)));
    }

    starDestroyAnim->SetFrames(destroyFrames, 10);
}
