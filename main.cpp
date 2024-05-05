#include <functional>
#include <iostream>
#include <map>
#include <any>

// template <typename T, typename RetTypeSig, typename RetTypeSlot, typename R, typename... Types>
// static inline void connect(T *sender, RetTypeSig (T::*signal)(Types...), R *receiver, RetTypeSlot (R::*slot)(Types...)) {
//     if (sender && signal && receiver && slot) {
//         auto signalFunction = std::bind(signal, sender);
//         auto slotFunction = std::bind(slot, receiver);

//     }
// }

template<typename... Args>
class Signal {
public:
    void operator()(Args... args) {
        for (auto& slot : slots) {
            try {
                std::any_cast<std::function<void(Args...)>>(slot.second)(std::forward<Args>(args)...);
            } catch (const std::bad_any_cast& e) {
                std::cerr << "Bad any cast: " << e.what() << std::endl;
            }
        }
    }

    template<typename T>
    void connect(T* object, void (T::*slot)(Args...)) {
        std::function<void(Args...)> f = std::bind_front(slot, object);
        slots[id++] = f;
    }

    template<typename R>
    void connect(R* object, std::function<void(Args...)> slot) {
        slots[id++] = slot;
    }

    template<typename T>
    void disconnect(T* object, void (T::*slot)(Args...)) {
        std::function<void(Args...)> f = std::bind_front(slot, object);
        size_t address = getAddress(f);
        for (auto it = slots.begin(); it != slots.end(); ++it) {
            std::cout << "Looking for slot to disconnect " << std::endl;
            try {
                std::function<void(Args...)> slot = std::any_cast<std::function<void(Args...)>>(it->second);
                if (address == getAddress(slot)) {
                    std::cout << "Disconnecting slot"<< std::endl;
                    slots.erase(it);
                    break;
                }
            } catch (const std::bad_any_cast& e) {
                std::cerr << "Bad any cast: " << e.what() << std::endl;
            }
        }
    }

private:
    std::map<int, std::any> slots;
    int id = 0;

    template<typename T, typename... U>
    size_t getAddress(std::function<T(U...)> f) {
        std::cout << "Getting address of function" << std::endl;
        typedef T(fnType)(U...);
        fnType ** fnPointer = f.template target<fnType*>();
        return (size_t) *fnPointer;
    }
};

class MyClass {
public:
    void mySlot(int a, int b) {
        std::cout << "My slot called with: " << a << " " << b << std::endl;
    }

    Signal<int,int> mySignal;
};

template <typename RetTypeSlot, typename R, typename... Types>
static inline void connect(Signal<Types...> *signal, R *receiver, RetTypeSlot (R::*slot)(Types...)) {
    signal->connect(receiver, slot);
}

template <typename RetTypeSlot, typename R, typename... Types>
static inline void disconnect(Signal<Types...> *signal, R *receiver, RetTypeSlot (R::*slot)(Types...)) {
    signal->disconnect(receiver, slot);
}

int main() {
    MyClass obj;

    connect(&obj.mySignal, &obj, &MyClass::mySlot);

    obj.mySignal(2,2);

    disconnect(&obj.mySignal, &obj, &MyClass::mySlot);

    obj.mySignal(3,3);
    return 0;
}
