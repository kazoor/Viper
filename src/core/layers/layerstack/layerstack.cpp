#include <vector>
#include <algorithm>
#include "layerstack.hpp"
#include "../layer/layer.hpp"

namespace Viper::Layers {

    LayerStack::LayerStack() {
        LayerInsert = Layers.begin();
    }

    LayerStack::~LayerStack() {
        for (auto Layer: Layers)
            delete Layer;
    }

    void LayerStack::PushLayer(Layer *Lay) {
        LayerInsert = Layers.emplace(LayerInsert, Lay);
    }

    void LayerStack::PushOverlay(Layer *Overlay) {
        Layers.emplace_back(Overlay);
    }

    void LayerStack::PopLayer(Layer *Lay) {
        auto It = std::find(Layers.begin(), Layers.end(), Lay);
        if (It != Layers.end()) {
            Layers.erase(It);
            LayerInsert--;
        }
    }

    void LayerStack::PopOverlay(Layer *Overlay) {
        auto It = std::find(Layers.begin(), Layers.end(), Overlay);
        if (It != Layers.end()) {
            Layers.erase(It);
        }
    }
}