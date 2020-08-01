#include "result.h"
#include"audio.h"
#include"mylib.h"
#include"audio.h"
#include"game.h"

//------< using >---------------------------------------------------------------
using namespace mylib;

//------< �ϐ� >----------------------------------------------------------------
Result Result::instance_;


//void Result_spr_road();


void Result::init() {
    Scene::init();


    tutorial = false;
    fade = 5.0;
    wide = 0;
    posX = 0;
    audio_init();
}

//--------------------------------
//  �X�V����
//--------------------------------
void Result::update()
{
    using namespace input;

    switch (state)
    {
    case 0:
        //////// �����ݒ� ////////
        timer = 0;                                  // �^�C�}�[��������
        mylib::setBlendMode(blender::BS_ALPHA);   // �ʏ�̃A���t�@����
                                     // �����������̏I��

       /* Result_spr_road();*/
        state++;

        /*  music::play(0, true);*/
          //Game::instance()->set_tutorial(1);

        break;
    case 1:
        fade -= 1.0f / (float)10.0;

        if (fade <= 0)
        {


            fade = 0;
            state++;

        }

        break;
    case 2:
        //////// �ʏ펞�̏��� ////////



        timer++;                            // �^�C�}�[�𑫂�

        if (TRG(0) & PAD_TRG1)
        {
            // PAD_TRG1�������ꂽ��
        /*    sound::play(3);*/

            state++;  // �Q�[���V�[���ɐ؂�ւ�
        }
        break;
    case 3:
        fade += 1.0f / 10.0f;
        if (fade > 5.0)
        {
            changeScene(Game::instance());
            music::stop(0);
        }
        break;


    }

}

//--------------------------------
//  �`�揈��
//--------------------------------
void Result::draw()
{
    // ��ʃN���A
    mylib::clear(VECTOR4(0.2f, 0.3f, 1.0f, 1));

    if (fade > 0.0f)
    {
        primitive::rect(0, 0, system::SCREEN_WIDTH, system::SCREEN_HEIGHT,
            0, 0, 0, 0, 0, 0, fade);
    }


}

void Result::uninit()
{


    // �e�N�X�`���̉��
    /*audio_stop();*/
  /*  texture::releaseAll();*/

    // ���y�̃N���A
    /*  music::clear()*/;
}