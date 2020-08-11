#include "block.h"
#include "mylib.h"
#include "user.h"
#include "sprite_data.h"
#include "game.h"

using namespace mylib;
Block block;
EraseBlock eraseblock;

void Block::move(OBJ2D* obj)
{
	switch (obj->state)
	{
	case 0:
		obj->color = { 1,1,1,1 };
		obj->size = { 64,64 };
		obj->speed = { 5,0 };
		obj->scale = { 1,1 };
		obj->judgeFlag = true;
		obj->state++;
		break;
	case 1:

		break;
	case 2:
		obj->eraseAlg = &eraseblock;
	}
}



void BlockManager::init(const char* filename)
{
	OBJ2DManager::init();
	SpriteData* blocktype[] =
	{
		&sprterrain_block0,
		&sprterrain_block1,
		&sprterrain_block2,
		&sprterrain_block3,
		&sprterrain_block4,
		&sprterrain_block5,
		&sprterrain_block6,
		&sprterrain_block7,
		&sprterrain_block8,
		&sprterrain_block9,
		&sprterrain_block10,
		&sprterrain_block11,
		&sprterrain_block12,
		&sprterrain_block13,
		&sprterrain_block14,
		&sprterrain_block15,
		&sprterrain_block16,
		&sprterrain_block17,
		&sprterrain_block18,
		&sprterrain_block19,
		&sprterrain_block20,
		&sprterrain_block21,
		&sprterrain_block22,
		&sprterrain_block23,
		&sprterrain_block24,
		&sprterrain_block25,
		&sprterrain_block26,
		&sprterrain_block27,
		&sprterrain_block28,
		&sprterrain_block29,
		&sprterrain_block30,
		&sprterrain_block31,
		&sprterrain_block32,
		&sprterrain_block33,
		&sprterrain_block34,
		&sprterrain_block35,
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
			add(&block,
				VECTOR2(static_cast<float>(x * CHIP_SIZE + CHIP_SIZE / 2),
					static_cast<float>(y * CHIP_SIZE + CHIP_SIZE / 2)))->data = blocktype[index];
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

void BlockManager::draw()
{
	for (auto& it : objList)            // objListの全ての要素をループし、itという名前で各要素にアクセス 
	{
		if (it.data)                                       // OBJ2Dのdataメンバにスプライトデータがあれば
		{
			it.data->draw(it.position - Game::instance()->bgManager()->getScrollPos(), it.scale, it.angle, it.color);  // dataのdrawメソッドでスプライトを描画する
		}
	}
}

void EraseBlock::erase(OBJ2D* obj)
{
	obj->mvAlg = nullptr;
}