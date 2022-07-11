#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sound.h"
#include "GameScene.h"
#include "PostEffect.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//背景色
	const XMFLOAT4 backColor = { 0.1f,0.25f, 0.5f, 0.0f };

	//ポインタ置き場
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	GameScene* gameScene = nullptr;
	Sound* sound = nullptr;
	PostEffect* postEffect = nullptr;

	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//入力の初期化
	//input = new Input();
	Input::GetIns()->Initialize(winApp);

	//Soundの初期化
	sound = new Sound();
	if (!sound->Initialize()) {
		assert(0);
		return 1;
	}
	//sound->PlayWave("Resources/Alarm01.wav", true, 0.2f);

	gameScene = new GameScene();
	gameScene->Initialize(dxCommon, sound);

	//PostEffectの初期化
	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	postEffect = new PostEffect();
	postEffect->Initialize();


	// DirectX初期化処理　ここまで

	while (true)  // ゲームループ
	{
		if (winApp->ProcessMessage()) {
			break;
		}

		if (Input::GetIns()->PushKey(DIK_0)) {
			break;
		}

		Input::GetIns()->Update();
		gameScene->Update();

		// DirectX毎フレーム処理　ここまで
		// ４．描画コマンドここから
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		gameScene->Draw();
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw(backColor);

		postEffect->Draw(dxCommon->GetCmdList());

		dxCommon->PostDraw();

	}

	//GameScene解放
	gameScene->Finalize();
	safe_delete(gameScene);
	//WindowsAPI解放
	winApp->Finalize();
	safe_delete(winApp);
	//DirectX解放
	safe_delete(dxCommon);
	//sound解放
	safe_delete(sound);
	//PostEffect解放
	safe_delete(postEffect);
	return 0;
}