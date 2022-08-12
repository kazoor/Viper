#include <iostream>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <cstring>
#include <optional>
#include <set>

const std::vector< const char* > validation_layers = { "VK_LAYER_KHRONOS_validation" };
const std::vector< const char* > device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
#ifdef VIPER_DEBUG
    const bool enable_validation_layers = false;
#else
    const bool enable_validation_layers = true;
#endif

namespace Viper {
    // <!-- Vulkan API Start (Debugging)
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector< VkSurfaceFormatKHR > formats;
        std::vector< VkPresentModeKHR > present_modes;
    };
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger) {
                auto fn = ( PFN_vkCreateDebugUtilsMessengerEXT )vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT");
                if(fn != nullptr)
                    return fn(instance, pCreateInfo, pAllocator, pDebugMessenger);
                else
                    return VK_ERROR_EXTENSION_NOT_PRESENT;
            };

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto fn = ( PFN_vkDestroyDebugUtilsMessengerEXT )vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" );
        if(fn != nullptr )
            fn( instance, debugMessenger, pAllocator );
    };

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat( const std::vector< VkSurfaceFormatKHR >& available_formats ) {

    };

    // Vulkan API End. (Debugging) --!>

    class VkViper {
    private:
        // definerar det här uppe annars blir vscode grinig.
        struct ViperQueueFamilyIndices_t {
            std::optional<uint32_t> graphics_family;
            std::optional<uint32_t> present_family;
            bool is_complete() { return graphics_family.has_value() && present_family.has_value(); };
        };
    public:
        VkViper(GLFWwindow* wnd) : window(wnd) {};
        void Vulkan_Init() {
            Vulkan_CreateInstance(); // Creates the instance.

            Vulkan_SetDebugMessenger(); // Set's up a brev-duva. for debugging purposes..

            Vulkan_CreateSurface();
            
            Vulkan_PickPhysicalDevice();

            Vulkan_CreateLogicalDevice();
        };

        void Vulkan_Deinit() {
            Vulkan_Cleanup();
        };

        // Create's an instance to the vulkan API.
        void Vulkan_CreateInstance() {
            if(enable_validation_layers && !Vulkan_CheckValidationLayerSupport()) {
                throw std::runtime_error("validation layers requested, but not available.");
            };
            
            VkApplicationInfo appinfo{};
            appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appinfo.pApplicationName = "Viper Game Engine - Vulkan";
            appinfo.applicationVersion = VK_API_VERSION_1_3;
            appinfo.pEngineName = "No Engine";
            appinfo.engineVersion = VK_API_VERSION_1_3;
            appinfo.apiVersion = VK_API_VERSION_1_3;

            VkInstanceCreateInfo createinfo{};
            createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createinfo.pApplicationInfo = &appinfo;
           

            //uint32_t glfwExtensionCount = 0;
            //const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
            //uint32_t extensionCount = 0;
            //vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            //std::vector< VkExtensionProperties > extensions(extensionCount);
//
            //vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            auto m_extensions = Vulkan_GetRequiredExtensions();

            std::cout << "available extensions:" << std::endl;
            for( const auto& ext : m_extensions )
                std::cout << "\t" << ext << std::endl;

            //for( const auto& ext : extensions )
            //    std::cout << "\t" << ext.extensionName << std::endl;

            //for( uint32_t ext_count = 0; ext_count < glfwExtensionCount; ext_count++ )
            //    std::cout << "glfwVulkanExt: " << glfwExtensions[ext_count] << std::endl;

            createinfo.enabledExtensionCount = static_cast< uint32_t >( m_extensions.size() );
            createinfo.ppEnabledExtensionNames = m_extensions.data();

            VkDebugUtilsMessengerCreateInfoEXT debugcreateinfo;
            if(enable_validation_layers) {
                createinfo.enabledLayerCount = static_cast< uint32_t >( validation_layers.size() );
                createinfo.ppEnabledLayerNames = validation_layers.data();
                Vulkan_PopulateDebugMessangerCreateInfo(debugcreateinfo);
                
                createinfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugcreateinfo;
            } else {
                createinfo.enabledLayerCount = 0;
                createinfo.pNext = nullptr;
            };

            VkResult result = vkCreateInstance(&createinfo, nullptr, &instance);
            if( result != VK_SUCCESS )
                throw std::runtime_error("failed to create instance.");
        };

        // Do we have validation layer support?
        bool Vulkan_CheckValidationLayerSupport() {
            uint32_t layer_count = 0;
            vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

            std::vector<VkLayerProperties> available_layers(layer_count);
            vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

            std::cout << "available layers:" << std::endl;
            for( const auto& layer : available_layers )
                std::cout << "\t" << layer.layerName << " :: " << layer.description << std::endl;


            for( const char* layer_name : validation_layers) {
                bool layer_found = false;
                
                for( const auto& layer_properties : available_layers ) {
                    if( std::strcmp(layer_name, layer_properties.layerName) == 0 ) {
                        layer_found = true;
                        break;
                    };
                };

                if(!layer_found)
                    return false;
            };
            return true;
        };

        // Cleanup and destroy the vulkan instance/callbacks.
        void Vulkan_Cleanup() {
            if( enable_validation_layers )
                DestroyDebugUtilsMessengerEXT(instance, debugmessenger, nullptr );

            vkDestroyDevice(logical_device, nullptr);
            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyInstance(instance, nullptr);
        };

        void Vulkan_PopulateDebugMessangerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createinfo) {
            createinfo = {};

            // hittade ni inte ett längre namn på ett macro?
            createinfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

            // Så det här är nice. Vi kan specificera vilka fel meddelanden vi ska spotta ut i
            // debuggern. Very nice.
            createinfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

            createinfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

            createinfo.pfnUserCallback = debugCallback;
            createinfo.pUserData = nullptr; // vet inte varför jag måste specificera denna
            // som en nullptr.
            // Det är redan nullptr när strukten blir skapad, och så dessutom så används
            // den aldrig :thonk:
        };

        // Setup some debugging tools for Vulkan.
        void Vulkan_SetDebugMessenger() {
            if(!enable_validation_layers)
                return;
            
            VkDebugUtilsMessengerCreateInfoEXT createinfo;
            Vulkan_PopulateDebugMessangerCreateInfo(createinfo);
            if(CreateDebugUtilsMessengerEXT(instance, &createinfo, nullptr, &debugmessenger) != VK_SUCCESS)
                throw std::runtime_error("failed to setup debug messenger. ah yes, the brev-duva.");
        };

        std::vector< const char* > Vulkan_GetRequiredExtensions() {
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector< const char* > extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            if( enable_validation_layers )
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

            return extensions;
        };

        void Vulkan_PickPhysicalDevice() {
            uint32_t device_count = 0;
            vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
            if( device_count == 0 )
                throw std::runtime_error("this GPU doesn't support Vulkan. Fallback to OpenGL.");
        
            std::vector< VkPhysicalDevice > m_devices(device_count);
            vkEnumeratePhysicalDevices(instance, &device_count, m_devices.data());

            for( const auto& device : m_devices ) {
                if(Vulkan_IsDeviceSuitable(device)) {
                    physical_device = device;
                    break;
                };
            };

            if( physical_device == VK_NULL_HANDLE ) {
                throw std::runtime_error("unable to find a suitable gpu. Fallback to OpenGL.");
            };
        };

        bool Vulkan_CheckDeviceExtensionSupport(VkPhysicalDevice device) {
            uint32_t extension_count = 0;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

            std::vector< VkExtensionProperties > available_extensions(extension_count);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

            std::set< std::string > required_extensions(device_extensions.begin(), device_extensions.end());

            for( const auto& ext : available_extensions )
                required_extensions.erase(ext.extensionName);

            return required_extensions.empty();
        };

        bool Vulkan_IsDeviceSuitable(VkPhysicalDevice device) {
            ViperQueueFamilyIndices_t indices = Vulkan_FindQueueFamilies(device);


            VkPhysicalDeviceProperties device_properties;
            vkGetPhysicalDeviceProperties(device, &device_properties);
            std::cout << "Device Properties: " << device_properties.deviceName << std::endl;
            
            VkPhysicalDeviceFeatures device_features;
            vkGetPhysicalDeviceFeatures(device, &device_features);
            std::cout << "Device Features: Can render geometry shader? " << device_features.geometryShader << std::endl;
//
            //return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
            //    device_features.geometryShader;

            bool extensions_supported = Vulkan_CheckDeviceExtensionSupport(device);
            std::cout << "[VULKAN] Can create swapchain? " << extensions_supported << std::endl;

            bool swap_chain_adequate = false;
            if( extensions_supported ) {
                SwapChainSupportDetails swap_chain_support = Vulkan_QuerySwapChainSupport(device);
                swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
            };

            return indices.is_complete() && extensions_supported && swap_chain_adequate;
        };

        ViperQueueFamilyIndices_t Vulkan_FindQueueFamilies(VkPhysicalDevice device) {
            ViperQueueFamilyIndices_t indices;

            uint32_t queue_families_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_families_count, nullptr);

            std::vector< VkQueueFamilyProperties > queue_families(queue_families_count);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_families_count, queue_families.data());

            int index = 0;
            for( const auto& queue_family : queue_families ) {
                if( queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
                    indices.graphics_family = index;
                };

                VkBool32 present_support = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &present_support);
                if( present_support )
                    indices.present_family = index;

                if(indices.is_complete())
                    break;

                index++;
            };
            return indices;
        };

        void Vulkan_CreateLogicalDevice() {
            ViperQueueFamilyIndices_t indices = Vulkan_FindQueueFamilies(physical_device);

            std::vector< VkDeviceQueueCreateInfo > queue_create_infos;
            std::set< uint32_t > unique_queue_families = {
                indices.graphics_family.value(),
                indices.present_family.value()
            };

            float queue_priority = 1.0f;
            //VkDeviceQueueCreateInfo queue_create_info{};
            //queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            //queue_create_info.queueFamilyIndex = indices.graphics_family.has_value();
            //queue_create_info.queueCount = 1;
//
            //queue_create_info.pQueuePriorities = &queue_priority;

            for( uint32_t queue_family : unique_queue_families ) {
                VkDeviceQueueCreateInfo queue_create_info{};
                queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queue_create_info.queueFamilyIndex = queue_family;
                queue_create_info.queueCount = 1;
                queue_create_info.pQueuePriorities = &queue_priority;
                queue_create_infos.push_back(queue_create_info);
            };

            VkPhysicalDeviceFeatures device_features{};

            VkDeviceCreateInfo createinfo{};
            createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createinfo.pQueueCreateInfos = queue_create_infos.data();//&queue_create_info;
            createinfo.queueCreateInfoCount = static_cast< uint32_t >( queue_create_infos.size() );
            createinfo.pEnabledFeatures = &device_features;

            /* For older versions of Vulkan */
            //createinfo.enabledExtensionCount = 0;
            createinfo.enabledExtensionCount = static_cast< uint32_t >( device_extensions.size() );
            createinfo.ppEnabledExtensionNames = device_extensions.data();

            if(enable_validation_layers) {
                createinfo.enabledLayerCount = static_cast< uint32_t >( validation_layers.size() );
                createinfo.ppEnabledLayerNames = validation_layers.data();
            } else {
                createinfo.enabledLayerCount = 0;
            };

            VkResult result = vkCreateDevice(physical_device, &createinfo, nullptr, &logical_device);
            if( result != VK_SUCCESS )
                throw std::runtime_error("failed to create a logical device.");

            vkGetDeviceQueue(logical_device, indices.graphics_family.value(), 0, &graphics_queue);
            vkGetDeviceQueue(logical_device, indices.present_family.value(), 0, &present_queue);
        };

        void Vulkan_CreateSurface() {
            VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
            if( result != VK_SUCCESS )
                throw std::runtime_error("failed to create a window surface for glfw/vulkan.");
        };

        SwapChainSupportDetails Vulkan_QuerySwapChainSupport(VkPhysicalDevice device) {
            SwapChainSupportDetails details;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

            uint32_t format_count;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);
            if( format_count != 0) {
                details.formats.resize(format_count);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
            };

            uint32_t present_mode_count;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

            if( present_mode_count != 0) {
                details.present_modes.resize(present_mode_count);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
            };
            return details;
        };

    private:
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        ) {
            std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
            return VK_FALSE;
        };
    private:
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugmessenger; // brev duva.
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        VkDevice logical_device;
        VkQueue graphics_queue;
        VkQueue present_queue;
        VkSurfaceKHR surface;

        GLFWwindow* window;

        const uint32_t viper_width = 800;
        const uint32_t viper_height = 600;
    };
};

int main() {
    if(!glfwInit()) {
        printf("unable to setup environment.\n");
        return -1;
    };

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* m_Window = glfwCreateWindow(800, 600, "Super bob", nullptr, nullptr);
    if(!m_Window) {
        printf("unable to create window.\n");
        return -1;
    };

    Viper::VkViper* viper_vulkan = new Viper::VkViper(m_Window);
    viper_vulkan->Vulkan_Init();

    while(!glfwWindowShouldClose(m_Window)) {
        glfwPollEvents();
    };
    viper_vulkan->Vulkan_Deinit();

    delete viper_vulkan;
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    return 0;
};