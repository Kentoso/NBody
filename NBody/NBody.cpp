#include <iostream>

#include "raylib.h"
// #ifdef _MSC_VER
// #define _CRT_SECURE_NO_WARNINGS
// #endif
#include <imgui.h>
#include <imgui_impl_opengl3.h>
// #define RAYGUI_IMPLEMENTATION
// #include "raylib/raygui.h"

#include <Eigen/Dense>
#include <cmath>

#include "src/dearimgui.h"
#define NUMBER_OF_BODIES 3


int main(void)
{
    double V[3] = {5., 40., 10.};
    double d[3] = {1., 10e5, 5.,};
    double m[3];
    for (int i = 0; i < NUMBER_OF_BODIES; ++i)
    {
        m[i] = V[i] * d[i];
    }
    // double G = 10e-5;
    double G = 10e-5;
    double epsilon = 10e-1;
    Eigen::MatrixXd p {
        {-100, 0},
        {0, 0},
        {200, 0}
    };
    Eigen::MatrixXd v {
        {-0.5, 2.5},
        {0, 0},
        {0, -3}
    };
    Eigen::MatrixXd a {
            {0, 0},
            {0, 0},
            {0, 0}
    };
    
    const int screenWidth = 1000;
    const int screenHeight = 600;
    int mapSize = 500;
    InitWindow(screenWidth, screenHeight, "n-body");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    initDearImGui(screenWidth, screenHeight);

    bool show = true;
    
    SetTargetFPS(60); 
    Eigen::MatrixXd D;
    Eigen::VectorXd norms(NUMBER_OF_BODIES);
    Camera2D camera;
    camera.offset = Vector2 {screenWidth / 2, screenHeight / 2};
    // camera.target = Vector2 {screenWidth / 2, screenHeight / 2};
    camera.target = Vector2 {0, 0};
    camera.rotation = 0.f;
    camera.zoom = 1.0f;
    
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (IsWindowResized())
        {
            float screenH = GetScreenHeight();
            float screenW = GetScreenWidth();
            std::cout << screenH << std::endl;
            camera.offset = Vector2 {screenW / 2, screenH / 2};
            // camera.target = Vector2 {screenW / 2, screenH / 2};
        }
        updateDearImGui(show);
        
        for (int simIter = 0; simIter < 30; ++simIter)
        {
            D = -2 * p * p.transpose();
            for (int i = 0; i < NUMBER_OF_BODIES; ++i)
            {
                norms(i) = p.row(i) * p.row(i).transpose();
            }
            D.colwise() += norms;
            D.rowwise() += norms.transpose();
            // float deltaT = GetFrameTime() / 2;
            float deltaT = GetFrameTime();
        
            for (int i = 0; i < NUMBER_OF_BODIES; ++i)
            {
                v.row(i) = v.row(i) + a.row(i) * deltaT / 2;
                p.row(i) = p.row(i) + v.row(i) * deltaT;
                for (int j = 0; j < NUMBER_OF_BODIES; ++j)
                {
                    if (i == j) continue;
                    double otherMass = m[j];
                    a.row(i) += otherMass * (p.row(j) - p.row(i)) / pow(D(i, j) + epsilon * epsilon, 1.5);
                }
                a.row(i) *= G;
                v.row(i) = v.row(i) + a.row(i) * deltaT / 2;
            }
        }
        if (IsKeyDown(KEY_LEFT_CONTROL))
        {
            camera.zoom += GetMouseWheelMove() / 10.f;
            if (camera.zoom <= 0) camera.zoom = 0.1f;
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        for (int i = 0; i < NUMBER_OF_BODIES; ++i)
        {
            DrawCircle(p(i, 0), p(i, 1), V[i], DARKGRAY);
        }
        EndMode2D();
        DrawText(std::to_string(GetFrameTime()).c_str(), 0, 0, 14, BLACK);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
