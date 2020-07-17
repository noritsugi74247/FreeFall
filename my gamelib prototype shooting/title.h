#ifndef INCLUDED_TITLE
#define	INCLUDED_TITLE
#include "scene.h"

class Title : public Scene
{
public:
    static Title* instance() { return &instance_; }

    void init();
    void update();
    void draw();
    void uninit();
private:
    static Title instance_;


public:


};
#endif