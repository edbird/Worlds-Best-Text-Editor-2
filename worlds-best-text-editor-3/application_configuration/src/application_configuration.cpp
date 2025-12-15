#include "spdlog_util.hpp"

#include "application_configuration.hpp"

#include <json/json.h>

std::optional<std::string> ApplicationConfiguration::getFontPath() const {

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

