cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs ")

project(Kittens)

add_subdirectory(extern/Q)
add_subdirectory(extern/RtMidi17)

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

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

add_library(ImGui ${IMGUI_SOURCES} ${IMGUI_HEADERS})

add_executable(kittens-imgui
        kittens-imgui/core/Processor.h
        kittens-imgui/core/SynthBase.h
        kittens-imgui/core/mixer.cpp
        kittens-imgui/core/mixer.h
        kittens-imgui/core/parameter.cpp
        kittens-imgui/core/Parameter.h

        kittens-imgui/misc/kittensinfo.h kittens-imgui/core/SynthBase.cpp kittens-imgui/core/Processor.cpp kittens-imgui/core/clock.cpp kittens-imgui/core/clock.h

        extern/duktape/duktape.c
        extern/duktape/duktape.h
        extern/duktape/duk_config.h

        kittens-imgui/main.cpp

        kittens-imgui/misc/settings.cpp
        kittens-imgui/misc/settings.h

        kittens-imgui/chaiscript/chaiscript_glue.cpp
        kittens-imgui/chaiscript/chaiscript_glue.h

        kittens-imgui/instrument/synth-wavsample.cpp
        kittens-imgui/instrument/synth-wavsample.h

        kittens-imgui/misc/logging.cpp

        kittens-imgui/misc/logging.h kittens-imgui/misc/util.cpp kittens-imgui/misc/util.h kittens-imgui/misc/globalstate.cpp kittens-imgui/misc/globalstate.h kittens-imgui/core/Status.cpp kittens-imgui/core/Status.h kittens-imgui/core/DeviceBase.cpp kittens-imgui/core/DeviceBase.h kittens-imgui/chaiscript/chaiscript_console.cpp kittens-imgui/chaiscript/chaiscript_console.h kittens-imgui/core/midicontrol.cpp kittens-imgui/core/midicontrol.h kittens-imgui/chaiscript/chaiscript_multiline_console.cpp kittens-imgui/chaiscript/chaiscript_multiline_console.h)

include_directories(extern/)
include_directories(extern/RtMidi17)

include_directories(libKittens/)

target_link_libraries(kittens-imgui libqio
        libq
        ${PORTMIDI_LIBRARY}
        ${PORTAUDIO_LIBRARY}
        ImGui
        RtMidi17
        ${SDL2_LIBRARIES}
        dl)
