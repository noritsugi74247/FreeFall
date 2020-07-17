#include "audio.h"
#include "mylib.h"
using namespace mylib;

void audio_init()
{
  
}

void audio_uninit()
{
    int i;
    for (i = 0; i < MUSIC_FILE_MAX; i++)
        music::unload(i);
}

void audio_stop()
{
    int i;
    for (i = 0; i < MUSIC_FILE_MAX; i++)
    {
        music::stop(i);
    }


}