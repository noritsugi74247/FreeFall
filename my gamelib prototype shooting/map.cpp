#include "map.h"
#include "sprite_data.h"
#include "block.h"
#include "game.h"
#include <fstream>

extern char** blockdata;
void MapChip::init(const char* filename)
{
	clear();

	if (!getCSVSize(filename, &chipnumX, &chipnumY))
	{
		assert(!"地形用CSVファイルの読み込みに失敗しました");
	}

	data = new char* [chipnumY];

	for (uint32_t j = 0; j < chipnumY; j++)
	{
		data[j] = new char[chipnumX];
	}



	// マップデータのロード

	if (!loadMapData(filename, data))
	{
		assert(!"地形データのロードに失敗");
	}

	WIDTH = static_cast<float>(chipnumX * CHIP_SIZE);
	HEIGHT = static_cast<float>(chipnumY * CHIP_SIZE);

}

void MapChip::clear() 
{
	scroll = {};
}

void MapChip::update()
{
	// プレイヤーのポインタ取得（確認）
	auto pl = &(*Game::instance()->playerManager()->getList()->begin());
	scroll.y += pl->speed.y;
	// スクロール処理
	// 右方向のスクロール処理
	//if (scroll.x < pl->position.x + pl->size.x - system::SCREEN_WIDTH + SCROLL_MERGIN_X)
	//	scroll.x = pl->position.x + pl->size.x - system::SCREEN_WIDTH + SCROLL_MERGIN_X;

	//// 左方向のスクロール処理
	//if (scroll.x > pl->position.x - pl->size.x - SCROLL_MERGIN_X)
	//	scroll.x = pl->position.x - pl->size.x - SCROLL_MERGIN_X;

	//// 下方向のスクロール処理
	//if (scroll.y < pl->position.y - system::SCREEN_HEIGHT + SCROLL_MERGIN_Y)
	//	scroll.y = pl->position.y - system::SCREEN_HEIGHT + SCROLL_MERGIN_Y;

	//// 上方向のスクロール処理
	//if (scroll.y > pl->position.y - pl->size.y - SCROLL_MERGIN_Y / 15)
	//	scroll.y = pl->position.y - pl->size.y - SCROLL_MERGIN_Y / 15;

	//// エリア制限（右）
	//if (scroll.x > WIDTH - system::SCREEN_WIDTH) scroll.x = WIDTH - system::SCREEN_WIDTH;

	//// エリア制限（左）
	//if (scroll.x < 0) scroll.x = 0;

	//// エリア制限（下）
	//if (scroll.y > HEIGHT - system::SCREEN_HEIGHT) scroll.y = HEIGHT - system::SCREEN_HEIGHT;

	//// エリア制限（上）
	//if (scroll.y < 0) scroll.y = 0;
}
void MapChip::uninit()
{
	for (u_int t = 0; t < chipnumY; t++)
	{
		delete[] data[t];
		data[t] = nullptr;
	}
	delete[] data;
	data = nullptr;
}
void MapChip::move(OBJ2D* obj)
{
}
bool getCSVSize(const char* filepath, u_int* buf_xSize, u_int* buf_ySize)
{
	std::ifstream input(filepath); 
	if (input.fail()) return false;

	std::string output;
	u_int w = 0, h = 0;
	while (std::getline(input, output))
	{
		if (h == 0)
		{
			w = std::count(output.cbegin(), output.cend(), ',') + 1;
		}
		h++;
	}

	*buf_xSize = w;
	*buf_ySize = h;
	return true;

}

bool MapChip::loadMapData(const char* filename, char** mapData)
{
	std::ifstream Filename(filename);
	if (Filename.fail()) {
		assert(!"cannot open file. please confirm directory or filepath.");
		return false;
	}

	u_int x = 0, y = 0;
	std::string output;
	while (std::getline(Filename, output))
	{
		std::istringstream iss(output);
		x = 0;
		while (std::getline(iss, output, ','))
		{
			//assert(y < chipnumY);
			//assert(x < chipnumX);
			mapData[y][x] = static_cast<char>(std::stoi(output));
			x++;
		}
		y++;
	}
	return true;
}

void MapChip::draw(int texno, char** map)
{
	const int DIV_X = static_cast<int>(scroll.x) / CHIP_SIZE; // division x
	const int DIV_Y = static_cast<int>(scroll.y) / CHIP_SIZE; // division y
	const int REM_X = static_cast<int>(scroll.x) % CHIP_SIZE; // remainder x
	const int REM_Y = static_cast<int>(scroll.y) % CHIP_SIZE; // remainder y

	texture::begin(texno);
	for (int y = 0; y < system::SCREEN_HEIGHT / CHIP_SIZE + 1; y++)
	{
		for (int x = 0; x < system::SCREEN_WIDTH / CHIP_SIZE + 1; x++)
		{
			if (DIV_X + x >= static_cast<int>(chipnumX) || DIV_Y + y >= static_cast<int>(chipnumY))
				continue; // 添え字の範囲チェック

			char chip = map[DIV_Y + y][DIV_X + x];
			if (chip < 0) continue;

			texture::draw(texno,
				static_cast<float>(x * CHIP_SIZE - REM_X),
				static_cast<float>(y * CHIP_SIZE - REM_Y),
				1, 1,
				static_cast<float>(chip % CHIP_NUM_PER_LINE * CHIP_SIZE),
				static_cast<float>(chip / CHIP_NUM_PER_LINE * CHIP_SIZE),
				static_cast<float>(CHIP_SIZE), static_cast<float>(CHIP_SIZE)
			);
		}
	}
	texture::end(texno);
}
void MapChip::drawchips()
{
	//draw(TEXNO::DUMMY, data);
}
MapChip::MapChip() : scroll(VECTOR2(0, 0)), data()
{
}

MapChip::~MapChip()
{
	uninit();
}
