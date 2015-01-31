#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <unordered_map>

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

  friend class MainWindow;
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

  // Mapping of properties to rectangles
  std::unordered_map<std::string, class Rectangle*> configMapping;
};

#endif // TOOLBAR_H
