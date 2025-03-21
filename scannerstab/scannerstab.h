#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_scannerstab.h"

class scannerstab : public QMainWindow {
    Q_OBJECT

public:
    scannerstab(QMainWindow* parent = nullptr);
    ~scannerstab();

private:
    Ui::scannerstab ui; // UI object
};