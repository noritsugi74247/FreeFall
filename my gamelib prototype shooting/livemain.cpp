#include "mylib.h"
#include "scene.h"
#include "game.h"
#include "title.h"


int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)// �g�p���Ȃ��ϐ��͋L�q���Ȃ�
{
    SceneManager sceneManager;
    sceneManager.execute(Title::instance());

    return 0;
}