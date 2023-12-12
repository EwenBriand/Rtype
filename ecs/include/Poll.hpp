/*
** EPITECH PROJECT, 2023
** Visual Studio Live Share (Workspace)
** File description:
** Poll.hpp
*/

#pragma once
#include <vector>

namespace myposix
{
    class Poll {
        public:
            struct aPollfd {
#ifdef _WIN32
                long long fd;
#else
                unsigned int fd;
#endif
                short events;
                short revents;
            };

            // struct aSockAddrIn {
            //     short sin_family;
            //     unsigned short sin_port;
            //     unsigned int sin_addr;
            //     char sin_zero[8];
            // };

            // struct aFdSet {
            //     unsigned int fds_bits[32];
            // };


            // struct aSocketLength {
            //     unsigned int length;
            // };

            static int poll(std::vector<aPollfd> &fds, int timeout);
            static bool isReadable(aPollfd &fd);
            static void initSocket(int &fd);
    };
} // namespace myposix
