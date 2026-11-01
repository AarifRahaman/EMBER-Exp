#pragma once

#include <stdexcept>
#include <string>

namespace ember {

class NotImplementedError : public std::runtime_error {
public:
    explicit NotImplementedError(const std::string& message)
        : std::runtime_error(message) {}
};

class BitwidthMismatch : public NotImplementedError {
public:
    explicit BitwidthMismatch(const std::string& message)
        : ember::NotImplementedError(message) {}
};

class InvalidArgument : public NotImplementedError {
public:
    explicit InvalidArgument(const std::string& message)
        : ember::NotImplementedError(message) {}
};


} // namespace ember