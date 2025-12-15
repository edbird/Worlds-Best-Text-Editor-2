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

    ApplicationConfiguration();

    std::optional<std::string> getFontPath() const;

    std::optional<unsigned int> getFontSize() const;

    Json::Value root;
};

enum class ApplicationConfigurationInitializationError {
    FILE_DOES_NOT_EXIST,
    JSON_PARSE_ERROR
};

std::expected<ApplicationConfiguration, ApplicationConfigurationInitializationError>
initialize_application_configuration();


#endif // APPLICATION_CONFIGURATION_HPP
