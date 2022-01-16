#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//デバックテキスト用テクスチャ番号
	const int debugTextNumber = 0;

	//ポインタ置き場
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	DebugText debugText;
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

	//Sprite & DebugTextの初期化
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	
	Sprite::LoadTexture(debugTextNumber, L"Resources/debugfont.png");
	debugText.Initialize(debugTextNumber);

	Sprite::LoadTexture(1, L"Resources/house.png");
	Sprite* sprite1 = Sprite::Create(1, { 0, 0 });

	//Object3d & Modelの初期化
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	Model* model1 = model1 = Model::CreateModel("Player");
	Object3d* object3d = Object3d::Create(model1);
	XMFLOAT3 scale = { 15, 15, 15 };
	XMFLOAT3 pos = { 0, 0, 0 };
	XMFLOAT3 rotation = { 0, 180, 0 };
	object3d->SetScale(scale);
	object3d->SetPosition(pos);
	object3d->SetRotation(rotation);

	HRESULT result;

	// DirectX初期化処理　ここまで

	while (true)  // ゲームループ
	{
		if (winApp->ProcessMessage()) {
			break;
		}

		// DirectX毎フレーム処理　ここから

		//デバッグテキスト
		if (input->PushKey(DIK_SPACE)) {
			debugText.Print("Test", 100, 100, 10.0f);
		}

		input->Update();
		object3d->Update();

		// DirectX毎フレーム処理　ここまで
		// ４．描画コマンドここから

		dxCommon->PreDraw();

		//スプライト描画処理(背景)

		//3Dオブジェクト描画処理
		Object3d::PreDraw(dxCommon->GetCmdList());
		object3d->Draw();
		Object3d::PostDraw();

		//スプライト描画処理(UI等)
		Sprite::PreDraw(dxCommon->GetCmdList());
		sprite1->Draw();
		debugText.DrawAll(dxCommon->GetCmdList());
		Sprite::PostDraw();

		// ４．描画コマンドここまで
		dxCommon->PostDraw();
	}

	//入力解放
	delete input;
	//WindowsAPI解放
	winApp->Finalize();
	delete winApp;
	//DirectX解放
	delete dxCommon;
	//Object3d解放
	delete object3d;
	//model解放
	delete model1;
	//sprite解放
	delete sprite1;
	//audio解放
	delete audio;
	return 0;
}