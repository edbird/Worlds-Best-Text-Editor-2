#ifndef KEYBINDING_CONFIGURATION_HPP
#define KEYBINDING_CONFIGURATION_HPP

#include "spdlog_util.hpp"

#include "command.hpp"

#include <SDL3/SDL.h>

#include <cstdint>
#include <optional>
#include <unordered_map>

struct KeyChord {

    KeyChord(const SDL_Keycode keycode, const SDL_Keymod keymod)
        : keycode{keycode}
        , keymod{keymod}
    {
    }

    SDL_Keycode keycode;
    SDL_Keymod keymod;

    friend bool operator==(const KeyChord& l, const KeyChord& r) = default;
};

template<>
struct std::hash<KeyChord> {
    std::size_t operator()(const KeyChord& keychord) const noexcept {
        //const std::size_t hash1 = std::hash<uint32_t>{}(static_cast<uint32_t>(keychord.keycode));
        const std::size_t hash1 = std::hash<SDL_Keycode>{}(keychord.keycode);
        //const std::size_t hash2 = std::hash<uint16_t>{}(static_cast<uint16_t>(keychord.keymod));
        const std::size_t hash2 = std::hash<SDL_Keymod>{}(keychord.keymod);
        return hash1 ^ (hash2 << 1);
    }
};

struct KeybindingConfiguration {

    std::unordered_map<KeyChord, Command> keybinding_configuration;

    void bind(const KeyChord& keychord, Command command) {
        SPDLOG_INFO("bind");

        if (keybinding_configuration.contains(keychord)) {
            SPDLOG_WARN("keybinding configuration already contains an entry for this keychord, ignoring");
            return ;
        }
        keybinding_configuration.insert(std::make_pair(keychord, command));
    }

    void unbind(const KeyChord& keychord) {
        SPDLOG_INFO("unbind");

        keybinding_configuration.erase(keychord);
    }

    // TODO: load this from JSON file
    void load_default() {
        SPDLOG_INFO("load default keybindings");
        bind(KeyChord(SDLK_Q, SDL_KMOD_CTRL), Command(CommandType::QUIT));
        bind(KeyChord(SDLK_S, SDL_KMOD_CTRL), Command(CommandType::SAVE_DOCUMENT));
        bind(KeyChord(SDLK_UP, SDL_KMOD_NONE), Command(CommandType::UP));
        bind(KeyChord(SDLK_DOWN, SDL_KMOD_NONE), Command(CommandType::DOWN));
        bind(KeyChord(SDLK_LEFT, SDL_KMOD_NONE), Command(CommandType::LEFT));
        bind(KeyChord(SDLK_RIGHT, SDL_KMOD_NONE), Command(CommandType::RIGHT));
        bind(KeyChord(SDLK_DELETE, SDL_KMOD_NONE), Command(CommandType::DELETE_));
        bind(KeyChord(SDLK_BACKSPACE, SDL_KMOD_NONE), Command(CommandType::BACKSPACE));
        bind(KeyChord(SDLK_RETURN, SDL_KMOD_NONE), Command(CommandType::RETURN));
    }

    std::optional<Command> find(const KeyChord& keychord) const {
        SPDLOG_INFO("find keychord with key code {} and key mod {}", keychord.keycode, keychord.keymod);

        const auto it{keybinding_configuration.find(keychord)};
        if (it == keybinding_configuration.cend()) {
            return std::nullopt;
        }
        return it->second;
    }
};

/*
class KeybindingLoader {
public:
    // Simple parser (you'd use a JSON library in practice)
    static void loadFromFile(const std::string& filepath,
                            KeybindingRegistry& registry) {
        // Parse JSON file
        // For each binding entry:
        //   - Parse key string to SDL_Keycode
        //   - Parse modifiers
        //   - Parse command string to CommandType
        //   - Call registry.bind()

        // Pseudo-code:
        // for (auto& binding : jsonData["keybindings"]) {
        //     KeyChord chord = parseKeyChord(binding);
        //     CommandType cmd = parseCommand(binding["command"]);
        //     registry.bind(chord, cmd);
        // }
    }

private:
    static SDL_Keycode stringToKeycode(const std::string& keyStr) {
        // Map string names to SDL keycodes
        static const std::unordered_map<std::string, SDL_Keycode> keyMap = {
            {"A", SDLK_A}, {"B", SDLK_B}, ...
            {"Left", SDLK_LEFT}, {"Right", SDLK_RIGHT},
            {"Up", SDLK_UP}, {"Down", SDLK_DOWN},
            {"Return", SDLK_RETURN}, {"Backspace", SDLK_BACKSPACE},
            // etc.
        };

        auto it = keyMap.find(keyStr);
        return (it != keyMap.end()) ? it->second : SDLK_UNKNOWN;
    }

    static CommandType stringToCommand(const std::string& cmdStr) {
        static const std::unordered_map<std::string, CommandType> cmdMap = {
            {"Save", CommandType::Save},
            {"Open", CommandType::Open},
            {"Copy", CommandType::Copy},
            // etc.
        };

        auto it = cmdMap.find(cmdStr);
        return (it != cmdMap.end()) ? it->second : CommandType::RawKey;
    }
};
*/

#endif