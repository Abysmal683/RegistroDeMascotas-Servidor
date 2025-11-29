#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controlwidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controlWidget = new ControlWidget(this);
    setCentralWidget(controlWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
