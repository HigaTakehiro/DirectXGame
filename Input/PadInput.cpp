#include "PadInput.h"

PadInput* PadInput::GetIns()
{
	static PadInput instance;
	return &instance;
}

void PadInput::Initialize(WinApp* winApp) {
	HRESULT result = S_FALSE;

	//WinAppのインスタンスを記録
	this->winApp = winApp;
	//DirectInputのインスタンス生成
	result = DirectInput8Create(winApp->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	//パッドデバイス生成
	result = dinput->CreateDevice(GUID_Joystick, &devPad, NULL);
	if (devPad != nullptr) {
		//入力データ形式のセット
		result = devPad->SetDataFormat(&c_dfDIJoystick);
		//排他制御レベルのセット
		result = devPad->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}
}

void PadInput::Update() {
	HRESULT result;

	if (devPad != nullptr) {
		//前回のパッドのボタン押下を保存
		prePadState = padState;
		//パッド情報の更新
		result = devPad->Acquire();
		//パッドの入力情報を取得する
		result = devPad->GetDeviceState(sizeof(DIJOYSTATE), &padState);
	}
}

bool PadInput::PushButton(BYTE buttonNumber) {
	//指定キーを押していればtrueを返す
	if (padState.rgbButtons[buttonNumber] & (0x80)) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool PadInput::TriggerButton(BYTE buttonNumber) {
	//前回は押していなくて、今回は押していればtrueを返す
	if (!prePadState.rgbButtons[buttonNumber] && padState.rgbButtons[buttonNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool PadInput::PushCrossKey(LONG crossKeyNumber) {
	if (padState.rgdwPOV[0] != 0xFFFFFFFF) {
		//指定キーを押していればtrueを返す
		if (padState.rgdwPOV[0] == crossKeyNumber) {
			return true;
		}
	}
	//そうでないならfalseを返す
	return false;
}