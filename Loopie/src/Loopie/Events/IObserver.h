#pragma once
#include <string>

namespace Loopie 
{
    class IObserver {
    public:
        virtual ~IObserver() = default;
        virtual void OnNotify(unsigned int id) = 0;
    };
}