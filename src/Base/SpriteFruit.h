#ifndef SPRITEFRUIT_H
#define SPRITEFRUIT_H

#include "Sprite.h"

class SpriteFruit : public Sprite {
public:

    SpriteFruit(float x = 0.0f, float y = 0.0f, float speed = 1.0f);

    void moveDown();       
    void update() override; //sobrecarga da funcao update
};

#endif
