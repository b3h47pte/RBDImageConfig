#include "Toolbar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include "Rectangle.h"
#include "MultiRectangle.h"
#include "StringUtility.h"
#include <functional>

Toolbar::
Toolbar(QWidget *parent) : 
  QWidget(parent) {
  SetupButtons();
  SetupPropertyList();
  SetupLayout();
  SetupConfigurationEditor();
}

Toolbar::
~Toolbar() {

}

void Toolbar::
SetupPropertyList() {
  propertyList = new QListWidget(); 
  connect(propertyList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ChangeSelectedProperty(QListWidgetItem*)));
}

void Toolbar::
SetupLayout() {
  mainLayout = new QVBoxLayout();
  mainLayout->addWidget(addRectangleButton);
  mainLayout->addWidget(addMultiRectangleButton);
  mainLayout->addWidget(deleteRectangleButton);
  mainLayout->addWidget(saveButton);
  mainLayout->addWidget(propertyList);
  setLayout(mainLayout);
}

void Toolbar::
SetupButtons() {
  addRectangleButton = new QPushButton(tr("Add Rectangle"));

  addMultiRectangleButton = new QPushButton(tr("Add Multi-Rectangle"));

  deleteRectangleButton = new QPushButton(tr("Delete Rectangle"));

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
  delete configMapping[property];
  configMapping[property] = rectangle;
  UpdateConfigurationEditor();
}

std::string Toolbar::
GetCurrentProperty() const {
  return propertyList->currentItem()->text().toStdString();
}

void Toolbar::
SetupConfigurationEditor() {
  AddConfigurationSetting("X Position");
  AddConfigurationSetting("Y Position");
  AddConfigurationSetting("Width");
  AddConfigurationSetting("Height");
  AddConfigurationSetting("X Step");
  AddConfigurationSetting("Y Step");
  AddConfigurationSetting("Total Rectangles");
  AddConfigurationSetting("Row Size");
}

void Toolbar::
UpdateConfigurationEditor() {
  std::string property = GetCurrentProperty();
  for(size_t i = 0; i < settingEditorWidgets.size(); ++i) {
    bool disable = !configMapping[property];
    if(i >= 4 && !disable) {
      disable = !configMapping[property]->CanContainMultipleRectangles();
    }
    std::get<2>(settingEditorWidgets[i])->setDisabled(disable);

    if(!disable) {
      switch(i) {
      case 0:
        std::get<2>(settingEditorWidgets[i])->setText(QString::number(configMapping[property]->GetX()));
        break;
      case 1:
        std::get<2>(settingEditorWidgets[i])->setText(QString::number(configMapping[property]->GetY()));
        break;
      case 2:
        std::get<2>(settingEditorWidgets[i])->setText(QString::number(configMapping[property]->GetWidth()));
        break;
      case 3:
        std::get<2>(settingEditorWidgets[i])->setText(QString::number(configMapping[property]->GetHeight()));
        break;
      case 4:
        std::get<2>(settingEditorWidgets[i])->setText(QString::number(static_cast<MultiRectangle*>(configMapping[property])->GetXStep()));
        break;
      case 5:
        std::get<2>(settingEditorWidgets[i])->setText(QString::number(static_cast<MultiRectangle*>(configMapping[property])->GetYStep()));
        break;
      case 6:
        std::get<2>(settingEditorWidgets[i])->setText(QString::number(static_cast<MultiRectangle*>(configMapping[property])->GetTotalRectangles()));
        break;
      case 7:
        std::get<2>(settingEditorWidgets[i])->setText(QString::number(static_cast<MultiRectangle*>(configMapping[property])->GetRowSize()));
        break;
      }
    }
  }
}

void Toolbar::
AddConfigurationSetting(const QString& title) {
  QHBoxLayout* layout = new QHBoxLayout();
  QLabel* configLabel = new QLabel(this);
  configLabel->setText(title);
  QLineEdit* configEdit = new QLineEdit(this);
  QPalette palette = configEdit->palette();
  palette.setColor(QPalette::Disabled, QPalette::Base, Qt::lightGray);    
  configEdit->setPalette(palette);
  layout->addWidget(configLabel);
  layout->addWidget(configEdit);

  connect(configEdit, &QLineEdit::textEdited, this, std::bind(&Toolbar::EditConfigurationProperty, this, settingEditorWidgets.size(), std::placeholders::_1));

  std::tuple<QHBoxLayout*, QLabel*, QLineEdit*> store = std::make_tuple(layout, configLabel, configEdit);
  settingEditorWidgets.push_back(store);
  mainLayout->addLayout(layout);
}

void Toolbar::
EditConfigurationProperty(int index, const QString& newText) {
  try {
    switch(index) {
    case 0:
      configMapping[GetCurrentProperty()]->SetX(std::stod(newText.toStdString()));
      break;
    case 1:
      configMapping[GetCurrentProperty()]->SetY(std::stod(newText.toStdString()));
      break;
    case 2:
      configMapping[GetCurrentProperty()]->SetWidth(std::stod(newText.toStdString()));
      break;
    case 3:
      configMapping[GetCurrentProperty()]->SetHeight(std::stod(newText.toStdString()));
      break;
    case 4:
      static_cast<MultiRectangle*>(configMapping[GetCurrentProperty()])->SetXStep(std::stod(newText.toStdString()));
      break;
    case 5:
      static_cast<MultiRectangle*>(configMapping[GetCurrentProperty()])->SetYStep(std::stod(newText.toStdString()));
      break;
    case 6:
      static_cast<MultiRectangle*>(configMapping[GetCurrentProperty()])->SetTotalRectangles(std::stoi(newText.toStdString()));
      break;
    case 7:
      static_cast<MultiRectangle*>(configMapping[GetCurrentProperty()])->SetRowSize(std::stoi(newText.toStdString()));
      break;
    }
    configMapping[GetCurrentProperty()]->Normalize();
    UpdateConfigurationProperty();
    UpdateConfigurationEditor();
  } catch (...) {
  }
}

void Toolbar::
ChangeSelectedProperty(class QListWidgetItem* item) {
  (void)item;
  UpdateConfigurationEditor();
}
