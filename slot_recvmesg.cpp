//图片名字
QByteArray m_fileName;
//接收到的数据
QByteArray m_recvData;
//实际图片数据大小
qint64 m_DataSize;
//接收图片数据大小
qint64 m_checkSize;
//缓存上一次或多次的未处理的数据
//这个用来处理，重新粘包
QByteArray m_buffer;



// 粘包问题， 处理并解决;

//接收消息
void ClientThread::slot_readmesg()
{
    //缓冲区没有数据，直接无视
    if( m_tcpClient->bytesAvailable() <= 0 )
    {
        return;
    }

    //临时获得从缓存区取出来的数据，但是不确定每次取出来的是多少。
    QByteArray buffer;
    //如果是信号readyRead触发的，使用readAll时会一次把这一次可用的数据全总读取出来
    //所以使用while(m_tcpClient->bytesAvailable())意义不大，其实只执行一次。
    buffer = m_tcpClient->readAll();


    //上次缓存加上这次数据
    /*
   上面有讲到混包的三种情况，数据A、B，他们过来时有可能是A+B、B表示A包+B包中一部分数据，
   然后是B包剩下的数据，或者是A、A+B表示A包一部分数据，然后是A包剩下的数据与B包组合。
   这个时候，我们解析时肯定会残留下一部分数据，并且这部分数据对于下一包会有效，所以我们
   要和下一包组合起来。
  */
    m_buffer.append(buffer);

    ushort type_id, mesg_len;

    int totalLen = m_buffer.size();

    while( totalLen )
    {
        //与QDataStream绑定，方便操作。
        QDataStream packet(m_buffer);
        packet.setByteOrder(QDataStream::BigEndian);

        //不够包头的数据直接就不处理。
        if( totalLen < MINSIZE )
        {
            break;
        }

        packet >> type_id >> mesg_len;

        //如果不够长度等够了在来解析
        if( totalLen < mesg_len )
        {
            break;
        }   

        //数据足够多，且满足我们定义的包头的几种类型
        switch(type_id)
        {
        case MSG_TYPE_ID:
            break;

        case MSG_TYPE_FILE_START:
        {
            packet >> m_fileName;
        }
            break;         

        case MSG_TYPE_FILE_SENDING:
        {
            QByteArray tmpdata;
            packet >> tmpdata;
            //这里我把所有的数据都缓存在内存中，因为我们传输的文件不大，最大才几M;
            //大家可以这里收到一个完整的数据包，就往文件里面写入，即使保存。
            m_recvData.append(tmpdata);
            //这个可以最后拿来校验文件是否传完，或者是否传的完整。
            m_checkSize += tmpdata.size();
            //打印提示，或者可以连到进度条上面。
            emit sig_displayMesg(QString("recv: %1").arg(m_checkSize));
        }
            break;         

        case MSG_TYPE_FILE_END:
        {
            packet >> m_DataSize;
            saveImage();
            clearData();
        }
            break;

        default:
            break;
        }

        //缓存多余的数据
        buffer = m_buffer.right(totalLen - mesg_len);

        //更新长度
        totalLen = buffer.size();

        //更新多余数据
        m_buffer = buffer;

    }
}
