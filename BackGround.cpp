#include "stdafx.h"
#include "BackGround.h"
#include "GameObjectManager.h"
#include "TimeManager.h"
#include "InputManager.h"

using namespace MyStd;

BackGround::BackGround()
{
}

BackGround::~BackGround()
{

}

void BackGround::Initialize()
{
	SetDrawLayer(-1);       // 背景なので一番下のレイヤーに設定
	CreateAnimation();

	//マウス用の★オブジェクトを生成し、Animationを設定する.
	mouseStar = GameObjectManager::GetInstance().Create<BackGroundStar>(Vec2f{ -100.0f, -100.0f }, Vec2f{ 0.0f, 0.0f }, 0.0f);
	mouseStar->SetAnimation(starIdleAnim, starDestroyAnim); 
}

void BackGround::Update()
{
	spawnTimer += TimeManager::GetInstance().GetRawDeltaTime(); //UI用のΔTimeを使うことで、ゲームが一時停止しても星の生成は止まらないようにする.

	if (mouseStar == nullptr) { //マウス用の★オブジェクトが破壊されていた場合、再生成する.
		mouseStar = GameObjectManager::GetInstance().Create<BackGroundStar>(Vec2f{ -100.0f, -100.0f }, Vec2f{ 0.0f, 0.0f }, 0.0f);
        mouseStar->SetAnimation(starIdleAnim, starDestroyAnim);
	}
	mouseStar->SetPosition(InputManager::GetInstance().GetMousePositionF());    //マウスカーソルの位置に追従させる.

    if (spawnTimer >= SPAWN_INTERVAL)
    {
        for (int i = 0; i < Config::STAR_COUNT; i++)
        {
            if (stars[i] == nullptr)
            {
				Vec2f pos{ 0.0f, 0.0f };    //星の生成位置を格納する変数.画面外に生成するため、初期値は(0,0)で問題ない.
				//TODO: sideをenum classにする.
				int side = IRange(0, 3);    //0:上, 1:下, 2:左, 3:右のいずれかの辺からランダムに生成する.
				const float MARGIN = 30.0f; //画面外に出るようにするためのマージン.

                switch (side)
                {
                case 0: pos.x = Cast<float>(IRange(0, Config::SCREEN_WIDTH));  pos.y = -MARGIN; break;
                case 1: pos.x = Cast<float>(IRange(0, Config::SCREEN_WIDTH));  pos.y = Cast<float>(Config::SCREEN_HEIGHT) + MARGIN; break;
                case 2: pos.x = -MARGIN;                                       pos.y = Cast<float>(IRange(0, Config::SCREEN_HEIGHT)); break;
                case 3: pos.x = Cast<float>(Config::SCREEN_WIDTH) + MARGIN;    pos.y = Cast<float>(IRange(0, Config::SCREEN_HEIGHT)); break;
                }

				//画面内のランダムな位置を目標地点として設定する.
                Vec2f targetPos = Vec2f{
                    Cast<float>(IRange(Cast<int>(Config::SCREEN_WIDTH * 0.3f), Cast<int>(Config::SCREEN_WIDTH * 0.7f))),
                    Cast<float>(IRange(Cast<int>(Config::SCREEN_HEIGHT * 0.3f), Cast<int>(Config::SCREEN_HEIGHT * 0.7f)))
                };

				//目標地点までのベクトルを計算する.
                Vec2f vec = targetPos - pos;

				//星のオブジェクトを生成し、アニメーションを設定する.
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
	DrawBox(200, 64, 200 + Config::FIELD_PIXEL_W, Config::FIELD_Y + Config::FIELD_PIXEL_H + 24 + 64, GetColor(50, 50, 150), true);
    DrawBox(Config::SCREEN_WIDTH / 2 + 200, 64, Config::SCREEN_WIDTH / 2 + 200 + Config::FIELD_PIXEL_W, Config::FIELD_Y + Config::FIELD_PIXEL_H + 24 + 64, GetColor(50, 50, 150), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void BackGround::CreateAnimation() 
{
	starIdleAnim = new Animation();
	starDestroyAnim = new Animation();

	// 背景に表示する星のアニメーションを作成する.
    std::vector<Frame> idleShapes;
    for (int i = 0; i < 15; i++)
    {
		// 15フレームのアニメーションを作成する. 0フレーム目は最も明るく、15フレーム目は最も暗い.
        idleShapes.push_back(MakeFrame(-1, { Circle({ 0.0f, 0.0f }, 3.0f, GetColor(255 - i * 10, 255 - i * 10, 255 - i * 10)) }));
    }
    for (int i = 15; i >= 0; i--)
    {
		// 15フレームのアニメーションを作成する. 0フレーム目は最も暗く、15フレーム目は最も明るい.
        idleShapes.push_back(MakeFrame(-1, { Circle({ 0.0f, 0.0f }, 3.0f, GetColor(255 - i * 10, 255 - i * 10, 255 - i * 10)) }));
    }
    starIdleAnim->SetFrames(idleShapes, 100);


	// 背景に表示する星の破壊アニメーションを作成する.
	// ロックマンの破壊エフェクトのように、星が8方向に飛び散るアニメーションを作成する.
    std::vector<Frame> destroyFrames;
    float size = 3.0f;

	unsigned int color = GetColor(255, 255, 255);

    for (int i = 1; i <= 100; ++i)
    {
        float d = Cast<float>(i * 2.0f); 

        std::vector<AnimShape> shapes = {
            Circle({ 0.0f,    d }, size, color), // 上
            Circle({    d,    d }, size, color), // 右上
            Circle({   -d,    d }, size, color), // 左上
            Circle({ 0.0f,   -d }, size, color), // 下
            Circle({    d,   -d }, size, color), // 右下
            Circle({   -d,   -d }, size, color), // 左下
            Circle({    d, 0.0f }, size, color), // 右
            Circle({   -d, 0.0f }, size, color)  // 左
        };

        destroyFrames.push_back(MakeFrame(-1, std::move(shapes)));
    }

    starDestroyAnim->SetFrames(destroyFrames, 10);
}
