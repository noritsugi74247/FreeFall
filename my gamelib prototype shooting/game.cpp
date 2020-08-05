//#include "all.h"
//
#include"result.h"
#include"sprite_data.h"
#include"mylib.h"
#include"game.h"
#include"Player.h"
#include"imgui.h"





//------< using >---------------------------------------------------------------
using namespace mylib;

//------< �ϐ� >----------------------------------------------------------------
Game Game::instance_;

//void game_spr_road();

extern VECTOR2 mouse_pos;


//--------------------------------
//  ����������
//--------------------------------
void Game::init()
{

    Scene::init();	    // ���N���X��init���Ă�
    playerManager_ = new PlayerManager;
    wallManager_ = new WallManager;
    fadeManager_ = new FadeManager;
    OBJ2D::damage = 0;
    flg_count = 0;
    count = 0;
    fade = 5.0;
  
    
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
        sprite_load(&sprTitle, L"./Data/Images/title.png",1U);
        sprite_load(&BG, L"./Data/Images/bg.png", 1U);
   

        mylib::setBlendMode(blender::BS_ALPHA);   // �ʏ�̃A���t�@����


       /* music::play(1, true);*/

        

        state++;
        break;
    case GAMESTATE::INIT:
      
        playerManager()->init();
        wallManager()->init();
        
        playerManager()->add(pPlayer, VECTOR2(system::SCREEN_WIDTH/2,300 ));
        wallManager()->add(pWallL, VECTOR2(-960/2, 1080));
        wallManager()->add(pWallR, VECTOR2(1920+(960/2), 1080));
        fadeManager()->init();
        fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
        timer++;
        if (timer >= 60)
        {

            state++;
            timer = 0;
        }
        
   
        break;
    case GAMESTATE::TITLE_FADEOUT:
        timer++;


        fadeManager()->update();
        playerManager()->update();

        break;
    case GAMESTATE::TITLE:
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

          if (timer >= 60)
          {
              fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
              state++;
              timer = 0;
          }

          break;



    case GAMESTATE::FADEOUT:
        timer++;
        fadeManager()->update();
          break;

    case GAMESTATE::UPDATE:
          playerManager()->update();
          wallManager()->update();
       

          if (OBJ2D::damage>=5)
          {
              state++;
          }

          break;
   
    case GAMESTATE::FADEINSTAY:
          timer++;

          if (timer >= 120)
          {
              
              state++;
              fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
          }

          break;


    case GAMESTATE::FADEIN:
        fadeManager()->update();

          break;

    case GAMESTATE::CHANGE_SCENE:
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
    mylib::clear(VECTOR4(0, 0, 0, 1));
    // �v���C���[�̕`��
    if (state>=GAMESTATE::TITLE_FADEOUT)
    {
        sprite_render(BG, 0, 0);
    }

    if (state == GAMESTATE::TITLE_FADEOUT||state==GAMESTATE::TITLE)
    {
        sprite_render(sprTitle, 300, 300);

    }

    wallManager()->draw();
    playerManager()->draw();
    fadeManager()->draw();

    if (state == GAMESTATE::INIT|| state == GAMESTATE::STAY||state==GAMESTATE::TITLE_FADEOUT&&timer<10 
        || state == GAMESTATE::FADEOUT && timer < 10||state==GAMESTATE::CHANGE_SCENE)
    {
        primitive::rect(0, 0, system::SCREEN_WIDTH, system::SCREEN_HEIGHT,
            0, 0, 0, 0, 0, 0, 1);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    

    ImGui::DragFloat("fade", &fade);

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    
      

}

//--------------------------------
//  �I������
//--------------------------------
void Game::uninit()
{
    // �e�}�l�[�W���̉��
   
    // �e�N�X�`���̉��
    texture::releaseAll();
    safe_delete(playerManager_);
    safe_delete(wallManager_);
    safe_delete(fadeManager_);
    // ���y�̃N���A
    music::clear();
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
