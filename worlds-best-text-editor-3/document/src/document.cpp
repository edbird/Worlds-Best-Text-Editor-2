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

    auto &[cursor_line_index, cursor_column_index]{document_cursor};

    if (cursor_line_index >= lines.size()) {
        SPDLOG_ERROR("cursor_line_index={}, lines.size()={}", cursor_line_index, lines.size());
        throw std::runtime_error(
            std::format("cursor_line_index={}, lines.size()={}", cursor_line_index, lines.size())
        );
    }

    auto &line{lines[cursor_line_index]};

    if (cursor_column_index > line.size()) {
        SPDLOG_ERROR("cursor_column_index={}, line.size()={}", cursor_column_index, line.size());
        throw std::runtime_error(
            std::format("cursor_column_index={}, line.size()={}", cursor_column_index, line.size())
        );
    }

    line.insert(cursor_column_index, utf8_character);
    ++ cursor_column_index;
}

void Document::delete_() {
    SPDLOG_INFO("delete");
    if (lines.size() < 1) {
        SPDLOG_INFO("document contains 0 lines, do nothing");
        return ;
    }

    auto [cursor_line_index, cursor_column_index]{document_cursor};

    if (cursor_line_index >= lines.size()) {
        SPDLOG_ERROR("cursor_line_index={}, lines.size()={}", cursor_line_index, lines.size());
        throw std::runtime_error(
            std::format("cursor_line_index={}, lines.size()={}", cursor_line_index, lines.size())
        );
    }

    auto &line{lines[cursor_line_index]};

    if (cursor_column_index < line.size()) {
        line.erase(cursor_column_index, 1);
    }

    // if cursor at end of line, need to combine next line with current line
    else if (cursor_column_index == line.size()) {
        // this can only be done if there is a next line
        if (cursor_line_index + 1 >= lines.size()) {
            // there is no next line
            // do nothing
        }
        else {
            auto next_line{std::move(lines[cursor_line_index + 1])};
            line.append(std::move(next_line));
            lines.erase(lines.begin() + cursor_line_index + 1);
        }
    }

    else if (cursor_column_index > line.size()) {
        SPDLOG_ERROR("cursor_column_index={}, line.size()={}", cursor_column_index, line.size());
        throw std::runtime_error(
            std::format("cursor_column_index={}, line.size()={}", cursor_column_index, line.size())
        );
    }
}

void Document::backspace() {
    SPDLOG_INFO("backspace");
    if (lines.size() < 1) {
        SPDLOG_INFO("document contains 0 lines, do nothing");
        return ;
    }

    auto &[cursor_line_index, cursor_column_index]{document_cursor};

    if (cursor_line_index >= lines.size()) {
        SPDLOG_ERROR("cursor_line_index={}, lines.size()={}", cursor_line_index, lines.size());
        throw std::runtime_error(
            std::format("cursor_line_index={}, lines.size()={}", cursor_line_index, lines.size())
        );
    }

    auto &line{lines[cursor_line_index]};

    if (cursor_column_index > 0) {
        line.erase(cursor_column_index - 1, 1);

        -- cursor_column_index;
    }

    // if cursor at beginning of line, need to combine previous line with current line
    else {
        if (cursor_line_index > 0) {
            auto &previous_line{lines[cursor_line_index - 1]};

            cursor_column_index = previous_line.size();

            previous_line.append(std::move(lines[cursor_line_index]));
            lines.erase(lines.begin() + cursor_line_index);

            -- cursor_line_index;
        }
        else {
            // there is no previous line
            // do nothing
        }
    }
    // TODO: add additional check on cursor_line_index compare to lines.size() as in delete() function
}

void Document::return_() {
    SPDLOG_INFO("return");

    auto &[cursor_line_index, cursor_column_index]{document_cursor};

    auto &line{lines[cursor_line_index]};

    const auto line_end_copy{line.substr(cursor_column_index)};
    line.erase(cursor_column_index);
    lines.insert(lines.begin() + cursor_line_index + 1, std::move(line_end_copy));

    cursor_column_index = 0;
    ++ cursor_line_index;
}

void Document::cursor_up() {
    SPDLOG_INFO("cursor up");

    auto &[cursor_line_index, cursor_column_index]{document_cursor};

    if (cursor_line_index > 0) {
        -- cursor_line_index;

        if (cursor_column_index > lines[cursor_line_index].size()) {
            cursor_column_index = lines[cursor_line_index].size();
        }
    }
}

void Document::cursor_down() {
    SPDLOG_INFO("cursor down");

    auto &[cursor_line_index, cursor_column_index]{document_cursor};

    if (cursor_line_index + 1 < lines.size()) {
        ++ cursor_line_index;

        if (cursor_column_index > lines[cursor_line_index].size()) {
            cursor_column_index = lines[cursor_line_index].size();
        }
    }
}

void Document::cursor_left() {
    SPDLOG_INFO("cursor left");

    auto &[cursor_line_index, cursor_column_index]{document_cursor};

    if (cursor_column_index > 0) {
        -- cursor_column_index;
    }
}

void Document::cursor_right() {
    SPDLOG_INFO("cursor right");

    auto &[cursor_line_index, cursor_column_index]{document_cursor};

    // Comparison is <= becuase cursor must be able to move 1 position beyond
    // end of line
    if (cursor_column_index + 1 <= lines[cursor_line_index].size()) {
        ++ cursor_column_index;
    }
}