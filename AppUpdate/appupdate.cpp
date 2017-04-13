#include "appupdate.h"
#include "ui_appupdate.h"

#include <QTcpSocket>
#include <QByteArray>
#include <QProcess>
#include <QTimer>
#include <QFile>
#include <QDebug>

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

int ys_errno = 0;

QString mesg[] = {
    /*  0  */    "No Error",
    /*  6  */    "子进程启动失败",
    /*  10  */   "网络异常",
    /*  11  */   "网络流量字节不可用",

    };


extern void ys_perror(const QString &info)
{
    if( !info.isEmpty() )
    {
         qDebug() << info << mesg[ys_errno];
         return;
    }

    qDebug() << "Error:" << mesg[ys_errno];

}
Appupdate::Appupdate(QWidget *parent) :

    ui(new Ui::Appupdate)
{
    Q_UNUSED(parent);
    ui->setupUi(this);

    t_label = new QLabel("准备接收数据;");
    QVBoxLayout *t_vboxlayout = new QVBoxLayout(this);
    t_vboxlayout->addWidget(t_label);

    this->centralWidget()->setLayout(t_vboxlayout);


    m_updateSocket = new QTcpSocket(this);

    m_updateSocket->abort();
    /*
        www.unimationtech.com is an alias for 236341.vhost119.cloudvhost.top.
        236341.vhost119.cloudvhost.top has address 122.114.118.83
    */
    //m_updateSocket->connectToHost("23.105.202.29", 55369);// 替换成域名 + 端口
    m_updateSocket->connectToHost("www.unimationtech.com", 55369);// 替换成域名 + 端口 即 122.114.118.83  55369


    m_time = new QTimer(this); // 启动定时器
    QObject::connect(m_time, SIGNAL(timeout()), this, SLOT(update()));
    m_time->start(1000);
}

Appupdate::~Appupdate()
{
    delete ui;
}



// private slots;
int Appupdate::update()
{
    QByteArray  line;

    if(!m_updateSocket->bytesAvailable())
        return YSNET_BYTENANLE;

    while(1)
    {
        line =  m_updateSocket->readAll();

        if(line.length() == 0)  break;

        m_bytearray.append(line);// 得到升级程序字节序， 需将其存放在file文件中， 并运行启动该程序;
    }

    m_file = new QFile("yunsheng.exe"); // 存放升级程序
    m_file->open(QIODevice::WriteOnly);

    m_file->write(m_bytearray); // 写入update.exe 文件中;

    t_label->setText("recv data length :  " + m_bytearray.length());

    m_file->close();


    if (QProcess::startDetached("yunsheng.exe"))
    {
        ys_errno = YSNET_FAIL;
        return YSNET_FAIL;
    }

    m_time->stop();

    ys_errno = YS_SUCCESS;
    return YS_SUCCESS;
}
