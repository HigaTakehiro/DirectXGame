#pragma once

#include <Windows.h>
#include <wrl.h>
#include "WinApp.h"

#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input final
{
private: //エイリアス
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Input() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Input() = default;

	/// <summary>
	/// コピーコンストラクタを禁止
	/// </summary>
	Input(const Input& obj) = delete;

	/// <summary>
	/// 代入演算子を禁止
	/// </summary>
	Input& operator=(const Input& obj) = delete;

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>キーが押されているか</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// インスタンス取得用関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static Input* GetIns();

private: //メンバ変数
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> dinput;
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//全キーの状態
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	//WindowsAPI
	WinApp* winApp = nullptr;
};

