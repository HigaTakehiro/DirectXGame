#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>

#include "WinApp.h"

class DirectXCommon
{
private: //エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* win);
	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();
	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();
	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDev() { return dev.Get(); }

	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }
private: //メンバ変数
	//ウィンドウズアプリケーション管理
	WinApp* winApp;

	ComPtr<ID3D12Device> dev;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

private: //メンバ関数
	/// <summary>
	/// デバイスの初期化
	/// </summary>
	void InitializeDev();
	/// <summary>
	/// コマンド関連の初期化
	/// </summary>
	void InitializeCmd();
	/// <summary>
	/// スワップチェーンの初期化
	/// </summary>
	void InitializeSwapChain();
	/// <summary>
	/// レンダーターゲットの初期化
	/// </summary>
	void InitializeRenderTarget();
	/// <summary>
	/// 深度バッファの初期化
	/// </summary>
	void InitializeDepthBuffer();
	/// <summary>
	/// フェンスの初期化
	/// </summary>
	void InitializeFence();
};

