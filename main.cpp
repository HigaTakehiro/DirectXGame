#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#include <d3dx12.h>

#include <vector>
#include <string>
#include <fstream>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <DirectXTex.h>

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"
#include "DebugText.h"

using namespace DirectX;
using namespace Microsoft::WRL;

//// チャンクヘッダ
//struct ChunkHeader
//{
//	char id[4]; // チャンク毎のID
//	int32_t size;  // チャンクサイズ
//};
//
//// RIFFヘッダチャンク
//struct RiffHeader
//{
//	ChunkHeader chunk;   // "RIFF"
//	char type[4]; // "WAVE"
//};
//
//// FMTチャンク
//struct FormatChunk
//{
//	ChunkHeader chunk; // "fmt "
//	WAVEFORMATEX fmt; // 波形フォーマット
//};
//
//class XAudio2VoiceCallback : public IXAudio2VoiceCallback
//{
//public:
//	// ボイス処理パスの開始時
//	STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};
//	// ボイス処理パスの終了時
//	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
//	// バッファストリームの再生が終了した時
//	STDMETHOD_(void, OnStreamEnd) (THIS) {};
//	// バッファの使用開始時
//	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
//	// バッファの末尾に達した時
//	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {};
//	// 再生がループ位置に達した時
//	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
//	// ボイスの実行エラー時
//	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
//};

//// 音声データ
//struct SoundData
//{
//	// 波形フォーマット
//	WAVEFORMATEX wfex;
//	// バッファの先頭アドレス
//	BYTE* pBuffer;
//	// バッファのサイズ
//	unsigned int bufferSize;
//};
//
//// 音声読み込み
//SoundData SoundLoadWave(const char* filename)
//{
//	// ファイル入力ストリームのインスタンス
//	std::ifstream file;
//	// .wavファイルをバイナリモードで開く
//	file.open(filename, std::ios_base::binary);
//	// ファイルオープン失敗を検出する
//	assert(file.is_open());
//
//	// RIFFヘッダーの読み込み
//	RiffHeader riff;
//	file.read((char*)&riff, sizeof(riff));
//	// ファイルがRIFFかチェック
//	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
//		assert(0);
//	}
//	// タイプがWAVEかチェック
//	if (strncmp(riff.type, "WAVE", 4) != 0) {
//		assert(0);
//	}
//
//	// Formatチャンクの読み込み
//	FormatChunk format = {};
//	// チャンクヘッダーの確認
//	file.read((char*)&format, sizeof(ChunkHeader));
//	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
//		assert(0);
//	}
//	// チャンク本体の読み込み
//	assert(format.chunk.size <= sizeof(format.fmt));
//	file.read((char*)&format.fmt, format.chunk.size);
//
//	// Dataチャンクの読み込み
//	ChunkHeader data;
//	file.read((char*)&data, sizeof(data));
//	// JUNKチャンクを検出した場合
//	if (strncmp(data.id, "JUNK ", 4) == 0) {
//		// 読み取り位置をJUNKチャンクの終わりまで進める
//		file.seekg(data.size, std::ios_base::cur);
//		// 再読み込み
//		file.read((char*)&data, sizeof(data));
//	}
//
//	if (strncmp(data.id, "data ", 4) != 0) {
//		assert(0);
//	}
//
//	// Dataチャンクのデータ部（波形データ）の読み込み
//	char* pBuffer = new char[data.size];
//	file.read(pBuffer, data.size);
//
//	// Waveファイルを閉じる
//	file.close();
//
//	// returnする為の音声データ
//	SoundData soundData = {};
//
//	soundData.wfex = format.fmt;
//	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
//	soundData.bufferSize = data.size;
//
//	return soundData;
//}
//
//// 音声データ解放
//void SoundUnload(SoundData* soundData)
//{
//	// バッファのメモリを解放
//	delete[] soundData->pBuffer;
//
//	soundData->pBuffer = 0;
//	soundData->bufferSize = 0;
//	soundData->wfex = {};
//}
//
//// 音声再生
//void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData) {
//
//	HRESULT result;
//
//	// 波形フォーマットを元にSourceVoiceの生成
//	IXAudio2SourceVoice* pSourceVoice = nullptr;
//	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
//	assert(SUCCEEDED(result));
//
//	// 再生する波形データの設定
//	XAUDIO2_BUFFER buf{};
//	buf.pAudioData = soundData.pBuffer;
//	buf.AudioBytes = soundData.bufferSize;
//	buf.Flags = XAUDIO2_END_OF_STREAM;
//
//	// 波形データの再生
//	result = pSourceVoice->SubmitSourceBuffer(&buf);
//	result = pSourceVoice->Start();
//}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//デバックテキスト用テクスチャ番号
	const int debugTextNumber = 0;

	//ポインタ置き場
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	DebugText debugText;

	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//入力の初期化
	input = new Input();
	input->Initialize(winApp);

	//Sprite & DebugTextの初期化
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	
	Sprite::LoadTexture(debugTextNumber, L"Resources/debugfont.png");
	debugText.Initialize(debugTextNumber);

	Sprite::LoadTexture(1, L"Resources/house.png");
	Sprite* sprite1 = Sprite::Create(1, { 0, 0 });


	//Object3d & Modelの初期化
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	Model* model1 = model1 = Model::CreateModel("Player");
	Object3d* object3d = Object3d::Create(model1);
	XMFLOAT3 scale = { 15, 15, 15 };
	XMFLOAT3 pos = { 0, 0, 0 };
	XMFLOAT3 rotation = { 0, 180, 0 };
	object3d->SetScale(scale);
	object3d->SetPosition(pos);
	object3d->SetRotation(rotation);

	HRESULT result;

	//ComPtr<IXAudio2> xAudio2;
	//IXAudio2MasteringVoice* masterVoice;
	//XAudio2VoiceCallback voiceCallback;

	//// XAudioエンジンのインスタンスを生成
	//result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//assert(SUCCEEDED(result));

	//// マスターボイスを生成
	//result = xAudio2->CreateMasteringVoice(&masterVoice);
	//assert(SUCCEEDED(result));

	//// 音声読み込み
	//SoundData soundData1 = SoundLoadWave("Resources/Alarm01.wav");

	// 音声再生
	//SoundPlayWave(xAudio2.Get(), soundData1);

	// DirectX初期化処理　ここまで

	while (true)  // ゲームループ
	{
		if (winApp->ProcessMessage()) {
			break;
		}

		// DirectX毎フレーム処理　ここから

		//デバッグテキスト
		debugText.Print("Test", 100, 100, 10.0f);

		input->Update();
		object3d->Update();

		// DirectX毎フレーム処理　ここまで
		// ４．描画コマンドここから

		dxCommon->PreDraw();

		//スプライト描画処理(背景)

		//3Dオブジェクト描画処理
		Object3d::PreDraw(dxCommon->GetCmdList());
		object3d->Draw();
		Object3d::PostDraw();

		//スプライト描画処理(UI等)
		Sprite::PreDraw(dxCommon->GetCmdList());
		sprite1->Draw();
		debugText.DrawAll(dxCommon->GetCmdList());
		Sprite::PostDraw();



		// ４．描画コマンドここまで

		dxCommon->PostDraw();
	}

	//// XAudio2解放
	//xAudio2.Reset();
	//// 音声データ解放
	//SoundUnload(&soundData1);
	//入力解放
	delete input;
	//WindowsAPI解放
	winApp->Finalize();
	delete winApp;
	//DirectX解放
	delete dxCommon;
	//Object3d解放
	delete object3d;
	//model解放
	delete model1;
	//sprite解放
	delete sprite1;
	return 0;
}