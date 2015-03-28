#include "Animation.h"
#include "../Cube.h"
#include "../Random.h"
//the maximum of active raindrops
#define MAX_RAINDROPS 20
#define SPAWN_RATE 20 //its percent so 20% chance to spawn a not active raindrop
#define UPDATE_TIME_RAINDROP 500 //in ms


class RainAnimation : public Animation
{
private:
    struct RainDrop
    {
        uint8_t x, y, z, r, b, g;
        bool active;
    };

    Cube *cube;
    unsigned long time_taken;
    RainDrop raindrops[MAX_RAINDROPS];
public:
    RainAnimation(Cube *c);
    void update(const short &delta);
};

RainAnimation::RainAnimation(Cube *c) : cube(c), time_taken(0)
{
    for (int i = 0; i < MAX_RAINDROPS; i++)
    {
        raindrops[i].active = false;
    }
}

void RainAnimation::update(const short &delta)
{
    time_taken += delta;
    if(time_taken > UPDATE_TIME_RAINDROP)
    {
        time_taken = 0;
        for (int i = 0; i < MAX_RAINDROPS; i++)
        {
            if (raindrops[i].active)
            {
                if (raindrops[i].z > 0 && raindrops[i].active)
                {
                    //reset old spot
                    cube->setRGB(raindrops[i].x, raindrops[i].y, raindrops[i].z, 0, 0, 0);
                    //update to new spot
                    raindrops[i].z--;
                    cube->setRGB(raindrops[i].x, raindrops[i].y, raindrops[i].z, raindrops[i].r,
                                 raindrops[i].g, raindrops[i].b);
                }
                else if (raindrops[i].z == 0 && raindrops[i].active)
                {
                    //if position is on the bottom shut the led down
                    raindrops[i].active = false;
                    cube->setRGB(raindrops[i].x, raindrops[i].y, raindrops[i].z, 0, 0, 0);
                }
            }
            else
            {
                //if not active generate a new spot to start;
                if (rnd() % 100 > SPAWN_RATE)
                {
                    raindrops[i].x = rnd() % 5;
                    raindrops[i].y = rnd() % 5;
                    raindrops[i].z = 4;
                    raindrops[i].r = rnd() % MAX_COLOR;
                    raindrops[i].g = rnd() % MAX_COLOR;
                    raindrops[i].b = rnd() % MAX_COLOR;
                    raindrops[i].active = true;
                    //dont forget to set the led ;)
                    cube->setRGB(raindrops[i].x, raindrops[i].y, raindrops[i].z, raindrops[i].r,
                                 raindrops[i].g, raindrops[i].b);
                }
            }
        }
    }

}