#include "Player.h"
#include "mylib.h"
#include "user.h"
#include "sprite_data.h"
using namespace mylib;

//******************************************************************************
//
//      �v���C���[�ړ�����
//
//******************************************************************************
//------< �v���C���[�̃A�j���f�[�^ >--------------------------------------------


const float stick = 0.125f;
//--------------------------------
//  Y�����ړ�
//--------------------------------
void Player::moveY(OBJ2D* obj)
{
    using namespace input;


    if (STATE(0) & PAD_UP || getPadState(0)->leftY < -stick)
    {
        obj->speed.y -= KASOKU;





    }

    else if (STATE(0) & PAD_DOWN || getPadState(0)->leftY > stick)
    { // �E������������Ă���ꍇ
        obj->speed.y += KASOKU;

    }
    // �ʏ펞
          // ���x�ɉ����x��������

    else
    {


        obj->speed.y = 0.0f;
    }

    // y�����̍R�͂̌v�Z
    //obj->speed.y += Game::instance()->bgManager()->calcResistance(obj, obj->speed.y);


    float maxSpeed = SPEED_MAX_Y;

    obj->speed.y = (std::max)(obj->speed.y, -maxSpeed);
    obj->speed.y = (std::min)(obj->speed.y, maxSpeed);
    obj->speed.y = clamp(obj->speed.y, -maxSpeed, maxSpeed);


    // �ʒu�X�V
    float oldY = obj->position.y;           // �ړ��O�̈ʒu��ێ�
    obj->position.y += obj->speed.y;
    float deltaY = obj->position.y - oldY;  // �ړ���̈ʒu����ړ��O�̈ʒu������

    obj->onGround = false;





}

//--------------------------------
//  X�����ړ�
//--------------------------------
void Player::moveX(OBJ2D* obj)
{
    using namespace input;

    // ���E���͂̎��o��

    if (STATE(0) & PAD_LEFT || getPadState(0)->leftX < -stick)
    {
        obj->speed.x -= KASOKU;





    }

    else if (STATE(0) & PAD_RIGHT || getPadState(0)->leftX > stick)
    { // �E������������Ă���ꍇ
        obj->speed.x += KASOKU;

    }

    else
    {


        obj->speed.x = 0.0f;
    }

    float maxSpeedx = SPEED_MAX_Y;
    // X�����ړ�
    obj->speed.x = (std::max)(obj->speed.x, -maxSpeedx);
    obj->speed.x = (std::min)(obj->speed.x, maxSpeedx);
    obj->speed.x = clamp(obj->speed.x, -maxSpeedx, maxSpeedx);


    float oldX = obj->position.x;
    obj->position.x += obj->speed.x;
    float deltaX = obj->position.x - oldX;
}

//--------------------------------
//  �v���C���[�W�����v����
//--------------------------------

//--------------------------------
//  �v���C���[�G���A����
//--------------------------------
void Player::areaCheck(OBJ2D* obj)
{
    float oldX = obj->position.x;   // x���W���ꎞ�I�ɕۑ�
    obj->position.x = clamp(obj->position.x, obj->size.x, system::SCREEN_WIDTH - obj->size.x);
    if (oldX != obj->position.x)
    {
        obj->speed.x = 0;  // �ۑ������l�ƈႦ�Ή�ʒ[�ɂ������Ă���
    }
    float oldY = obj->position.y;   // y���W���ꎞ�I�ɕۑ�
    obj->position.y = clamp(obj->position.y, obj->size.y, system::SCREEN_HEIGHT);
    if (oldY != obj->position.y)    // �ۑ������l�ƈႦ�Ή�ʒ[�ɂ������Ă���
    {
        obj->speed.y = 0;           // y�����̑��x��0�ɂ���

    }
}

//--------------------------------
//  �v���C���[�̂͂�������
//--------------------------------

void playerJumpSet(OBJ2D* obj)
{
    obj->speed.y = -15.0f;
    obj->jumpTimer = 16;
}

//--------------------------------
//  �v���C���[�ʏ펞�̍s��
//--------------------------------
void Player::move(OBJ2D* obj)
{
    using namespace input;  // �֐����œ��͏������s���Ƃ��ɋL�q����



    switch (obj->state)
    {
    case 0:
        //////// �����ݒ� ////////

        // �A�j���̏����ݒ�

        obj->data = &sprPlayer_idle0;
        obj->xFlip = true;                                 // �T�C�Y�ݒ�i���������S�ł��邽�߁A���͂�����Ƃ��Ďg�p���锼���E�c�͂��̂܂܂������₷���j
        obj->size = VECTOR2(48 / 2, 96 - 2);
        obj->scale.x = -1;
        //obj->isDrawHitRect = true;
        obj->judgeFlag = true;
     
        obj->state++;

        break;

    case 1:
        //////// �ʏ펞 ////////


        // �v���C���[�c�����̈ړ�����


        moveY(obj);

        // �v���C���[�������̈ړ�����
        moveX(obj);


        // �v���C���[�̃G���A�`�F�b�N
        areaCheck(obj);

        break;
    }

    // �A�j���X�V

}

//--------------------------------
//  ����
//--------------------------------
void ErasePlayer::erase(OBJ2D* obj)
{

    obj->clear();           // OBJ2D����������
}