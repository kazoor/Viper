#include <windows.h>
#include <iostream>
#include <viper/base.hpp>
#include <main.hpp>

int main() {
    API::APIWindow* n = new API::APIWindow();
    n->Init();
    return 0;
}