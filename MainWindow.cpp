#include "MainWindow.h"
#include <QDebug>
#include <QPen>
#include <QColor>
#include <QScrollBar>
#include <QMouseEvent>
#include <QPalette>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QScrollArea>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QPushButton>
#include <QGraphicsView>
#include <QEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStatusBar>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include "Toolbar.h"
#include "Settings.h"
#include "ImageScene.h"
#include "Rectangle.h"
#include "MultiRectangle.h"
#include <fstream>
#include <unordered_map>

MainWindow::
MainWindow(QWidget *parent)
    : QMainWindow(parent), settings(NULL), imageItem(NULL), createRectangle(false),
      createMultiRectangle(false) {
  GenerateImageDisplay();
  GenerateMenu();
  GenerateToolbar();
  
  setWindowTitle(tr("RBD -- Image Configuration"));
  resize(1280,720);

  LoadProgramSettings();

  statusBar()->showMessage("Loaded");
}

MainWindow::
~MainWindow() {
  delete settings;
  delete imageItem;
}

void MainWindow::
closeEvent(class QCloseEvent* e) {
  (void)e;
  SaveProgramSettings();
}

void MainWindow::
LoadProgramSettings() {
  delete settings;
  settings = new Settings();
  if(!settings->HasSettings()) return;
  std::string imagePath;
  if(settings->GetSetting(Settings::ES_IMAGE, imagePath)) {
    OpenImage(QString::fromStdString(imagePath));
  }

  std::string propertiesPath;
  if(settings->GetSetting(Settings::ES_PROPERTIES, propertiesPath)) {
    LoadProperties(QString::fromStdString(propertiesPath));  
  }

  // MUST LOAD CONFIGURATION AFTER PROPERTIES.
  // Otherwise we won't know which properties are valid when loading the configuration.
  std::string configPath;
  if(settings->GetSetting(Settings::ES_CONFIG, configPath)) {
    LoadConfiguration(QString::fromStdString(configPath));
  }

  UpdateConfigurationDisplay();
}

void MainWindow::
SaveProgramSettings() {
  settings->SetSetting(Settings::ES_IMAGE, currentImagePath.toStdString());
  settings->SetSetting(Settings::ES_PROPERTIES, currentPropertiesPath.toStdString());
  settings->SetSetting(Settings::ES_CONFIG, currentConfigurationPath.toStdString());
  settings->Save();
}

void MainWindow::
OpenImageDialog() {
  QString imagePath = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::currentPath());
  if(imagePath.isEmpty()) {
    return;
  }
  OpenImage(imagePath);
}

void MainWindow::
OpenImage(const QString& imagePath) {
  QImage image(imagePath);
  if(image.isNull()) {
    return;
  }
  currentImagePath = imagePath;
  delete imageItem;
  QPixmap pixmap = QPixmap::fromImage(image);
  imageItem = new QGraphicsPixmapItem(pixmap);
  imageScene->addItem(imageItem);
  imageView->setScene(imageScene);
}


void MainWindow::
OpenPropertiesDialog() {
  QString propertiesPath = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::currentPath());
  if(propertiesPath.isEmpty()) {
    return;
  }
  LoadProperties(propertiesPath);
}

void MainWindow::
LoadProperties(const QString& propertiesPath) {
  std::ifstream fs(propertiesPath.toStdString().c_str());
  if(!fs.is_open()) return;
  currentPropertiesPath = propertiesPath;
  std::string line;
  sideToolbar->ClearProperties();
  while(getline(fs, line)) {
    sideToolbar->AddProperty(line);
  }
}

void MainWindow::
OpenConfigurationDialog() {
  QString configurationPath = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::currentPath());
  if(configurationPath.isEmpty()) {
    return;
  }
  LoadConfiguration(configurationPath);
}

void MainWindow::
SaveAsConfigurationDialog() {
  SaveConfigurationDialog(true);
}

