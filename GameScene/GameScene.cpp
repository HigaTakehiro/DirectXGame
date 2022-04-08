#include "GameScene.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {
	safe_delete(model);
	safe_delete(player);
	safe_delete(map1_a);
	safe_delete(map1_b);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio) {
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	//�J����������
	camera = new Camera;

	//Sprite & DebugText�̏�����
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	Sprite::LoadTexture(debugTextNumber, L"Resources/debugfont.png");
	debugText.Initialize(debugTextNumber);

	Sprite::LoadTexture(1, L"Resources/house.png");
	sprite = Sprite::Create(1, { 0, 0 });

	//Object3d�̏�����
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	model = Model::CreateModel("Player");
	player = Object3d::Create(model);
	playerScale = { 5, 5, 5 };
	playerPos = { 0, 100, 0 };
	playerRot = { 0, 180, 0 };
	player->SetScale(playerScale);
	player->SetPosition(playerPos);
	player->SetRotation(playerRot);

	//MapChip�̏�����
	mapchip = new MapChip;
	mapchip->MapChipInitialize();
	map1_a = mapchip->MapLoad("test1", 7, 4);
	map1_b = mapchip->MapLoad("test2", 7, 4);
	objects = mapchip->MapSet(map1_a, 7, 4, 0);
	objects2 = mapchip->MapSet(map1_b, 7, 4, 1);
}

void GameScene::Update() {
	// DirectX���t���[�������@��������
	camera->SetTarget(playerPos);

	//�f�o�b�O�e�L�X�g
	if (input->PushKey(DIK_SPACE)) {
		debugText.Print("Test", 100, 100, 10.0f);
	}

	if (input->PushKey(DIK_RIGHT)) {
		camera->CameraMoveEyeVector({ +2.0f, 0.0f, 0.0f });
	}
	if (input->PushKey(DIK_LEFT)) {
		camera->CameraMoveEyeVector({ -2.0f, 0.0f, 0.0f });
	}
	if (input->PushKey(DIK_UP)) {
		camera->CameraMoveEyeVector({ 0.0f, 0.0f, +2.0f });
	}
	if (input->PushKey(DIK_DOWN)) {
		camera->CameraMoveEyeVector({ 0.0f, 0.0f, -2.0f });
	}
	if (input->PushKey(DIK_W)) {
		playerPos.z += 0.5f;
	}
	if (input->PushKey(DIK_S)) {
		playerPos.z -= 0.5f;
	}

	input->Update();
	player->SetPosition(playerPos);
	player->Update();
	for (auto object : objects) {
		object->Update();
	}
	for (auto object : objects2) {
		object->Update();
	}
}

void GameScene::Draw() {
	dxCommon->PreDraw(color);

	//�X�v���C�g�`�揈��(�w�i)

	//3D�I�u�W�F�N�g�`�揈��
	Object3d::PreDraw(dxCommon->GetCmdList());
	player->Draw();
	for (auto object : objects) {
		object->Draw();
	}
	for (auto object : objects2) {
		object->Draw();
	}
	Object3d::PostDraw();

	//�X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(dxCommon->GetCmdList());
	sprite->Draw();
	debugText.DrawAll(dxCommon->GetCmdList());
	Sprite::PostDraw();

	// �S�D�`��R�}���h�����܂�
	dxCommon->PostDraw();
}