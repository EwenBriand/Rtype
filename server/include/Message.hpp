/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Message.hpp
*/

#pragma once
#include "CircularBuffer.hpp"
#include <utility>
#include <boost/asio.hpp>

namespace serv {

    struct Message {
        bytes data;
        boost::asio::ip::udp::endpoint endpoint;

        bytes& operator<<(bytes& data)
        {
            this->data = data;
            return data;
        }
    };

    class MalformedInstructionException : public std::exception {
    public:
        MalformedInstructionException(const std::string& message)
            : _message(message)
        {
        }

        const char* what() const noexcept override
        {
            return _message.c_str();
        }

    private:
        std::string _message;
    };

    /**
     * @brief Messages can be packed in instructions to follow the rfc, and vice versa.
     *
     */
    struct Instruction {
        uint32_t opcode;
        uint8_t expectsAnswer;
        bytes data;

        Instruction();

        Instruction(uint32_t opcode, uint8_t expectsAnswer, bytes data);

        Instruction(const bytes& bytes);

        bytes ToBytes() const;

        Message ToMessage(boost::asio::ip::udp::endpoint endpoint) const;

        static Instruction FromMessage(const Message& message);
    };
} // namespace serv
