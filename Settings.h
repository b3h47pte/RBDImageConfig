#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <vector>

class Settings
{
public:
  enum ESettings {
    ES_IMAGE,
    ES_PROPERTIES,
    ES_CONFIG,
    ES_MAX
  };

  Settings();
  ~Settings();

  void Save();
  bool HasSettings() const { return ((size_t)ES_MAX == settings.size()); }

  bool GetSetting(ESettings id, std::string& out) const;
  void SetSetting(ESettings id, const std::string& input);

  static std::string SETTINGS_FILENAME;
private:
  std::vector<std::string> settings;
  void Load();
};

#endif // SETTINGS_H
