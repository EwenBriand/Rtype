/*
** EPITECH PROJECT, 2023
** Visual Studio Live Share (Workspace)
** File description:
** Poll.hpp
*/

#pragma once
#include <vector>

namespace myposix {
    class Poll {
    public:
        struct aPollfd {
#ifdef _WIN32
            long long fd;
#else
            int fd;
#endif
            short events;
            short revents;
        };

        static int poll(std::vector<aPollfd>& fds, int timeout);
        static bool isReadable(aPollfd& fd);
        static void initSocket(int& fd);
    };
} // namespace myposix
