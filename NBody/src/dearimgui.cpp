#include "dearimgui.h"

#include <raylib.h>

void initDearImGui(int screenWidth, int screenHeight)
{
    ImGuiContext* context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = screenWidth;
    io.DisplaySize.y = screenHeight;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init();
}

void updateDearImGui(bool& show)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(&show);
    ImGuiIO & io = ImGui::GetIO();
    io.MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    io.MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    io.MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);
    io.MouseWheel += (float)GetMouseWheelMove();
    
    // io.DeltaTime = GetFrameTime();
    io.DisplaySize.x = (float)GetScreenWidth();
    io.DisplaySize.y = (float)GetScreenHeight();

    if (io.WantSetMousePos) {
        SetMousePosition((int)io.MousePos.x, (int)io.MousePos.y);
    } else {
        io.MousePos.x = GetMousePosition().x;
        io.MousePos.y = GetMousePosition().y;
    }
}