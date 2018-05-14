#include "async.h"


async::handle_t async::connect(std::size_t bulk_){
    auto h = std::make_shared<Handle>(bulk_);
    handlers.emplace(h);
    return static_cast<handle_t>(h.get());
};

void async::receive(async::handle_t handle, const char *data, std::size_t size){
    auto x = reinterpret_cast<async::Handle*>(handle);
    x->readString(data, size);
};

void async::disconnect(async::handle_t handle) {
    auto x = std::find_if(handlers.begin(), handlers.end(), [&handle](decltype(*handlers.begin()) i){return i.get() == handle;} );
    x->get()->lastBulk();
    handlers.erase(x);
};

void async::receive(async::handle_t handle, std::string line){
    auto x = reinterpret_cast<async::Handle*>(handle);
    x->readString(line);
};

void async::receive(async::handle_t handle, bulk_message bulk_){
    auto x = reinterpret_cast<async::Handle*>(handle);
    bulk_.push_back("}");
    for(auto msg : bulk_){
        x->readString(msg);
    }
};

void async::last_session_bulk(async::handle_t handle){
    auto x = reinterpret_cast<async::Handle*>(handle);
    x->lastBulk();
};
