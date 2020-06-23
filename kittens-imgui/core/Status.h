//
// Created by devbat on 6/8/20.
//

#ifndef KITTENS_STATUS_H
#define KITTENS_STATUS_H

#include "clock.h"

namespace Kittens::Imgui::Status {
void RenderStatusBar();
}  // namespace Kittens::Imgui::Status

namespace Kittens::Status {
extern Kittens::Core::Clock clock;
}
#endif  // KITTENS_STATUS_H
