#include "Toolbar.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QListWidget>
#include <QPushButton>
#include "Rectangle.h"
#include "MultiRectangle.h"
#include "StringUtility.h"

Toolbar::
Toolbar(QWidget *parent) : 
  QWidget(parent) {
  SetupButtons();
  SetupPropertyList();
  SetupLayout();
}

Toolbar::
~Toolbar() {

}

void Toolbar::
SetupPropertyList() {
  propertyList = new QListWidget(); 
}

void Toolbar::
SetupLayout() {
  mainLayout = new QVBoxLayout();
  mainLayout->addWidget(addRectangleButton);
  mainLayout->addWidget(addMultiRectangleButton);
  mainLayout->addWidget(saveButton);
  mainLayout->addWidget(propertyList);
  setLayout(mainLayout);
}

void Toolbar::
SetupButtons() {
  addRectangleButton = new QPushButton(tr("Add Rectangle"));

  addMultiRectangleButton = new QPushButton(tr("Add Multi-Rectangle"));

  saveButton = new QPushButton(tr("Save Configuration"));
}

void Toolbar::
ClearProperties() {
  propertyList->clear();
}

void Toolbar::
AddProperty(const std::string& property) {
  propertyList->addItem(QString::fromStdString(property));
  if(propertyList->currentRow() < 0) {
    propertyList->setCurrentRow(0);
  }
  configMapping[property] = NULL;
}

void Toolbar::
AddConfiguration(const std::string& configuration) {
  std::string property;
  std::string rectangle;
  size_t equalSign = configuration.find('=');
  if(equalSign == std::string::npos) {
    return;
  }
  property = configuration.substr(0, equalSign);

  if (configMapping.find(property) == configMapping.end()) {
    return;
  }

  rectangle = configuration.substr(equalSign + 1);
  std::vector<std::string> rectangleSections = SplitString(rectangle);
  Rectangle* newRect = NULL;
  if(rectangleSections.size() == 4) {
    newRect = new Rectangle();
  } else if (rectangleSections.size() == 8) {
    newRect = new MultiRectangle();
  } else {
    return;
  }
  newRect->FromSplitString(rectangleSections);
  configMapping[property] = newRect;
}

void Toolbar::
SetCurrentConfiguration(Rectangle* rectangle) {
  std::string property = GetCurrentProperty();
  configMapping[property] = rectangle;
}

std::string Toolbar::
GetCurrentProperty() const {
  return propertyList->currentItem()->text().toStdString();
}
