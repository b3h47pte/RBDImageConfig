#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void OpenImageDialog();
  void OpenPropertiesDialog();
  void OpenConfigurationDialog();
  void SaveConfigurationDialog(bool forceShow = false);
  void SaveAsConfigurationDialog();

  void AddRectangle();
  void AddMultiRectangle();
  void DeleteRectangle();

  void ChangeSelectedProperty(class QListWidgetItem* item);

  void NotifyImageMousePress(class QGraphicsSceneMouseEvent* e);
  void NotifyImageMouseRelease(class QGraphicsSceneMouseEvent* e);

protected:
  void closeEvent(class QCloseEvent* e);

private:
  // Program Settings 
  class Settings* settings;
  void LoadProgramSettings();
  void SaveProgramSettings();

  // Menu
  class QMenu* mainMenu;
  void GenerateMenu();
  void GenerateMenuActions();

  // Open Image
  QString currentImagePath;
  class QAction* openAction;
  void OpenImage(const QString& imagePath);

  // Load Properties File
  QString currentPropertiesPath;
  class QAction* openPropertiesAction;
  void LoadProperties(const QString& propertiesPath);

  // Configuration File
  QString currentConfigurationPath;
  class QAction* openConfigAction;
  class QAction* saveConfigAction;
  class QAction* saveAsConfigAction;
  void LoadConfiguration(const QString& configurationPath);
  void SaveConfiguration();

  // Left Side (Image Display)
  void GenerateImageDisplay();
  void UpdateConfigurationDisplay();
  class QGraphicsView* imageView;
  class ImageScene* imageScene;
  class QGraphicsPixmapItem* imageItem;
  std::vector<class QGraphicsRectItem*> imageRectangles;

  // Right Side (Settings/Tools for adding properties to the image)
  void GenerateToolbar();
  class Toolbar* sideToolbar;
  class QDockWidget* toolbarDock;

  // Handle Creation of Rectangles
  bool createRectangle;
  bool createMultiRectangle;
  void CreateInitialRectangle(const QPointF& p);
  void FinializeRectangle(const QPointF& p);
  class Rectangle* newRectangle;
};

#endif // __MAINWINDOW_H
