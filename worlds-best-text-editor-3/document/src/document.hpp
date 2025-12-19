#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "spdlog_util.hpp"

struct Document {

    Document() {

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

    std::vector<std::string> lines;
};

#endif