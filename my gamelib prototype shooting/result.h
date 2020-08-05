#ifndef INCLUDED_RESULT
#define	INCLUDED_RESULT
#include "scene.h"
#include "fade.h"
class Result : public Scene
{
public:
    static Result* instance() { return &instance_; }

    void init();
    void update();
    void draw();
    void uninit();
    RFadeManager* rfadeManager() { return rfadeManager_; }

private:
    static Result instance_;
    RFadeManager* rfadeManager_;

public:


};

enum  RESULTSTATE
{
    RESILT_LOAD,
    RESILT_FADEOUT,
    RESILT_UPDATE,
    RESILT_FADEIN,
    CHANGE_GAME_SCENE,


};

#endif