#include "mylib.h"
#include "sprite_data.h"
#include "obj2d_data.h"
#include "Obj2D.h"
#include "mylib.h"
#include "sprite.h"
//------< using >---------------------------------------------------------------
using namespace mylib;

//------< �f�[�^ >---------------------------------------------------------------


sprite* sprTitle;
sprite* sprPushA;
sprite* sprINFO;
sprite* sprFUKIDASHI;
// 2D�摜���[�h�f�[�^
LoadTexture loadTexture[] = {
    { TEXNO::PLAYER,    L"./Data/Images/hamums.png",    1U },// �v���C���[
    { -1, nullptr }	// �I���t���O
};



//------< �}�N�� >--------------------------------------------------------------
#define SPRITE_CENTER(texno,left,top,width,height)	{ (texno),(left),(top),(width),(height),(width)/2,(height)/2 }  // �摜�̐^�񒆂����S
#define SPRITE_BOTTOM(texno,left,top,width,height)	{ (texno),(left),(top),(width),(height),(width)/2,(height)   }  // �摜�̑��������S

SpriteData sprPlayer_idle0 = SPRITE_BOTTOM(TEXNO::PLAYER, 96 * 0, 96 * 0, 96, 96);