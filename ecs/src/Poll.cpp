/*
** EPITECH PROJECT, 2023
** Visual Studio Live Share (Workspace)
** File description:
** Poll.cpp
*/

#include "Poll.hpp"
#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#else
#include <fcntl.h>
#include <poll.h>
#endif
#include <stdexcept>

namespace myposix {
    int Poll::poll(std::vector<aPollfd>& fds, int timeout)
    {
#ifdef _WIN32
        return WSAPoll(reinterpret_cast<LPWSAPOLLFD>(fds.data()), fds.size(), timeout);
#else
        return ::poll(reinterpret_cast<pollfd*>(fds.data()), fds.size(), timeout);
#endif
    }

    bool Poll::isReadable(aPollfd& fd)
    {
#ifdef _WIN32
        return fd.revents & (POLLRDNORM | POLLRDBAND);
#else
        return fd.revents & POLLIN;
#endif
    }

    void Poll::initSocket(int& fd)
    {
#ifdef _WIN32
        u_long mode = 1;
        ioctlsocket(fd, FIONBIO, &mode);
#else
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1)
            throw std::runtime_error("fcntl");
        flags |= O_NONBLOCK;
        if (fcntl(fd, F_SETFL, flags) == -1)
            throw std::runtime_error("fcntl");
#endif
    }

} // namespace myposix
