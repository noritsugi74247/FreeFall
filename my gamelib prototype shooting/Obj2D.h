#ifndef INCLUDED_OBJ2D
#define	INCLUDED_OBJ2D
//******************************************************************************
//
//
//      OBJ2D�N���X
//
//
//******************************************************************************

//------< �C���N���[�h >---------------------------------------------------------
#include <list>
#include "Vector.h"
#include "obj2d_data.h"

using namespace mylib;
// �O���錾
class OBJ2D;

//==============================================================================

// �ړ��A���S���Y���N���X�i���ۃN���X�j
class MoveAlg
{
public:
    virtual void move(OBJ2D* obj) = 0;  // �������z�֐������̂ŁAMoveAlg�^�̃I�u�W�F�N�g�͐錾�ł��Ȃ��i�h�������Ďg���j
};

// �����A���S���Y���N���X�i���ۃN���X�j
class EraseAlg
{
public:
    virtual void erase(OBJ2D* obj) = 0; // ��Ɠ��l
};

//==============================================================================

//==============================================================================
//
//      OBJ2D�N���X
//
//==============================================================================

class OBJ2D
{
public:
    // �����o�ϐ�
    mylib::SpriteData* data;               // �X�v���C�g�f�[�^
    mylib::Anime          anime;              // �A�j���[�V�����N���X
    mylib::AnimeData* animeData;          // �A�j���[�V�����f�[�^

    bool                    onGround;           // �n�ʃt���O�i�n��ɂ���j
    bool                    xFlip;              // x�������]�t���O
    bool                    kabeFlag;           // �ǃt���O�i����悪�ǁj
    bool                    gakeFlag;           // �R�t���O�i����悪�R�j

    VECTOR2                 position;           // �ʒu
    VECTOR2                 scale;              // �X�P�[��
    float                   angle;              // �p�x
    VECTOR4                 color;              // �`��F
    VECTOR2                 size;               // ������p�T�C�Y�i�c���j

    mylib::fRECT          hitRect = {};           // �����蔻��RECT
    MoveAlg* mvAlg;              // �ړ��A���S���Y��
    EraseAlg* eraseAlg;           // �����A���S���Y��

    float                   velocity;           // ���x
    VECTOR2                 speed;              // �u�Ԃ̈ړ��ʃx�N�g��
    int                     state;              // �X�e�[�g
    int                     timer;              // �^�C�}�[
    int               damageCount;              // �_���[�W�J�E���g

    int                     score;              // �X�R�A
    int                     param;              // �ėp�p�����[�^
    int                     jumpTimer;          // �������W�����v�^�C�}�[
    int                     flags;              // �e��t���O�p(�g������player.h��player�N���X���Q��)
    int                     iWork[16];          // �ėp�i�g�����͎��R�j
    float                   fWork[16];          // �ėp�i�g�����͎��R�j

    static int              damage;

    bool                    hashigoFlag;        // �͂����t���O
    bool                    drawHitRectFlag;    // �����蔻��`��t���O
    bool                    judgeFlag;          // �����蔻��̗L���itrue:�L�� / false:�����j
    BYTE                    pad[1];             // 1�o�C�g�̋�
public:

    OBJ2D();        // �R���X�g���N�^
    void clear();   // �����o�ϐ��̃N���A
    void move();    // �ړ�
    void draw();    // �`��

    void hoseirect();

    bool animeUpdate();    // �A�j���[�V�����̃A�b�v�f�[�g
};

//==============================================================================

// OBJ2DManager�N���X
class OBJ2DManager
{
protected:
    std::list<OBJ2D>  objList; // ���X�g�iOBJ2D�̔z��̂悤�Ȃ��́j
public:
    // �f�X�g���N�^
    ~OBJ2DManager();

   virtual void init();    // ������
   virtual void update();  // �X�V
   virtual void draw();    // �`��

    OBJ2D* add(MoveAlg* mvAlg, const VECTOR2& pos = VECTOR2(0, 0)); // objList�ɐV����OBJ2D��ǉ�����
    std::list<OBJ2D>* getList() { return &objList; }                // objList���擾����
};




//******************************************************************************

#endif// ! INCLUDED_OBJ2D