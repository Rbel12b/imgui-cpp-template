#pragma once
#ifndef APP_STATE_H
#define APP_STATE_H

class AppState;

#include "imgui.h"
#include "SDL2/SDL.h"
#include <filesystem>
#include <string>
#include <vector>
#include <atomic>
#include <cstdint>
#include "Renderer.hpp"
#include "Version.hpp"

class AppState
{
public:
    Renderer* renderer;
    bool progamShouldExit = false;
    ImVec2 mainWindowSize = ImVec2(0, 0);
    std::filesystem::path logFile;
    Version* version = nullptr;
};

#endif // APP_STATE_H