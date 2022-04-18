#pragma once
#include "Input.h"//
#include "DirectXCommon.h"//
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"//
#include "Camera.h"
#include "MapChip.h"
#include "SafeDelete.h"
#include "FbxLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;

class GameScene
{
private: //静的メンバ変数
	//デバックテキスト用テクスチャ番号
	static const int debugTextNumber = 0;

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

private: //メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
	Camera* camera = nullptr;
	MapChip* mapchip = nullptr;

	//ゲームオブジェクト & スプライト
	Model* model = nullptr;
	Object3d* player = nullptr;
	Sprite* sprite = nullptr;

	//マップチップ用変数
	int** map1_a = nullptr;
	int** map1_b = nullptr;
	std::vector<Object3d*> objects;
	std::vector<Object3d*> objects2;

	//ゲームシーン用変数
	XMFLOAT3 playerPos = { 0, 0, 0 };
	XMFLOAT3 playerScale = { 0, 0, 0 };
	XMFLOAT3 playerRot = { 0, 0, 0 };

	XMFLOAT4 color = { 0.1f,0.25f, 0.5f,0.0f };
};

