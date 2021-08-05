#pragma once
#include <QIODevice>
#include <QObject>
#include <QStringList>
#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QTextCodec>
#else
#include <QtCore5Compat/QRegExp>
#include <QtCore5Compat/QTextCodec>
#endif

#include <QtGlobal>

class CSV
{
public:
    QString readLine();
    bool parseLine(QStringList& ret);

    static bool getGameData(int gameID, unsigned char* table, QString& gamename);

private:
    CSV(QIODevice* device);

    QIODevice* m_device;
    QString m_string;
    int m_pos;

    static QTextCodec const* const m_codec;
    static const QRegExp m_rx, m_rx2;
};
