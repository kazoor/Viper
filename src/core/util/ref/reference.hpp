#pragma once
#include <memory>

namespace Viper {
    template< typename T >
    using Scope = std::unique_ptr< T >;

    template< typename T, typename... TArgs >
    constexpr Scope< T > CreateScope( TArgs&&... args ) {
        return std::make_unique< T >( std::forward< TArgs >( args )... );
    };

    template< typename T >
    using Ref = std::shared_ptr< T >;

    template< typename T, typename... TArgs >
    constexpr Ref< T > CreateRef(TArgs&&... args) {
        return std::make_shared< T >( std::forward< TArgs >( args )... );
    };
};

#define MAKE_REF( x, y ) Viper::Scope< x > g_ ##x = Viper::CreateScope< x >y