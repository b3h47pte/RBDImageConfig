#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <unordered_map>
#include <vector>
#include <tuple>

class Toolbar : public QWidget
{
  Q_OBJECT
public:
  explicit Toolbar(QWidget *parent = 0);
  ~Toolbar();

  void AddProperty(const std::string& property);
  void ClearProperties();

  void AddConfiguration(const std::string& configuration);

  void SetCurrentConfiguration(class Rectangle* rectangle);
  std::string GetCurrentProperty() const;
  void UpdateConfigurationEditor();

  friend class MainWindow;

signals:
  void UpdateConfigurationProperty();  

private slots:
  void ChangeSelectedProperty(class QListWidgetItem* item);

private:
  // Layout
  void SetupLayout();
  class QVBoxLayout* mainLayout;

  // Buttons to add rectangles
  void SetupButtons();
  class QPushButton* addRectangleButton;
  class QPushButton* addMultiRectangleButton;

  // Save Button
  class QPushButton* saveButton;

  // Property List
  void SetupPropertyList();
  class QListWidget* propertyList;

  // Settings for the currently selected Rectangle
  std::vector<std::tuple<class QHBoxLayout*, class QLabel*, class QLineEdit*>> settingEditorWidgets;
  void SetupConfigurationEditor();
  void AddConfigurationSetting(const QString& title);
  void EditConfigurationProperty(int index, const QString& newText);

  // Mapping of properties to rectangles
  std::unordered_map<std::string, class Rectangle*> configMapping;
};

#endif // TOOLBAR_H
