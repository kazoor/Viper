#pragma once

#include <vector>
#include <string>
#include "../layer/layer.hpp"

namespace Viper::Layers {
    class LayerStack {
    public:
        LayerStack();

        ~LayerStack();

        void PushLayer(Layer *Lay);

        void PushOverlay(Layer *Overlay);

        void PopLayer(Layer *Lay);

        void PopOverlay(Layer *Overlay);

        std::vector<Layer *>::iterator begin() { return Layers.begin(); }
        std::vector<Layer *>::iterator end() { return Layers.end(); }

        std::vector<Layer *>::const_iterator begin() const { return Layers.begin(); }
        std::vector<Layer *>::const_iterator end() const { return Layers.end(); }

        std::vector<Layer *>::reverse_iterator rbegin() { return Layers.rbegin(); };
        std::vector<Layer *>::reverse_iterator rend() { return Layers.rend(); };

        std::vector<Layer *>::const_reverse_iterator rbegin() const { return Layers.rbegin(); };
        std::vector<Layer *>::const_reverse_iterator rend() const { return Layers.rend(); };

        int GetSize() { return Layers.size(); }

    private:
        std::vector<Layer *> Layers;
        std::vector<Layer *>::iterator LayerInsert;
    };
}


