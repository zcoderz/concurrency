//
// Created by usman on 2/24/22.
//

#ifndef CONCURRENCY_CALL_ONCE_H
#define CONCURRENCY_CALL_ONCE_H
#include <mutex>

struct connection_info
{};

struct data_packet
{};

struct connection_handle
{
    void send_data(data_packet const&)
    {}
    data_packet receive_data()
    {
        return data_packet();
    }
};

struct remote_connection_manager
{
    connection_handle open(connection_info const&)
    {
        return connection_handle();
    }
} connection_manager;


class X
{
private:
    connection_info connection_details;
    connection_handle connection;
    //call once flag
    std::once_flag connection_init_flag;

    void open_connection()
    {
        connection=connection_manager.open(connection_details);
    }
public:
    X(connection_info const& connection_details_):
            connection_details(connection_details_)
    {}
    void send_data(data_packet const& data)
    {
        //call once
        std::call_once(connection_init_flag,&X::open_connection,this);
        connection.send_data(data);
    }
    data_packet receive_data()
    {
        std::call_once(connection_init_flag,&X::open_connection,this);
        return connection.receive_data();
    }
};

int main()
{}

#endif //CONCURRENCY_CALL_ONCE_H
