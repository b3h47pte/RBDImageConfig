#include "Settings.h"
#include <fstream>
#include <algorithm>

std::string Settings::SETTINGS_FILENAME = "RBDIC_SETTINGS.txt";

Settings::
Settings() {
  Load();
}

Settings::
~Settings() {

}

void Settings::
Save() {
  std::ofstream fs(SETTINGS_FILENAME.c_str());
  if(!fs.is_open()) return;
  for(size_t i = 0; i < std::min((size_t)ES_MAX, settings.size()); ++i) {
    fs << settings[i] << std::endl;
  }
  fs.close();
}

void Settings::
Load() {
  std::ifstream fs(SETTINGS_FILENAME.c_str());
  if(!fs.is_open()) return;
  std::string line;
  while(getline(fs, line)) {
    settings.push_back(line); 
  }
  fs.close();
}

bool Settings::
GetSetting(ESettings id, std::string& out) const {
  if ((size_t)id >= settings.size()) return false;
  out = settings[id];
  return true;
}

void Settings::
SetSetting(ESettings id, const std::string& input) {
  if((size_t)id >= settings.size()) {
    settings.resize(id + 1);
  }
  settings[id] = input;
}
