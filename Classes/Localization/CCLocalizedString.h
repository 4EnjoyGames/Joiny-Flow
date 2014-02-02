#ifndef _CCLocalizedString_h
#define _CCLocalizedString_h
#include "string"
#include <memory>

const char *_(const std::string &key);
void parse_data(const char *data, size_t length);
uint32_t swap_le(const char *buf, size_t ofs);
uint32_t swap_be(const char *buf, size_t ofs);
void parse_file(const char *filename);

#endif
