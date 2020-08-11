#include "effect.h"
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
        { nullptr, -1 },// �I���t���O
    };





}

void Effectparticle::move(OBJ2D* obj)
{
    switch (obj->state)
    {
    case 0:
        //////// �����ݒ� ////////

        // �A�j���̏����ݒ�
       
         obj->animeData = animeparticle;
         
        // �����l�Ƃ��ĉ������̃A�j����ݒ肷��

        obj->xFlip = false;                                 // �T�C�Y�ݒ�i���������S�ł��邽�߁A���͂�����Ƃ��Ďg�p���锼���E�c�͂��̂܂܂������₷���j
        obj->size = VECTOR2(24 / 2, 96 - 2);

        //obj->isDrawHitRect = true;


        obj->state++;

        break;

    case 1:
        obj->position = Game::instance()->playerManager()->getList()->begin()->position;
        obj->animeUpdate();

        if ((obj->animeData + 1)->frame < 0)     // �I���R�[�h�̂Ƃ�
        {
            // �擪�֖߂�
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

