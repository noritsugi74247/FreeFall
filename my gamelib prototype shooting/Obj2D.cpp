#include "mylib.h"
#include "Obj2D.h"
#include "game.h"
using namespace mylib;

//--------------------------------
//  �R���X�g���N�^
//--------------------------------
OBJ2D::OBJ2D()
{
    clear();

    scale = VECTOR2(1, 1);          // �������Y�ꂪ�����悤�Ɂi�X�P�[���[���Ȃ牽���\������Ȃ��j
    color = VECTOR4(1, 1, 1, 1);    // �������Y�ꂪ�����悤�Ɂi�s�����x�[���Ȃ牽���\������Ȃ��j
}

//--------------------------------
//  �����o�ϐ��̃N���A
//--------------------------------
void OBJ2D::clear()
{
    data = nullptr;
    SecureZeroMemory(&anime, sizeof(anime));

    position = VECTOR2(0, 0);
    scale = VECTOR2(0, 0);
    angle = 0.0f;
    color = VECTOR4(0, 0, 0, 0);
    size = VECTOR2(0, 0);

    mvAlg = nullptr;
    eraseAlg = nullptr;

    velocity = 0.0f;
    speed = VECTOR2(0, 0);
    state = 0;
    timer = 0;

    param = 0;
    jumpTimer = 0;

    SecureZeroMemory(iWork, sizeof(iWork));
    SecureZeroMemory(fWork, sizeof(fWork));

    hashigoFlag = false;
    drawHitRectFlag = false;
    judgeFlag = false;
    pad[0] = 0;
}

//--------------------------------
//  �ړ�
//--------------------------------
void OBJ2D::move()
{
    if (mvAlg) mvAlg->move(this);                   // �ړ��A���S���Y�������݂���΁Amove���\�b�h���Ă�
    if (eraseAlg) eraseAlg->erase(this);            // �����A���S���Y�������݂���΁Aerase���\�b�h���Ă�
}

//--------------------------------
//  �`��
//--------------------------------
void OBJ2D::draw()
{
    if (data)                                       // OBJ2D��data�����o�ɃX�v���C�g�f�[�^�������
    {
        data->draw(position, scale, angle, color);  // data��draw���\�b�h�ŃX�v���C�g��`�悷��
    }
}

void OBJ2D::hoseirect()
{
    hitRect.right = position.x + size.x;
    hitRect.left = position.x - size.x;
    hitRect.top = position.y - size.y;
    hitRect.bottom = position.y;

}

//--------------------------------
//  �A�j���[�V�����X�V
//--------------------------------
//  �߂�l�Ftrue  �A�j�����擪�ɖ߂�u��
//        :false ����ȊO
//--------------------------------
bool OBJ2D::animeUpdate()
{
    if (animeData == nullptr) assert(!"animeUpdate�֐���AnimeData��nullptr");

    if (anime.pPrev != animeData)           // �A�j���f�[�^���؂�ւ�����Ƃ�
    {
        anime.pPrev = animeData;
        anime.patNum = 0;	                // �擪����Đ�
        anime.frame = 0;
    }

    animeData += anime.patNum;
    data = animeData->data;                 // ���݂̃p�^�[���ԍ��ɊY������摜��ݒ�

    anime.frame++;
    if (anime.frame >= animeData->frame)    // �ݒ�t���[�����\��������
    {
        anime.frame = 0;
        anime.patNum++;                     // ���̃p�^�[����
        if ((animeData + 1)->frame < 0)     // �I���R�[�h�̂Ƃ�
        {
            anime.patNum = 0;               // �擪�֖߂�
            return true;
        }
    }

    return false;
}

//******************************************************************************
//
//
//      OBJ2DManager�N���X
//
//
//******************************************************************************

//--------------------------------
//  ������
//--------------------------------
void OBJ2DManager::init()
{
    //���X�g��OBJ2D��S�ăN���A����
    objList.clear();
}

//--------------------------------
//  ���X�g�֒ǉ�
//--------------------------------
OBJ2D* OBJ2DManager::add(MoveAlg* mvAlg, const VECTOR2& pos)
{
    OBJ2D obj;                          // OBJ2D��錾����
    obj.mvAlg = mvAlg;                  // mvAlg�Ɉ�����mvalg����
    obj.position = pos;                 // position�Ɉ�����pos����
    obj.scale = VECTOR2(1, 1);          // �X�P�[���͓��{
    obj.color = VECTOR4(1, 1, 1, 1);    // �F�͔��i�s�����x1.0f�j

    objList.push_back(obj);             // ���X�g��obj��ǉ�����
    return &(*objList.rbegin());        // ���ǉ�����obj�̃A�h���X��Ԃ��i�����Ŏg����悤�Ɂj
}

//--------------------------------
//  �X�V
//--------------------------------
void OBJ2DManager::update()
{
    // �ړ�
    for (auto& it : objList)            // objList�̑S�Ă̗v�f�����[�v���Ait�Ƃ������O�Ŋe�v�f�ɃA�N�Z�X
    {
        it.move();                      // it��move���\�b�h���Ă�
    }

    // ��m�[�h�̍폜�i���͋C�ɂ��Ȃ��ėǂ��j
    auto it = objList.begin();
    while (it != objList.end())
    {
        if (it->mvAlg)
        {
            it++;
        }
        else
        {
            it = objList.erase(it);
        }
    }
}


//--------------------------------
//  �`��
//--------------------------------
void OBJ2DManager::draw()
{
    for (auto& it : objList)            // objList�̑S�Ă̗v�f�����[�v���Ait�Ƃ������O�Ŋe�v�f�ɃA�N�Z�X 
    {
        it.draw();                      // it��draw���\�b�h���Ă�
    }
}

//--------------------------------
//  �f�X�g���N�^
//--------------------------------
OBJ2DManager::~OBJ2DManager()
{
    //���X�g��OBJ2D��S�ăN���A����
    objList.clear();
}
