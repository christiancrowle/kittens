//
// Created by devbat on 6/8/20.
//

#include <map>
#include <string>
#include "Parameter.h"

#ifndef KITTENS_DEVICEBASE_H
#define KITTENS_DEVICEBASE_H

namespace Kittens::Core {
class DeviceBase {
   public:
    virtual std::string get_name() = 0;
    std::map<std::string, Parameter> params;

    virtual void queue() = 0;
    virtual void stop() = 0;

   protected:
    std::string name;
};
}  // namespace Kittens::Core

#endif  // KITTENS_DEVICEBASE_H
