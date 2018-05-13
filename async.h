#pragma once

#include <cstddef>
#include <vector>
#include <set>
#include <memory>
#include <string>
#include "bulk.h"

namespace async {
using bulk_message = std::vector<std::string>;
using handle_t = void *;

std::set<std::shared_ptr<bulk::Handle>> handlers{};
int id = 0;

handle_t connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size);
void receive(handle_t handle, std::string cmd);
void receive(handle_t handle, bulk_message bulk_);
void last_session_bulk(handle_t);
void disconnect(handle_t handle);

}
