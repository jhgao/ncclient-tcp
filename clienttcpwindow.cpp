#include "clienttcpwindow.h"
#include "ui_clienttcpwindow.h"

ClientTcpWindow::ClientTcpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientTcpWindow)
{
    ui->setupUi(this);
}

ClientTcpWindow::~ClientTcpWindow()
{
    delete ui;
}
