#ifndef APPUPDATE_H
#define APPUPDATE_H

#include <QMainWindow>

class QTcpSocket;
class QByteArray;
class QTimer;
class QString;
class QFile;
class QLabel;

#define   YS_SUCCESS   0

#define   YS_STARTFAIL 6   /* 子进程启动失败 */

// network;
#define YSNET_FAIL     10  /* 网络错误*/
#define YSNET_BYTENANLE  11 /*网络字节不可用*/

#ifdef __cplusplus
extern "C"{
#endif

    extern int ys_errno;
    extern void ys_perror(const QString &info);
    
#ifdef __cplusplus
}
#endif

namespace Ui {
    class Appupdate;
}

class Appupdate : public QMainWindow
{
    Q_OBJECT

public:
    explicit Appupdate(QWidget *parent = 0);
    ~Appupdate();



private slots:
    //更新并运行函数;
    int update();

private:
    Ui::Appupdate *ui;

    QTcpSocket  *m_updateSocket;
    QByteArray m_bytearray;
    QTimer *m_time;
    QFile  *m_file;


    QLabel *t_label;
};

#endif // APPUPDATE_H
