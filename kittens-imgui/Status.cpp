//
// Created by devbat on 6/8/20.
//

#include "Status.h"
#include "imgui/imgui.h"
#include "kittensinfo.h"
#include "settings.h"

namespace Kittens::Status {
Kittens::Core::Clock clock;
}

namespace Kittens::Imgui::Status {
void RenderStatusBar() {
    int windowWidth = Kittens::GlobalSettings["window_width"].get_value<int>();
    int windowHeight = Kittens::GlobalSettings["window_height"].get_value<int>();
    ImGui::SetNextWindowPos(ImVec2(0, windowHeight - 32));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, 32));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 255));

    ImGui::Begin("statusbar", __null, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    ImGui::Text("Kittens %s |", Kittens::Info::KITTENS_VERSION.c_str());
    // ImGui::SameLine();
    // ImGui::Text("Clock backup: %d", Kittens::Status::clock.get_pulses_since_last_ready());

    ImGui::End();

    ImGui::PopStyleColor();
}
}  // namespace Kittens::Imgui::Status