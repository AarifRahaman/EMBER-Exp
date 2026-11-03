#pragma once

namespace ember {

class IObject {
  public:
    virtual const char* id() = 0;
};

} // namespace ember
