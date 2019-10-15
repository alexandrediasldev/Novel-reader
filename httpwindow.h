#ifndef HTTPWINDOW_H
#define HTTPWINDOW_H

#include <QtNetwork>
#include <QtCore>

#include <QPlainTextEdit>
#include <QElapsedTimer>
#include <QTimer>
#include <QDialog>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLCDNumber>
#include <QSlider>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTextEdit>
#include <QPainter>
#include <QTextDocument>
#include <QColorDialog>
#include <QProgressDialog>
#include <QFile>
#include <QDialogButtonBox>
#include <QNetworkRequest>
#include <QFile>
#include <QObject>
#include <QFileInfo>
#include <QNetworkReply>
#include <QDir>
#include <QSpinBox>
#include <QProgressBar>

class HttpWindow : public QDialog
{

    Q_OBJECT
public:
    //explicit HttpWindow(QObject *parent = nullptr);
    HttpWindow();
    void append(const QUrl &url);
    void append(const QStringList &urls);
    QString saveFileName(const QUrl &url);

    void createDirSafe(QString path);

signals:
    void finished();

private slots:
    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

    void downloadFile();



private:
    bool isHttpRedirect() const;
    void reportRedirect();

    bool okdownload;
    QLabel *statusLabel;
    QLabel *logLabel;
    QLabel *progressLabel;
    QLabel *urlLabel;
    QLabel *nameLabel;
    QLineEdit *urlLineEdit;
    QLineEdit *nameLineEdit;
    QSpinBox *lower;
    QSpinBox *upper;

    QPlainTextEdit *log;
    QProgressBar *progressBar;
    QPushButton *downloadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;



    QNetworkAccessManager manager;
    QQueue<QUrl> downloadQueue;
    QNetworkReply *currentDownload = nullptr;
    QFile output;
    QTime downloadTime;


    int downloadedCount = 0;
    int totalCount = 0;


};

#endif
