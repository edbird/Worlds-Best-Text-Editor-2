#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "spdlog_util.hpp"

#include <format>

enum class CommandType {
    QUIT,
    CLOSE_WINDOW,
    CLOSE_DOCUMENT,
    SAVE_DOCUMENT,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    HOME,
    END,
    DELETE_,
    BACKSPACE,
    RETURN,
    INSERT_CHAR
};

constexpr std::string_view to_string(const CommandType command_type) noexcept {
    if (command_type == CommandType::QUIT) {
        return "QUIT";
    }
    else if (command_type == CommandType::CLOSE_WINDOW) {
        return "CLOSE_WINDOW";
    }
    else if (command_type == CommandType::CLOSE_DOCUMENT) {
        return "CLOSE_DOCUMENT";
    }
    else if (command_type == CommandType::SAVE_DOCUMENT) {
        return "SAVE_DOCUMENT";
    }
    else if (command_type == CommandType::UP) {
        return "UP";
    }
    else if (command_type == CommandType::DOWN) {
        return "DOWN";
    }
    else if (command_type == CommandType::LEFT) {
        return "LEFT";
    }
    else if (command_type == CommandType::RIGHT) {
        return "RIGHT";
    }
    else if (command_type == CommandType::HOME) {
        return "HOME";
    }
    else if (command_type == CommandType::END) {
        return "END";
    }
    else if (command_type == CommandType::DELETE_) {
        return "DELETE";
    }
    else if (command_type == CommandType::BACKSPACE) {
        return "BACKSPACE";
    }
    else if (command_type == CommandType::RETURN) {
        return "RETURN";
    }
    else if (command_type == CommandType::INSERT_CHAR) {
        return "INSERT_CHAR";
    }
    return "(COMMAND_TYPE)";
}

template<>
struct std::formatter<CommandType> : std::formatter<std::string_view> {

    auto format(const CommandType command_type, std::format_context& context) const {
        return std::formatter<std::string_view>::format(to_string(command_type), context);
    }
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