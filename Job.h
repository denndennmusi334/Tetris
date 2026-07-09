#pragma once


class Job
{
public:
    virtual ~Job()              {};
    virtual void Initialize()   =0; //初期化
    virtual void Finalize()     {}; //終了処理
    virtual void Update()       {}; //更新
    virtual void Draw()         {}; //描画
};
