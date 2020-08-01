#include "Wall.h"
#include "Player.h"
#include "mylib.h"
#include "user.h"
#include "sprite_data.h"
using namespace mylib;


VECTOR2 vec;
VECTOR2 vec2;
VECTOR2 vecOld;
float NewposX;
bool right;
bool left;
float goalR;
float goalL;

void WallR::move(OBJ2D* obj)
{
    using namespace input;  // 関数内で入力処理を行うときに記述する



    switch (obj->state)
    {
    case 0:
        //////// 初期設定 ////////

        // アニメの初期設定

        obj->data = &sprWall;
        obj->size = VECTOR2(48 / 2, 96 - 2);
        obj->scale.x = 1.0f;
        obj->scale.y = 1.0f;
        //obj->isDrawHitRect = true;
        obj->judgeFlag = true;
        NewposX = 1920 / 2;
        right = false;
        obj->state++;

        break;

    case 1:
        //////// 通常時 ////////


        if (TRG(0) & PAD_TRG1&&!right
            ||
            TRG(0) & PAD_TRG2 && !right
            ||
            TRG(0) & PAD_TRG3 && !right
            ||
            TRG(0) & PAD_TRG4 && !right
            )
        {
            right = true;
            goalR = obj->position.x - 192;
            obj->state++;
          
        }
        break;

    case 2:
        if (right)
        {
            obj->position.x -= 10;
            if (goalR >= obj->position.x)
            {
                obj->position.x = goalR;
                right = false;
                OBJ2D::damage++;
                obj->state = 1;
            }

        }


        break;



    }
}


void WallL::move(OBJ2D* obj)
{
    using namespace input;  // 関数内で入力処理を行うときに記述する



    switch (obj->state)
    {
    case 0:
        //////// 初期設定 ////////

        // アニメの初期設定

        obj->data = &sprWall;
        obj->size = VECTOR2(48 / 2, 96 - 2);
        obj->scale.x = 1.0f;
        obj->scale.y = 1.0f;
        //obj->isDrawHitRect = true;
        obj->judgeFlag = true;
        left = false;
        NewposX = 1920 / 2;
        obj->state++;

        break;

    case 1:
        //////// 通常時 ////////


        if (TRG(0) & PAD_TRG1
            ||
            TRG(0) & PAD_TRG2
            ||
            TRG(0) & PAD_TRG3
            ||
            TRG(0) & PAD_TRG4
            )
        {
            left = true;
            goalL = obj->position.x + 192;
            obj->state++;

        }

      


        break;


    case 2:

        if (left)
        {
            obj->position.x += 10;
            if (goalL <= obj->position.x)
            {
                obj->position.x = goalL;
                left = false;
                obj->state = 1;
            }

        }


        break;

    }
}