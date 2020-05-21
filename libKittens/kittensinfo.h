#include <string>

#ifndef KITTENSINFO_H
#define KITTENSINFO_H

///
/// \brief A class containing constants such as library version, etc
///
class KittensInfo {
public:
    std::string const KITTENS_VERSION = "alpha";
    std::string const WINDOW_TITLE = "Kittens " + KITTENS_VERSION;
};

#endif // KITTENSINFO_H
