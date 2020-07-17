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
// 2D画像ロードデータ
LoadTexture loadTexture[] = {
    { TEXNO::PLAYER,    L"./Data/Images/hamums.png",    1U },// プレイヤー
    { -1, nullptr }	// 終了フラグ
};



//------< マクロ >--------------------------------------------------------------
#define SPRITE_CENTER(texno,left,top,width,height)	{ (texno),(left),(top),(width),(height),(width)/2,(height)/2 }  // 画像の真ん中が中心
#define SPRITE_BOTTOM(texno,left,top,width,height)	{ (texno),(left),(top),(width),(height),(width)/2,(height)   }  // 画像の足元が中心

SpriteData sprPlayer_idle0 = SPRITE_BOTTOM(TEXNO::PLAYER, 96 * 0, 96 * 0, 96, 96);