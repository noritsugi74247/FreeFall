#ifndef INCLUDED_PLAYER
#define INCLUDED_PLAYER

//******************************************************************************
//
//
//      Player.h
//
//
//******************************************************************************

//==============================================================================
//
//      �ړ��A���S���Y��
//
//==============================================================================

#include "Obj2D.h"
#include "obj2d_data.h"
#include "Singleton.h"
// �v���C���[����N���X
class Player : public MoveAlg
{
public:
    static constexpr float KASOKU = 0.5f;         // �������E�c�����̉����x
    static constexpr float SPEED_MAX_X = 4.0f;         // �������̍ő呬�x
    static constexpr float SPEED_MAX_Y = 8.0f;         // �ő�̗������x
    static constexpr float SPEED_JUMP_Y = -6.0f;        // �W�����v���̏㏸�X�s�[�h

    static constexpr float HASHIGO_KASOKU = 0.2f;       // �͂������~���̉����x
    static constexpr float HASHIGO_SPEED_MAX_X = 4.0f;  // �������̍ő呬�x
    static constexpr float HASHIGO_SPEED_MAX_Y = 2.0f;  // �ő�̗������x

    void move(OBJ2D* obj);

    static Player* getInstance()
    {
        static Player instance;
        return	&instance;
    }


private:

    //TODO_02
    void moveY(OBJ2D*);
    void moveX(OBJ2D*);
    void areaCheck(OBJ2D*);
    ;
};

// �ړ��A���S���Y���̎���
#define	pPlayer	Player::getInstance()
void playerJumpSet(OBJ2D*);


// �����A���S���Y��
class ErasePlayer : public EraseAlg
{
    void erase(OBJ2D* obj);
};

// �����A���S���Y���̎���
//EXTERN ErasePlayer      erasePlayer;


class PlayerManager : public OBJ2DManager
{
public:
    static constexpr float PLAYER_MOVE = 4.0f;
    enum IWORK {        // �����ނł�15����t�Ɏg���Ă����i0���珇�ԂɍD���ȗp�r�Ŏg���Ă悢�j
        PAD_STATE = 13, // 13 �L�[�̏�ԕێ��p
        PAD_TRG,        // 14 �L�[�̃g���K�[�ێ��p
        ATTACK_TIMER,   // 15 �U���\�ɂȂ�܂ł̃^�C�}�[
    };
public:
    //OBJ2DManager�N���X�̂��̂��Ăяo�����Ηǂ��̂ŁA���L�I�[�o�[���C�h�͕s�v�ł���

    //void init();    // ������
    //void update();  // �X�V
    //void draw();    // �`��
};

//------< ���[�N�p >-------------------------------------------------------------

#endif // !INCLUDED_PLAYER
