#ifndef APPLICATION_CONFIGURATION_HPP
#define APPLICATION_CONFIGURATION_HPP

#include "spdlog_util.hpp"

#include <json/json.h>

#include <expected>
#include <optional>
#include <iostream>
#include <fstream>
#include <map>

struct ApplicationConfigurationInitializationFileDoesNotExistError : std::runtime_error {
    explicit ApplicationConfigurationInitializationFileDoesNotExistError(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

struct ApplicationConfigurationInitializationJsonParseError : std::runtime_error {
    explicit ApplicationConfigurationInitializationJsonParseError(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

struct ApplicationConfiguration {

    ApplicationConfiguration() {

        // Load configuration from json file
        Json::CharReaderBuilder reader;
        std::string json_parse_errors;

        const auto config_file_path = "config.json";
        std::ifstream config_file(config_file_path, std::ifstream::binary);
        if (!config_file.is_open()) {
            SPDLOG_ERROR("failed to open configuration file {}", config_file_path);
            throw ApplicationConfigurationInitializationFileDoesNotExistError(
                std::format("failed to open configuration file, {} does not exist", config_file_path)
            );
        }

        if (!Json::parseFromStream(reader, config_file, &root, &json_parse_errors)) {
            SPDLOG_ERROR("errors while parsing json document: {}", json_parse_errors);
            throw ApplicationConfigurationInitializationJsonParseError("errors while parsing json document");
        }
    }

    std::optional<std::string> getFontPath() const {

        #ifdef _WIN32
            const auto FONT_PATH_KEY = "font_path_windows";
        #else
            const auto FONT_PATH_KEY = "font_path_linux";
        #endif

        if (root.isMember(FONT_PATH_KEY)) {
            std::string font_path = root[FONT_PATH_KEY].asString();
            return font_path;
        }

        SPDLOG_ERROR("failed to get font path from configuration file");
        return std::nullopt;
    }

    Json::Value root;
};

enum class ApplicationConfigurationInitializationError {
    FILE_DOES_NOT_EXIST,
    JSON_PARSE_ERROR
};

std::expected<ApplicationConfiguration, ApplicationConfigurationInitializationError>
initialize_application_configuration() {
    try {
        const auto config = ApplicationConfiguration();
        return config;
    }
    catch (ApplicationConfigurationInitializationFileDoesNotExistError &error) {
        SPDLOG_ERROR("failed to initialize application configuration, file does not exist: {}", error.what());
        return std::unexpected(ApplicationConfigurationInitializationError::FILE_DOES_NOT_EXIST);
    }
    catch (ApplicationConfigurationInitializationJsonParseError &error) {
        SPDLOG_ERROR("failed to initialize application configuration, parsing json data failed: {}", error.what());
        return std::unexpected(ApplicationConfigurationInitializationError::JSON_PARSE_ERROR);
    }
    catch (...) {
        SPDLOG_ERROR("unhandled error in initialize_application_configuration");
        throw;
    }
}


#endif // APPLICATION_CONFIGURATION_HPP
