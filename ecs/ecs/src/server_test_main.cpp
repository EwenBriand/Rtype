/*
** EPITECH PROJECT, 2023
** ecs
** File description:
** server_test_main.cpp
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <coroutine>
#include "Server.hpp"

class TestClient : public serv::ServerImpl::IClient {
    public:
        TestClient() {
            std::cout << "TestClient created" << std::endl;
        };

        serv::ServerImpl::Message HandleRequest(const std::string &data) override
        {
            std::cout << "Received in TestClient: " << data << std::endl;
            int value = atoi(data.c_str());
            if (value == 0) {
                throw serv::ServerImpl::HandleRequestException("Invalid value", 400);
            }

            for (int i = 0; i < value; i++) {
                std::cout << "ping for value: " << value << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            }
            return serv::ServerImpl::Message("OK", 100);
        }
    private:
        std::mutex _mutex;
        std::condition_variable _cv;
        int value;
};

class TestClient2 : public serv::ServerImpl::IClient {
    public:
        TestClient2() {
            std::cout << "TestClient2 created" << std::endl;
        };

        serv::ServerImpl::Message HandleRequest(const std::string &data) override
        {
            std::cout << "Received in TestClient2: " << data << std::endl;
            if (data == "crash") {
                throw serv::ServerImpl::HandleRequestException("Crash test", 502);
            }
            return serv::ServerImpl::Message("OK", 200);
        }
};

int main()
{
    serv::ServerImpl::Get()->SetPort(4242);
    serv::ServerImpl::Get()->Start();
    serv::ServerImpl::Get()->registerHandshake(TestClient);
    serv::ServerImpl::Get()->registerHandshake(TestClient2);

    while (true) {
        serv::ServerImpl::Get()->Poll();
    }
    return 0;
}
