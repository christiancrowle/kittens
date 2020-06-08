#include <iostream>

#include "SDL.h"
//#undef main

#include "aixlog.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_sdl.h"

#include "kittensinfo.h"

//#include "duktape_glue.h"
//#include "configfile.h"

#include "chaiscript_glue.h"
#include "logging.h"
#include "settings.h"
#include "util.h"

#include "synth-wavsample.h"

#include "imgui/imnodes.h"
#include "imguichain.h"
#include "mixer.h"

#include "globalstate.h"

#define FRAMES_PER_SECOND 30
#define FPS_INTERVAL 1.0

int main() {
    Kittens::Logging::initialize_logging("logfile.log");

    Kittens::Info info;

    LOG(INFO) << "kittens " << info.KITTENS_VERSION << " starting\n";

    Kittens::ChaiScript::initialize_config("main.cha");

    LOG(INFO) << "starting SDL\n";
    LOG(INFO) << "\t(sdl_init)\n";
    SDL_Init(SDL_INIT_EVERYTHING);

    LOG(INFO) << "\t(window)\n";
    SDL_Window* window =
        SDL_CreateWindow(info.WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         Kittens::GlobalSettings["window_width"].get_value<int>(),
                         Kittens::GlobalSettings["window_height"].get_value<int>(), SDL_WINDOW_RESIZABLE);

    LOG(INFO) << "\t(renderer)\n";
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    LOG(INFO) << "\t(imgui-sdl)\n";
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, Kittens::GlobalSettings["window_width"].get_value<int>(),
                         Kittens::GlobalSettings["window_height"].get_value<int>());
    imnodes::Initialize();

    LOG(INFO) << "\t(texture)\n";
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 100, 100);
    {
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);
    }

    LOG(INFO) << "done!\n";

    LOG(INFO) << "kittens " << info.KITTENS_VERSION << " started!\n";

    Kittens::Imgui::ImguiChain chain;
    Kittens::Instrument::SynthWavSample sample = Kittens::Instrument::SynthWavSample("audio_files/Low E.wav");
    chain.set_synth(&sample);

    Kittens::Core::Mixer mixer = Kittens::Core::Mixer(sample.get_sps(), sample.get_channels());
    mixer.append_chain(chain);

    mixer.start();
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
                    if (e.key.keysym.sym == SDLK_LCTRL) {
                        Kittens::GlobalState::IsParameterControlMode = (e.type == SDL_KEYDOWN);
                        if (Kittens::GlobalState::IsParameterControlMode)
                            SDL_SetRelativeMouseMode(SDL_TRUE);
                        else
                            SDL_SetRelativeMouseMode(SDL_FALSE);
                    }
                    int key = e.key.keysym.scancode;
                    IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
                    io.KeysDown[key] = (e.type == SDL_KEYDOWN);
                    io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
                    io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
                    io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
                    io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
                }
            }
        }

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
        if (frames == 60) {
            frames = 0;
            sample.restart();
        }

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

        ImGui::Begin("node editor");
        imnodes::BeginNodeEditor();

        chain.Render();

        imnodes::EndNodeEditor();
        ImGui::End();

        SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
        SDL_RenderClear(renderer);

        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());

        draw_xy_pad_rect();

        SDL_RenderPresent(renderer);
    }
    mixer.stop();

    LOG(INFO) << "kittens " << info.KITTENS_VERSION << " shutting down... \n";
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
