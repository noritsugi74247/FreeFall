#include "fade.h"
#include "mylib.h"
#include "user.h"
#include "sprite_data.h"
#include "game.h"
#include"result.h"
using namespace mylib;
namespace
{   // �����̃f�[�^�͒����̂ŁAVisual Studio�̋@�\�ŕ�����悤��namespace�𕪂��Ă���

    //------< �v���C���[�̃A�j���f�[�^ >------------------------------------------
    //�����

    AnimeData animefade[] = {
        { &sprfadecircle0, 10 },
        { &sprfadecircle1, 6 },
        { &sprfadecircle2, 6 },
        { &sprfadecircle3, 6 },
        { &sprfadecircle4, 6 },
        { &sprfadecircle5, 6 },
        { &sprfadecircle6, 6 },
        { nullptr, -1 },// �I���t���O
    };

    AnimeData animefadeback[] = {
        { &sprfadecircle6, 6 },
        { &sprfadecircle5, 6 },
        { &sprfadecircle4, 6 },
        { &sprfadecircle3, 6 },
        { &sprfadecircle2, 6 },
        { &sprfadecircle1, 6 },
        { &sprfadecircle0, 6 },
        { nullptr, -1 },// �I���t���O
    };
   


}
void Fade::move(OBJ2D* obj)
{
    switch (obj->state)
    {
    case 0:
        //////// �����ݒ� ////////
     
        // �A�j���̏����ݒ�
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
         // �����l�Ƃ��ĉ������̃A�j����ݒ肷��

        obj->xFlip = false;                                 // �T�C�Y�ݒ�i���������S�ł��邽�߁A���͂�����Ƃ��Ďg�p���锼���E�c�͂��̂܂܂������₷���j
        obj->size = VECTOR2(24 / 2, 96 - 2);

        //obj->isDrawHitRect = true;
  
       
        obj->state++;

        break;

    case 1:
        obj->animeUpdate();

        if ((obj->animeData + 1)->frame < 0)     // �I���R�[�h�̂Ƃ�
        {
                          // �擪�֖߂�
            obj->state++;
        }
       

        break;

    case 2:
        Game::instance()->state++;
        obj->mvAlg = nullptr;

        break;
    }

    // �A�j���X�V
 
}


void RFade::move(OBJ2D* obj)
{
    switch (obj->state)
    {
    case 0:
        //////// �����ݒ� ////////

        // �A�j���̏����ݒ�
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
        // �����l�Ƃ��ĉ������̃A�j����ݒ肷��

        obj->xFlip = false;                                 // �T�C�Y�ݒ�i���������S�ł��邽�߁A���͂�����Ƃ��Ďg�p���锼���E�c�͂��̂܂܂������₷���j
        obj->size = VECTOR2(24 / 2, 96 - 2);

        //obj->isDrawHitRect = true;


        obj->state++;

        break;

    case 1:
        obj->animeUpdate();

        if ((obj->animeData + 1)->frame < 0)     // �I���R�[�h�̂Ƃ�
        {
            // �擪�֖߂�
            obj->state++;
        }


        break;

    case 2:
        Result::instance()->state++;
        obj->mvAlg = nullptr;

        break;
    }

    // �A�j���X�V

}


