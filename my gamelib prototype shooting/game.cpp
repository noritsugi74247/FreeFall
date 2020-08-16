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
    fadeManager_ = new FadeManager;
    effectparticleManager_ = new EffectparticleManager;
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
    using std::chrono::system_clock;
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
    case GAMESTATE::LOAD:
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
    case GAMESTATE::INIT:

        playerManager()->init();

        playerManager()->add(pPlayer, VECTOR2(50, 640));
        
        // bgManager()->init("./Data/Maps/stage1.csv");
        
        blockManager()->init("./Data/Maps/test.csv");
        effectparticleManager()->init();

        wallManager()->init();

        wallManager()->add(pWallL, VECTOR2(-960 / 2, 1080));
        wallManager()->add(pWallR, VECTOR2(1920 + (960 / 2), 1080));

        fadeManager()->init();
        fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
        
        //ctime�ŁBclock_t = int �̑���͂ł���݂����B(��������clock_t �� long��typedef����...)
        //start = end = time = 0;

        start = end = system_clock::now(); // �J�n�E�I�����Ɍ��݂̎��Ԃ��擾���^���I�ɏ������B
        elapsed_time = 0;    // elapsed_time �� double�^�Ȃ̂�int�� 0 �ŏ������ł���B
        state++;
        break;
    case GAMESTATE::TITLE_FADEOUT:

        timer++;


        
        playerManager()->update();
        fadeManager()->update();
        effectparticleManager()->update();
        break;

    case GAMESTATE::TITLE:
        playerManager()->update();
        effectparticleManager()->update();

        if (TRG(0) & PAD_TRG1
            ||
            TRG(0) & PAD_TRG2
            ||
            TRG(0) & PAD_TRG3
            ||
            TRG(0) & PAD_TRG4
            )
        {
            fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
            timer = 0;
            state++;
        }


        break;

    case GAMESTATE::TITLE_FADEIN:
        fadeManager()->update();


        break;

    case GAMESTATE::STAY:
        timer++;

        if (timer >= CHANGE_FRAME * 2)
        {

            state++;
            fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
            timer = 0;
        }

        break;



    case GAMESTATE::FADEOUT:
        timer++;
        fadeManager()->update();
        // ctime�ŁBstart��0�ȉ��Ȃ猻�ݎ��Ԃ��擾�B
      /*  if(start <= 0 )
            start = clock();*/
        start = system_clock::now(); // ���ݎ��Ԃ��擾����start�ɑ������B
        break;

    case GAMESTATE::UPDATE:

        //�@�����蔻��̗L����
        judge();
       
        wallManager()->update();


        playerManager()->update();

        blockManager()->update();

        bgManager()->update();

        boostjudge();

        effectparticleManager()->update();

        if (OBJ2D::damage >= 5)
        {
            state++;
        }

        break;

    case GAMESTATE::FADEINSTAY:
       /* if (end <= 0)
        {
            // ctime�ŁBend�Ɍ��ݎ��Ԃ��擾���đ���B
            //time�ɂ�end����start������������(�o�ߎ���)��������B
            
            end = clock();
            time = end - start;

            // time�ɂ̓~���b(1/1000�b)�P�ʂŐ��l�������Ă��邽�߁A
            // CLOCKS_PER_SEC �ŕb�P�ʂɕϊ�����B
            // (�Ⴆ�΁A8.301�b���o�߂����Ƃ�time�ɂ� "8301"�Ƃ������l�������Ă���)
            // (CLOCKS_PER_SEC �� #define�� 1000 �ƒ�`����Ă���)

            time /= CLOCKS_PER_SEC; 
        }*/
        if (elapsed_time <= 0) // elapsed_time�������X�V����ĂȂ�������c
        {
            end = system_clock::now(); // ���ݎ��Ԃ��擾����end�ɑ���B
            // �o�ߎ��Ԃ��}�C�N���b�ɕϊ�(����1�b�P�ʂɕϊ����Ă��܂��Ə����_�ȉ������Ȃ��Ȃ邽��)
            elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            elapsed_time /= MICRO_TO_SECOND; // �}�C�N���b����1�b�P�ʂɕϊ�����(100���Ŋ����1�b�P�ʂɕϊ��ł���)

            // ����ȍ~�Aelapsed_time�͎��̃X�e�[�W�ȂǂɈڍs����܂ŏ��0�ȊO�̐��l�������Ă��邽�ߍX�V����Ȃ�
            // (�Œ�ł�180�t���[���͂��̃u���b�N�ɂ���̂ŁA���̊ԉߏ�Ɏ��Ԃ��v�������̂�h������)
        }
        timer++;

        if (timer >= CHANGE_FRAME * 3)
        {

            state++;
            fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
        }
       

        break;


    case GAMESTATE::FADEIN:
        fadeManager()->update();


        break;

    case  GAMESTATE::CHANGE_SCENE:
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
     // �v���C���[�̕`��
    if (state >= GAMESTATE::TITLE_FADEOUT)
    {
        sprite_render(BG, 0, 0);
    }

    if (state == GAMESTATE::TITLE_FADEOUT || state == GAMESTATE::TITLE)
    {
        sprite_render(sprTitle, 300, 300);

    }

    wallManager()->draw();

    bgManager()->drawchips();

    blockManager()->draw();

    effectparticleManager()->draw();

    playerManager()->draw();


    fadeManager()->draw();

    if (state == GAMESTATE::INIT || state == GAMESTATE::STAY || state == GAMESTATE::TITLE_FADEOUT && timer < 10
        || state == GAMESTATE::FADEOUT && timer < 10 || state == GAMESTATE::CHANGE_SCENE)
    {
        primitive::rect(0, 0, system::SCREEN_WIDTH, system::SCREEN_HEIGHT,
            0, 0, 0, 0, 0, 0, 1);
    }

    //�ז���������ŃR�����g
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();


    ImGui::DragFloat("fade", &fade);
    if (state == GAMESTATE::UPDATE)
    {
        ImGui::DragFloat("player.speed.x", &(playerManager()->getList()->begin()->speed.x));
        ImGui::DragFloat("player.speed.y", &(playerManager()->getList()->begin()->speed.y));
        ImGui::DragInt("score", &(playerManager()->getList()->begin()->score));
    }
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

   
}

//--------------------------------
//  �I������
//--------------------------------
void Game::uninit()
{
    // �e�}�l�[�W���̉��
    safe_delete(playerManager_);
    safe_delete(blockManager_);
    safe_delete(fadeManager_);
    safe_delete(effectparticleManager_);
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
