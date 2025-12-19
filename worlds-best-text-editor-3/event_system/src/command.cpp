#include "spdlog_util.hpp"

#include "command.hpp"

Command create_insert_char_command(const std::string utf8_character) {
    auto command{Command(CommandType::INSERT_CHAR)};
    command.utf8_character = std::move(utf8_character);
    return command;
}
