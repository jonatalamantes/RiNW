#include "Scroll.h"

///Constructor
Scroll::Scroll()
{
    Scroll::maximoScrollX = 0;
    Scroll::actualScrollX = 0;
    Scroll::maximoScrollY = 0;
    Scroll::actualScrollY = 0;
}

///Getters
float Scroll::getMaximoScrollX()
{
    return Scroll::maximoScrollX;
}

float Scroll::getActualScrollX()
{
    return Scroll::actualScrollX;
}

float Scroll::getMaximoScrollY()
{
    return Scroll::maximoScrollY;
}

float Scroll::getActualScrollY()
{
    return Scroll::actualScrollY;
}

///Setters
void Scroll::setMaximoScrollX(float x)
{
    Scroll::maximoScrollX = x;
}

void Scroll::setActualScrollX(float x)
{
    Scroll::actualScrollX = x;
}

void Scroll::setMaximoScrollY(float x)
{
    Scroll::maximoScrollY = x;
}

void Scroll::setActualScrollY(float x)
{
    Scroll::actualScrollY = x;
}
