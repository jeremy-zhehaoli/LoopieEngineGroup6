#include "Event.h"

namespace Loopie
{
    void Event::AddObserver(IObserver* obs) {
        observers.push_back(obs);
    }

    void Event::RemoveObserver(IObserver* obs) {
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void Event::Notify(unsigned int id) const {
        for (auto* obs : observers) {
            obs->OnNotify(id);
        }
    }
}