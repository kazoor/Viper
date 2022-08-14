#include <iostream>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <cstring>
#include <optional>
#include <set>
#include <algorithm>
#include <limits>
#include <fstream>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const std::vector< const char* > validation_layers = { "VK_LAYER_KHRONOS_validation" };
const std::vector< const char* > device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

//#define VIPER_NDEBUG // NDEBUG meaning NO DEBUG.

#ifdef VIPER_NDEBUG
    const bool enable_validation_layers = false;
#else
    const bool enable_validation_layers = true;
#endif

namespace Viper {
    // <!-- Vulkan API Start (Debugging)
    struct Vertex { // TDL; Fixa instanced rendering för att hantera
    // en jävla massa grejer för renderpass.
        glm::vec3 position;
        glm::vec4 color;

        static VkVertexInputBindingDescription GetDescription() {
            VkVertexInputBindingDescription bind_descriptions = {};
            bind_descriptions.stride = sizeof(Vertex);
            bind_descriptions.binding = 0;
            bind_descriptions.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bind_descriptions;
        };

        static std::array< VkVertexInputAttributeDescription, 2U > GetAttributeDescriptions() {
            std::array< VkVertexInputAttributeDescription, 2U > attribute_descriptions = {};
            attribute_descriptions[0].binding = 0;
            attribute_descriptions[0].location = 0;
            attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // Vec2
            attribute_descriptions[0].offset = offsetof(Vertex, position);

            attribute_descriptions[1].binding = 0;
            attribute_descriptions[1].location = 1;
            attribute_descriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT; // Vec3
            attribute_descriptions[1].offset = offsetof(Vertex, color);
            return attribute_descriptions;
        };
    };

