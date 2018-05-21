#pragma once

#include <cstddef>
#include <vector>
#include <set>
#include <memory>
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <boost/lexical_cast.hpp>

enum class Status{
    nothing, start, stop, last_bulk, start_dynamic
};

namespace async {

using bulk_message = std::vector<std::string>;
using handle_t = void *;

class CommandBlock;
class Observer{
public:
    Observer(std::shared_ptr<CommandBlock> cb_) : cb(cb_){};
    virtual void update(Status s) = 0;
    virtual ~Observer() = default;
    Observer(Observer && other) = default;
    std::shared_ptr<CommandBlock> cb;
};


class CommandBlock{
public:

    CommandBlock(long N_):status(Status::nothing),N(N_),dynamic(false) {};
    void notify();
    void subscribe(std::unique_ptr<Observer> obs);
    void setStatus(Status status);
    void appendCommnad(std::string& command);

    std::vector<std::string> commands;

private:
    std::vector<std::unique_ptr<Observer>> subs;
    Status status;
    const long N;
    bool dynamic;
};

class CoutObserver : public Observer{
public:
    CoutObserver(std::shared_ptr<CommandBlock> cb_): Observer(cb_){};
    ~CoutObserver() = default;
    void update(Status s) override;
};

class LogObserver : public Observer{
public:
    LogObserver(std::shared_ptr<CommandBlock> cb_) : Observer(cb_),
                        bulkCounter(0){};
    ~LogObserver() = default;
    void update(Status s) override;

private:
    int bulkCounter;
    std::string bulkBeginTime;
    std::string bulkFileName;
};

//Интерпретатор комманд
class Interpreter{
public:
    Interpreter(long N_);
    void readCommand(std::string &command);
    void lastBulk();

private:
    std::shared_ptr<CommandBlock> cb;
    long dynamic_counter;
};

class Handle{
public:
    Handle(int n) : interpreter(n){};
    void readString(const char * data, std::size_t size);
    void readString(std::string cmd);
    void lastBulk();
private:
    Interpreter interpreter;
    std::string stash;
};

static std::set<std::shared_ptr<Handle>> handlers{};
static int id = 0;

handle_t connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size);
void receive(handle_t handle, std::string cmd);
void receive(handle_t handle, bulk_message bulk_);
void last_session_bulk(handle_t);
void disconnect(handle_t handle);

}
