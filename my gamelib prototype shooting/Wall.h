
#ifndef INCLUDED_WALL
#define INCLUDED_WALL


//==============================================================================
//
//      �ړ��A���S���Y��
//
//==============================================================================

#include "Obj2D.h"
#include "obj2d_data.h"

// �v���C���[����N���X
class WallR : public MoveAlg
{
public:
    const float STAGE_HALFX = 700.0f;

    void move(OBJ2D* obj);

    static WallR* getInstance()
    {
        static WallR instance;
        return	&instance;
    }


private:
};

class WallL : public MoveAlg
{
public:
    const float STAGE_HALFX = 700.0f;

    void move(OBJ2D* obj);

    static WallL* getInstance()
    {
        static WallL instance;
        return	&instance;
    }


private:
};

// �ړ��A���S���Y���̎���
#define	pWallR	WallR::getInstance()
#define	pWallL	WallL::getInstance()
void playerJumpSet(OBJ2D*);


// �����A���S���Y��
class EraseWall : public EraseAlg
{
    void erase(OBJ2D* obj);
};

// �����A���S���Y���̎���
//EXTERN ErasePlayer      erasePlayer;


class WallManager : public OBJ2DManager
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