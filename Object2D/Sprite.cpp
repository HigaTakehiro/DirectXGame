#include "Sprite.h"
#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Sprite::device = nullptr;
UINT Sprite::descriptorHandleIncrementSize;
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Sprite::rootSignature;
ComPtr<ID3D12PipelineState> Sprite::pipelineState;
XMMATRIX Sprite::matProjection;
ComPtr<ID3D12DescriptorHeap> Sprite::descHeap;
ComPtr<ID3D12Resource> Sprite::texBuff[srvCount];

bool Sprite::StaticInitialize(ID3D12Device* device, int window_width, int window_height) {
	//nullチェック
	assert(device);

	Sprite::device = device;

	//デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	HRESULT result;

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl",  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl",   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,      0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	//サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	//ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;              // 背面カリングをしない

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // 標準設定
	blenddesc.BlendEnable = true;                   // ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;    // 加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;      // ソースの値を100% 使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;    // デストの値を   0% 使う
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;             // 加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;         // ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;    // 1.0f-ソースのアルファ値

	// デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthEnable = false;    // 深度テストをしない
	//gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;       // 常に上書きルール
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	//形状設定(三角形)
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0〜255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0); // 定数バッファビューとして初期化(b0レジスタ)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	if (FAILED(result)) {
		assert(0);
		return false;
	}
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	gpipeline.pRootSignature = rootSignature.Get();
	//グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//射影行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width,
		(float)window_height, 0.0f,
		0.0f, 1.0f
	);

	//デスクリプタヒープを生成 
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = srvCount;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

bool Sprite::LoadTexture(UINT texnumber, const wchar_t* filename) {
	//nullチェック
	assert(device);

	HRESULT result;
	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	//リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texBuff[texnumber]));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//テクスチャバッファにデータ転送
	result = texBuff[texnumber]->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,    // 1ラインサイズ
		(UINT)img->slicePitch   // 1枚サイズ
	);

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//ヒープのtexnumber番目にシェーダーリソースビュー作成
	device->CreateShaderResourceView(
		texBuff[texnumber].Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	return true;
}

void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList) {
	//PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Sprite::cmdList == nullptr);
	//コマンドリストをセット
	Sprite::cmdList = cmdList;
	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::PostDraw() {
	//コマンドリストを解除
	Sprite::cmdList = nullptr;
}

Sprite* Sprite::Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY) {
	XMFLOAT2 size = { 100.0f, 100.0f };

	if (texBuff[texNumber]) {
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();
		//スプライトのサイズをテクスチャのサイズに設定
		size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	//Spriteのインスタンスを生成
	Sprite* sprite = new Sprite(texNumber, position, size, color, anchorpoint, isFlipX, isFlipY);
	if (sprite == nullptr) {
		return nullptr;
	}

	//初期化
	if (!sprite->Initialize()) {
		delete sprite;
		assert(0);
		return nullptr;
	}

	return sprite;
}

Sprite::Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY) {
	this->position = position;
	this->size = size;
	this->anchorpoint = anchorpoint;
	this->matWorld = XMMatrixIdentity();
	this->color = color;
	this->texNumber = texNumber;
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;
	this->texSize = size;
}

bool Sprite::Initialize() {
	//nullチェック
	assert(device);

	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// 頂点バッファデータ転送
	TransferVertices();

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // 色指定（RGBA）
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = matProjection;
		constMap->time = time;
		constBuff->Unmap(0, nullptr);
	}

	return true;
}

void Sprite::SetRotation(float rotation) {
	this->rotation = rotation;

	//頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetPosition(XMFLOAT2 position) {
	this->position = position;

	//頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetSize(XMFLOAT2 size) {
	this->size = size;

	//頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetAnchorPoint(XMFLOAT2 anchorpoint) {
	this->anchorpoint = anchorpoint;

	//頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetIsFlipX(bool isFlipX) {
	this->isFlipX = isFlipX;

	//頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetIsFlipY(bool isFlipY) {
	this->isFlipY = isFlipY;

	//頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize) {
	this->texBase = texBase;
	this->texSize = texSize;

	//頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::Draw() {
	// ワールド行列の更新
	this->matWorld = XMMatrixIdentity();
	// Z軸回転
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	// 平行移動
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// 定数バッファに転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->mat = matWorld * matProjection;
		constMap->color = color;
		this->constBuff->Unmap(0, nullptr);
	}

	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	//デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			this->texNumber,
			descriptorHandleIncrementSize
		)
	);
	//描画
	cmdList->DrawInstanced(4, 1, 0, 0);

}

void Sprite::TransferVertices() {
	HRESULT result = S_FALSE;

	//   左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	//左右反転
	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (isFlipX) {
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}

	//頂点データ
	VertexPosUv vertices[vertNum];

	vertices[LB].pos = { left, bottom, 0.0f }; //左下
	vertices[LT].pos = { left, top, 0.0f }; //左上
	vertices[RB].pos = { right, bottom, 0.0f }; //右下
	vertices[RT].pos = { right, top, 0.0f }; //右上

	//テクスチャ情報取得
	if (texBuff[texNumber]) {
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();

		float tex_left = texBase.x / resDesc.Width;
		float tex_right = (texBase.x + texSize.x) / resDesc.Width;
		float tex_top = texBase.y / resDesc.Height;
		float tex_bottom = (texBase.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left, tex_bottom };
		vertices[LT].uv = { tex_left, tex_top };
		vertices[RB].uv = { tex_right, tex_bottom };
		vertices[RT].uv = { tex_right, tex_top };
	}

	//頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}