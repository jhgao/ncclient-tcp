#include "clienttcpwindow.h"
#include "ui_clienttcpwindow.h"

ClientTcpWindow::ClientTcpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientTcpWindow)
{
    ui->setupUi(this);

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(100);
    //TODO connect progress signal

    ui->graphicsView->setScene(&m_scene);
    connect(ui->graphicsView, SIGNAL(sig_resized(QSize)),
            &m_scene, SLOT(arragneToView(QSize)));
}

ClientTcpWindow::~ClientTcpWindow()
{
    delete ui;
}

void ClientTcpWindow::updateProgress(const unsigned int p)
{
    ui->progressBar->setValue(p);
}

void ClientTcpWindow::onGotBlock(const quint32 bsn)
{
    m_scene.gotBlock(bsn);
}
