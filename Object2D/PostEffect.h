#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// 描画コマンド
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

};

