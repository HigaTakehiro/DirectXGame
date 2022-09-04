#include "KeyInput.h"

void KeyInput::Initialize(WinApp* winApp) {
	HRESULT result = S_FALSE;

	//WinAppのインスタンスを記録
	this->winApp = winApp;
	//DirectInputのインスタンス生成
	result = DirectInput8Create(winApp->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	//キーボードデバイス生成
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	//入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void KeyInput::Update() {
	HRESULT result;

	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));
	//キーボード情報の取得開始
	result = devkeyboard->Acquire();
	//全キーの入力情報を取得する
	result = devkeyboard->GetDeviceState(sizeof(key), key);
}

bool KeyInput::PushKey(BYTE keyNumber) {
	//指定キーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool KeyInput::TriggerKey(BYTE keyNumber) {
	//前回は押していなくて、今回は押していればtrueを返す
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

KeyInput* KeyInput::GetIns()
{
	static KeyInput instance;
	return &instance;
}
