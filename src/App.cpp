#include "App.hpp"
#include "imgui.h"
#include <thread>
#include <chrono>
#include "Utils.hpp"
#include <filesystem>
#include <iostream>
#include <atomic>

#include "version.def"

App::App()
{
    std::string versionStr(reinterpret_cast<const char *>(____version),
                           static_cast<size_t>(____version_len));
    state.version = new Version(versionStr);
}

App::~App()
{
}

void App::init()
{
}

void App::render()
{
    state.mainWindowSize = renderer.getWindowSize();
    gui.render(state);
}

void App::keyCallback(const SDL_KeyboardEvent &keyEvent)
{
    if (keyEvent.type == SDL_KEYDOWN)
    {
        if (keyEvent.keysym.sym == SDLK_F4 && (keyEvent.keysym.mod & KMOD_ALT))
        {
            state.progamShouldExit = true;
        }
    }
}

int App::run(int argc, char **argv, std::filesystem::path logFile)
{
    state.logFile = logFile;
    state.renderer = &renderer;
    init();
    std::filesystem::path exeDir = Utils::getExecutableDir();
    try
    {
        std::filesystem::current_path(exeDir);
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        // If changing directory fails, log the error but continue
        std::cerr << "Warning: Could not change working directory to executable directory: " << e.what() << std::endl;
    }

    renderer.setRenderFunction(std::bind(&App::render, this));
    renderer.setKeyCallback(std::bind(&App::keyCallback, this, std::placeholders::_1));

    if (renderer.startRenderLoop(&state) != 0)
        return -1; // Failed to start render loop

    while (renderer.isRunning())
    {
        if (state.progamShouldExit)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout.flush();
    }

    renderer.join();

    return 0;
}