#pragma once
#include "Loopie/Events/IObserver.h"

#include <vector>
#include <algorithm>
#include <string>

namespace Loopie
{
    class Event {
    public:
        void AddObserver(IObserver* obs);
        void RemoveObserver(IObserver* obs);

        void Notify(unsigned int id) const;

    private:
        std::vector<IObserver*> observers;
    };
}