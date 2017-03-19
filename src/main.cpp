#include "rxcpp/rx.hpp"
#include <deque>
#include <thread>
#include <string>
#include <thread>
#include <algorithm>
#include <utility>
#include <functional>
#include <type_traits>
#include <array>
#include <tuple>

using std::begin;
using std::end;

template <typename A, typename B> auto fmap(std::deque<A> fa, std::function<B(A)> f) -> std::deque<B>
{
    std::deque<B> fb { fa.size(), typename std::deque<B>::allocator_type { } };

    std::transform(begin(fa), end(fa), begin(fb), f);

    return fb;
}

template <typename T> using Seq = std::deque<T>;

int main(int, char**) {

    auto wat = Seq<int> { 1, 2, 3, 4, 5, 6 };

    auto watt = fmap(wat, std::function<auto (int) -> std::future<int>> { [] (auto i) {
        std::promise<int> p;
        p.set_value(i);
        return p.get_future();
    }});

    for (auto& omj: watt) {
        omj.wait();
        std::cout << omj.get() << "\n";
    }

    return 0;
}
