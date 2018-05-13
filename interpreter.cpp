#include "bulk.h"

bulk::Interpreter::Interpreter(long N_):dynamic_counter(0){
    cb = std::make_shared<bulk::CommandBlock>(N_);
    cb->subscribe(std::make_unique<bulk::CoutObserver>(cb));
    cb->subscribe(std::make_unique<bulk::LogObserver>(cb));
}

void bulk::Interpreter::readCommand(std::string &command){
    
    if(command == "{"){
        if(!dynamic_counter)
            cb->setStatus(Status::start_dynamic);
        dynamic_counter++;
    }
    else if(command == "}"){
        dynamic_counter--;
        if(!dynamic_counter)
            cb->setStatus(Status::stop);
    }
    else{
        cb->appendCommnad(command);
    }
};

void bulk::Interpreter::lastBulk(){
    cb->setStatus(Status::last_bulk);
}
