#include "MapChip.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

void MapChip::MapChipInitialize() {
	blockModel = Model::CreateModel("Block");
}

int** MapChip::MapLoad(const std::string& mapname, int map_width, int map_height) {

	map = (int**)malloc(sizeof(int*) * map_height); //â°
	map[0] = (int*)malloc(map_width * map_height * sizeof(int)); //èc
	for (int i = 0; i < map_height; i++) {
		map[i] = map[0] + i * map_width;
	}

	const string& filename = "Resources/map/" + mapname + ".csv";
	const char* mapfile = filename.c_str();

	//àÍçsÇ∏Ç¬ì«Ç›çûÇﬁ
	FILE* fp;
	fopen_s(&fp, mapfile, "r");
	for (int i = 0; i < map_height; i++) {
		for (int j = 0; j < map_width; j++) {
			fscanf_s(fp, "%d,", &map[i][j]);
		}
	}

	fclose(fp);

	return map;
}

std::vector<Object3d*> MapChip::MapSet(int** map, int map_width, int map_height, int height) {
	const float LAND_SCALE = 30.0f;

	std::vector<Object3d*> objects;

	Model* modeltable[] = {
		nullptr,
		blockModel,
	};

	for (int i = 0; i < map_height; i++) {
		for (int j = 0; j < map_width; j++) {

			int modelIndex = map[i][j];

			if (modeltable[modelIndex] != nullptr) {
				Object3d* object = Object3d::Create(modeltable[modelIndex]);
				object->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
				object->SetPosition({ j * LAND_SCALE - 50, height * LAND_SCALE + 50, i * LAND_SCALE + 50 });
				objects.push_back(object);
			}
		}
	}

	return objects;
}


void MapChip::MapChipFinalize() {
	safe_delete(blockModel);
}