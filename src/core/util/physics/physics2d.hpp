#pragma once
#include "../../components/transform.hpp"

using namespace Viper::Components;
namespace Viper::Physics2D {

    float Linecast2D(const Transform& from, const Transform& to);
    
    bool Raycast2D( const Transform& from, const Transform& to );
};