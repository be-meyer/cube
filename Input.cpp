/*
* Input.cpp
*
* Created: 15.03.2015 19:18:34
* Author: Benjamin
*/


#include "Input.h"
// default constructor
Input::Input() : enc_delta(0), last(0)
{
    //make the INC as inputs
    INC_DDR &= ~((1 << INC_PHASE1_PIN) | (1 << INC_PHASE2_PIN) |
                 (1 << INC_TASER_PIN));

    //enable pullups
    INC_PORT |= ((1 << INC_PHASE1_PIN) | (1 << INC_PHASE2_PIN) |
                 (1 << INC_TASER_PIN));
    //first init
    int8_t newv;

    newv = 0;
    if( PHASE1 )
        newv = 3;
    if( PHASE2 )
        newv ^= 1;                   // convert gray to binary
    last = newv;                   // power on state
} //Input

// default destructor
Input::~Input()
{
} //~Input

void Input::update()
{
    int8_t newv, diff;
    newv = 0;
    if( PHASE1 )
        newv = 3;
    if( PHASE2 )
        newv ^= 1;                   // convert gray to binary

    diff = last - newv;                // difference last - new
    if( diff & 1 )                // bit 0 = value (1)
    {
        this->last = newv;                 // store new as next last
        this->enc_delta += (diff & 2) - 1;        // bit 1 = direction (+/-)
    }
}

bool Input::isPressed()
{
    return !(INC_PIN & (1 << INC_TASER_PIN)) ? true : false;
}

int8_t Input::getIncDelta()
{
    return this->enc_delta;
}