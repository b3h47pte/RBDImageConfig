#pragma once
#ifndef _STRINGUTILITY_H
#define _STRINGUTILITY_H

#include <vector>
#include <string>
#include <algorithm>

inline std::vector<std::string> SplitString(const std::string& input, char delimiter = ' ') {
  std::vector<std::string> sections;
  size_t previousPosition = 0;
  size_t position = input.find(delimiter);
  while (position != std::string::npos) {
    std::string s = input.substr(previousPosition, position);
    sections.push_back(s);
    previousPosition = position + 1;
    position = input.find(delimiter, position + 1);
  }

  std::string lastSection = input.substr(previousPosition);
  std::remove_if(lastSection.begin(), lastSection.end(), [](char c) {return c == ' ';});
  if (!lastSection.empty()) {
    sections.push_back(lastSection);
  }

  return sections;
}
#endif
