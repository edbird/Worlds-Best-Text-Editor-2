#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "spdlog_util.hpp"

#include <format>

struct DocumentCursor {
    std::size_t line_index;
    std::size_t column_index;
};

template<>
struct std::formatter<DocumentCursor> : std::formatter<std::string> {
    auto format(const DocumentCursor& document_cursor, auto& context) const {
        return std::format_to(
            context.out(),
            "(line={}, column={})",
            document_cursor.line_index,
            document_cursor.column_index
        );
    }
};

struct Document {

    Document()
        : lines{std::vector<std::string>()}
        , document_cursor{DocumentCursor{0ul, 0ul}}
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
    void backspace();
    void return_();

    void cursor_up();
    void cursor_down();
    void cursor_left();
    void cursor_right();

    std::vector<std::string> lines;
    DocumentCursor document_cursor;
};

#endif