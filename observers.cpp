#include "bulk.h"

void bulk::CoutObserver::update(Status s){
    if(s == Status::stop){
        std::cout<<"bulk: ";
        for(auto i : cb->commands){
            std::cout<<i<<" ";
        }
        std::cout<<std::endl;
    }
};

void bulk::LogObserver::update(Status s){
    if(s == Status::start){
        bulkBeginTime = boost::lexical_cast<std::string>(time(nullptr));
        bulkFileName = bulkBeginTime;
        bulkFileName.append(".log");
    }
    else if(s == Status::stop){
        std::ofstream bulkFile(bulkFileName, std::ios::out | std::ios::app);
        bulkFile<<"bulk: ";
        for(auto i : cb->commands){
            bulkFile<<i<<" ";
        }
        bulkFile<<std::endl;
        bulkFile.close();
    }
};
