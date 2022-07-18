#include "GameScene.h"
#include "FBXObject3d.h"
#include <algorithm>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	safe_delete(sprite);
	safe_delete(model);
	safe_delete(player);
	safe_delete(map1_a);
	safe_delete(map1_b);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Sound* sound) {
	assert(dxCommon);
	assert(sound);

	this->dxCommon = dxCommon;
	this->sound = sound;
	input = Input::GetIns();

	//カメラ初期化
	camera = new Camera;

	//Sprite & DebugTextの初期化
	//Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	//Sprite::LoadTexture(debugTextNumber, L"Resources/debugfont.png");
	//debugText.Initialize(debugTextNumber);

	Sprite::LoadTexture(1, L"Resources/house.png");
	sprite = Sprite::Create(1, { 0, 0 });
	Sprite::LoadTexture(2, L"Resources/background.png");
	background = Sprite::Create(2, { 0, 0 });

	//Object3dの初期化
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	model = Model::CreateModel("Player");
	player = Object3d::Create(model);
	playerScale = { 5, 5, 5 };
	playerPos = { 0, 0, 0 };
	playerRot = { 0, 180, 0 };
	player->SetScale(playerScale);
	player->SetPosition(playerPos);
	player->SetRotation(playerRot);

	//MapChipの初期化
	mapchip = new MapChip;
	mapchip->MapChipInitialize();
	map1_a = mapchip->MapLoad("test1", 7, 4);
	map1_b = mapchip->MapLoad("test2", 7, 4);
	objects = mapchip->MapSet(map1_a, 7, 4, 0);
	objects2 = mapchip->MapSet(map1_b, 7, 4, 1);

	//FBXの初期化
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
	FBXObject3d::SetDevice(dxCommon->GetDev());
	FBXObject3d::SetCamera(camera);
	FBXObject3d::CreateGraphicsPipeline();
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	object1 = new FBXObject3d;
	object1->Initialize();
	object1->SetModel(model1);
	object1->SetScale({ 1.0f, 1.0f, 1.0f });
	object1->PlayAnimation();

}

void GameScene::Update() {
	// DirectX毎フレーム処理　ここから
	camera->SetTarget(object1->GetPosition());

	//デバッグテキスト
	if (input->TriggerKey(DIK_SPACE)) {
		debugText.Print("Test", 100, 100, 10.0f);
		if (object1->GetIsAnimation() == true) {
			object1->StopAnimation();
		}
		else {
			object1->PlayAnimation();
		}
	}

	if (input->PushKey(DIK_D)) {
		camera->CameraMoveEyeVector({ +2.0f, 0.0f, 0.0f });
	}
	if (input->PushKey(DIK_A)) {
		camera->CameraMoveEyeVector({ -2.0f, 0.0f, 0.0f });
	}
	if (input->PushKey(DIK_W)) {
		camera->CameraMoveEyeVector({ 0.0f, 0.0f, +2.0f });
	}
	if (input->PushKey(DIK_S)) {
		camera->CameraMoveEyeVector({ 0.0f, 0.0f, -2.0f });
	}
	if (input->PushKey(DIK_Q)) {
		camera->CameraMoveEyeVector({ 0.0f, +2.0f, 0.0f });
	}
	if (input->PushKey(DIK_E)) {
		camera->CameraMoveEyeVector({ 0.0f, -2.0f, 0.0f });
	}

	object1->SetPosition(playerPos);
	player->Update();
	object1->Update();

	for (auto object : objects) {
		object->Update();
	}
	for (auto object : objects2) {
		object->Update();
	}
}

void GameScene::Draw() {

	//スプライト描画処理(背景)
	Sprite::PreDraw(dxCommon->GetCmdList());
	background->Draw();
	Sprite::PostDraw();

	//3Dオブジェクト描画処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	//player->Draw();
	//for (auto object : objects) {
	//	object->Draw();
	//}
	//for (auto object : objects2) {
	//	object->Draw();
	//}
	object1->Draw(dxCommon->GetCmdList());
	Object3d::PostDraw();

	//スプライト描画処理(UI等)
	Sprite::PreDraw(dxCommon->GetCmdList());
	//sprite->Draw();
	//debugText.DrawAll(dxCommon->GetCmdList());
	Sprite::PostDraw();

	// ４．描画コマンドここまで
}

void GameScene::Finalize() {
	safe_delete(camera);
	mapchip->MapChipFinalize();
	objects.clear();
	objects.shrink_to_fit();
	objects2.clear();
	objects2.shrink_to_fit();
	safe_delete(mapchip);
	safe_delete(object1);
	safe_delete(model1);
	FbxLoader::GetInstance()->Finalize();
}