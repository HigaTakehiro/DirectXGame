#pragma once
#include "Input.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"
#include "Camera.h"
#include "MapChip.h"
#include "SafeDelete.h"

using namespace DirectX;
using namespace Microsoft::WRL;

class GameScene
{
private: //�ÓI�����o�ϐ�
	//�f�o�b�N�e�L�X�g�p�e�N�X�`���ԍ�
	static const int debugTextNumber = 0;

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private: //�����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
	Camera* camera = nullptr;
	MapChip* mapchip = nullptr;

	//�Q�[���I�u�W�F�N�g & �X�v���C�g
	Model* model = nullptr;
	Object3d* player = nullptr;
	Sprite* sprite = nullptr;

	//�}�b�v�`�b�v�p�ϐ�
	int** map1_a = nullptr;
	int** map1_b = nullptr;
	std::vector<Object3d*> objects;
	std::vector<Object3d*> objects2;

	//�Q�[���V�[���p�ϐ�
	XMFLOAT3 playerPos = { 0, 0, 0 };
	XMFLOAT3 playerScale = { 0, 0, 0 };
	XMFLOAT3 playerRot = { 0, 0, 0 };

	XMFLOAT4 color = { 0.1f,0.25f, 0.5f,0.0f };
};
