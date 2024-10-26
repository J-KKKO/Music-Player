#include "lyric.h"
#include <QDebug>

Lyric::Lyric(QObject *parent) : QObject(parent)
{

}

void Lyric::getLineLyric(QString path)
{
    int pos = 2; // 前面2个元素为空
    //初始化
    this->lyricSize=2; // pos变了这里跟着变

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error: Unable to open file " << path;
        return;
    }

    while (!file.atEnd())
    {
          QString lineStr = file.readLine(); // 获取一行文字
          lineStr = lineStr.trimmed(); // 去掉换行符
//          qDebug() << lineStr<<endl;

          lyricSize++; // 后面数组要添加元素

          /*分割一行的内容*/
          QStringList resultList=lineStr.split(']');

          QString timeStr= resultList.at(0);          //时间
          timeStr = timeStr.split('[').at(1);
          QString lyricStr= resultList.at(1);         //歌词

          QStringList templist=timeStr.split(':');
          QString min=templist.at(0);                 //分钟

          QString tempStr=templist.at(1);
          templist=tempStr.split('.');
          QString sec=templist.at(0);                 //秒

          this->lyrics[pos].pos=pos;
          this->lyrics[pos].time=min.toInt()*60*1000+sec.toInt()*1000; // 转成ms
          this->lyrics[pos].lyricStr=lyricStr;
    //      qDebug()<<lyrics[pos].pos<<lyrics[pos].time<<lyrics[pos].lyricStr;

          pos++;
    }

    file.close();
}

/*time为目前播放到的时间*/
int Lyric::findNeedPos(qint64 time)
{
    //找到需到播放位置
    int needPos = 0;
    for (int n = 0; n < lyricSize; n++)
    {
        if (time == lyrics[n].time)
        {
            // 找到精确的歌词行位置，将歌词位置返回
            needPos = lyrics[n].pos;
            qDebug()<< "n:" << n;
            qDebug()<< "time:" << time;
            qDebug()<< "lyrics[n].time:" << lyrics[n].time;
            qDebug()<< "精准" << needPos;
            return needPos;
        }
        else if (time < lyrics[n].time)
        {
            qDebug()<< "n:" << n;
            qDebug()<< "time:" << time;
            qDebug()<< "lyrics[n].time:" << lyrics[n].time;
            // 返回前一行歌词的位置
            needPos = (lyrics[n].pos - 1);
            qDebug()<< "前一行" << needPos;
            return needPos;
        }
        else
        {
            qDebug()<< "n:" << n;
            qDebug()<< "time:" << time;
            qDebug()<< "lyrics[n].time:" << lyrics[n].time;
            qDebug()<< "时间多了" << needPos;
        }
    }
    // 如果循环结束未找到合适的时间位置，返回最后一行歌词的位置
    qDebug()<< "找不到，歌词数量为" << lyricSize;
    needPos = lyricSize - 1;
    return needPos;
}

void Lyric::lyricCenterColor(QGroupBox * groupBox)
{
    //歌词显示,设置颜色
    QLabel *labelCentre = (QLabel*)groupBox->childAt(0, 180);
    QLabel *labelUp = (QLabel*)groupBox->childAt(0, 120);
    QLabel *labelDown = (QLabel*)groupBox->childAt(0, 240);
    if (labelCentre != nullptr && labelUp != nullptr && labelDown != nullptr)
    {
        labelCentre->setStyleSheet("QLabel{color:white;}");
        QFont fontCentre("Display", 20, QFont::Bold);// 字体名称、大小、样式
        labelCentre->setFont(fontCentre);

        labelUp->setStyleSheet("QLabel{color:grey;}");
        QFont fontUp("Display", 15, QFont::Light);// 字体名称、大小、样式
        labelUp->setFont(fontUp);

        labelDown->setStyleSheet("QLabel{color:grey;}");
        labelDown->setFont(fontUp);
    }
}

void Lyric::lyricMove(int distance,QLabel* lyric_p[],QGroupBox *grounpBox)
{
    for(int n=0;n<100;n++)
    {
        lyric_p[n]->move(0,lyric_p[n]->y()+distance); // move能将label移动到指定的x，y处
    }

    lyricCenterColor(grounpBox);
    currentpos=currentpos-distance/60;
}

void Lyric::movePos(qint64 time,QLabel* lyric_p[],QGroupBox * grounpBox)
{
    //获得需要播放位置
    int needPos=0;                     //需到播放位置
    needPos=findNeedPos(time);
    qDebug()<< "needPos：" << needPos;
    qDebug()<< "currentPos：" << currentpos;
    if(needPos==currentpos)            // 需要到的位置跟当前位置相同，退出不操作
    {
        qDebug()<< "精确" << needPos;
        return;
    }
    else if(needPos>currentpos)        // 需要到的位置大于当前位置，说明要向上
    {
        int distance=0-(needPos-currentpos)* 60; // distance为负数
        qDebug()<< "向上" << needPos-currentpos;
        lyricMove(distance,lyric_p,grounpBox);
    }
    else                                    // 需要到的位置小于当前位置，说明要向下
    {
        t++;
        qDebug()<<t;
        int distance=(currentpos-needPos) * 60;
        qDebug()<< "向下" << currentpos-needPos;
        lyricMove(distance,lyric_p,grounpBox);
    }
}

void Lyric::restateLyric(QLabel* lyric_p[])
{
    //清空所有lyric——label回复开始状态
    for(int n=0;n<100;n++)
    {
        lyric_p[n]->setText("");
        lyric_p[n]->move(0,n*60); // 回归原位
        QFont font("Display", 15, QFont::Light);// 字体名称、大小、样式
        lyric_p[n]->setFont(font);
        lyric_p[n]->setStyleSheet("QLabel{color:grey}"); // 把颜色也还原了
    }
    currentpos=3; // 很关键，影响着歌词高亮是否正确对应.
    qDebug()<< "重置！！！！！！！！！！！！！" ;
}

void Lyric::setText(QLabel* lyric_p[])
{
    //歌词复位
    restateLyric(lyric_p);
    //依次填充歌词
    for(int n=0;n<lyricSize;n++)
    {
        lyric_p[n]->setText(lyrics[n].lyricStr);
    }
}

void Lyric::lyricDown(QLabel* lyric_p[],QGroupBox *grounpBox)
{
    lyricMove(60,lyric_p,grounpBox);
}
void Lyric::lyricUp(QLabel* lyric_p[],QGroupBox *grounpBox)
{
    lyricMove(-60,lyric_p,grounpBox);
}





