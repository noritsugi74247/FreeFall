#ifndef INCLUDED_RESULT
#define	INCLUDED_RESULT
#include "scene.h"

class Result : public Scene
{
public:
    static Result* instance() { return &instance_; }

    void init();
    void update();
    void draw();
    void uninit();
private:
    static Result instance_;


public:


};
#endif