void MainWindow::
SaveConfigurationDialog(bool forceShow) {
  if (currentConfigurationPath.isEmpty() || forceShow) {
    QString configurationPath = QFileDialog::getSaveFileName(this, tr("Save Configuration"), QDir::currentPath());
    if(configurationPath.isEmpty()) {
      return;
    }
    currentConfigurationPath = configurationPath;
  }
  SaveConfiguration();
}

void MainWindow::
SaveConfiguration() {
  std::ofstream fs(currentConfigurationPath.toStdString().c_str());
  if(!fs.is_open()) return;
  fs << "[Default]" << std::endl;
  for(auto it = sideToolbar->configMapping.begin(); it != sideToolbar->configMapping.end(); ++it) {
    if(!it->second)continue;
    fs << it->first << "=" << it->second->ToString() << std::endl; 
  }
  fs.close();
}

void MainWindow::
LoadConfiguration(const QString& configurationPath) {
  std::ifstream fs(configurationPath.toStdString().c_str());
  if(!fs.is_open()) return;
  currentConfigurationPath = configurationPath;
  std::string line;
  while(getline(fs, line)) {
    if(line == "[Default]") continue;
    sideToolbar->AddConfiguration(line);
  }
  sideToolbar->UpdateConfigurationEditor();
}

void MainWindow::
GenerateImageDisplay() {
  imageView = new QGraphicsView();
  imageView->setBackgroundRole(QPalette::Dark);
  imageView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  imageView->setDragMode(QGraphicsView::RubberBandDrag);

  imageScene = new ImageScene();
  connect(imageScene, &ImageScene::NotifyMousePress, this, &MainWindow::NotifyImageMousePress);
  connect(imageScene, &ImageScene::NotifyMouseRelease, this, &MainWindow::NotifyImageMouseRelease);

  setCentralWidget(imageView);
}

void MainWindow::
NotifyImageMousePress(QGraphicsSceneMouseEvent* e) {
  if(!createRectangle && !createMultiRectangle) return;
  CreateInitialRectangle(e->scenePos());
}

void MainWindow::
NotifyImageMouseRelease(QGraphicsSceneMouseEvent* e) {
  if(!createRectangle && !createMultiRectangle) return;
  FinializeRectangle(e->scenePos());
}

void MainWindow::
GenerateMenuActions() {
  openAction = new QAction(tr("&Open"), this);
  openAction->setShortcut(tr("Ctrl+O"));
  connect(openAction, SIGNAL(triggered()), this, SLOT(OpenImageDialog()));

  openConfigAction = new QAction(tr("Open &Config"), this);
  connect(openConfigAction, SIGNAL(triggered()), this, SLOT(OpenConfigurationDialog()));

  saveConfigAction = new QAction(tr("&Save Config"), this);
  saveConfigAction->setShortcut(tr("Ctrl+S"));
  connect(saveConfigAction, SIGNAL(triggered()), this, SLOT(SaveConfigurationDialog()));

  saveAsConfigAction = new QAction(tr("Save As Config"), this);
  connect(saveAsConfigAction, SIGNAL(triggered()), this, SLOT(SaveAsConfigurationDialog()));

  openPropertiesAction = new QAction(tr("Open &Properties"), this);
  connect(openPropertiesAction, SIGNAL(triggered()), this, SLOT(OpenPropertiesDialog()));
}

void MainWindow::
GenerateMenu() {
  GenerateMenuActions();
  mainMenu = new QMenu(tr("&File"), this);
  mainMenu->addAction(openAction);
  mainMenu->addAction(openConfigAction);
  mainMenu->addAction(saveConfigAction);
  mainMenu->addAction(saveAsConfigAction);
  mainMenu->addAction(openPropertiesAction);

  menuBar()->addMenu(mainMenu);
}

