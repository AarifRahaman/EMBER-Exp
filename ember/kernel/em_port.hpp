#ifndef __EMBER_PORT_HPP__
#define __EMBER_PORT_HPP__

#include <sstream>
#include <tuple>

#include "em_iport.hpp"

namespace ember
{

// Bidirectional Port
template <typename T>
class Port : public IPort<T>
{
protected:
    bool isBinded;
    bool selfBinded;
    std::string name;

    T *data;

public:
    const char *id();
    void bind(IPort<T> &other);  // Bind to a different port
    void bind();                 // For self binding
    void write(const T &value);  // write to port
    const T &read() const;       // read from port

    Port(const char *portId);
    template <typename... Args>
    Port(const char *portId, Args... _args);
    Port(const std::string portId);
    template <typename... Args>
    Port(const std::string portId, Args... _args);
    ~Port();

    Port(const Port&) = delete;
    Port& operator=(const Port&) = delete;
    Port(Port&& other) noexcept;
    Port& operator=(Port&& other) noexcept;
};

// Aliases
template<typename T>
using port    = Port<T>;

template<typename T>
using inPort  = Port<T>;

template<typename T>
using outPort = Port<T>;

} // namespace ember

#include "tpp/em_port.tpp"

#endif
