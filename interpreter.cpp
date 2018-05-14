#include "async.h"

async::Interpreter::Interpreter(long N_):dynamic_counter(0){
    cb = std::make_shared<CommandBlock>(N_);
    cb->subscribe(std::make_unique<CoutObserver>(cb));
    cb->subscribe(std::make_unique<LogObserver>(cb));
}

void async::Interpreter::readCommand(std::string &command){

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

void async::Interpreter::lastBulk(){
    cb->setStatus(Status::last_bulk);
}
