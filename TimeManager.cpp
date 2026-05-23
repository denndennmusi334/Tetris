#include "stdafx.h"
#include "TimeManager.h"

void TimeManager::SetStartTime()
{
    startTime = GetNowHiPerformanceCount();
}

BOOL TimeManager::UpdateTiming()
{
    if (GetActiveFlag() == FALSE)
    {
        SetStartTime();
        return FALSE;
    }

    us = GetNowHiPerformanceCount() - startTime;

    if (us > 500000) // 0.5秒以上
    {
        SetStartTime();
        return FALSE;
    }

    if (us < 1000000 / Config::SCREEN_FPS)
    {
        return FALSE;
    }
    rawDeltaTime = (float)us / 1000000.0f;
    deltaTime = rawDeltaTime;
    if (pause)
    {
        deltaTime = 0.0f;
    }
    startTime = GetNowHiPerformanceCount();

    if (us > 0)
    {
        fps = (UINT)(1000000 / us);
    }

    return TRUE;
}

float TimeManager::GetScaledDeltaTime(float localTimeScale) const
{
    return deltaTime * localTimeScale;
}
