#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "spdlog_util.hpp"

#include <format>

struct DocumentCursorPosition {
    std::size_t line_index;
    std::size_t column_index;
};

template<>
struct std::formatter<DocumentCursorPosition> : std::formatter<std::string> {
    auto format(const DocumentCursorPosition& document_cursor_position, auto& context) const {
        return std::format_to(
            context.out(),
            "(line={}, column={})",
            document_cursor_position.line_index,
            document_cursor_position.column_index
        );
    }
};

struct Document {

    Document()
        : lines{std::vector<std::string>()}
        , document_cursor{DocumentCursorPosition{0ul, 0ul}}
    {
    }

    ~Document() {
        SPDLOG_WARN("~Document");
    }

    Document(const Document& document) = delete;
    Document(Document&& document) = delete;
    Document& operator=(const Document& document) = delete;
    Document& operator=(Document&& document) = delete;

    void write_to_file(const char* filename) const;

    void read_from_file(const char* filename);

    void insert_char(const std::string& utf8_character);

    void delete_();

    void cursor_up();
    void cursor_down();
    void cursor_left();
    void cursor_right();

    std::vector<std::string> lines;
    DocumentCursorPosition document_cursor;
};

#endif