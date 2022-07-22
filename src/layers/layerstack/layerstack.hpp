#pragma once
#include <vector>
#include "../layer/layer.hpp"
#include "../../events/event/event.hpp"
#include "../../events/eventhandler/eventhandler.hpp"

namespace Viper::Layers {
    class LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer *Lay);
        void PushOverlay(Layer *Overlay);

        void PopLayer(Layer *Lay);
        void PopOverlay(Layer *Overlay);

       std::vector<Layer*>::iterator begin() { return Layers.begin(); }
       std::vector<Layer*>::iterator end() { return Layers.end(); }

    private:
        std::vector<Layer*> Layers;
        std::vector<Layer*>::iterator LayerInsert;
    };
}


