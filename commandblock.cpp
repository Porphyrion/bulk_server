#include "bulk.h"

void bulk::CommandBlock::notify(){
    for(auto& i : subs){
        i->update(status);
    }
    if(status == Status::stop) {
        commands.clear();
        if(dynamic) dynamic = false;
    }
};

void bulk::CommandBlock::subscribe(std::unique_ptr<Observer> obs){
    subs.push_back(std::move(obs));
};

void bulk::CommandBlock::setStatus(Status s){
    status = s;
    if(status == Status::start) commands.clear();
    else if(status == Status::start_dynamic){
        if(commands.size())
            setStatus(Status::stop);
        dynamic = true;
        status = Status::start;
    }
    else if(status == Status::last_bulk){
        if(!dynamic &&  commands.size() > 0) status = Status::stop;
    }
    notify();
}

void bulk::CommandBlock::appendCommnad(std::string & command){
    if(!dynamic){
        if(commands.size() == 0){
            setStatus(Status::start);
        }
        commands.push_back(command);
        if(commands.size() == N){
            setStatus(Status::stop);
        }
    }
    else{
        commands.push_back(command);
    }
}
