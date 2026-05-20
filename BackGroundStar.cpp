#include "stdafx.h"
#include "BackGroundStar.h"
#include "TimeManager.h"
#include "ColliderManager.h"

using namespace MyStd;

void BackGroundStar::OnCollision(BaseCollider* other)
{
	LineInfo info;
	for (auto& pos : starPoss)
	{
		if (pos.starPos.Distance(this->GetPosition()) > other->GetOwner()->GetPosition().Distance(this->GetPosition()))
		{
			info = {
				other->GetOwner()->GetPosition(),
				Cast<float>(LineAlphaCalculation(other->GetOwner()->GetPosition()))
			};
			break;
		}
	}

	starPoss.push_back(info);

	if (starPoss.size() > 2)
	{
		starPoss.erase(starPoss.begin());
	}
}

BackGroundStar::BackGroundStar(Vec2f startPos,Vec2f _vec, float speed)
	: vec(_vec.Normalize()), speed(speed), state(StarState::IDLE), idle(nullptr), destroy(nullptr)
{
	SetPosition(startPos);
}

BackGroundStar::~BackGroundStar()
{
	
}

void BackGroundStar::Initialize()
{
	SetDrawLayer(-2);
	ColliderManager::GetInstance().Create<CircleCollider>(this, Vec2f{ 0,0 }, 150.0f);
}

void BackGroundStar::Update()
{
	if(state == StarState::DESTROY && !animator.IsPlaying())
	{
		GameObject::Destroy();
		return;
	}
	starPoss.clear();
	SetPosition(GetPosition() + vec * speed * TimeManager::GetInstance().GetRawDeltaTime());
	if (GetPosition().x < -50 || GetPosition().x > Config::SCREEN_WIDTH + 50 ||
		GetPosition().y < -50 || GetPosition().y > Config::SCREEN_HEIGHT + 50)
	{
		GameObject::Destroy();
	}

	animator.Update();
}

void BackGroundStar::Draw(const Camera& camera)
{
	animator.Draw(GetPosition());
	if (state == StarState::DESTROY) return;
	for (const auto& pos : starPoss)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, MyStd::Cast<int>(pos.alpha));
		MyDrawDxlib::M_DrawLineAA(GetPosition(), pos.starPos, GetColor(255, 255, 255), 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void BackGroundStar::Destroy()
{
	starPoss.clear();
	animator.ChangeAnimation(*destroy, false);
	state = StarState::DESTROY;
}

int BackGroundStar::LineAlphaCalculation(Vec2f _starPos)
{
	float distance = Cast<float>(_starPos.Distance(GetPosition()));
	if (distance > 150.0f) return 0;
	return Cast<int>(MyStd::Lerp(255.0f, 0.0f, distance / 150.0f));
}

