#pragma once
#include "Model.h"
#include "Object3d.h"
#include "SafeDelete.h"

class MapChip
{
public: //メンバ関数
	/// <summary>
	/// マップチップの初期化
	/// </summary>
	void MapChipInitialize();

	/// <summary>
	/// マップチップのロード
	/// </summary>
	/// <param name="mapname">csvファイル名</param>
	/// <param name="map_Width">横幅</param>
	/// <param name="map_Height">縦幅</param>
	/// <returns>マップチップ</returns>
	int** MapLoad(const std::string& mapname, int map_width, int map_height);

	/// <summary>
	/// マップチップのセット
	/// </summary>
	/// <param name="map">読み込んだマップチップ</param>
	/// <param name="map_Width">横幅</param>
	/// <param name="map_Height">縦幅</param>
	/// <param name="height">マップの高さ(0なら地面)</param>
	/// <returns>マップチップをオブジェクトにセット</returns>
	std::vector<Object3d*> MapSet(int** map, int map_Width, int map_Height, int height);

	/// <summary>
	/// マップチップの解放
	/// </summary>
	void MapChipFinalize();

private: //メンバ変数
	//ブロック一個分の大きさ
	//const float LAND_SCALE = 30.0f;
	int** map;

	Model* blockModel = nullptr;
};

