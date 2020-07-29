//#include "all.h"
//
#include "sprite_data.h"
#include "mylib.h"
//#include "Player.h"
//#include "enemy.h"
#include "title.h"
#include "game.h"
#include "judge.h"



//------< using >---------------------------------------------------------------
using namespace mylib;

//------< �ϐ� >----------------------------------------------------------------
Game Game::instance_;

//void game_spr_road();

extern VECTOR2 mouse_pos;
//extern Block block;

//--------------------------------
//  ����������
//--------------------------------
void Game::init()
{

    Scene::init();	    // ���N���X��init���Ă�
    playerManager_ = new PlayerManager;
    blockManager_ = new BlockManager;
    bgManager_ = std::make_unique<MapChip>();

    flg_count = 0;
    count = 0;
    fade = 5.0;
    wide = Title::instance()->wide;
    posX = Title::instance()->posX;
    
    description = false;   // �|�[�Y�t���O�̏�����
    for (int i = 0; i < 10; i++) {
        stop_flg[i] = { false };
    }
}

//--------------------------------
//  �X�V����
//--------------------------------
void Game::update()
{

    using namespace input;

    // �\�t�g���Z�b�g
    if ((STATE(0) & PAD_SELECT) &&  // 0�R���̃Z���N�g�{�^����������Ă����Ԃ�
        (TRG(0) & PAD_START))       // 0�R���̃X�^�[�g�{�^���������ꂽ�u��
    {
        changeScene(Title::instance());   // �^�C�g���V�[���ɐ؂�ւ���
        return;
    }

    // �f�o�b�O������\��
  /*  debug::setString("state:%d", state);
    debug::setString("timer:%d", timer);*/

    // �|�[�Y����
    //if (TRG(0) & PAD_START)
        //isPaused = !isPaused;       // 0�R���̃X�^�[�g�{�^���������ꂽ��|�[�Y��Ԃ����]
    //if (description) return;           // ���̎��_�Ń|�[�Y���Ȃ烊�^�[��

    switch (state)
    {
    case 0:
        //////// �����ݒ� ////////
        //if (!tutorial) { state = 4; }
        //else { state++; }
       // �e�N�X�`���̓ǂݍ���
        texture::load(loadTexture);

        timer = 0;
        mylib::setBlendMode(blender::BS_ALPHA);   // �ʏ�̃A���t�@����


       /* music::play(1, true);*/

        

        state++;
        break;
    case 1:

        playerManager()->init();

        playerManager()->add(pPlayer, VECTOR2(50, 640));
        
        bgManager()->init("./Data/Maps/test.csv"); 
        
        blockManager()->init("./Data/Maps/test.csv");
        state++;
        break;
    case 2:

        fade -= 1.0f/10.0f;

        if (fade <= 0)
        {
            fade = 0;
            state++;
        }
        break;
      case 3:

          //�@�����蔻��̗L����
          judge();

          playerManager()->update();

          blockManager()->update();

          bgManager()->update();
       
          boostjudge();
        break;
   

    }
}

//--------------------------------
//  �`�揈��
//--------------------------------
void Game::draw()
{
    // ��ʃN���A
    mylib::clear(VECTOR4(1, 0, 0, 1));
    // �v���C���[�̕`��
    bgManager()->drawchips();

    blockManager()->draw();

    playerManager()->draw();

    if (fade > 0.0f)
    {
        primitive::rect(0, 0, system::SCREEN_WIDTH, system::SCREEN_HEIGHT,
            0, 0, 0, 0, 0, 0, fade);
    }
}

//--------------------------------
//  �I������
//--------------------------------
void Game::uninit()
{
    // �e�}�l�[�W���̉��
    safe_delete(playerManager_);
    safe_delete(blockManager_);
    // �e�N�X�`���̉��
    texture::releaseAll();
    // ���y�̃N���A
    /*music::clear();*/
}

float Sine::easeIn(float t, float b, float c, float d) {
    // t: ����(�i�s�x)
    // b : �J�n�̒l(�J�n���̍��W��X�P�[���Ȃ�)
    // c : �J�n�ƏI���̒l�̍���
    // d : Tween(�g�D�C�[��)�̍��v����
    return -c * cosf(t / d * (DirectX::XM_PI / 2)) + c + b;
}

float Sine::easeOut(float t, float b, float c, float d) {
    return c * sinf(t / d * (DirectX::XM_PI / 2)) + b;
}

float Sine::easeInOut(float t, float b, float c, float d) {
    return -c / 2 * (cosf(DirectX::XM_PI * t / d) - 1) + b;
}
