#include <string>

#include "aixlog.hpp"

#include "logging.h"

namespace Kittens::Logging {
    void initialize_logging(std::string logfile) {
        auto sink_cout = std::make_shared<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
        auto sink_file = std::make_shared<AixLog::SinkFile>(AixLog::Severity::trace, AixLog::Type::all, logfile);
        AixLog::Log::init({sink_cout, sink_file});
    }
}
