#include "fade.h"
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

    AnimeData animefade[] = {
        { &sprfadecircle0, 10 },
        { &sprfadecircle1, 6 },
        { &sprfadecircle2, 6 },
        { &sprfadecircle3, 6 },
        { &sprfadecircle4, 6 },
        { &sprfadecircle5, 6 },
        { &sprfadecircle6, 6 },
        { nullptr, -1 },// 終了フラグ
    };

    AnimeData animefadeback[] = {
        { &sprfadecircle6, 6 },
        { &sprfadecircle5, 6 },
        { &sprfadecircle4, 6 },
        { &sprfadecircle3, 6 },
        { &sprfadecircle2, 6 },
        { &sprfadecircle1, 6 },
        { &sprfadecircle0, 6 },
        { nullptr, -1 },// 終了フラグ
    };
   


}
void Fade::move(OBJ2D* obj)
{
    switch (obj->state)
    {
    case 0:
        //////// 初期設定 ////////
     
        // アニメの初期設定
        switch (Game::instance()->state)
        {
        case GAMESTATE::FADEOUT:
            //breakthrough
        case GAMESTATE::TITLE_FADEOUT:
            obj->animeData = animefade;
            break;
        case GAMESTATE::FADEIN:
        case GAMESTATE::TITLE_FADEIN:
            obj->animeData = animefadeback;
            break;

        }
         // 初期値として下向きのアニメを設定する

        obj->xFlip = false;                                 // サイズ設定（足元が中心であるため、幅はあたりとして使用する半分・縦はそのままが扱いやすい）
        obj->size = VECTOR2(24 / 2, 96 - 2);

        //obj->isDrawHitRect = true;
  
       
        obj->state++;

        break;

    case 1:
        obj->animeUpdate();

        if ((obj->animeData + 1)->frame < 0)     // 終了コードのとき
        {
                          // 先頭へ戻る
            obj->state++;
        }
       

        break;

    case 2:
        Game::instance()->state++;
        obj->mvAlg = nullptr;

        break;
    }

    // アニメ更新
 
}


void RFade::move(OBJ2D* obj)
{
    switch (obj->state)
    {
    case 0:
        //////// 初期設定 ////////

        // アニメの初期設定
        switch (Result::instance()->state)
        {
        case RESULTSTATE::RESULT_FADEOUT:
            //breakthrough
       
            obj->animeData = animefade;
            break;
        case RESULTSTATE::RESULT_FADEIN:
      
            obj->animeData = animefadeback;
            break;

        }
        // 初期値として下向きのアニメを設定する

        obj->xFlip = false;                                 // サイズ設定（足元が中心であるため、幅はあたりとして使用する半分・縦はそのままが扱いやすい）
        obj->size = VECTOR2(24 / 2, 96 - 2);

        //obj->isDrawHitRect = true;


        obj->state++;

        break;

    case 1:
        obj->animeUpdate();

        if ((obj->animeData + 1)->frame < 0)     // 終了コードのとき
        {
            // 先頭へ戻る
            obj->state++;
        }


        break;

    case 2:
        Result::instance()->state++;
        obj->mvAlg = nullptr;

        break;
    }

    // アニメ更新

}


