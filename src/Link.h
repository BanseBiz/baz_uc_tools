#pragma once

#include <stdint.h>
#include <stddef.h>

class Link : public Client{
  public:
    virtual void begin() = 0;
    virtual void end() = 0;
    virtual bool isUp() = 0;
};