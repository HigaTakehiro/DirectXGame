#pragma once
#include "Input.h"//
#include "DirectXCommon.h"//
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Sound.h"//
#include "Camera.h"
#include "MapChip.h"
#include "SafeDelete.h"
#include "FbxLoader.h"
#include "FBXObject3d.h"

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
	void Initialize(DirectXCommon* dxCommon, Sound* sound);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

private: //�����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DebugText debugText;
	Sound* sound = nullptr;
	Camera* camera = nullptr;
	MapChip* mapchip = nullptr;

	//�Q�[���I�u�W�F�N�g & �X�v���C�g
	Model* model = nullptr;
	Object3d* player = nullptr;
	Sprite* sprite = nullptr;
	Sprite* background = nullptr;

	//�}�b�v�`�b�v�p�ϐ�
	int** map1_a = nullptr;
	int** map1_b = nullptr;
	std::vector<Object3d*> objects;
	std::vector<Object3d*> objects2;

	//FBX�p�ϐ�
	FBXModel* model1 = nullptr;
	FBXObject3d* object1 = nullptr;

	//�Q�[���V�[���p�ϐ�
	XMFLOAT3 playerPos = { 0, 0, 0 };
	XMFLOAT3 playerScale = { 0, 0, 0 };
	XMFLOAT3 playerRot = { 0, 0, 0 };

};

