#ifndef INCLUDED_GAME
#define	INCLUDED_GAME

#include "Player.h"
#include "block.h"
#include "scene.h"
#include "map.h"
#include "Wall.h"
//#include <memory>
//==============================================================================
//
//      Game�N���X
//
//==============================================================================

class Game : public Scene
{
    int game_time;
    int dawn_time;
    int flg_count;

    int count;
public:
    // �N���X���ł̒萔�̐錾�̎d��
    // int�^�ł����const�ŗǂ����A����ȊO��constexpr���g�p����
    //static const world_wide = ;

    //static constexpr float GROUND_POS_Y = 400.0f;


    bool stop_flg[10];
    bool description;

    int get_gametime() { return game_time; }
    void countup_flg_count() { flg_count++; }
    int get_flg_count() { return flg_count; }

  /*  void count_fire() {
        count = 0;
        for (auto fires : *Game::instance()->objectManager()->getList()) {
            if (fires.iWork[0] == 1) { count++; }
        }
    }

    bool clear_judeg() {
        if (count == 0) { return true; }
        return false;
    }*/


public:
    static Game* instance() { return &instance_; }


    void init();
    void update();
    void draw();
    void uninit();
    //todo:s
    // �Q�b�^�[�͌���_�����Ȃ�
 /*   Player_manager* PL_manager() { return PL_manager_; }*/
    PlayerManager* playerManager() { return playerManager_; }
    BlockManager* blockManager() { return blockManager_; }
    MapChip* bgManager() { return bgManager_.get(); }
    WallManager* wallManager() { return wallManager_; }
private:


    // �����o�ϐ��͌���_������
    PlayerManager* playerManager_;
    BlockManager* blockManager_;
    WallManager* wallManager_;
    std::unique_ptr<MapChip> bgManager_;
    static Game instance_;
};

class Sine {

public:

    static float easeIn(float t, float b, float c, float d);
    static float easeOut(float t, float b, float c, float d);
    static float easeInOut(float t, float b, float c, float d);

};


//******************************************************************************

#endif // !INCLUDED_GAME