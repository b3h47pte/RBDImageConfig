#pragma once
#ifndef _STRINGUTILITY_H
#define _STRINGUTILITY_H

#include <vector>
#include <string>

inline std::vector<std::string> SplitString(const std::string& input, char delimiter = ' ') {
  std::vector<std::string> sections;
  size_t previousPosition = 0;
  size_t position = input.find(delimiter);
  while (position != std::string::npos) {
    std::string s = input.substr(previousPosition, position);
    sections.push_back(s);
    previousPosition = position;
    position = input.find(delimiter, position + 1);
  }
  return sections;
}
#endif