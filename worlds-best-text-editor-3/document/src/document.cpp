#include "spdlog_util.hpp"

#include "document.hpp"

#include <fstream>

void Document::write_to_file(const char* filename) const {
    std::ofstream ofile(filename);
    if (!ofile.is_open()) {
        throw std::runtime_error(
            std::format("failed to open file: {}", filename)
        );
    }

    for (const auto& line: lines) {
        ofile << line << '\n';
    }

    ofile.flush();
    ofile.close();

    SPDLOG_INFO("text data written to file {}", filename);
}

void Document::read_from_file(const char* filename) {
    lines.clear();

    std::ifstream ifile(filename);
    if (!ifile.is_open()) {
        throw std::runtime_error(
            std::format("failed to open file: {}", filename)
        );
    }

    std::string line;
    while (std::getline(ifile, line)) {
        lines.push_back(line);
    }

    ifile.close();

    SPDLOG_INFO("text data read from file {}", filename);
}

void Document::insert_char(const std::string& utf8_character) {
    SPDLOG_INFO("insert_char: {}", utf8_character);
    if (lines.size() < 1) {
        SPDLOG_INFO("document contains 0 lines, do nothing");
        return ;
    }

    auto &first_line{lines[0]};
    first_line.insert(0, utf8_character);
}