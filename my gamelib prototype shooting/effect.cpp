#include "effect.h"
#include "mylib.h"
#include "user.h"
#include "sprite_data.h"
#include "game.h"
#include"result.h"
using namespace mylib;
namespace
{   // ※このデータは長いので、Visual Studioの機能で閉じられるようにnamespaceを分けている

    //------< プレイヤーのアニメデータ >------------------------------------------
    //上方向

    AnimeData animeparticle[] = {
        { &sprbomb0, 2 },
        { &sprbomb1, 2 },
        { &sprbomb2, 2 },
        { &sprbomb3, 2 },
        { &sprbomb4, 2 },
        { &sprbomb5, 2 },
        { &sprbomb6, 2 },
        { &sprbomb7, 2 },
        { &sprbomb8, 2 },
        { &sprbomb9, 2 },
        { &sprbomb10, 2 },
        { &sprbomb11, 2 },
        { &sprbomb12, 2 },
        { &sprbomb13, 2 },
        { &sprbomb14, 2 },
        { &sprbomb15, 2 },
        { nullptr, -1 },// 終了フラグ
    };





}

void Effectparticle::move(OBJ2D* obj)
{
    switch (obj->state)
    {
    case 0:
        //////// 初期設定 ////////

        // アニメの初期設定
       
         obj->animeData = animeparticle;
         
        // 初期値として下向きのアニメを設定する

        obj->xFlip = false;                                 // サイズ設定（足元が中心であるため、幅はあたりとして使用する半分・縦はそのままが扱いやすい）
        obj->size = VECTOR2(24 / 2, 96 - 2);

        //obj->isDrawHitRect = true;


        obj->state++;

        break;

    case 1:
        obj->position = Game::instance()->playerManager()->getList()->begin()->position;
        obj->animeUpdate();

        if ((obj->animeData + 1)->frame < 0)     // 終了コードのとき
        {
            // 先頭へ戻る
            obj->state++;
        }


        break;

    case 2:
        obj->mvAlg = nullptr;

        break;
    }
}

void EffectparticleManager::draw()
{

        for (auto& ef : objList)
        {
            if (ef.data)
            {
                ef.data->draw(ef.position - Game::instance()->bgManager()->getScrollPos(), ef.scale, ef.angle, ef.color);
            }
        }
}

