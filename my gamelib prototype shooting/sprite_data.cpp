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
// 2D画像ロードデータ
LoadTexture loadTexture[] = {
    { TEXNO::PLAYER,    L"./Data/Images/hamums.png",    1U },// プレイヤー
    { TEXNO::CHIP,    L"./Data/Maps/map.png",540U },
    { TEXNO::DUMMY,    L"./Data/Images/SICアセット終/枠.png",500U},
    { -1, nullptr }	// 終了フラグ
};



//------< マクロ >--------------------------------------------------------------
#define SPRITE_CENTER(texno,left,top,width,height)	{ (texno),(left),(top),(width),(height),(width)/2,(height)/2 }  // 画像の真ん中が中心
#define SPRITE_BOTTOM(texno,left,top,width,height)	{ (texno),(left),(top),(width),(height),(width)/2,(height)   }  // 画像の足元が中心

SpriteData sprPlayer_idle0 = SPRITE_BOTTOM(TEXNO::PLAYER, 96 * 0, 96 * 0, 96, 96);

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
