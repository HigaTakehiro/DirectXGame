#include "FbxLoader.h"

const std::string FbxLoader::baseDirectory = "Resources/";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device) {
    //nullチェック
    assert(fbxManager == nullptr);
    //引数からメンバ変数に代入
    this->device = device;
    //FBXマネージャの生成
    fbxManager = FbxManager::Create();
    //FBXマネージャの入出力設定
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    //FBXインポータの生成
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize() {
    //各種FBXインスタンスの破棄
    fbxImporter->Destroy();
    fbxManager->Destroy();
}

void FbxLoader::LoadModelFromFile(const string& modelName) {
    //モデルと同じ名前のフォルダから読み込む
    const string directoryPath = baseDirectory + modelName + "/";
    //拡張子
    const string fileName = modelName + ".fbx";
    //連結する
    const string fullPath = directoryPath + fileName;

    //ファイル名を指定してFBXファイルを読み込む
    if (!fbxImporter->Initialize(fullPath.c_str(), -1, fbxManager->GetIOSettings())) {
        assert(0);
    }

    //シーン生成
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //ファイルからロードしたFBXの情報をシーンにインポート
    fbxImporter->Import(fbxScene);
}