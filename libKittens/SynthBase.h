/*
 * SynthBase.h
 *
 *  Created on: May 16, 2020
 *      Author: devbat
 */

#include <vector>
#include <map>
#include <string>

#include "Parameter.h"

#ifndef SYNTH_H_
#define SYNTH_H_

namespace Kittens::Core {
///
/// \brief The SynthBase interface. Base class for creating samples.
///
    class SynthBase {
    public:
        virtual float get_sample() = 0;

        virtual std::string get_name() = 0;

        virtual ~SynthBase() {}

        std::map<std::string, std::variant<std::string, int, float, bool, double>> params;
    protected:
        std::string name;
    };
}
#endif /* SYNTH_H_ */
