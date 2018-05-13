#include "bulk.h"

void bulk::Handle::readString(const char * data, std::size_t size){
    for(std::size_t i = 0; i < size; i++, ++data){
        if(*data != '\n' ){
            std::string x(data, 1);
            stash.append(x);
        }
        else{
            interpreter.readCommand(stash);
            stash.clear();
        }
    }
};


void bulk::Handle::readString(std::string cmd){
    if(!cmd.empty())
        interpreter.readCommand(cmd);
};


void bulk::Handle::lastBulk(){
    if(!stash.empty())
        interpreter.readCommand(stash);
    stash.clear();
    interpreter.lastBulk();
};
