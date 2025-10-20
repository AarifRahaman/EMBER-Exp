#ifndef __EMBER_PORT_TPP__
#define __EMBER_PORT_TPP__


template<typename T>
const char* ember::Port<T>::id()
{
    return name.c_str();
}

template<typename T>
void ember::Port<T>::bind(ember::IPort<T>& other)
{
    if(this == &other) {
        if(!isBinded) {
            bind();
        }
        return;
    }

    if(isBinded) {
        std::stringstream error_message;
        error_message << name << " is already binded";
        throw std::logic_error(error_message.str());
    }

    // Downcasting
    ember::Port<T>* foo = dynamic_cast<ember::Port<T>*>(&other);

    if(foo->isBinded) {
        delete data;
        data = foo->data;
    }
    else {
        bind();
        foo->bind(*this);
    }
#ifdef DEBUG
        std::cout << "-I: Binding " << name << " to Port: " << other.id() << std::endl;
#endif

    isBinded = true;
}

template<typename T>
void ember::Port<T>::bind()
{
    if(isBinded) {
        std::stringstream error_message;
        error_message << name << " is already binded";
        throw std::logic_error(error_message.str());
    }

    isBinded = true;
    selfBinded = true;
}

template<typename T>
void ember::Port<T>::write(const T& value)
{
    assert(isBinded && " Cannot read from a not binded Port");

    *data = value;
#ifdef DEBUG
    std::cout << "-I: " << value << " written to " << name << std::endl;
#endif
}

template<typename T>
const T& ember::Port<T>::read() const
{
    assert(isBinded && " Cannot read from a not binded Port");

#ifdef DEBUG
    std::cout << "-I: Reading " << name << " " << std::flush;
    std::cout << *data << std::endl;
#endif

    return const_cast<T&>(*data);
}

// CTOR
template<typename T>
ember::Port<T>::Port(const char* _portId)
    : ember::IPort<T>::IPort(),
    name(_portId)
{
    selfBinded = false;
    isBinded   = false;
    
    data = new T;
}

template<typename T>
ember::Port<T>::Port(const std::string _portId)
    : ember::IPort<T>::IPort(),
    name(_portId)
{
    selfBinded = false;
    isBinded   = false;
    
    data = new T;
}

template<typename T>
template<typename... Args>
ember::Port<T>::Port(const char* _portId, Args... _args)
    : ember::IPort<T>::IPort(),
    name(_portId)
{
    selfBinded = false;
    isBinded   = false;
    
    data = new T(std::forward<Args>(_args)...);
}

template<typename T>
template<typename... Args>
ember::Port<T>::Port(const std::string _portId, Args... _args)
    : ember::IPort<T>::IPort(),
    name(_portId)
{
    selfBinded = false;
    isBinded   = false;
    
    data = new T(std::forward<Args>(_args)...);
}

template<typename T>
ember::Port<T>::~Port()
{
    if(selfBinded && data) { delete data; }
}

template<typename T>
ember::Port<T>::Port(Port&& other) noexcept
    : name(std::move(other.name)),
      data(other.data),
      isBinded(other.isBinded),
      selfBinded(other.selfBinded)
{
    other.data = nullptr;
    other.isBinded = false;
    other.selfBinded = false;
}

template<typename T>
ember::Port<T>& ember::Port<T>::operator=(Port<T>&& other) noexcept
{
    if (this != &other) {
        if (selfBinded && data) {
            delete data;
        }
        name = std::move(other.name);
        data = other.data;
        isBinded = other.isBinded;
        selfBinded = other.selfBinded;

        other.data = nullptr;
        other.isBinded = false;
        other.selfBinded = false;
    }
    return *this;
}

#endif
