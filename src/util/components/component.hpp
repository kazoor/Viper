#pragma once
#include "transform.hpp"

class Component : public Transform {
public:
    virtual ~Component() = 0;
    virtual void OnUpdate( const float elapsed ) = 0;
protected:
    Transform transform;
};