#pragma once
class Camera
{
private:
	Vector2D<float> pos = { 0, 0 };
public:

	/// <summary>
	/// cameraの位置を変えるときに使う関数
	/// </summary>
	/// <param name="cameraPos">ここに移動させたい座標を送るとその位置に移動する.</param>
	void SetCameraPos(const Vector2D<float> cameraPos)
	{
		pos = cameraPos;
	}

	/// <summary>
	/// cameraのworld座標を取れる.
	/// </summary>
	/// <returns>cameraのworld座標</returns>
	const Vector2D<float>& GetPos() const
	{
		return pos;
	}

	/// <summary>
	/// camera基準の描画をしたいときに使う関数.
	/// 自分の現在座標を送ると描画すべき場所が帰ってくる.
	/// 使う場合は帰ってきた値が画面外の場合は描画しないなどの処理を入れたほうがいい.
	/// </summary>
	/// <param name="worldPos">自分のworld座標</param>
	/// <returns>camera基準の座標</returns>
	Vector2D<float> ToScreen(const Vector2D<float>& worldPos) const
	{
		return worldPos - pos;
	}
};

