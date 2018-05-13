#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <algorithm>
#include "async.h"
#include "bulk.h"

namespace async {

handle_t connect(std::size_t bulk_){
    auto h = std::make_shared<bulk::Handle>(bulk_);
    handlers.emplace(h);
    return static_cast<handle_t>(h.get());
};

void receive(handle_t handle, const char *data, std::size_t size){
    auto x = reinterpret_cast<bulk::Handle*>(handle);
    x->readString(data, size);
};

void disconnect(handle_t handle) {
    auto x = std::find_if(handlers.begin(), handlers.end(), [&handle](decltype(*handlers.begin()) i){return i.get() == handle;} );
    x->get()->lastBulk();
    handlers.erase(x);
}

void receive(handle_t handle, std::string line){
    auto x = reinterpret_cast<bulk::Handle*>(handle);
    x->readString(line);
};

void receive(handle_t handle, bulk_message bulk_){
    auto x = reinterpret_cast<bulk::Handle*>(handle);
    bulk_.push_back("}");
    for(auto msg : bulk_){
        x->readString(msg);
    }
}

void last_session_bulk(handle_t handle){
    auto x = reinterpret_cast<bulk::Handle*>(handle);
    x->lastBulk();
}

};
