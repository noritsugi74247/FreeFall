#include "scene.h"
#include "mylib.h"
Scene* Scene::execute()
{
    using namespace mylib;

    // ����������
    init();

    // �Q�[�����[�v
    while (mylib::gameLoop(false))    // false��true�ɂ���ƃ^�C�g���o�[�Ƀt���[�����[�g�\��
    {
        // ���͏���
        input::update();


        // �X�V����
        update();

        // �`�揈��
        draw();

        // �f�o�b�O������̕`��
       /* debug::display(1, 0, 0, 1, 1);*/
        //debug::setString("GP2_03");

        // ��ʃt���b�v
        mylib::present(1, 0);

        // �I���`�F�b�N
        if (nextScene) break;
    }

    // �I������
    uninit();

    return nextScene;	// ���̃V�[����Ԃ�
}

//******************************************************************************
//
//      SceneManager�N���X
//
//******************************************************************************

//--------------------------------
//  ���s
//--------------------------------
void SceneManager::execute(Scene* scene)
{
    using namespace mylib;

    bool isFullScreen = true;	// �t���X�N���[���ɂ���Ȃ�true�ɕς���
                                //�iRelease���[�h�̂݁j

    // �Q�[�����C�u�����̏���������
    mylib::init(L"shooting",
        static_cast<int>(system::SCREEN_WIDTH),
        static_cast<int>(system::SCREEN_HEIGHT),
        isFullScreen);

    //#ifndef _DEBUG
    ShowCursor(1/*!isFullScreen*/);	// �t���X�N���[�����̓J�[�\��������
//#endif

    // ���C�����[�v
    while (scene)
    {
        scene = scene->execute();
    }

    // �Q�[�����C�u�����̏I������
    mylib::uninit();
}