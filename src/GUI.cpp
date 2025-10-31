#include "GUI.hpp"
#include <vector>
#include <string>
#include "Utils.hpp"

void GUI::render(AppState &state)
{
    if (ImGui::BeginMainMenuBar())
    {
        renderMenuBar(state);
        ImGui::EndMainMenuBar();
    }

    ImVec2 remainingSize = ImVec2(state.mainWindowSize.x, state.mainWindowSize.y - 24);
    ImVec2 mainImGuiWindowPos(0, state.mainWindowSize.y - remainingSize.y);
    ImVec2 mainImGuiWindowSize = remainingSize;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::SetNextWindowPos(mainImGuiWindowPos);
    ImGui::SetNextWindowSize(mainImGuiWindowSize);

    if (ImGui::Begin("main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        renderMain(state);
        ImGui::End();
    }

    ImGui::PopStyleVar();
}

void GUI::renderMenuBar(AppState &state)
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Exit", "Alt+F4"))
        {
            // Signal the application to exit
            state.progamShouldExit = true;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("About"))
    {
        ImGui::MenuItem("Version: ", state.version ? state.version->m_version.c_str() : "unknown", nullptr, false);
        ImGui::EndMenu();
    }
}

void GUI::renderMain(AppState &state)
{
    
}