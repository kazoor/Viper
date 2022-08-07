#include <viper/application.hpp>
#include <imguieditor/imguieditor.hpp>
#include <imguieditor/scene/scenelayer.hpp>
#include <scene/entitycomponents.hpp>
#include <scene/scene.hpp>
#include <layers/layer/layer.hpp>

#include <graphics/renderer/api/color.hpp>
#include <graphics/renderer/rendercommand.hpp>
#include <graphics/renderer/camera/orthographic_camera_controller.hpp>
#include <graphics/renderer/renderer2d.hpp>
#include <graphics/renderer/sprite2d.hpp>

#include <graphics/renderer/framebuffer.hpp>
#include <util/input/input.hpp>
#include <util/input/keycodes.hpp>

#define VIPER_BUILD_DATE __DATE__