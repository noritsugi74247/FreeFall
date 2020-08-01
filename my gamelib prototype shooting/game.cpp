//#include "all.h"
//
#include "sprite_data.h"
#include "mylib.h"
//#include "Player.h"
//#include "enemy.h"
#include "title.h"
#include"result.h"
#include "game.h"
#include "judge.h"
#include"imgui.h"



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
    wallManager_ = new WallManager;
    bgManager_ = std::make_unique<MapChip>();
    OBJ2D::damage = 0;

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
        sprite_load(&sprTitle, L"./Data/Images/title.png", 1U);
        sprite_load(&BG, L"./Data/Images/bg.png", 1U);
        mylib::setBlendMode(blender::BS_ALPHA);   // �ʏ�̃A���t�@����


       /* music::play(1, true);*/

        

        state++;
        break;
    case 1:

        playerManager()->init();

        playerManager()->add(pPlayer, VECTOR2(50, 640));
        
        bgManager()->init("./Data/Maps/test.csv"); 
        
        blockManager()->init("./Data/Maps/test.csv");

        wallManager()->init();

        wallManager()->add(pWallL, VECTOR2(-960 / 2, 1080));
        wallManager()->add(pWallR, VECTOR2(1920 + (960 / 2), 1080));
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
        playerManager()->update();

        if (TRG(0) & PAD_TRG1
            ||
            TRG(0) & PAD_TRG2
            ||
            TRG(0) & PAD_TRG3
            ||
            TRG(0) & PAD_TRG4
            )
        {
            state++;
        }


        break;

    case 4:
        fade += 1.0f / 10.0f;

        if (fade >= 1)
        {
            fade = 1;
            state++;
        }

        break;

    case 5:
        timer++;

        if (timer >= 120)
        {

            state++;
            timer = 0;
        }

        break;



    case 6:

        fade -= 1.0f / 10.0f;

        if (fade <= 0)
        {
            fade = 0;
            state++;
        }
        break;

    case 7:

        //�@�����蔻��̗L����
        judge();
       
        wallManager()->update();


        playerManager()->update();

        blockManager()->update();

        bgManager()->update();

        boostjudge();

        if (OBJ2D::damage >= 5)
        {
            state++;
        }

        break;

    case 8:
        timer++;

        if (timer >= 180)
        {

            state++;
        }

        break;


    case 9:
        fade += 1.0f / 10.0f;

        if (fade >= 1)
        {
            fade = 1;
            state++;
        }

        break;

    case 10:
        Scene::changeScene(Result::instance());

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
    sprite_render(BG, 0, 0);
    if (state <= 4)
    {
        sprite_render(sprTitle, 300, 300);
    }

    wallManager()->draw();

    bgManager()->drawchips();

    blockManager()->draw();

    playerManager()->draw();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();


    ImGui::DragFloat("fade", &fade);

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

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
