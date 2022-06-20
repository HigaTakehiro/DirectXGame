#include <d3dx12.h>	
#include "WinApp.h"
#include "PostEffect.h"

using namespace DirectX;

PostEffect::PostEffect() : Sprite(
	100, //�e�N�X�`���ԍ�
	{ 0, 0 }, //���W
	{ 500.0f, 500.0f }, //�T�C�Y
	{ 1, 1, 1, 1 }, //�F
	{ 0.0f, 0.0f }, //�A���J�[�|�C���g
	false,
	false) 
{

}

void PostEffect::Initialize() {
	HRESULT result;

	//���N���X�̏�����
	Sprite::Initialize();

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	assert(SUCCEEDED(result));

	//�e�N�X�`�����N���A
	const UINT pixelCount = WinApp::window_width * WinApp::window_height;
	const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
	const UINT depthPicth = rowPitch * WinApp::window_height;
	UINT* img = new UINT[pixelCount];
	for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPicth);
	assert(SUCCEEDED(result));
	delete[] img;

	//SRV�p�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC  srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV���쐬
	device->CreateShaderResourceView(texBuff.Get(), &srvDesc, descHeapSRV->GetCPUDescriptorHandleForHeapStart());

}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList) {
	// ���[���h�s��̍X�V
	this->matWorld = XMMatrixIdentity();
	// Z����]
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	// ���s�ړ�
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// �萔�o�b�t�@�ɓ]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->mat = matWorld * matProjection;
		constMap->color = color;
		this->constBuff->Unmap(0, nullptr);
	}

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//�f�X�N���v�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());
	//�`��
	cmdList->DrawInstanced(4, 1, 0, 0);
}
