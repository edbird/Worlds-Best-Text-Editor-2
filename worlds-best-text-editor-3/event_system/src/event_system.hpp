#ifndef EVENT_SYSTEM_HPP
#define EVENT_SYSTEM_HPP

#include "spdlog_util.hpp"

#include "command.hpp"
#include "keybinding_configuration.hpp"

#include <SDL3/SDL.h>

#include <optional>



struct EventSystem {

    EventSystem(const KeybindingConfiguration& keybinding_configuration)
        : keybinding_configuration{keybinding_configuration}
    {
    }

    std::optional<Command> handle_event(const SDL_Event& event) const {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            SDL_KeyboardEvent keyboard_event = event.key;
            SDL_Keycode keycode = keyboard_event.key;
            SDL_Keymod keymod = keyboard_event.mod;

            const auto keychord{KeyChord(keycode, keymod)};
            const auto command{keybinding_configuration.find(keychord)};

            return command;
            /*if (command.has_value()) {
                return command
            }*/

            /*const bool ctrl = keymod & SDL_KMOD_CTRL;
            const bool shift = keymod & SDL_KMOD_SHIFT;
            const bool alt = keymod & SDL_KMOD_ALT;

            if (ctrl & !shift & !alt) {
                if (keycode == SDLK_Q) {
                    const auto command = Command(CommandType::QUIT);
                    return command;
                }
                else if (keycode == SDLK_S) {
                    const auto command = Command(CommandType::SAVE_DOCUMENT);
                    return command;
                }
            }
            else if (!ctrl & !shift & !alt) {
                if (keycode == SDLK_UP) {
                    const auto command = Command(CommandType::UP);
                    return command;
                }
                else if (keycode == SDLK_DOWN) {
                    const auto command = Command(CommandType::DOWN);
                    return command;
                }
                else if (keycode == SDLK_LEFT) {
                    const auto command = Command(CommandType::LEFT);
                    return command;
                }
                else if (keycode == SDLK_RIGHT) {
                    const auto command = Command(CommandType::RIGHT);
                    return command;
                }
            }*/
        }
        else if (event.type == SDL_EVENT_KEY_UP) {

        }
        else if (event.type == SDL_EVENT_TEXT_INPUT) {
            SDL_TextInputEvent text_input_event = event.text;
            const char* text = text_input_event.text;

            std::string utf8_character = text;
            SPDLOG_INFO("utf8_character={}", utf8_character);
            const auto command = create_insert_char_command(std::move(utf8_character));
            return command;
        }
        else if (event.type == SDL_EVENT_MOUSE_MOTION) {
            // ignored to prevent repeat output of logging message below
        }
        else {
            SPDLOG_INFO("unhandled event type in EventSystem::handle_event");
        }

        return std::nullopt;
    }

    const KeybindingConfiguration& keybinding_configuration;

};


#endif