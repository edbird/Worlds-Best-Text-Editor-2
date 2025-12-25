#include "spdlog_util.hpp"

#include "keybinding_configuration.hpp"

void KeybindingConfiguration::load_default() {

    SPDLOG_INFO("load default keybindings");

    bind(KeyChord(SDLK_Q, SDL_KMOD_CTRL), Command(CommandType::QUIT));
    bind(KeyChord(SDLK_S, SDL_KMOD_CTRL), Command(CommandType::SAVE_DOCUMENT));
    bind(KeyChord(SDLK_UP, SDL_KMOD_NONE), Command(CommandType::UP));
    bind(KeyChord(SDLK_DOWN, SDL_KMOD_NONE), Command(CommandType::DOWN));
    bind(KeyChord(SDLK_LEFT, SDL_KMOD_NONE), Command(CommandType::LEFT));
    bind(KeyChord(SDLK_RIGHT, SDL_KMOD_NONE), Command(CommandType::RIGHT));
    bind(KeyChord(SDLK_HOME, SDL_KMOD_NONE), Command(CommandType::HOME));
    bind(KeyChord(SDLK_END, SDL_KMOD_NONE), Command(CommandType::END));
    bind(KeyChord(SDLK_DELETE, SDL_KMOD_NONE), Command(CommandType::DELETE_));
    bind(KeyChord(SDLK_BACKSPACE, SDL_KMOD_NONE), Command(CommandType::BACKSPACE));
    bind(KeyChord(SDLK_RETURN, SDL_KMOD_NONE), Command(CommandType::RETURN));
}