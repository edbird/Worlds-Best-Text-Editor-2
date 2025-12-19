#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "spdlog_util.hpp"

enum class CommandType {
    QUIT,
    CLOSE_WINDOW,
    CLOSE_DOCUMENT,
    SAVE_DOCUMENT,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    INSERT_CHAR
};

struct Command {

    Command(const CommandType command_type)
        : command_type{command_type}
    {
    }

    CommandType command_type;
    std::string utf8_character;
};

Command create_insert_char_command(const std::string utf8_character);

#endif