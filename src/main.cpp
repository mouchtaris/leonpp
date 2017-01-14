#include "rxcpp/rx.hpp"
#include "caf/all.hpp"

#include <iostream>
#include <string>

namespace util {

    template <typename ...Args>
    struct replies_to {
        template <typename ...Results>
        struct with_impl {
            using type = typename caf::replies_to<Args...>::template with<Results...>;
        };

        template <typename ...Results>
        struct with_impl <std::tuple<Results...>> {
            using type = typename with_impl<Results...>::type;
        };

        template <typename ...R>
        using with = typename with_impl<R...>::type;
    };

}

#define attr(NAME, INDEX) auto& NAME (const mtype& obj) { return std::get<INDEX>(obj); }
namespace message {
    namespace PackageRequest {
        using mtype = std::tuple<std::string>;
        attr(url, 0);
    }

    namespace CacheResult {
        using mtype = std::string;
    }
}

namespace atom {
    using get = caf::atom_constant<caf::atom("get")>;
}

namespace actor {
    using Controller = caf::typed_actor<
        caf::reacts_to<atom::get, message::PackageRequest::mtype>
    >;

    using Cache = caf::typed_actor<
        util::replies_to<atom::get, message::PackageRequest::mtype>::with<message::CacheResult::mtype>
    >;
}

namespace behavior {
    actor::Cache::behavior_type Cache (actor::Cache::pointer self) {
        using namespace message;
        using namespace PackageRequest;
        using namespace CacheResult;
        return {
            [=](atom::get, PackageRequest::mtype req) {
                return CacheResult::mtype { "Hello bob " };
            }
        };
    }

    actor::Controller::behavior_type Controller (actor::Controller::pointer self, actor::Cache const& bob) {
        using namespace message;
        using namespace PackageRequest;
        using namespace CacheResult;
        return {
            [=](atom::get, PackageRequest::mtype req) {
                auto aout = caf::aout(self);
                aout << "Somebody is asking for " << url(req) << "\n";
                aout << "... Send to Bob: ...\n";
                self->request(bob, std::chrono::seconds(1), atom::get::value, req).then([=](std::string data) -> void {
                    auto aout = caf::aout(self);
                    aout << "... ... Bob says: " << data << "\n"; // .value_or(" I AINT SAYIN SHIT BITCH") << "\n";
                });
            }
        };
    }
}

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

    caf::actor_system_config cfg;
    caf::actor_system sys { cfg };
    caf::scoped_actor ak { sys };

    actor::Cache cache = sys.spawn(behavior::Cache);
    actor::Controller controller = sys.spawn(behavior::Controller, cache);
    ak->send(controller, atom::get::value, message::PackageRequest::mtype { "Hello bob" } );

    return 0;
}
