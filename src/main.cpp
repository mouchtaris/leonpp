#include "rxcpp/rx.hpp"
#include "caf/all.hpp"

#include <iostream>

int main(int, char**) {
    auto ints = rxcpp::observable<>::create<int>( [](rxcpp::subscriber<int> s) {
        s.on_next(1);
        s.on_next(2);
        s.on_next(3);
        s.on_completed();
    });

    ints.subscribe(
        [](int v) { std::cout << "OnNext: " << v << "\n"; },
        []() { std::cout << "OnCompleted: " << "\n"; }
    );

    return 0;
}
