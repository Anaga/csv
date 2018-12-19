#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QDateTime>
static QFile outFile;


void process_line(QString input)
{
    QStringList qsl = input.split(',');
    if(qsl.size()==3){
        double lux = qsl.at(0).toDouble();
        QString qsTemp = QString::number(lux,'f',6);
        qsl[0]=qsTemp;
        qDebug() << "QSL at 1 :" << qsl.at(1);
        qint64 unixTimeMilSec = static_cast<qint64>(qsl.at(1).toLongLong());
        qint64 unixTimeSec = (unixTimeMilSec/ static_cast<qint64>(1000));
        QDateTime dt =QDateTime::fromMSecsSinceEpoch(unixTimeMilSec);
        qsTemp = dt.toString();
        qDebug() << "Lux :" << lux << "unixTimeSec" << unixTimeSec << "dt: " << qsTemp << "Current hour: " << dt.time().hour();
        qsl.append(qsTemp);
        for (int i = 0; i < qsl.size(); ++i) {
            qDebug() << "QSL at "<<i << " : "<< qsl.at(i);
        }
        QString outStr = qsl.join(';');
        outStr.append("\n");
        outFile.write(qPrintable(outStr));
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString message;
    if (argc==2){
        QString qsFileName = argv[1];
        QFile file(qsFileName);
        if (!file.exists()){
            message = "Can't find file %1 to read";
            message = message.arg(qsFileName);
            qDebug() << message;
            return -1;
        }
        if(!file.open(QFile::ReadOnly | QFile::Text))  {
            message = "Can't open the file %1 for reading";
            message = message.arg(qsFileName);
            qDebug() << message;
            return -1;
        }
        QString outFileName = qsFileName+".log";
        outFile.setFileName(outFileName);
        outFile.open(QFile::Append | QFile::Text);

        message = "Start parsing the file %1, write output to file %2";
        message = message.arg(qsFileName).arg(outFileName);
        qDebug() << message;

        QString line;
        int i=0;
        while(!file.atEnd()){
            line = file.readLine();
            line = line.trimmed();
            process_line(line);
            i++;
            if (i%1000==1){
                qDebug() << "Parsed" << i << "lines from input file";
            }
        }
        qDebug() << "Parsed all " << i << "lines from input file";
        outFile.close();
        message = "Finish parsing the file %1, write output to file %2";
        message = message.arg(qsFileName).arg(outFileName);
        qDebug() << message;
        return 0;
    }
    message = "Enter input csv file name";
    qDebug() << message;
    return a.exec();
}
