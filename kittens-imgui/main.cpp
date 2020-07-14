#include <iostream>

#include "SDL2/SDL.h"
//#undef main

#include "aixlog.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_sdl.h"

#include "misc/kittensinfo.h"

#include "portable-file-dialogs.h"

//#include "duktape_glue.h"
//#include "configfile.h"

#include "chaiscript/chaiscript_glue.h"
#include "misc/logging.h"
#include "misc/settings.h"
#include "misc/util.h"

#include "instrument/synth-wavsample.h"

#include "core/mixer.h"
#include "imgui/imnodes.h"

#include "chaiscript/chaiscript_console.h"
#include "core/Status.h"
#include "core/input.h"
#include "misc/globalstate.h"

#define FRAMES_PER_SECOND 30
#define FPS_INTERVAL 1.0

static bool ImGui_ImplSDL2_Init() {
    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;  // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |=
        ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendPlatformName = "imgui_impl_sdl";

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
    io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
    io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = SDL_SCANCODE_KP_ENTER;
    io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

    return true;
}

int main() {
    Kittens::Logging::initialize_logging("logfile.log");

    LOG(INFO) << "kittens " << Kittens::Info::KITTENS_VERSION << " starting\n";

    Kittens::ChaiScript::initialize_config("main.cha");

    LOG(INFO) << "starting SDL\n";
    LOG(INFO) << "\t(sdl_init)\n";
    SDL_Init(SDL_INIT_EVERYTHING);

    LOG(INFO) << "\t(window)\n";
    SDL_Window* window =
        SDL_CreateWindow(Kittens::Info::WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         Kittens::GlobalSettings["window_width"].get_value<int>(),
                         Kittens::GlobalSettings["window_height"].get_value<int>(), SDL_WINDOW_RESIZABLE);

    LOG(INFO) << "\t(renderer)\n";
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    LOG(INFO) << "\t(imgui-sdl)\n";
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, Kittens::GlobalSettings["window_width"].get_value<int>(),
                         Kittens::GlobalSettings["window_height"].get_value<int>());
    imnodes::Initialize();

    ImGui_ImplSDL2_Init();

    LOG(INFO) << "\t(texture)\n";
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 100, 100);
    {
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);
    }

    LOG(INFO) << "the clock too.\n";
    Kittens::Status::clock.start();

    LOG(INFO) << "oh yeah, also input\n";
    Kittens::Core::Input::InputHandler* inputHandler = nullptr;
    if (Kittens::GlobalSettings["serial_enabled"].get_value<bool>()) {
        inputHandler =
            new Kittens::Core::Input::InputHandler(Kittens::GlobalSettings["serial_port_name"].get_value<std::string>(),
                                                   Kittens::GlobalSettings["serial_port_baud"].get_value<int>());
    } else {
        try {
            inputHandler = new Kittens::Core::Input::InputHandler;
        } catch (std::exception e) {
        }
    }

    LOG(INFO) << "done!\n";

    LOG(INFO) << "kittens " << Kittens::Info::KITTENS_VERSION << " started!\n";

    Kittens::GlobalState::mixer.synths = {
        new Kittens::Instrument::SynthWavSample("audio_files/Low E.wav", SDL_SCANCODE_A),
        new Kittens::Instrument::SynthWavSample("audio_files/Low E-but lower.wav", SDL_SCANCODE_S)};

    // Kittens::GlobalState::mixer.Clear();

    LOG(INFO)
        << "calculated output latency: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(Kittens::GlobalState::mixer.output_latency()).count()
        << "\n";
    Kittens::GlobalState::mixer.start();

    bool run = true;
    long frames = 0;
    Uint32 fps_lasttime = SDL_GetTicks();  // the last recorded time.
    Uint32 fps_current;                    // the current FPS.
    Uint32 fps_frames = 0;
    bool ctrl = false;

    float x_clamped = 0.0f;
    float y_clamped = 0.0f;

    while (run) {
        ImGuiIO& io = ImGui::GetIO();

        int wheel = 0;

        Kittens::GlobalState::ParameterChangeX = 0;
        Kittens::GlobalState::ParameterChangeY = 0;

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: {
                    run = false;
                }
                case SDL_WINDOWEVENT: {
                    if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        io.DisplaySize.x = static_cast<float>(e.window.data1);
                        io.DisplaySize.y = static_cast<float>(e.window.data2);

                        Kittens::GlobalSettings["window_width"].set_value(e.window.data1);
                        Kittens::GlobalSettings["window_height"].set_value(e.window.data2);
                    }
                }
                case SDL_MOUSEWHEEL: {
                    wheel = e.wheel.y;
                }
                case SDL_MOUSEMOTION: {
                    if (Kittens::GlobalState::IsParameterControlMode) {
                        float x_clamped_new = std::clamp((static_cast<float>(e.motion.xrel) / 100), -1.0f, 1.0f);
                        float y_clamped_new = std::clamp((static_cast<float>(e.motion.yrel) / 100), -1.0f, 1.0f);

                        if (Kittens::GlobalSettings["sticky_xy"].get_value<bool>()) {
                            if (x_clamped_new != 0)
                                Kittens::GlobalState::ParameterChangeX = x_clamped_new;
                            if (y_clamped_new != 0)
                                Kittens::GlobalState::ParameterChangeY = y_clamped_new;
                        } else {
                            if (x_clamped_new != Kittens::GlobalState::ParameterChangeX)
                                Kittens::GlobalState::ParameterChangeX = x_clamped_new;

                            if (y_clamped_new != Kittens::GlobalState::ParameterChangeX)
                                Kittens::GlobalState::ParameterChangeY = y_clamped_new;
                        }

                        /*SDL_WarpMouseInWindow(window,
                                              Kittens::GlobalSettings["window_width"].get_value<int>() / 2,
                                              Kittens::GlobalSettings["window_height"].get_value<int>() / 2);*/
                        if (Kittens::GlobalSettings["verbose_logging"].get_value<bool>())
                            LOG(INFO) << x_clamped << " " << y_clamped << "\n";
                    }
                }
                case SDL_KEYDOWN:
                case SDL_KEYUP: {
                    inputHandler->HandleKeyboard(e, io);
                }
            }
        }

        if (Kittens::GlobalSettings["serial_enabled"].get_value<bool>())
            inputHandler->HandleSerial();

        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

        // Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write
        // to those fields from your Windows message loop handlers, etc.)

        io.DeltaTime = 1.0f / 60.0f;
        io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
        io.MouseWheel = static_cast<float>(wheel);

        ImGui::NewFrame();
        frames++;
        Kittens::Status::clock.tick();

        fps_frames++;
        if (fps_lasttime < SDL_GetTicks() - FPS_INTERVAL * 1000) {
            fps_lasttime = SDL_GetTicks();
            fps_current = fps_frames;
            fps_frames = 0;
        }

        ImGui::Begin("Stats");
        ImGui::Text("%u", fps_current);
        ImGui::Text("%d", ctrl);
        ImGui::End();

        // ImGui::ShowDemoWindow();

        // ImGui::Begin("Image");
        // ImGui::Image(texture, ImVec2(100, 100));
        // ImGui::End();

        auto draw_xy_pad_rect = [&]() {
            SDL_Rect rect;
            rect.x = 0;
            rect.y = 0;
            rect.w = Kittens::GlobalSettings["window_width"].get_value<int>() * 0.1;
            rect.h = Kittens::GlobalSettings["window_height"].get_value<int>() * 0.1;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);

            rect.x = (rect.w / 2) + (Kittens::GlobalState::ParameterChangeX * (rect.w / 2)) - 10;
            rect.y = (rect.h / 2) + (Kittens::GlobalState::ParameterChangeY * (rect.h / 2)) - 10;
            rect.w = 20;
            rect.h = 20;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        };

        // ImGui::Begin("node editor");
        // imnodes::BeginNodeEditor();

        // ImGui::ShowDemoWindow();

        Kittens::GlobalState::mixer.Render();

        // imnodes::EndNodeEditor();
        // ImGui::End();

        Kittens::Imgui::Status::RenderStatusBar();

        SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
        SDL_RenderClear(renderer);

        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());

        draw_xy_pad_rect();

        SDL_RenderPresent(renderer);
    }
    Kittens::GlobalState::mixer.stop();

    LOG(INFO) << "kittens " << Kittens::Info::KITTENS_VERSION << " shutting down... \n";
    ImGuiSDL::Deinitialize();

    LOG(INFO) << "shutting down sdl...\n";
    LOG(INFO) << "\t(renderer)\n";
    SDL_DestroyRenderer(renderer);
    LOG(INFO) << "\t(window)\n";
    SDL_DestroyWindow(window);

    LOG(INFO) << "\t(imgui-sdl)\n";
    ImGui::DestroyContext();

    LOG(INFO) << "done! have a lot of fun...\n";

    return 0;
}
