#include "CullendulaMainWindow.h"
#include "ui_CullendulaMainWindow.h"

CullendulaMainWindow::CullendulaMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CullendulaMainWindow)
{
    ui->setupUi(this);
}

CullendulaMainWindow::~CullendulaMainWindow()
{
    delete ui;
}
