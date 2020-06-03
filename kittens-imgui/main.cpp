#include <iostream>

#include "SDL.h"
//#undef main

#include "imgui/imgui.h"
#include "imgui/imgui_sdl.h"
#include "aixlog.hpp"

#include "kittensinfo.h"

//#include "duktape_glue.h"
//#include "configfile.h"

#include "chaiscript_glue.h"
#include "settings.h"
#include "logging.h"
#include "util.h"

#include "synth-wavsample.h"

#include "imgui/imnodes.h"
#include "imguichain.h"
#include "mixer.h"

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
    SDL_Window* window = SDL_CreateWindow(info.WINDOW_TITLE.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            std::get<int>(Kittens::GlobalSettings["window_width"].get_value()),
            std::get<int>(Kittens::GlobalSettings["window_height"].get_value()),
            SDL_WINDOW_RESIZABLE);

    LOG(INFO) << "\t(renderer)\n";
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    LOG(INFO) << "\t(imgui-sdl)\n";
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer,
            std::get<int>(Kittens::GlobalSettings["window_width"].get_value()),
            std::get<int>(Kittens::GlobalSettings["window_height"].get_value()));
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
    Uint32 fps_lasttime = SDL_GetTicks(); //the last recorded time.
    Uint32 fps_current; //the current FPS.
    Uint32 fps_frames = 0;
    while (run)
    {
        ImGuiIO& io = ImGui::GetIO();

        int wheel = 0;

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) run = false;
            else if (e.type == SDL_WINDOWEVENT)
            {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    io.DisplaySize.x = static_cast<float>(e.window.data1);
                    io.DisplaySize.y = static_cast<float>(e.window.data2);
                }
            }
            else if (e.type == SDL_MOUSEWHEEL)
            {
                wheel = e.wheel.y;
            }
        }

        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

        // Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)

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
        if (fps_lasttime < SDL_GetTicks() - FPS_INTERVAL*1000) {
            fps_lasttime = SDL_GetTicks();
            fps_current = fps_frames;
            fps_frames = 0;
        }

        ImGui::Begin("Stats");
        ImGui::Text("%u", fps_current);
        ImGui::End();

        //ImGui::ShowDemoWindow();

        //ImGui::Begin("Image");
        //ImGui::Image(texture, ImVec2(100, 100));
        //ImGui::End();

        ImGui::Begin("node editor");
        imnodes::BeginNodeEditor();

        chain.Render();

        imnodes::EndNodeEditor();
        ImGui::End();

        SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
        SDL_RenderClear(renderer);

        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());

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
