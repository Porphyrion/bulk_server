#pragma once
#include <algorithm>
#include <list>
#include <queue>
#include <map>
#include "async.h"

using boost::asio::ip::tcp;
using namespace async;

class bulk_session;

class bulk_room
{
    using bulk_session_ptr = std::shared_ptr<bulk_session>;

public:

    bulk_room(int bulk_size_) : handle(connect(bulk_size_)){}

    void leave(bulk_session_ptr participant){
        last_session_bulk(handle);
        participants_.erase(participant);
    }

    void deliver( std::string& msg, bulk_session_ptr session_ptr){
        auto ptr = participants_.find(session_ptr);
        if(ptr != participants_.end()){
            ptr->second.readCommand(msg);
        }
    }

    class server_interpreter{
    public:
        server_interpreter(handle_t handle_):handle(handle_),dynamic_counter(0){};
        void readCommand(std::string &msg){
            if(msg == "{"){
                if(!dynamic_counter) dynamic_bulk.push_back("{");
                dynamic_counter++;
            }
            else if(msg == "}"){
                dynamic_counter--;
                if(!dynamic_counter){
                    receive(handle, dynamic_bulk);
                    dynamic_bulk.clear();
                }
            }
            else{
                if(!dynamic_counter){
                    receive(handle, msg);
                }
                else{
                    dynamic_bulk.push_back(msg);
                }
            }
        }

    private:
        handle_t handle;
        long dynamic_counter;
        std::vector<std::string> dynamic_bulk;
    };

    void join(bulk_session_ptr participant){
        participants_.emplace(participant, server_interpreter(handle));
    }

private:
    std::queue<std::vector<std::string>> queue_bulk;
    std::map<bulk_session_ptr, server_interpreter> participants_;
    std::queue<std::string> recent_msgs_;
    handle_t handle;
};

class bulk_session : public std::enable_shared_from_this<bulk_session>{
public:
    bulk_session(tcp::socket socket, bulk_room& room_)
    : socket_(std::move(socket)), room(room_)
    {
    }

    void run()
    {
        room.join(shared_from_this());
        do_read();
    }

    void do_read()
    {
       auto self(shared_from_this());
       boost::asio::async_read_until(socket_,
           sb, '\n',
           [this,self](boost::system::error_code ec, std::size_t byte)
           {
             sb.commit(byte);
             std::istream istrm(&sb);
             std::string line;
             istrm>>line;
             auto bicycle = std::all_of(line.begin(), line.end(), [](char c){return c == '\0';});

             if (!ec && !bicycle)
             {
                room.deliver(line, shared_from_this());
                do_read();
             }
             else{
                 if(ec.value() == 2 || bicycle) {
                     room.leave(shared_from_this());
                 }
             }
           });
     }

private:
     tcp::socket socket_;
     boost::asio::streambuf sb;
     bulk_room& room;
};


class bulk_server
{
public:
    bulk_server(boost::asio::io_service& io_service,
            const tcp::endpoint& endpoint,int  bulk_size_)
    : acceptor_(io_service, endpoint),
      socket_(io_service),room(bulk_size_)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec)
            {
            if (!ec)
            {
                std::make_shared<bulk_session>(std::move(socket_), room)->run();
            }
            do_accept();
        });
    }
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    bulk_room room;
};
