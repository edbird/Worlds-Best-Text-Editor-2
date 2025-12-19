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

    auto &[cursor_y, cursor_x]{cursor};

    if (cursor_y >= lines.size()) {
        SPDLOG_ERROR("cursor_y={}, lines.size()={}", cursor_y, lines.size());
        throw std::runtime_error(
            std::format("cursor_y={}, lines.size()={}", cursor_y, lines.size())
        );
    }

    auto &line{lines[cursor_y]};

    if (cursor_x > line.size()) {
        SPDLOG_ERROR("cursor_x={}, line.size()={}", cursor_x, line.size());
        throw std::runtime_error(
            std::format("cursor_x={}, line.size()={}", cursor_x, line.size())
        );
    }

    line.insert(cursor_x, utf8_character);
    ++ cursor_x;
}

void Document::delete_() {
    SPDLOG_INFO("delete");
    if (lines.size() < 1) {
        SPDLOG_INFO("document contains 0 lines, do nothing");
        return ;
    }

    auto [cursor_y, cursor_x]{cursor};

    if (cursor_y >= lines.size()) {
        SPDLOG_ERROR("cursor_y={}, lines.size()={}", cursor_y, lines.size());
        throw std::runtime_error(
            std::format("cursor_y={}, lines.size()={}", cursor_y, lines.size())
        );
    }

    auto &line{lines[cursor_y]};

    if (cursor_x < line.size()) {
        line.erase(cursor_x, 1);
    }

    // if cursor at end of line, need to combine next line with current line
    else if (cursor_x == line.size()) {
        // this can only be done if there is a next line
        if (cursor_y + 1 >= lines.size()) {
            // there is no next line
            // do nothing
        }
        else {
            auto next_line{std::move(lines[cursor_y + 1])};
            line.append(std::move(next_line));
            lines.erase(lines.begin() + cursor_y + 1);
        }
    }

    else if (cursor_x > line.size()) {
        SPDLOG_ERROR("cursor_x={}, line.size()={}", cursor_x, line.size());
        throw std::runtime_error(
            std::format("cursor_x={}, line.size()={}", cursor_x, line.size())
        );
    }
}

void Document::cursor_up() {
    SPDLOG_INFO("cursor up");

    auto &[cursor_y, cursor_x]{cursor};

    if (cursor_y > 0) {
        -- cursor_y;
    }
}

void Document::cursor_down() {
    SPDLOG_INFO("cursor down");

    auto &[cursor_y, cursor_x]{cursor};

    if (cursor_y + 1 < lines.size()) {
        ++ cursor_y;

        if (cursor_x > lines[cursor_y].size()) {
            cursor_x = lines[cursor_y].size();
        }
    }
}

void Document::cursor_left() {
    SPDLOG_INFO("cursor left");

    auto &[cursor_y, cursor_x]{cursor};

    if (cursor_x > 0) {
        -- cursor_x;
    }
}

void Document::cursor_right() {
    SPDLOG_INFO("cursor right");

    auto &[cursor_y, cursor_x]{cursor};

    if (cursor_x + 1 < lines[cursor_y].size()) {
        ++ cursor_x;
    }
}