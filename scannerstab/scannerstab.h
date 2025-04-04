#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_scannerstab.h"
#include <QPixmap>

class scannerstab : public QMainWindow {
    Q_OBJECT

public:
    explicit scannerstab(QWidget* parent = nullptr);
    ~scannerstab();

private:
    Ui::scannerstab ui; 

    QPixmap leftPixmap;  // Left foot scan
    QPixmap rightPixmap;   // Right foot scan

    void computeAndDisplayArea();
};
