#pragma once
#include <string>
#include "../../events/event/event.hpp"
#include "../../events/eventhandler/eventhandler.hpp"

namespace Viper::Layers {
    class Layer {
    public:
        Layer(const std::string &LayerName = "NewLayer") : LayerName(LayerName) {}
        virtual ~Layer() {}

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Viper::Events::Event *Event) {}

        inline const std::string &GetLayerName() const { return LayerName; }
    protected:
        std::string LayerName;
    };
}