void MainWindow::
GenerateToolbar() {
  sideToolbar = new Toolbar(this); 
  connect(sideToolbar->addRectangleButton, SIGNAL(clicked()), this, SLOT(AddRectangle()));
  connect(sideToolbar->addMultiRectangleButton, SIGNAL(clicked()), this, SLOT(AddMultiRectangle()));
  connect(sideToolbar->deleteRectangleButton, SIGNAL(clicked()), this, SLOT(DeleteRectangle()));
  connect(sideToolbar->saveButton, SIGNAL(clicked()), this, SLOT(SaveConfigurationDialog()));
  connect(sideToolbar->propertyList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ChangeSelectedProperty(QListWidgetItem*)));
  connect(sideToolbar, &Toolbar::UpdateConfigurationProperty, this, &MainWindow::UpdateConfigurationDisplay);

  toolbarDock = new QDockWidget(tr("Toolbar"), this);
  toolbarDock->setWidget(sideToolbar);
  toolbarDock->setAllowedAreas(Qt::RightDockWidgetArea);
  toolbarDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
  addDockWidget(Qt::RightDockWidgetArea, toolbarDock);
}

void MainWindow::
AddRectangle() {
  if(createMultiRectangle) return;
  statusBar()->showMessage("Adding Rectangle...");
  createRectangle = true;
}

void MainWindow::
AddMultiRectangle() {
  if(createRectangle) return;
  statusBar()->showMessage("Adding Multi-Rectangle...");
  createMultiRectangle = true;
}

void MainWindow::
DeleteRectangle() {
  if(createMultiRectangle || createRectangle) return;
  sideToolbar->SetCurrentConfiguration(NULL);
  UpdateConfigurationDisplay();
}

void MainWindow::
CreateInitialRectangle(const QPointF& p) {
  if(createRectangle) {
    newRectangle = new Rectangle();
  }
  else if (createMultiRectangle) {
    newRectangle = new MultiRectangle();
    static_cast<MultiRectangle*>(newRectangle)->SetTotalRectangles(1);
    static_cast<MultiRectangle*>(newRectangle)->SetRowSize(1);
    static_cast<MultiRectangle*>(newRectangle)->SetXStep(0);
    static_cast<MultiRectangle*>(newRectangle)->SetYStep(0);
  }
  newRectangle->SetX(p.x());
  newRectangle->SetY(p.y());
}

void MainWindow::
FinializeRectangle(const QPointF& p) {
  if(newRectangle) {
    newRectangle->SetWidth(p.x() - newRectangle->GetX());
    newRectangle->SetHeight(p.y() - newRectangle->GetY());
    newRectangle->Normalize();

    // Set rectangle as the configuration for this property
    sideToolbar->SetCurrentConfiguration(newRectangle);
  }

  createRectangle = false;
  createMultiRectangle = false;

  // Update Rectangles on Image
  UpdateConfigurationDisplay();

  statusBar()->showMessage("Added new rectangle");
}

void MainWindow::
ChangeSelectedProperty(QListWidgetItem* item) {
  (void)item;
  UpdateConfigurationDisplay();
}

void MainWindow::
UpdateConfigurationDisplay() {
  for(size_t i = 0; i < imageRectangles.size(); ++i) {
    imageScene->removeItem(imageRectangles[i]);
    delete imageRectangles[i];
  }

  imageRectangles.clear();

  for(auto it = sideToolbar->configMapping.begin(); it != sideToolbar->configMapping.end(); ++it) {
    Rectangle* rect = it->second;
    if(!rect) continue;
    QColor color = QColor(255, 255, 255);
    if (it->first == sideToolbar->GetCurrentProperty()) {
      color = QColor(255, 0, 0);
    }

    if(!rect->CanContainMultipleRectangles()) {
      QGraphicsRectItem* rectItem = imageScene->addRect(rect->ToQRectF(), QPen(color));
      imageRectangles.push_back(rectItem);
    } else {
      MultiRectangle* multiRect = static_cast<MultiRectangle*>(rect);
      for(int i = 0; i < multiRect->GetTotalRectangles(); ++i) {
        QGraphicsRectItem* rectItem = imageScene->addRect(multiRect->ToQRectF(i), QPen(color));
        imageRectangles.push_back(rectItem);
      }
    }
  }
}
