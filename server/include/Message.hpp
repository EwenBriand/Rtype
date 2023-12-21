/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Message.hpp
*/

#pragma once
#include "CircularBuffer.hpp"
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

        Instruction()
            : opcode(0)
            , expectsAnswer(0)
        {
        }

        Instruction(uint32_t opcode, uint8_t expectsAnswer, bytes data)
            : opcode(opcode)
            , expectsAnswer(expectsAnswer)
            , data(data)
        {
        }

        Instruction(const bytes& bytes)
        {
            if (bytes.size() < 5)
                throw MalformedInstructionException("Instruction is too small");
            std::vector<unsigned char> byteData = bytes._data;
            opcode = *(uint32_t*)byteData.data();
            expectsAnswer = *(uint8_t*)(byteData.data() + 4);
            data = std::vector<unsigned char>(byteData.begin() + 5, byteData.end());
        }

        bytes ToBytes() const
        {
            std::vector<unsigned char> bytes;
            bytes.insert(bytes.end(), (unsigned char*)&opcode, (unsigned char*)&opcode + sizeof(opcode));
            bytes.insert(bytes.end(), (unsigned char*)&expectsAnswer, (unsigned char*)&expectsAnswer + sizeof(expectsAnswer));
            bytes.insert(bytes.end(), data._data.begin(), data._data.end());
            return bytes;
        }

        Message ToMessage() const
        {
            Message message;
            message.data = ToBytes();
            return message;
        }

        static Instruction FromMessage(const Message& message)
        {
            return Instruction(message.data);
        }
    };
} // namespace serv