    const std::vector< Vertex > m_vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0}}
    };

    const std::vector< uint16_t > m_indices = {
        0, 1, 2,    2, 3, 0  
    };

    struct UniformBufferObjects_t {
        glm::mat4 projection;
    };
    
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
     static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<VkViper*>(glfwGetWindowUserPointer(window));
        app->framebuffer_resized = true;
        printf("resized.\n");
    }

        VkViper(GLFWwindow* wnd) : window(wnd) {};
        void Vulkan_Init() {
            
            Vulkan_RegisterGlfwCallbacks();

            Vulkan_CreateInstance(); // Creates the instance.

            Vulkan_SetDebugMessenger(); // Set's up a brev-duva. for debugging purposes..

            Vulkan_CreateSurface();
            
            Vulkan_PickPhysicalDevice();

            Vulkan_CreateLogicalDevice();

            Vulkan_CreateSwapChain();

            Vulkan_CreateImageViews();

            Vulkan_CreateRenderPass();

            Vulkan_CreateDescriptorSetLayout();

            Vulkan_CreateGraphicsPipeline();

            Vulkan_CreateFramebuffers();

            Vulkan_CreateCommandPool();

            Vulkan_CreateVertexBuffer();

            Vulkan_CreateIndexBuffer();

            Vulkan_CreateUniformBuffers();

            Vulkan_CreateDescriptorPool();

            Vulkan_CreateDescriptorSets();

            Vulkan_CreateCommandBuffers();

            Vulkan_CreateSyncObjects();
        };

        void Vulkan_Deinit() {
            vkDeviceWaitIdle(logical_device);

            Vulkan_Cleanup();
        };

        void Vulkan_CreateDescriptorPool() {
            VkDescriptorPoolSize pool_size = {};
            pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            pool_size.descriptorCount = static_cast< uint32_t >( max_frames_in_flight );

            VkDescriptorPoolCreateInfo pool_info = {};
            pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            pool_info.poolSizeCount = 1;
            pool_info.pPoolSizes = &pool_size;
            pool_info.maxSets = static_cast< uint32_t >( max_frames_in_flight );

            VkResult result = vkCreateDescriptorPool(logical_device, &pool_info, nullptr, &descriptor_pool);
            if(result != VK_SUCCESS)
                throw std::runtime_error("unable to create descriptor pool.");
        };

        void Vulkan_CreateDescriptorSets() {
            std::vector<VkDescriptorSetLayout> layouts(max_frames_in_flight, descriptor_set_layout);
            VkDescriptorSetAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            alloc_info.descriptorPool = descriptor_pool;
            alloc_info.descriptorSetCount = static_cast< uint32_t >(max_frames_in_flight);
            alloc_info.pSetLayouts = layouts.data();

            descriptor_sets.resize(max_frames_in_flight);
            if (vkAllocateDescriptorSets(logical_device, &alloc_info, descriptor_sets.data()) != VK_SUCCESS) {
                throw std::runtime_error("failed to allocate descriptor sets!");
            };

            for( size_t i = 0; i < max_frames_in_flight; i++ ) {
                VkDescriptorBufferInfo buffer_info = {};
                buffer_info.buffer = uniform_buffers[i];
                buffer_info.offset = 0;
                buffer_info.range = sizeof(UniformBufferObjects_t);

                VkWriteDescriptorSet descriptor_write = {};
                descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptor_write.dstSet = descriptor_sets[i];
                descriptor_write.dstBinding = 0;
                descriptor_write.dstArrayElement = 0;
                descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptor_write.descriptorCount = 1;
                descriptor_write.pBufferInfo = &buffer_info;

                // both of these are optional.
                descriptor_write.pImageInfo = nullptr;
                descriptor_write.pTexelBufferView = nullptr;

                vkUpdateDescriptorSets(logical_device, 1, &descriptor_write, 0, nullptr);
            };
        }

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
            Vulkan_CleanSwapchain();

            vkDestroyDescriptorPool(logical_device, descriptor_pool, nullptr);

            vkDestroyDescriptorSetLayout(logical_device, descriptor_set_layout, nullptr);

            for (size_t i = 0; i < max_frames_in_flight; i++) {
                vkDestroyBuffer(logical_device, uniform_buffers[i], nullptr);
                vkFreeMemory(logical_device, uniform_buffers_memory[i], nullptr);
            };

            //vkDestroyDescriptorSetLayout(logical_device, descriptor_set_layout, nullptr);

            vkDestroyBuffer(logical_device, vertex_buffer, nullptr);
            vkFreeMemory(logical_device, vertex_buffer_memory, nullptr);

            vkDestroyBuffer(logical_device, index_buffer, nullptr);
            vkFreeMemory(logical_device, index_buffer_memory, nullptr);

            for( size_t i = 0; i < max_frames_in_flight; i++ ) {
                vkDestroySemaphore(logical_device, render_finished_semaphore[i], nullptr);
                vkDestroySemaphore(logical_device, image_available_semaphore[i], nullptr);
                vkDestroyFence(logical_device, in_flight_fences[i], nullptr);
            }
            
            vkDestroyCommandPool(logical_device, command_pool, nullptr);

            //for( auto frame_buffer : swap_chain_frame_buffers )
            //    vkDestroyFramebuffer(logical_device, frame_buffer, nullptr );
           
            vkDestroyPipeline(logical_device, graphics_pipeline, nullptr);
            vkDestroyPipelineLayout(logical_device, pipeline_layout, nullptr);
            vkDestroyRenderPass(logical_device, render_pass, nullptr);
            
            //for( auto image_view : swap_chain_image_views )
            //    vkDestroyImageView(logical_device, image_view, nullptr);

            //vkDestroySwapchainKHR(logical_device, swap_chain, nullptr);
            vkDestroyDevice(logical_device, nullptr);

            if( enable_validation_layers )
                DestroyDebugUtilsMessengerEXT(instance, debugmessenger, nullptr );

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

        void Vulkan_CreateDescriptorSetLayout() {
            VkDescriptorSetLayoutBinding ubo_layout_binding = {};
            ubo_layout_binding.binding = 0;
            ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            ubo_layout_binding.descriptorCount = 1;
            ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            ubo_layout_binding.pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layout_info = {};
            layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layout_info.bindingCount = 1;
            layout_info.pBindings = &ubo_layout_binding;

            VkResult result = vkCreateDescriptorSetLayout(logical_device, &layout_info, nullptr, &descriptor_set_layout);
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to create a descriptor set layout.");
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

        VkExtent2D ChooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities ) {
            if( capabilities.currentExtent.width != std::numeric_limits< uint32_t >::max() ) {
                return capabilities.currentExtent;
            } else {
                int width, height;
                glfwGetFramebufferSize(window, &width, &height);
                VkExtent2D actual_extent = {
                    static_cast< uint32_t >( width ),
                    static_cast< uint32_t >( height )
                };

                actual_extent.width = std::clamp(actual_extent.width,
                    capabilities.minImageExtent.width, capabilities.maxImageExtent.width);

                actual_extent.height = std::clamp(actual_extent.height, 
                    capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

                    return actual_extent;
            };
        };

        void Vulkan_CreateSwapChain() {
            SwapChainSupportDetails swap_chain_support = Vulkan_QuerySwapChainSupport(physical_device);

            VkSurfaceFormatKHR surface_format = ChooseSwapSurfaceFormat(swap_chain_support.formats);

            VkPresentModeKHR present_mode = ChooseSwapPresentMode(swap_chain_support.present_modes);

            VkExtent2D extent = ChooseSwapExtent(swap_chain_support.capabilities); 

            uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

            if( swap_chain_support.capabilities.maxImageCount > 0 
                && image_count > swap_chain_support.capabilities.maxImageCount) {
                image_count = swap_chain_support.capabilities.maxImageCount;
            };

            VkSwapchainCreateInfoKHR createinfo{};
            createinfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createinfo.surface = surface;
            createinfo.minImageCount = image_count;
            createinfo.imageFormat = surface_format.format;
            createinfo.imageColorSpace = surface_format.colorSpace;
            createinfo.imageExtent = extent;
            createinfo.imageArrayLayers = 1;
            createinfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            ViperQueueFamilyIndices_t indices = Vulkan_FindQueueFamilies(physical_device);
            uint32_t queue_family_indices[] = {indices.graphics_family.value(), indices.present_family.value()};

            if( indices.graphics_family != indices.present_family ) {
                createinfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createinfo.queueFamilyIndexCount = 2;
                createinfo.pQueueFamilyIndices = queue_family_indices;
            } else {
                createinfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createinfo.queueFamilyIndexCount = 0; // opt.
                createinfo.pQueueFamilyIndices = nullptr; // opt.
            };

            createinfo.preTransform = swap_chain_support.capabilities.currentTransform;
            createinfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createinfo.presentMode = present_mode;
            createinfo.clipped = VK_TRUE;
            createinfo.oldSwapchain = VK_NULL_HANDLE;

            VkResult result = vkCreateSwapchainKHR(logical_device, &createinfo, nullptr, &swap_chain);
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to create a swap chain.");

            vkGetSwapchainImagesKHR(logical_device, swap_chain, &image_count, nullptr);
            swap_chain_images.resize(image_count);
            vkGetSwapchainImagesKHR(logical_device, swap_chain, &image_count, swap_chain_images.data());
        
            swap_chain_image_format = surface_format.format;
            swap_chain_extent = extent;
        };

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat( const std::vector< VkSurfaceFormatKHR >& available_formats ) {
            for( const auto& available_format : available_formats ) {
                if( available_format.format == VK_FORMAT_B8G8R8A8_SRGB && 
                    available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR ) {
                    return available_format;
                };
            };
            return available_formats[ 0 ];
        };
    
        VkPresentModeKHR ChooseSwapPresentMode( const std::vector< VkPresentModeKHR >& available_present_modes) {
            for( const auto& available_present_mode : available_present_modes ) {
                if( available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR )
                    return available_present_mode;
            };
            return VK_PRESENT_MODE_FIFO_KHR;
        };

        void Vulkan_CreateUniformBuffers() {
            VkDeviceSize buffer_size = sizeof(UniformBufferObjects_t);
            uniform_buffers.resize(max_frames_in_flight);
            uniform_buffers_memory.resize(max_frames_in_flight);

            for (size_t i = 0; i < max_frames_in_flight; i++) {
                Vulkan_CreateBuffer(buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniform_buffers[i], uniform_buffers_memory[i]);
            }
        };

        void Vulkan_CreateImageViews() {
            swap_chain_image_views.resize(swap_chain_images.size());
            for( size_t i = 0; i < swap_chain_images.size(); i++) {
                VkImageViewCreateInfo createinfo{};
                createinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                createinfo.image = swap_chain_images[i];
                createinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                createinfo.format = swap_chain_image_format;
                
                createinfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                createinfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                createinfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                createinfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

                createinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                createinfo.subresourceRange.baseMipLevel = 0;
                createinfo.subresourceRange.levelCount = 1;
                createinfo.subresourceRange.baseArrayLayer = 0;
                createinfo.subresourceRange.layerCount = 1;

                VkResult result = vkCreateImageView(logical_device, &createinfo, nullptr, &swap_chain_image_views[i]);
                if( result != VK_SUCCESS )
                    throw std::runtime_error( "failed to create image views.");
            };
        };

        uint32_t Vulkan_FindMemoryType( uint32_t type_filter, VkMemoryPropertyFlags prop ) {
            VkPhysicalDeviceMemoryProperties mem_props = {};
            vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_props);

            for( uint32_t i = 0; i < mem_props.memoryTypeCount; i++ ) {
                if( ( type_filter & (1 << i) ) && ( mem_props.memoryTypes[i].propertyFlags & prop ) == prop )
                    return i;
            };

            throw std::runtime_error("failed to find suitable mem type.");
        };

        void Vulkan_CreateVertexBuffer() {
            VkDeviceSize buffer_size = sizeof(m_vertices[0]) * m_vertices.size();

            VkBuffer staging_buffer;
            VkDeviceMemory staging_buffer_memory;
            Vulkan_CreateBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_memory);

            void* data;
            vkMapMemory(logical_device, staging_buffer_memory, 0, buffer_size, 0, &data);
            memcpy(data, m_vertices.data(), (size_t) buffer_size);
            vkUnmapMemory(logical_device, staging_buffer_memory);

            Vulkan_CreateBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertex_buffer, vertex_buffer_memory);
            Vulkan_CopyBuffer(staging_buffer, vertex_buffer, buffer_size);
            vkDestroyBuffer(logical_device, staging_buffer, nullptr);
            vkFreeMemory(logical_device, staging_buffer_memory, nullptr);
        };

        void Vulkan_CreateIndexBuffer() {
            VkDeviceSize buffer_size = sizeof( m_indices[0] ) * m_indices.size();
            
            VkBuffer staging_buffer;
            VkDeviceMemory staging_buffer_memory;

            Vulkan_CreateBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_memory);
        
            void* data;
            vkMapMemory(logical_device, staging_buffer_memory, 0, buffer_size, 0, &data);
            memcpy(data, m_indices.data(), (size_t)buffer_size);
            vkUnmapMemory(logical_device, staging_buffer_memory);

            Vulkan_CreateBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, index_buffer, index_buffer_memory);
        
            Vulkan_CopyBuffer(staging_buffer, index_buffer, buffer_size);
            vkDestroyBuffer(logical_device, staging_buffer, nullptr);
            vkFreeMemory(logical_device, staging_buffer_memory, nullptr);
        };

        void Vulkan_CreateRenderPass() {
            VkAttachmentDescription color_attachment{};
            color_attachment.format = swap_chain_image_format;
            color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
            color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference color_attachment_ref{};
            color_attachment_ref.attachment = 0;
            color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &color_attachment_ref;

            VkSubpassDependency dependency{};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.dstSubpass = 0;
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.srcAccessMask = 0;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            VkRenderPassCreateInfo render_pass_info{};
            render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            render_pass_info.attachmentCount = 1;
            render_pass_info.pAttachments = &color_attachment;
            render_pass_info.subpassCount = 1;
            render_pass_info.pSubpasses = &subpass;
            render_pass_info.dependencyCount = 1;
            render_pass_info.pDependencies = &dependency;

            VkResult result = vkCreateRenderPass(logical_device, &render_pass_info, nullptr, &render_pass);
            
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to create a render pass.");
        };

        void Vulkan_CreateGraphicsPipeline() {
            auto vert_shader_coder = ReadFile("shader.v.spv");
            auto frag_shader_coder = ReadFile("shader.f.spv");

            VkShaderModule vertex_shader_module = Vulkan_CreateShaderModule(vert_shader_coder);
            VkShaderModule fragment_shader_module = Vulkan_CreateShaderModule(frag_shader_coder);
        
            VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
            vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
            vert_shader_stage_info.module = vertex_shader_module;
            vert_shader_stage_info.pName = "main";

            VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
            frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            frag_shader_stage_info.module = fragment_shader_module;
            frag_shader_stage_info.pName = "main";

            VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_info, frag_shader_stage_info };

            auto binding_description = Vertex::GetDescription();
            auto attribute_description = Vertex::GetAttributeDescriptions();

            VkPipelineVertexInputStateCreateInfo vertex_input_info{};
            vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertex_input_info.vertexBindingDescriptionCount = 1;
            vertex_input_info.pVertexBindingDescriptions = &binding_description; // opt.
            vertex_input_info.vertexAttributeDescriptionCount = static_cast< uint32_t >( attribute_description.size() );
            vertex_input_info.pVertexAttributeDescriptions = attribute_description.data();

            VkPipelineInputAssemblyStateCreateInfo input_assembly{};
            input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            input_assembly.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float)swap_chain_extent.width;
            viewport.height = (float)swap_chain_extent.height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = swap_chain_extent;

            VkPipelineViewportStateCreateInfo viewport_state{};
            viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewport_state.viewportCount = 1;
            viewport_state.scissorCount = 1;
            viewport_state.pViewports = &viewport;
            viewport_state.pScissors = &scissor;

            VkPipelineRasterizationStateCreateInfo rasterizer{};
            rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizer.depthClampEnable = VK_FALSE;
            rasterizer.rasterizerDiscardEnable = VK_FALSE;
            rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
            rasterizer.lineWidth = 1.0f;
            rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
            rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
            rasterizer.depthBiasEnable = VK_FALSE;
            rasterizer.depthBiasConstantFactor = 0.0f; // opt
            rasterizer.depthBiasClamp = 0.0f; // opt
            rasterizer.depthBiasSlopeFactor = 0.0f; // opt

            VkPipelineMultisampleStateCreateInfo multi_sampling{};
            multi_sampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multi_sampling.sampleShadingEnable = VK_FALSE;
            multi_sampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            multi_sampling.minSampleShading = 1.0f; // opt
            multi_sampling.pSampleMask = nullptr; // opt (stencil testing/mask)
            multi_sampling.alphaToCoverageEnable = VK_FALSE; // opt
            multi_sampling.alphaToOneEnable = VK_FALSE; // opt

            VkPipelineColorBlendAttachmentState color_blend_attachment{};
            color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            color_blend_attachment.blendEnable = VK_FALSE;
            color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // opt
            color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // opt
            color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD; // opt
            color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // opt
            color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // opt
            color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD; // opt

            VkPipelineColorBlendStateCreateInfo color_blending{};
            color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            color_blending.logicOpEnable = VK_FALSE;
            color_blending.logicOp = VK_LOGIC_OP_COPY; // opt
            color_blending.attachmentCount = 1;
            color_blending.pAttachments = &color_blend_attachment;
            color_blending.blendConstants[0] = 0.0f; // opt
            color_blending.blendConstants[1] = 0.0f; // opt
            color_blending.blendConstants[2] = 0.0f; // opt
            color_blending.blendConstants[3] = 0.0f; // opt

            std::vector<VkDynamicState> dynamic_states = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
            };

            VkPipelineDynamicStateCreateInfo dynamicstate{};
            dynamicstate.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            dynamicstate.dynamicStateCount = static_cast< uint32_t >(dynamic_states.size());
            dynamicstate.pDynamicStates = dynamic_states.data();

            VkPipelineLayoutCreateInfo pipeline_layout_info{};
            pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipeline_layout_info.setLayoutCount = 1; // opt
            pipeline_layout_info.pSetLayouts = &descriptor_set_layout; // opt
            pipeline_layout_info.pushConstantRangeCount = 0; // opt
            pipeline_layout_info.pPushConstantRanges = nullptr; // opt

            VkResult result = vkCreatePipelineLayout(logical_device, &pipeline_layout_info, nullptr, &pipeline_layout);
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to create a pipeline layout.");

            VkGraphicsPipelineCreateInfo pipeline_info{};
            pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipeline_info.stageCount = 2;
            pipeline_info.pStages = shader_stages;
            pipeline_info.pVertexInputState = &vertex_input_info;
            pipeline_info.pInputAssemblyState = &input_assembly;
            pipeline_info.pViewportState = &viewport_state;
            pipeline_info.pRasterizationState = &rasterizer;
            pipeline_info.pMultisampleState = &multi_sampling;
            pipeline_info.pDepthStencilState = nullptr; // Optional
            pipeline_info.pColorBlendState = &color_blending;
            pipeline_info.pDynamicState = &dynamicstate;
            pipeline_info.layout = pipeline_layout;
            pipeline_info.renderPass = render_pass;
            pipeline_info.subpass = 0;
            pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // opt
            pipeline_info.basePipelineIndex = -1; // opt

            VkResult result2 = vkCreateGraphicsPipelines(logical_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &graphics_pipeline);
            if(result2 != VK_SUCCESS)
                throw std::runtime_error("failed to create graphics pipeline.");

            vkDestroyShaderModule(logical_device, fragment_shader_module, nullptr);
            vkDestroyShaderModule(logical_device, vertex_shader_module, nullptr);
        };

        VkShaderModule Vulkan_CreateShaderModule(const std::vector<char>& code ){
            VkShaderModuleCreateInfo createinfo{};
            createinfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            createinfo.codeSize = code.size();
            createinfo.pCode = reinterpret_cast< const uint32_t* >(code.data());

            VkShaderModule shader_module;
            VkResult result = vkCreateShaderModule(logical_device, &createinfo, nullptr, &shader_module);
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to create shader module.");

            return shader_module;
        };

        void Vulkan_CreateFramebuffers() {
            swap_chain_frame_buffers.resize(swap_chain_image_views.size());

            for(size_t i = 0; i < swap_chain_image_views.size(); i++) {
                VkImageView attachments[] = { swap_chain_image_views[ i ] }; // ??

                VkFramebufferCreateInfo frame_buffer_info{};
                frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                frame_buffer_info.renderPass = render_pass;
                frame_buffer_info.attachmentCount = 1;
                frame_buffer_info.pAttachments = attachments;
                frame_buffer_info.width = swap_chain_extent.width;
                frame_buffer_info.height = swap_chain_extent.height;
                frame_buffer_info.layers = 1;

                VkResult result = vkCreateFramebuffer(logical_device, &frame_buffer_info, nullptr, &swap_chain_frame_buffers[i]);

                if(result != VK_SUCCESS)
                    throw std::runtime_error("failed to create a framebuffer.");
            };
        };

        void Vulkan_CreateCommandPool() {
            ViperQueueFamilyIndices_t queue_family_indices = Vulkan_FindQueueFamilies(physical_device);

            VkCommandPoolCreateInfo pool_info{};
            pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

            VkResult result = vkCreateCommandPool(logical_device, &pool_info, nullptr, &command_pool);
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to create a command pool");
        };

        void Vulkan_CreateCommandBuffers() {
            command_buffers.resize(max_frames_in_flight); // shouldn't this be swap chain images? the amount of them?

            VkCommandBufferAllocateInfo alloc_info{};
            alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            alloc_info.commandPool = command_pool;
            alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            alloc_info.commandBufferCount = static_cast< uint32_t >(command_buffers.size());

            VkResult result = vkAllocateCommandBuffers(logical_device, &alloc_info, command_buffers.data());
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to allocate command buffer(s).");
        };

        void Vulkan_RecordCommandBuffer(VkCommandBuffer commandbuffer, uint32_t imageindex) {
            VkCommandBufferBeginInfo begin_info{};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags = 0; // opt
            begin_info.pInheritanceInfo = nullptr; // opt

            VkResult result = vkBeginCommandBuffer(commandbuffer, &begin_info);
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to begin recording of the command buffer.");

            VkRenderPassBeginInfo render_pass_info{};
            render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            render_pass_info.renderPass = render_pass;
            render_pass_info.framebuffer = swap_chain_frame_buffers[ imageindex ];
            render_pass_info.renderArea.offset = {0, 0};
            render_pass_info.renderArea.extent = swap_chain_extent;

            VkClearValue clear_color = {{{0.05f, 0.05f, 0.05f, 1.0f}}};
            render_pass_info.clearValueCount = 1;
            render_pass_info.pClearValues = &clear_color;

            vkCmdBeginRenderPass(commandbuffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast< float >( swap_chain_extent.width );
            viewport.height = static_cast< float >( swap_chain_extent.height );
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(commandbuffer, 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.offset = {0,0};
            scissor.extent = swap_chain_extent;
            vkCmdSetScissor(commandbuffer, 0, 1, &scissor);

            VkBuffer vertex_buffers[] = { vertex_buffer };
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertex_buffers, offsets);

            vkCmdBindIndexBuffer(commandbuffer, index_buffer, 0, VK_INDEX_TYPE_UINT16);

            vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0,
                1, &descriptor_sets[current_frame], 0, nullptr);

            vkCmdDrawIndexed(commandbuffer, static_cast< uint32_t >( m_indices.size() ), 1, 0, 0, 0);

            vkCmdEndRenderPass(commandbuffer);

            VkResult result2 = vkEndCommandBuffer(commandbuffer);
            if(result2 != VK_SUCCESS)
                throw std::runtime_error("failed to record the command buffer.");
        };

        void Vulkan_CreateSyncObjects() {
            image_available_semaphore.resize(max_frames_in_flight);
            render_finished_semaphore.resize(max_frames_in_flight);
            in_flight_fences.resize(max_frames_in_flight);

            VkSemaphoreCreateInfo semaphore_info{};
            semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkFenceCreateInfo fence_info{};
            fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            for( size_t i = 0; i < max_frames_in_flight; i++ ) {
                if(vkCreateSemaphore(logical_device, &semaphore_info, nullptr, &image_available_semaphore[i]) != VK_SUCCESS ||
                vkCreateSemaphore(logical_device, &semaphore_info, nullptr, &render_finished_semaphore[i]) != VK_SUCCESS ||
                vkCreateFence(logical_device, &fence_info, nullptr, &in_flight_fences[i]) != VK_SUCCESS) {
                    throw std::runtime_error("failed to create semaphores.");
                };
            }
        };

        // This is where the shenanigans happens.
        void Vulkan_UpdateUniformBuffer(uint32_t current_image) {
            //code.
            UniformBufferObjects_t ubo;
            static float m_flUpdater = 0.0f;
            m_flUpdater += 0.1f;

            ubo.projection = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_flUpdater), glm::vec3(0.0f, 0.0f, 1.0f));

            void* data;
            vkMapMemory(logical_device, uniform_buffers_memory[current_image], 0, sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(logical_device, uniform_buffers_memory[current_image]);
        };

        void Vulkan_DrawFrame() {
            vkWaitForFences(logical_device, 1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

            uint32_t image_index;
            VkResult res = vkAcquireNextImageKHR(logical_device, swap_chain, UINT64_MAX, image_available_semaphore[current_frame], VK_NULL_HANDLE, &image_index);

            if (res == VK_ERROR_OUT_OF_DATE_KHR) {
                Vulkan_RecreateSwapchain();
                return;
            } else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
                throw std::runtime_error("failed to acquire swap chain image!");
            };

            Vulkan_UpdateUniformBuffer(current_frame);

            vkResetFences(logical_device, 1, &in_flight_fences[current_frame]);
           
            vkResetCommandBuffer(command_buffers[current_frame], 0);
            Vulkan_RecordCommandBuffer(command_buffers[current_frame], image_index);

            VkSubmitInfo submit_info{};
            submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            
            VkSemaphore wait_semaphores[] = { image_available_semaphore[current_frame] };
            VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
            submit_info.waitSemaphoreCount = 1;
            submit_info.pWaitSemaphores = wait_semaphores;
            submit_info.pWaitDstStageMask = wait_stages;
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &command_buffers[current_frame];

            VkSemaphore signal_semaphores[] = { render_finished_semaphore[current_frame] };
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = signal_semaphores;

            VkResult result = vkQueueSubmit(graphics_queue, 1, &submit_info, in_flight_fences[current_frame]);
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to submit draw command buffer.");

            VkPresentInfoKHR present_info{};
            present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores = signal_semaphores;

            VkSwapchainKHR swap_chains[] = {swap_chain};
            present_info.swapchainCount = 1;
            present_info.pSwapchains = swap_chains;
            present_info.pImageIndices = &image_index;
            present_info.pResults = nullptr; // opt?

            VkResult res2 = vkQueuePresentKHR(present_queue, &present_info);

            if (res2 == VK_ERROR_OUT_OF_DATE_KHR || res2 == VK_SUBOPTIMAL_KHR || framebuffer_resized) {
                framebuffer_resized = false;
                Vulkan_RecreateSwapchain();
            } else if (res2 != VK_SUCCESS) {
                throw std::runtime_error("failed to present swap chain image!");
            };

            current_frame = ( current_frame + 1 ) % max_frames_in_flight;
        };

        void Vulkan_CleanSwapchain() {
            for( auto framebuffer : swap_chain_frame_buffers )
                vkDestroyFramebuffer(logical_device, framebuffer, nullptr);

            for( auto imageview : swap_chain_image_views )
                vkDestroyImageView(logical_device, imageview, nullptr);

            vkDestroySwapchainKHR(logical_device, swap_chain, nullptr);
        };

        void Vulkan_RecreateSwapchain() {
            int _w = 0, _h = 0;
            glfwGetFramebufferSize(window, &_w, &_h);
            while (_w == 0 || _h == 0) {
                glfwGetFramebufferSize(window, &_w, &_h);
                glfwWaitEvents();
            };

            vkDeviceWaitIdle(logical_device);

            // clean the previous swapchain.
            Vulkan_CleanSwapchain();
            
            // Create a new swap chain
            Vulkan_CreateSwapChain();

            // Create a new image views
            Vulkan_CreateImageViews();

            // Create framebuffers
            Vulkan_CreateFramebuffers();
        };

        void Vulkan_RegisterGlfwCallbacks() {
            glfwSetWindowUserPointer(window, this);
            glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        };

        void Vulkan_CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& buffer_memory) {
            VkBufferCreateInfo buffer_info = {};
            buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            buffer_info.size = size;
            buffer_info.usage = usage;
            buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VkResult result = vkCreateBuffer(logical_device, &buffer_info, nullptr, &buffer);
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to create buffer.");

            VkMemoryRequirements memory_reqs = {};
            vkGetBufferMemoryRequirements(logical_device, buffer, &memory_reqs);

            VkMemoryAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            alloc_info.allocationSize = memory_reqs.size;
            alloc_info.memoryTypeIndex = Vulkan_FindMemoryType(memory_reqs.memoryTypeBits,
                properties);

            VkResult result2 = vkAllocateMemory(logical_device, &alloc_info, nullptr, &buffer_memory);
            if(result2 != VK_SUCCESS)
                throw std::runtime_error("failed to allocate buffer memory.");

            vkBindBufferMemory(logical_device, buffer, buffer_memory, 0);
        };

        void Vulkan_CopyBuffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size) {
            VkCommandBufferAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            alloc_info.commandPool = command_pool;
            alloc_info.commandBufferCount = 1;

            VkCommandBuffer command_buffer;
            vkAllocateCommandBuffers( logical_device, &alloc_info, &command_buffer);

            // Maybe the wrong place to put this in?
            // Pretty sure this should be placed inside the RecordCommandBuffer function.
            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(command_buffer, &begin_info);
            
            VkBufferCopy copy_region = {};
            copy_region.srcOffset = 0;
            copy_region.dstOffset = 0;
            copy_region.size = size;
            vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

            vkEndCommandBuffer(command_buffer);

            VkSubmitInfo submit_info = {};
            submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &command_buffer;

            vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
            vkQueueWaitIdle(graphics_queue);

            vkFreeCommandBuffers(logical_device, command_pool, 1, &command_buffer);
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

        static std::vector<char> ReadFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }
    public:
        bool framebuffer_resized = false;
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugmessenger; // brev duva.
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        VkDevice logical_device = VK_NULL_HANDLE;
        VkQueue graphics_queue = VK_NULL_HANDLE;
        VkQueue present_queue = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkSwapchainKHR swap_chain = VK_NULL_HANDLE;
        VkFormat swap_chain_image_format;
        VkExtent2D swap_chain_extent;
        VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
        VkRenderPass render_pass = VK_NULL_HANDLE;
        VkPipeline graphics_pipeline = VK_NULL_HANDLE;
        VkCommandPool command_pool = VK_NULL_HANDLE;
        VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;

        VkBuffer vertex_buffer = VK_NULL_HANDLE;
        VkDeviceMemory vertex_buffer_memory = VK_NULL_HANDLE;

        VkBuffer index_buffer = VK_NULL_HANDLE;
        VkDeviceMemory index_buffer_memory = VK_NULL_HANDLE;

        VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;

        //VkCommandBuffer command_buffer;
        
        //VkFence in_flight_fence;

        std::vector<VkFence> in_flight_fences;
        std::vector<VkSemaphore> image_available_semaphore;
        std::vector<VkSemaphore> render_finished_semaphore;
        std::vector<VkCommandBuffer> command_buffers;
        std::vector<VkImage> swap_chain_images;
        std::vector<VkImageView> swap_chain_image_views;
        std::vector<VkFramebuffer> swap_chain_frame_buffers;
        std::vector<VkBuffer> uniform_buffers;
        std::vector<VkDeviceMemory> uniform_buffers_memory;
        std::vector<VkDescriptorSet> descriptor_sets;


        uint32_t current_frame = 0;
    private:
        GLFWwindow* window;

        const uint32_t viper_width = 800;
        const uint32_t viper_height = 600;
        const int max_frames_in_flight = 2;
    };
};

#define assert_vk_res( res ) if( res != VK_SUCCESS ) \
    throw std::runtime_error("error.");

int main() {
    if(!glfwInit()) {
        printf("unable to setup environment.\n");
        return -1;
    };

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* m_Window = glfwCreateWindow(800, 600, "Viper Engine - Renderer :: Vulkan (x64)", nullptr, nullptr);
    if(!m_Window) {
        printf("unable to create window.\n");
        return -1;
    };

    Viper::VkViper* viper_vulkan = new Viper::VkViper(m_Window);
    viper_vulkan->Vulkan_Init();
    while(!glfwWindowShouldClose(m_Window)) {
        glfwPollEvents();
        viper_vulkan->Vulkan_DrawFrame();
    };
    //vkDestroyDescriptorPool(viper_vulkan->logical_device, imguiPool, nullptr);
    viper_vulkan->Vulkan_Deinit();

    delete viper_vulkan;
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    return 0;
};