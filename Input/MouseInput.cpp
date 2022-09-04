#include "MouseInput.h"

MouseInput* MouseInput::GetIns()
{
	static MouseInput instance;
	return &instance;
}

void MouseInput::Initialize(WinApp* winApp) {
	HRESULT result = S_FALSE;

	//WinAppのインスタンスを記録
	this->winApp = winApp;
	//DirectInputのインスタンス生成
	result = DirectInput8Create(winApp->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	//マウスデバイス生成
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (devMouse != nullptr) {
		//入力データ形式のセット
		result = devMouse->SetDataFormat(&c_dfDIMouse);
		//排他制御レベルのセット
		result = devMouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}
}

void MouseInput::Update() {
	HRESULT result;

	//前回のマウスのクリックを保存
	preMouseState = mouseState;
	//マウス情報の更新
	if (devMouse != nullptr) {
		result = devMouse->Acquire();
		//マウスの入力情報を取得する
		result = devMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
		//マウスカーソルの位置を取得
		GetCursorPos(&mousePoint);
		//取得したカーソルの位置をウィンドウ上の位置に変換
		ScreenToClient(winApp->GetHwnd(), &mousePoint);
	}
}

bool MouseInput::PushClick(BYTE mouseClick) {
	//指定キーを押していればtrueを返す
	if (mouseState.rgbButtons[mouseClick] & (0x80)) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool MouseInput::TriggerClick(BYTE mouseClick) {
	//前回は押していなくて、今回は押していればtrueを返す
	if (!preMouseState.rgbButtons[mouseClick] && mouseState.rgbButtons[mouseClick]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}