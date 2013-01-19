#ifndef CLIENTTCPWINDOW_H
#define CLIENTTCPWINDOW_H

#include <QMainWindow>

namespace Ui {
class ClientTcpWindow;
}

class ClientTcpWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ClientTcpWindow(QWidget *parent = 0);
    ~ClientTcpWindow();
    
private:
    Ui::ClientTcpWindow *ui;
};

#endif // CLIENTTCPWINDOW_H
