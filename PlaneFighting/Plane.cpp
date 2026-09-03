#include "Plane.h"

Plane::Plane()
{
    this->mTimerExplosion=new QTimer();
}
Plane::~Plane()
{
    delete this->mTimerExplosion;
}
