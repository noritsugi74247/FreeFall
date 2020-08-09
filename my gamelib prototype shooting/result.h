#ifndef INCLUDED_RESULT
#define	INCLUDED_RESULT
#include "scene.h"
#include "fade.h"
class Result : public Scene
{
public:
    static Result* instance() 
    { 
        static Result instance_;
        return &instance_; 
    }
    RFadeManager* rfadeManager() { return rfadeManager_; }
    void init();
    void update();
    void draw();
    void uninit();
private:

    RFadeManager* rfadeManager_;

public:


};

enum  RESULTSTATE
{
    RESULT_LOAD,
    RESULT_FADEOUT,
    RESULT_UPDATE,
    RESULT_FADEIN,
    CHANGE_GAME_SCENE,


};
#endif