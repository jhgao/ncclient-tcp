#ifndef CLIENTTCPWINDOW_H
#define CLIENTTCPWINDOW_H

#include <QMainWindow>
#include "gui/rcvprogressscene.h"
#include "connection.h"
#include "connectionthread.h"
#include "protocol/ports_define.h"

namespace Ui {
class ClientTcpWindow;
}

class ClientTcpWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ClientTcpWindow(QWidget *parent = 0);
    ~ClientTcpWindow();
signals:
    void sig_onConAbortCmd();
    void sig_onConConnectToHostCmd(QString,quint16);

private slots:
    void updateProgress(const unsigned int);  // percent
    void onGotBlock(const quint32 bsn); //got block i

    void onConnected();
    void onDisconnected();

    void on_pushButton_linkServer_clicked();

private:
    Ui::ClientTcpWindow *ui;
    RcvProgressScene m_scene;
    bool m_isConnected;
    Connection* m_con;
    ConnectionThread* m_conThread;
};

#endif // CLIENTTCPWINDOW_H
