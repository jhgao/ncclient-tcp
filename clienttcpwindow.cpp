#include "clienttcpwindow.h"
#include "ui_clienttcpwindow.h"

ClientTcpWindow::ClientTcpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientTcpWindow),m_isConnected(false)
{
    ui->setupUi(this);

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(100);
    //TODO connect progress signal

    ui->graphicsView->setScene(&m_scene);
    connect(ui->graphicsView, SIGNAL(sig_resized(QSize)),
            &m_scene, SLOT(arragneToView(QSize)));

    //connection
    connect(&m_con, SIGNAL(sig_progressPercent(uint)),
            this, SLOT(updateProgress(uint)));
    connect(&m_con, SIGNAL(sig_gotBlockSN(quint32)),
            this, SLOT(onGotBlock(quint32)));
    connect(&m_con, SIGNAL(connected()),
            this, SLOT(onConnected()));
    connect(&m_con, SIGNAL(disconnected()),
            this, SLOT(onDisconnected()));
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

void ClientTcpWindow::onConnected()
{
    m_isConnected = true;
    ui->groupBox_server->setDisabled(true);
    ui->pushButton_linkServer->setText("Disconnect");
}

void ClientTcpWindow::onDisconnected()
{
    m_isConnected = false;
    ui->groupBox_server->setEnabled(true);
    ui->pushButton_linkServer->setText("Connect");
}

void ClientTcpWindow::on_pushButton_linkServer_clicked()
{
    if(m_isConnected){
        m_con.abort();
    }else{
        m_con.connectToHost(QHostAddress(ui->lineEdit_serverAddr->text()),
                            (quint16)ui->lineEdit_serverPort->text().toInt());
    }
}
