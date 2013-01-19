#ifndef CLIENTTCPWINDOW_H
#define CLIENTTCPWINDOW_H

#include <QMainWindow>
#include "gui/rcvprogressscene.h"

namespace Ui {
class ClientTcpWindow;
}

class ClientTcpWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ClientTcpWindow(QWidget *parent = 0);
    ~ClientTcpWindow();
private slots:
    void updateProgress(const unsigned int);  // percent
    void onGotBlock(const quint32 bsn); //got block i

private:
    Ui::ClientTcpWindow *ui;
    RcvProgressScene m_scene;
};

#endif // CLIENTTCPWINDOW_H
