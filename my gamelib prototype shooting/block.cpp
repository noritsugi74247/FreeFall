#include "block.h"
#include "mylib.h"
#include "user.h"
#include "sprite_data.h"
#include "game.h"

using namespace mylib;
Block block;
EraseBlock eraseblock;
void Block::moveX(OBJ2D* obj)
{
	obj->position.x += obj->speed.x;
	if (obj->position.x > 640)
		obj->speed.x = -5;
	if (obj->position.x < 0)
		obj->speed.x = 5;
}

void Block::move(OBJ2D* obj)
{
	switch (obj->state)
	{
	case 0:
		obj->data = &sprterrain_block2;
		obj->color = { 1,1,1,1 };
		obj->size = { 64,64 };
		obj->speed = { 5,0 };
		obj->scale = { 1,1 };
		obj->judgeFlag = true;
		obj->state++;
		break;
	case 1:
		//moveX(obj);
		break;
	case 2:
		obj->eraseAlg = &eraseblock;
	}
}

void BlockManager::init(const char* filename)
{
	MoveAlg* dummy = {}, *dummy2 = {};
	OBJ2DManager::init();
	MoveAlg* blocktype[] =
	{
		dummy,
		dummy2,
		&block,
	};
	char** blockdata;
	u_int chipNumX = Game::instance()->bgManager()->chipnumX;
	u_int chipNumY = Game::instance()->bgManager()->chipnumY;
	if (!getCSVSize(filename, &chipNumX, &chipNumY))
	{
		assert("ブロック用CSVファイルの読み込みに失敗");
	}
	blockdata = new char* [chipNumY];

	for (u_int i = 0; i < chipNumY; i++)
	{
		blockdata[i] = new char[chipNumX];
	}

	if (!Game::instance()->bgManager()->loadMapData(filename, blockdata))
	{
		assert(!"ブロックデータの読み込みに失敗");
	}
	for (u_int y = 0; y < chipNumY; y++)
	{
		for (u_int x = 0; x < chipNumX; x++)
		{
			char index = blockdata[y][x];
			if (index < 0) continue;
			add(blocktype[index],
				VECTOR2(
					static_cast<float>(x * CHIP_SIZE + CHIP_SIZE / 2),
					static_cast<float>(y * CHIP_SIZE + CHIP_SIZE / 2))
			);
		}
	}

	for (uint32_t t = 0; t < chipNumY; t++)
	{
		delete[] blockdata[t];
		blockdata[t] = nullptr;
	}
	delete[] blockdata;
	blockdata = nullptr;
}

void EraseBlock::erase(OBJ2D* obj)
{
	obj->mvAlg = nullptr;
}