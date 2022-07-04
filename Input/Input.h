#pragma once

#include <Windows.h>
#include <wrl.h>
#include "WinApp.h"

#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input final
{
private: //�G�C���A�X
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Input() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Input() = default;

	/// <summary>
	/// �R�s�[�R���X�g���N�^���֎~
	/// </summary>
	Input(const Input& obj) = delete;

	/// <summary>
	/// ������Z�q���֎~
	/// </summary>
	Input& operator=(const Input& obj) = delete;

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WinApp* winApp);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�</param>
	/// <returns>�L�[��������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// �C���X�^���X�擾�p�֐�
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Input* GetIns();

private: //�����o�ϐ�
	//DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> dinput;
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//�S�L�[�̏��
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	//WindowsAPI
	WinApp* winApp = nullptr;
};

