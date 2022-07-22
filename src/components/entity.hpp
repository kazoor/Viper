#pragma once
#include <vector>
#include <memory>
#include "transform.hpp"

namespace Viper::Components {
    class Entity {
    public:
        virtual void OnUpdate() = 0;

        virtual void OnGui() {};

        template< typename T = Transform, typename... TArgs >
        T* AddComponent( TArgs&&... args ) {
            auto ptr = std::make_unique< T >( std::forward< TArgs >( args )... );
            T* result = ptr.get();
            m_Components.push_back(std::move(ptr));
            return result;
        };

        size_t GetComponentSize() const;

        Transform GetTransform() const;

        const char* GetTag() const;
        
    protected:
        Transform transform;

        const char* tag;
    private:
        using container_t = std::vector< std::unique_ptr< Transform > >;
        
        container_t m_Components;
    };
};