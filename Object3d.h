#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3dx12.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Object3d
{
private: //エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス
	//頂点データ構造体
	//(モデルクラスに移す際に消す)
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData {
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat;   // ３Ｄ変換行列
	};

public: //静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面の横幅</param>
	/// <param name="window_height">画面の縦幅</param>
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);
	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object3d* Create();
	/// <summary>
	/// 始点座標のゲッター
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetEye() { return eye; }
	/// <summary>
	/// 始点座標のセッター
	/// </summary>
	/// <param name="eye"></param>
	static void SetEye(XMFLOAT3 eye);
	/// <summary>
	/// 注視点のゲッター
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetTarget() { return target; }
	/// <summary>
	/// 注視点のセッター
	/// </summary>
	/// <param name="target"></param>
	static void SetTarget(XMFLOAT3 target);
	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="pos">移動量</param>
	static void CameraMoveVector(XMFLOAT3 pos);

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//ビュー行列
	static XMMATRIX matView;
	//射影行列
	static XMMATRIX matProjection;
	//視点座標
	static XMFLOAT3 eye;
	//注視点座標
	static XMFLOAT3 target;
	//上方向ベクトル
	static XMFLOAT3 up;
	//ここから下のメンバ変数はモデルクラスに移す
	//デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	//デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	//頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファ
	static ComPtr<ID3D12Resource> indexBuff;
	//テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff;
	//シェーダーリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	//シェーダーリソースビューのハンドル(GPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	//インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;
	//頂点データ配列
	static std::vector<VertexPosNormalUv> vertices;
	//頂点インデックス配列
	static std::vector<unsigned short> indices;

private: //静的メンバ関数
	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int window_width, int window_height);
	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();
	/// <summary>
	/// グラフィックスパイプラインの初期化
	/// </summary>
	static bool InitializeGraphicsPipeline();
	//ここから下の静的メンバ関数はモデルクラスに移す
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	static bool InitializeDescriptorHeap();
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	static void LoadTexture();
	/// <summary>
	/// モデル作成
	/// </summary>
	static void CreateModel();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	/// <summary>
	/// モデルのセット
	/// </summary>
	//void SetModel(Model* model) { this->model = model; }

private: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//大きさ
	XMFLOAT3 scale = { 1, 1, 1 };
	//回転角
	XMFLOAT3 rotation = { 0, 0, 0 };
	//座標
	XMFLOAT3 position = { 0, 0, 0 };
	//ワールド変換行列
	XMMATRIX matWorld;
	//親オブジェクト
	Object3d* parent = nullptr;
	//モデルデータ
	//Model* model = nullptr;

};