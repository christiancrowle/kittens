cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs ")

project(Kittens)

add_subdirectory(extern/Q)

set(IMGUI_SOURCES
        "extern/imgui/imgui.cpp"
        "extern/imgui/imgui_demo.cpp"
        "extern/imgui/imgui_draw.cpp"
        "extern/imgui/imgui_sdl.cpp"
        "extern/imgui/imgui_widgets.cpp"
        "extern/imgui/imnodes.cpp"
        "extern/imgui/misc/cpp/imgui_stdlib.cpp")

set(IMGUI_HEADERS
        "extern/imgui/imconfig.h"
        "extern/imgui/imgui.h"
        "extern/imgui/imgui_internal.h"
        "extern/imgui/imstb_rectpack.h"
        "extern/imgui/imstb_textedit.h"
        "extern/imgui/imstb_truetype.h"
        "extern/imgui/imgui_sdl.h"
        "extern/imgui/imnodes.h")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_library(ImGui ${IMGUI_SOURCES} ${IMGUI_HEADERS})

add_executable(kittens-imgui
        kittens-imgui/Processor.h
        kittens-imgui/SynthBase.h
        kittens-imgui/mixer.cpp
        kittens-imgui/mixer.h
        kittens-imgui/chain.cpp
        kittens-imgui/chain.h
        kittens-imgui/parameter.cpp
        kittens-imgui/Parameter.h

        kittens-imgui/kittensinfo.h kittens-imgui/SynthBase.cpp kittens-imgui/Processor.cpp kittens-imgui/clock.cpp kittens-imgui/clock.h

        extern/duktape/duktape.c
        extern/duktape/duktape.h
        extern/duktape/duk_config.h

        kittens-imgui/main.cpp

        kittens-imgui/settings.cpp
        kittens-imgui/settings.h

        kittens-imgui/chaiscript_glue.cpp
        kittens-imgui/chaiscript_glue.h

        kittens-imgui/synth-wavsample.cpp
        kittens-imgui/synth-wavsample.h

        kittens-imgui/logging.cpp
        kittens-imgui/logging.h kittens-imgui/util.cpp kittens-imgui/util.h kittens-imgui/imguichain.cpp kittens-imgui/imguichain.h kittens-imgui/globalstate.cpp kittens-imgui/globalstate.h kittens-imgui/Status.cpp kittens-imgui/Status.h)

include_directories(extern/)

include_directories(libKittens/)

target_link_libraries(kittens-imgui libqio
        libq
        ${PORTMIDI_LIBRARY}
        ${PORTAUDIO_LIBRARY}
        ImGui
        ${SDL2_LIBRARIES}
        dl)
