#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3dx12.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Object3d
{
private: //�G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�T�u�N���X
	//���_�f�[�^�\����
	//(���f���N���X�Ɉڂ��ۂɏ���)
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		XMFLOAT4 color; // �F (RGBA)
		XMMATRIX mat;   // �R�c�ϊ��s��
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʂ̉���</param>
	/// <param name="window_height">��ʂ̏c��</param>
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);
	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static Object3d* Create();
	/// <summary>
	/// �n�_���W�̃Q�b�^�[
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetEye() { return eye; }
	/// <summary>
	/// �n�_���W�̃Z�b�^�[
	/// </summary>
	/// <param name="eye"></param>
	static void SetEye(XMFLOAT3 eye);
	/// <summary>
	/// �����_�̃Q�b�^�[
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetTarget() { return target; }
	/// <summary>
	/// �����_�̃Z�b�^�[
	/// </summary>
	/// <param name="target"></param>
	static void SetTarget(XMFLOAT3 target);
	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="pos">�ړ���</param>
	static void CameraMoveVector(XMFLOAT3 pos);

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//�r���[�s��
	static XMMATRIX matView;
	//�ˉe�s��
	static XMMATRIX matProjection;
	//���_���W
	static XMFLOAT3 eye;
	//�����_���W
	static XMFLOAT3 target;
	//������x�N�g��
	static XMFLOAT3 up;
	//�������牺�̃����o�ϐ��̓��f���N���X�Ɉڂ�
	//�f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	//�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	//���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	static ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuff;
	//�V�F�[�_�[���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	//�V�F�[�_�[���\�[�X�r���[�̃n���h��(GPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	//�C���f�b�N�X�o�b�t�@�r���[
	static D3D12_INDEX_BUFFER_VIEW ibView;
	//���_�f�[�^�z��
	static std::vector<VertexPosNormalUv> vertices;
	//���_�C���f�b�N�X�z��
	static std::vector<unsigned short> indices;

private: //�ÓI�����o�֐�
	/// <summary>
	/// �J����������
	/// </summary>
	/// <param name="window_width">��ʉ���</param>
	/// <param name="window_height">��ʏc��</param>
	static void InitializeCamera(int window_width, int window_height);
	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();
	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̏�����
	/// </summary>
	static bool InitializeGraphicsPipeline();
	//�������牺�̐ÓI�����o�֐��̓��f���N���X�Ɉڂ�
	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	static bool InitializeDescriptorHeap();
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	static void LoadTexture();
	/// <summary>
	/// ���f���쐬
	/// </summary>
	static void CreateModel();

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() { return position; }
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	//void SetModel(Model* model) { this->model = model; }

private: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�F
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//�傫��
	XMFLOAT3 scale = { 1, 1, 1 };
	//��]�p
	XMFLOAT3 rotation = { 0, 0, 0 };
	//���W
	XMFLOAT3 position = { 0, 0, 0 };
	//���[���h�ϊ��s��
	XMMATRIX matWorld;
	//�e�I�u�W�F�N�g
	Object3d* parent = nullptr;
	//���f���f�[�^
	//Model* model = nullptr;

};