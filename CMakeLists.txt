cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs")

project (Kittens)

add_subdirectory(extern/Q)

set(IMGUI_SOURCES
"extern/imgui/imgui.cpp"
"extern/imgui/imgui_demo.cpp"
"extern/imgui/imgui_draw.cpp"
"extern/imgui/imgui_sdl.cpp"
"extern/imgui/imgui_widgets.cpp"
        "extern/imgui/imnodes.cpp")

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

add_library(libKittens
        libKittens/Processor.h
        libKittens/SynthBase.h
    libKittens/mixer.cpp
    libKittens/mixer.h
    libKittens/chain.cpp
    libKittens/chain.h
        libKittens/parameter.cpp
        libKittens/Parameter.h

    libKittens/synth-wavsample.cpp
    libKittens/synth-wavsample.h

    libKittens/kittensinfo.h)

target_link_libraries(libKittens libqio
   libq
   ${PORTMIDI_LIBRARY}
   ${PORTAUDIO_LIBRARY}
   ImGui
   ${SDL2_LIBRARIES})

add_executable(kittens-imgui
    extern/duktape/duktape.c
    extern/duktape/duktape.h
    extern/duktape/duk_config.h

    kittens-imgui/main.cpp

    kittens-imgui/settings.cpp
    kittens-imgui/settings.h

    kittens-imgui/chaiscript_glue.cpp
    kittens-imgui/chaiscript_glue.h

    kittens-imgui/logging.cpp
    kittens-imgui/logging.h kittens-imgui/util.cpp kittens-imgui/util.h kittens-imgui/imguichain.cpp kittens-imgui/imguichain.h)

include_directories(extern/)

include_directories(libKittens/)

target_link_libraries(kittens-imgui libqio
        libq
        ${PORTMIDI_LIBRARY}
        ${PORTAUDIO_LIBRARY}
        ImGui
        ${SDL2_LIBRARIES}
        libKittens
        dl)
