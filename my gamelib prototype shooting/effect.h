#pragma once
#include "Obj2D.h"
#include "obj2d_data.h"
class Effectparticle : public MoveAlg
{
public:
    const float STAGE_HALFX = 700.0f;

    void move(OBJ2D* obj);


    static Effectparticle* getInstance()
    {
        static Effectparticle instance;
        return	&instance;
    }


private:
};

#define	instanceEffect	Effectparticle::getInstance()
class EffectparticleManager : public OBJ2DManager
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
    void draw() override;    // �`��
};