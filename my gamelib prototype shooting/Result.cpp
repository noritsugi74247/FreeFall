#include "result.h"
#include"audio.h"
#include"mylib.h"
#include"sprite_data.h"
#include"audio.h"
#include"game.h"

//------< using >---------------------------------------------------------------
using namespace mylib;

void Result::init() {
    Scene::init();


    tutorial = false;
    fade = 5.0;
    wide = 0;
    posX = 0;
    rfadeManager_ = new RFadeManager;
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
    case RESULTSTATE::RESILT_LOAD:
        //////// �����ݒ� ////////
        timer = 0;
        texture::load(loadTexture);
        // �^�C�}�[��������
        mylib::setBlendMode(blender::BS_ALPHA);   // �ʏ�̃A���t�@����
                                     // �����������̏I��
        rfadeManager()->init();
        rfadeManager()->add(pRFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
        /* Result_spr_road();*/
        state++;

        /*  music::play(0, true);*/
          //Game::instance()->set_tutorial(1);

        break;
    case RESULTSTATE::RESILT_FADEOUT:
        timer++;


        rfadeManager()->update();

        break;
    case RESULTSTATE::RESILT_UPDATE:
        //////// �ʏ펞�̏��� ////////



        timer++;                            // �^�C�}�[�𑫂�

        if (TRG(0) & PAD_TRG1)
        {
            // PAD_TRG1�������ꂽ��
        /*    sound::play(3);*/
            rfadeManager()->add(pRFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
            state++;  // �Q�[���V�[���ɐ؂�ւ�
        }
        break;
    case RESULTSTATE::RESILT_FADEIN:



        rfadeManager()->update();
        break;
    case RESULTSTATE::CHANGE_GAME_SCENE:

        changeScene(Game::instance());

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



    rfadeManager()->draw();

    if (state == RESULTSTATE::RESILT_LOAD || state == RESULTSTATE::RESILT_FADEOUT && timer < 10
        || state == RESULTSTATE::CHANGE_GAME_SCENE)
    {
        primitive::rect(0, 0, system::SCREEN_WIDTH, system::SCREEN_HEIGHT,
            0, 0, 0, 0, 0, 0, 1);
    }


}

void Result::uninit()
{


    // �e�N�X�`���̉��
    /*audio_stop();*/
    texture::releaseAll();

    safe_delete(rfadeManager_);

    // ���y�̃N���A
    /*  music::clear()*/;
}