/*
** EPITECH PROJECT, 2023
** RTypeV0.0
** File description:
** AsioClone.hpp
*/

#pragma once

#include <boost/asio.hpp>

namespace serv {
    struct EndpointWrapper {
        boost::asio::ip::udp::endpoint endpoint;
        EndpointWrapper(boost::asio::ip::udp protocol, unsigned short port) : endpoint(protocol, port) {}
        EndpointWrapper() = default;
        bool operator==(const EndpointWrapper &other) const {
            return endpoint == other.endpoint;
        }
    };

    class AsioClone {
        friend class AClient;
        private:
            boost::asio::ip::udp::socket _socket;
            boost::asio::io_service _ioService;
        public:
            typedef boost::asio::ip::udp udp;
            typedef boost::asio::ip::udp::socket socket;
            typedef EndpointWrapper endpoint;
            typedef boost::system::error_code error_code;
            typedef boost::asio::io_service io_service;
            typedef boost::asio::mutable_buffers_1 mutable_buffer;
            typedef std::vector<boost::asio::detail::buffered_stream_storage::byte_type> bytes;

            // typedef boost::asio::error

            AsioClone() : _socket(_ioService) {};

            AsioClone(io_service &io) : _socket(io) {}

            inline void open(const udp &protocol) {
                _socket.open(protocol);
            }

            inline error_code open(const udp &protocol, error_code &ec) {
                _socket.open(protocol, ec);
                return ec;
            }

            inline void set_option_reuseaddr(void) {
                _socket.set_option(boost::asio::socket_base::reuse_address(true));
            }

            inline void bind(const endpoint &ep, error_code &ec) {
                _socket.bind(ep.endpoint, ec);
            }

            inline void stop(io_service &io) {
                io.stop();
            }

            inline void stop() {
                _ioService.stop();
            }

            inline void run(io_service &io) {
                io.run();
            }

            inline void run() {
                _ioService.run();
            }

            inline size_t receive_from(const mutable_buffer &buffer, endpoint &ep) {
                return _socket.receive_from(buffer, ep.endpoint);
            }

            inline size_t send_to(const mutable_buffer &buffer, endpoint &ep) {
                return _socket.send_to(buffer, ep.endpoint);
            }

            // inline size_

            inline error_code get_message_size_error() {
                return boost::asio::error::message_size;
            }
    };
}
