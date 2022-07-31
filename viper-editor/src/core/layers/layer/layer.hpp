#pragma once
#include <string>
#include <viper/viper.hpp>
#include <util/timer/timestep.hpp>
#include "../../events/event/event.hpp"

namespace Viper::Layers {
    class VIPER_API Layer {
    public:
        Layer(const std::string &LayerName = "NewLayer") : LayerName(LayerName) {}
        virtual ~Layer() {}

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep::Timestep ts) {}
        virtual void OnEvent(Events::Event& event) = 0;

        inline const std::string &GetLayerName() const { return LayerName; }
    protected:
        std::string LayerName;
    };
}