#include "DirectXCommon.h"
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

void DirectXCommon::Initialize(WinApp* winApp) {
	//nullチェック
	assert(winApp);
	//メンバ変数に保存
	this->winApp = winApp;

	//デバイスの生成
	InitializeDev();
	//コマンド関連の初期化
	InitializeCmd();
	//スワップチェーンの初期化
	InitializeSwapChain();
	//レンダーターゲットビューの初期化
	InitializeRenderTarget();
	//深度バッファの初期化
	InitializeDepthBuffer();
	//フェンスの初期化
	InitializeFence();
}

void DirectXCommon::InitializeDev() {
	HRESULT result = S_FALSE;

	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter = nullptr;
	for (int i = 0;
		dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter); // 動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);  // アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		std::wstring strDesc = adesc.Description;   // アダプター名
		// Intel UHD Graphics（オンボードグラフィック）を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];   // 採用
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
}

void DirectXCommon::InitializeCmd() {
	HRESULT result = S_FALSE;

	// コマンドアロケータを生成
	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));

	// コマンドリストを生成
	result = dev->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&cmdList));

	// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
}

void DirectXCommon::InitializeSwapChain() {
	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 色情報の書式
	swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapchainDesc.BufferCount = 2;  // バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapchain1;

	// スワップチェーンの生成
	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		winApp->GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);

	// 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapchain1.As(&swapchain);
}

void DirectXCommon::InitializeRenderTarget() {
	HRESULT result = S_FALSE;

	// 各種設定をしてデスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	heapDesc.NumDescriptors = 2;    // 裏表の２つ
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	// 裏表の２つ分について
	backBuffers.resize(2);
	for (int i = 0; i < 2; i++)
	{
		// スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

		// レンダーターゲットビューの生成
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i,
				dev->GetDescriptorHandleIncrementSize(heapDesc.Type)
			)
		);
	}
}

void DirectXCommon::InitializeDepthBuffer() {
	HRESULT result = S_FALSE;

	// 深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WinApp::window_width,
		WinApp::window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	// 深度バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	//ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::InitializeFence() {
	HRESULT result = S_FALSE;

	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}