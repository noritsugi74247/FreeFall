#include "mylib.h"
#include "sprite_data.h"
#include "obj2d_data.h"
#include "Obj2D.h"
#include "mylib.h"
#include "sprite.h"

//------< using >---------------------------------------------------------------
using namespace mylib;

//------< データ >---------------------------------------------------------------


sprite* sprTitle;
sprite* sprPushA;
sprite* sprINFO;
sprite* sprFUKIDASHI;
sprite* sprEnemy;
sprite* BG;
// 2D画像ロードデータ
LoadTexture loadTexture[] = {
    { TEXNO::PLAYER,    L"./Data/Images/hamums.png",    1U },// プレイヤー
    { TEXNO::CHIP,    L"./Data/Maps/map.png",540U },
    { TEXNO::WALL,    L"./Data/Images/wall.png",    256U },// プレイヤー
    {TEXNO::FADE,   L"./Data/Images/fadecircle .png",1U},
      { TEXNO::BOMB,    L"./Data/Images/bakuex.png",    256U },// プレイヤー
    { -1, nullptr }	// 終了フラグ
};



//------< マクロ >--------------------------------------------------------------
#define SPRITE_CENTER(texno,left,top,width,height)	{ (texno),(left),(top),(width),(height),(width)/2,(height)/2 }  // 画像の真ん中が中心
#define SPRITE_BOTTOM(texno,left,top,width,height)	{ (texno),(left),(top),(width),(height),(width)/2,(height)   }  // 画像の足元が中心

SpriteData sprPlayer_idle0 = SPRITE_BOTTOM(TEXNO::PLAYER, 96 * 0, 96 * 0, 96, 96);
SpriteData sprWall = SPRITE_BOTTOM(TEXNO::WALL, 96 * 0, 96 * 0, 960, 1088);
// マップチップのブロック（ブロックの実体生成に必要）
SpriteData sprterrain_block0  = SPRITE_CENTER(TEXNO::CHIP, 64 * 0, 64 * 0, 64, 64);
SpriteData sprterrain_block1  = SPRITE_CENTER(TEXNO::CHIP, 64 * 1, 64 * 0, 64, 64);
SpriteData sprterrain_block2  = SPRITE_CENTER(TEXNO::CHIP, 64 * 2, 64 * 0, 64, 64);
SpriteData sprterrain_block3  = SPRITE_CENTER(TEXNO::CHIP, 64 * 3, 64 * 0, 64, 64);
SpriteData sprterrain_block4  = SPRITE_CENTER(TEXNO::CHIP, 64 * 4, 64 * 0, 64, 64);

SpriteData sprterrain_block5  = SPRITE_CENTER(TEXNO::CHIP, 64 * 0, 64 * 1, 64, 64);
SpriteData sprterrain_block6  = SPRITE_CENTER(TEXNO::CHIP, 64 * 1, 64 * 1, 64, 64);
SpriteData sprterrain_block7  = SPRITE_CENTER(TEXNO::CHIP, 64 * 2, 64 * 1, 64, 64);
SpriteData sprterrain_block8  = SPRITE_CENTER(TEXNO::CHIP, 64 * 3, 64 * 1, 64, 64);
SpriteData sprterrain_block9  = SPRITE_CENTER(TEXNO::CHIP, 64 * 4, 64 * 1, 64, 64);

SpriteData sprterrain_block10 = SPRITE_CENTER(TEXNO::CHIP, 64 * 0, 64 * 2, 64, 64);
SpriteData sprterrain_block11 = SPRITE_CENTER(TEXNO::CHIP, 64 * 1, 64 * 2, 64, 64);
SpriteData sprterrain_block12 = SPRITE_CENTER(TEXNO::CHIP, 64 * 2, 64 * 2, 64, 64);
SpriteData sprterrain_block13 = SPRITE_CENTER(TEXNO::CHIP, 64 * 3, 64 * 2, 64, 64);
SpriteData sprterrain_block14 = SPRITE_CENTER(TEXNO::CHIP, 64 * 4, 64 * 2, 64, 64);

SpriteData sprterrain_block15 = SPRITE_CENTER(TEXNO::CHIP, 64 * 0, 64 * 3, 64, 64);
SpriteData sprterrain_block16 = SPRITE_CENTER(TEXNO::CHIP, 64 * 1, 64 * 3, 64, 64);
SpriteData sprterrain_block17 = SPRITE_CENTER(TEXNO::CHIP, 64 * 2, 64 * 3, 64, 64);
SpriteData sprterrain_block18 = SPRITE_CENTER(TEXNO::CHIP, 64 * 3, 64 * 3, 64, 64);
SpriteData sprterrain_block19 = SPRITE_CENTER(TEXNO::CHIP, 64 * 4, 64 * 3, 64, 64);

