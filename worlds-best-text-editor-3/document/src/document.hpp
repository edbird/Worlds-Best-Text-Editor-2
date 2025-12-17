#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "spdlog_util.hpp"

struct Document {

    void write_to_file(const char* filename) const;

    void read_from_file(const char* filename);

    std::vector<std::string> lines;
};

#endif