/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Message.hpp
*/

#pragma once
#include "CircularBuffer.hpp"
#include <cstdint>
#include <memory>

namespace serv {
    struct EndpointWrapper;

    struct Message {
        bytes data;
        std::shared_ptr<EndpointWrapper> endpointW;

        Message(bytes data, std::shared_ptr<EndpointWrapper> endpointW)
            : data(data), endpointW(endpointW) {}
        Message(const Message& other)
            : data(other.data), endpointW(other.endpointW) {}

        bytes& operator<<(bytes& data)
        {
            this->data = data;
            return data;
        }
        Message& operator=(const Message& other)
        {
            if (this != &other) {
                data = other.data;
                endpointW = other.endpointW;
            }
            return *this;
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

        Message ToMessage(EndpointWrapper endpointW) const;

        static Instruction FromMessage(const Message& message);
    };
} // namespace serv
