#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "GameScene.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ポインタ置き場
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	GameScene* gameScene = nullptr;
	Audio* audio = nullptr;

	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//入力の初期化
	input = new Input();
	input->Initialize(winApp);

	//Audioの初期化
	audio = new Audio();
	if (!audio->Initialize()) {
		assert(0);
		return 1;
	}
	audio->PlayWave("Resources/Alarm01.wav", true, 0.2f);

	gameScene = new GameScene();
	gameScene->Initialize(dxCommon, input, audio);

	// DirectX初期化処理　ここまで

	while (true)  // ゲームループ
	{
		if (winApp->ProcessMessage()) {
			break;
		}

		if (input->PushKey(DIK_0)) {
			break;
		}

		input->Update();
		gameScene->Update();

		// DirectX毎フレーム処理　ここまで
		// ４．描画コマンドここから

		gameScene->Draw();
	}

	//入力解放
	safe_delete(input);
	//GameScene解放
	safe_delete(gameScene);
	//WindowsAPI解放
	winApp->Finalize();
	safe_delete(winApp);
	//DirectX解放
	safe_delete(dxCommon);
	//audio解放
	safe_delete(audio);
	return 0;
}