SpriteData sprterrain_block20 = SPRITE_CENTER(TEXNO::CHIP, 64 * 0, 64 * 4, 64, 64);
SpriteData sprterrain_block21 = SPRITE_CENTER(TEXNO::CHIP, 64 * 1, 64 * 4, 64, 64);
SpriteData sprterrain_block22 = SPRITE_CENTER(TEXNO::CHIP, 64 * 2, 64 * 4, 64, 64);
SpriteData sprterrain_block23 = SPRITE_CENTER(TEXNO::CHIP, 64 * 3, 64 * 4, 64, 64);
SpriteData sprterrain_block24 = SPRITE_CENTER(TEXNO::CHIP, 64 * 4, 64 * 4, 64, 64);

SpriteData sprterrain_block25 = SPRITE_CENTER(TEXNO::CHIP, 64 * 0, 64 * 5, 64, 64);
SpriteData sprterrain_block26 = SPRITE_CENTER(TEXNO::CHIP, 64 * 1, 64 * 5, 64, 64);
SpriteData sprterrain_block27 = SPRITE_CENTER(TEXNO::CHIP, 64 * 2, 64 * 5, 64, 64);
SpriteData sprterrain_block28 = SPRITE_CENTER(TEXNO::CHIP, 64 * 3, 64 * 5, 64, 64);
SpriteData sprterrain_block29 = SPRITE_CENTER(TEXNO::CHIP, 64 * 4, 64 * 5, 64, 64);

SpriteData sprterrain_block30 = SPRITE_CENTER(TEXNO::CHIP, 64 * 0, 64 * 6, 64, 64);
SpriteData sprterrain_block31 = SPRITE_CENTER(TEXNO::CHIP, 64 * 1, 64 * 6, 64, 64);
SpriteData sprterrain_block32 = SPRITE_CENTER(TEXNO::CHIP, 64 * 2, 64 * 6, 64, 64);
SpriteData sprterrain_block33 = SPRITE_CENTER(TEXNO::CHIP, 64 * 3, 64 * 6, 64, 64);
SpriteData sprterrain_block34 = SPRITE_CENTER(TEXNO::CHIP, 64 * 4, 64 * 6, 64, 64);

SpriteData sprterrain_block35 = SPRITE_CENTER(TEXNO::CHIP, 64 * 0, 64 * 7, 64, 64);

SpriteData sprfadecircle0 = SPRITE_BOTTOM(TEXNO::FADE, system::SCREEN_WIDTH * 0, system::SCREEN_HEIGHT * 0, 1920, 1080);
SpriteData sprfadecircle1 = SPRITE_BOTTOM(TEXNO::FADE, system::SCREEN_WIDTH * 1, system::SCREEN_HEIGHT * 0, 1920, 1080);
SpriteData sprfadecircle2 = SPRITE_BOTTOM(TEXNO::FADE, system::SCREEN_WIDTH * 2, system::SCREEN_HEIGHT * 0, 1920, 1080);
SpriteData sprfadecircle3 = SPRITE_BOTTOM(TEXNO::FADE, system::SCREEN_WIDTH * 3, system::SCREEN_HEIGHT * 0, 1920, 1080);
SpriteData sprfadecircle4 = SPRITE_BOTTOM(TEXNO::FADE, system::SCREEN_WIDTH * 4, system::SCREEN_HEIGHT * 0, 1920, 1080);

SpriteData sprfadecircle5 = SPRITE_BOTTOM(TEXNO::FADE, system::SCREEN_WIDTH * 5, system::SCREEN_HEIGHT * 0, 1920, 1080);
SpriteData sprfadecircle6 = SPRITE_BOTTOM(TEXNO::FADE, system::SCREEN_WIDTH * 6, system::SCREEN_HEIGHT * 0, 1920, 1080);

SpriteData sprbomb0 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 0, 128 * 0, 128, 128);
SpriteData sprbomb1 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 1, 128 * 0, 128,128);
SpriteData sprbomb2 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 2, 128 * 0, 128,128);
SpriteData sprbomb3 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 3, 128 * 0, 128,128);
SpriteData sprbomb4 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 0, 128 * 1, 128,128);
SpriteData sprbomb5 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 1, 128 * 1, 128,128);
SpriteData sprbomb6 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 2, 128 * 1, 128,128);
SpriteData sprbomb7 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 3, 128 * 1, 128,128);
SpriteData sprbomb8 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 0, 128 * 2, 128,128);
SpriteData sprbomb9 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 1, 128 * 2, 128,128);
SpriteData sprbomb10 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 2, 128 * 2, 128,128);
SpriteData sprbomb11 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 3, 128 * 2, 128,128);
SpriteData sprbomb12 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 0, 128 * 3, 128,128);
SpriteData sprbomb13 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 1, 128 * 3, 128,128);
SpriteData sprbomb14 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 2, 128 * 3, 128,128);
SpriteData sprbomb15 = SPRITE_BOTTOM(TEXNO::BOMB, 128 * 3, 128 * 3, 128,128);
