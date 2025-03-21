#ifndef SCANNERSTAB_H
#define SCANNERSTAB_H

#include <QMainWindow>
#include <QLabel>
#include "ui_scannerstab.h"  // Include the generated UI header file

class scannerstab : public QMainWindow
{
    Q_OBJECT

public:
    scannerstab(QWidget* parent = nullptr);
    ~scannerstab();

private:
    Ui::scannerstab ui;  // Add this to link the UI elements
};

#endif // SCANNERSTAB_H
