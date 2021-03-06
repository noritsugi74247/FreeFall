#include "texture.h"
#include "mylib.h"

#include "user.h"
#include "Obj2D.h"

enum TEXNO
{
    // �Q�[��
    PLAYER,
    CHIP,
    DUMMY,
    WALL,
    FADE,
    BOMB,
};

extern mylib::LoadTexture loadTexture[];

extern mylib::SpriteData sprPlayer_idle0;

extern mylib::SpriteData sprWall;

extern sprite* sprTitle;
extern sprite* BG;

extern mylib::SpriteData sprterrain_block0;
extern mylib::SpriteData sprterrain_block1;
extern mylib::SpriteData sprterrain_block2;
extern mylib::SpriteData sprterrain_block3;
extern mylib::SpriteData sprterrain_block4;
extern mylib::SpriteData sprterrain_block5;
extern mylib::SpriteData sprterrain_block6;
extern mylib::SpriteData sprterrain_block7;
extern mylib::SpriteData sprterrain_block8;
extern mylib::SpriteData sprterrain_block9;
extern mylib::SpriteData sprterrain_block10;
extern mylib::SpriteData sprterrain_block11;
extern mylib::SpriteData sprterrain_block12;
extern mylib::SpriteData sprterrain_block13;
extern mylib::SpriteData sprterrain_block14;
extern mylib::SpriteData sprterrain_block15;
extern mylib::SpriteData sprterrain_block16;
extern mylib::SpriteData sprterrain_block17;
extern mylib::SpriteData sprterrain_block18;
extern mylib::SpriteData sprterrain_block19;
extern mylib::SpriteData sprterrain_block20;
extern mylib::SpriteData sprterrain_block21;
extern mylib::SpriteData sprterrain_block22;
extern mylib::SpriteData sprterrain_block23;
extern mylib::SpriteData sprterrain_block24;
extern mylib::SpriteData sprterrain_block25;
extern mylib::SpriteData sprterrain_block26;
extern mylib::SpriteData sprterrain_block27;
extern mylib::SpriteData sprterrain_block28;
extern mylib::SpriteData sprterrain_block29;
extern mylib::SpriteData sprterrain_block30;
extern mylib::SpriteData sprterrain_block31;
extern mylib::SpriteData sprterrain_block32;
extern mylib::SpriteData sprterrain_block33;
extern mylib::SpriteData sprterrain_block34;
extern mylib::SpriteData sprterrain_block35;


extern mylib::SpriteData sprfadecircle0;
extern mylib::SpriteData sprfadecircle1;
extern mylib::SpriteData sprfadecircle2;
extern mylib::SpriteData sprfadecircle3;
extern mylib::SpriteData sprfadecircle4;

extern mylib::SpriteData sprfadecircle5;
extern mylib::SpriteData sprfadecircle6;


extern mylib::SpriteData sprbomb0;
extern mylib::SpriteData sprbomb1;
extern mylib::SpriteData sprbomb2;
extern mylib::SpriteData sprbomb3;
extern mylib::SpriteData sprbomb4;
extern mylib::SpriteData sprbomb5;
extern mylib::SpriteData sprbomb6;
extern mylib::SpriteData sprbomb7;
extern mylib::SpriteData sprbomb8;
extern mylib::SpriteData sprbomb9;
extern mylib::SpriteData sprbomb10;
extern mylib::SpriteData sprbomb11;
extern mylib::SpriteData sprbomb12;
extern mylib::SpriteData sprbomb13;
extern mylib::SpriteData sprbomb14;
extern mylib::SpriteData sprbomb15;