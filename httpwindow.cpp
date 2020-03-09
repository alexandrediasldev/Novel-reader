
#include "httpwindow.h"

HttpWindow::HttpWindow()
{


    urlLineEdit = new QLineEdit("https://www.wuxiaworld.com/novel/the-great-ruler/tgr-chapter-@");
    urlLineEdit->setWhatsThis("Input your url here");

    nameLineEdit = new QLineEdit("");


    log = new QPlainTextEdit;
    log->setReadOnly(true);
    log->setWhatsThis("Log console: show you what's happening");



    urlLabel = new QLabel(tr("&URL:"));
    urlLabel->setBuddy(urlLineEdit);
    statusLabel = new QLabel(tr("Please enter the URL of a file you want to "
                                "download."));
    statusLabel->setWordWrap(true);
    logLabel = new QLabel(tr("Log:"));
    progressLabel = new QLabel(tr("Progress:"));
    nameLabel = new QLabel(tr("Name of the Directory:"));

    lower = new QSpinBox();
    upper = new QSpinBox();
    lower->setRange(1,10000);
    upper->setRange(1,10000);
    lower->setWhatsThis("Set your lower bound for downloading multiple chapters");
    upper->setWhatsThis("Set your upper bound for downloading multiple chapters");

    downloadButton = new QPushButton(tr("Download"));
    downloadButton->setDefault(true);
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);


    progressBar = new QProgressBar();
    progressBar->setWhatsThis("Show the progress for each chapter being downloaded");




    connect(downloadButton, SIGNAL(clicked()), this, SLOT(downloadFile()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameLineEdit);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(urlLabel);
    topLayout->addWidget(urlLineEdit);
    topLayout->addWidget(lower);
    topLayout->addWidget(upper);




    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(nameLayout);
    mainLayout->addWidget(statusLabel);

    mainLayout->addWidget(buttonBox);
    mainLayout->addWidget(progressLabel);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(logLabel);
    mainLayout->addWidget(log);


    setLayout(mainLayout);

    setWindowTitle(tr("HTTP"));
    urlLineEdit->setFocus();


}

void HttpWindow::append(const QStringList &urls)
{
    for (const QString &urlAsString : urls)
        append(QUrl::fromEncoded(urlAsString.toLocal8Bit()));

    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SIGNAL(finished()));
}

void HttpWindow::append(const QUrl &url)
{
    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextDownload()));

    downloadQueue.enqueue(url);
    ++totalCount;
}

QString HttpWindow::saveFileName(const QUrl &url)
{


    QString path = url.path();

    QString basename = QFileInfo(path).fileName();

    QString nameDir= nameLineEdit->text();
    if(nameDir.isEmpty())
    {
        nameDir = "download";
    }

    if (basename.isEmpty())
    {
        basename="download";
        if (QFile::exists("Novel/"+nameDir+"/Raw/"+basename)) {
            // already exists, don't overwrite
            int i = 0;
            basename += '.';
            while (QFile::exists("Novel/"+nameDir+"/Raw/"+basename + QString::number(i)))
            {
                ++i;

            }


            basename += QString::number(i);
        }
    }



    createDirSafe("/Novel");
    createDirSafe("/Novel/"+nameDir);
    createDirSafe("/Novel/"+nameDir+"/Raw");

    return "Novel/"+nameDir+"/Raw/"+basename;
}
void HttpWindow::createDirSafe(QString path)
{
    if (!QDir(QDir::currentPath()+path).exists())
        QDir().mkdir(QDir::currentPath()+path);
}
void HttpWindow::startNextDownload()
{
    QString *info = new QString("files downloaded successfully");
    info->append(QString::number(downloadedCount));
    info->append("/");
    info->append(QString::number(totalCount));
    if (downloadQueue.isEmpty()) {
        QMessageBox::information(this, "Download state",*info);

        downloadButton->setEnabled(true);
        emit finished();
        return;
    }

    QUrl url = downloadQueue.dequeue();

    QString filename = saveFileName(url);
    log->appendPlainText(filename);
    output.setFileName(filename);

    if (!output.open(QIODevice::WriteOnly)) {
    log->appendPlainText("Problem opening save file "+filename+" for download "+url.toEncoded().constData()+" : "+output.errorString()+"\n");


        startNextDownload();
        return;                 // skip this download
    }

    QNetworkRequest request(url);
    currentDownload = manager.get(request);
    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));
    connect(currentDownload, SIGNAL(finished()),
            SLOT(downloadFinished()));
    connect(currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));
    QString logstring = url.toEncoded().constData();
    log->appendPlainText("Downloading "+(logstring)+" ...\n");

    downloadTime.start();
}

void HttpWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    //progressBar.setStatus(bytesReceived, bytesTotal);
    progressBar->show();
    progressBar->setMaximum(bytesTotal);
    progressBar->setValue(bytesReceived);
    progressBar->show();
    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }



    //progressBar.setMessage(QString::fromLatin1("%1 %2")
                          // .arg(speed, 3, 'f', 1).arg(unit));
    //progressBar.update();
}

void HttpWindow::downloadFinished()
{

    //progressBar.clear();
    output.close();

    if (currentDownload->error()) {
        //download failed
        log->appendPlainText("Failed: "+currentDownload->errorString()+"\n");

        output.remove();
    } else {
        // let's check if it was actually a redirect
        if (isHttpRedirect()) {
            reportRedirect();
            output.remove();
        } else {
            log->appendPlainText("Succeeded.\n");

            ++downloadedCount;
        }
    }

    currentDownload->deleteLater();
    startNextDownload();
}

void HttpWindow::downloadReadyRead()
{
    output.write(currentDownload->readAll());
}

bool HttpWindow::isHttpRedirect() const
{
    int statusCode = currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void HttpWindow::reportRedirect()
{
    int statusCode = currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QUrl requestUrl = currentDownload->request().url();
    log->appendPlainText("Request: "+requestUrl.toDisplayString()+" was redirected with code: "+QString::number(statusCode)+"\n");


    QVariant target = currentDownload->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (!target.isValid())
        return;
    QUrl redirectUrl = target.toUrl();
    if (redirectUrl.isRelative())
        redirectUrl = requestUrl.resolved(redirectUrl);
    log->appendPlainText("Redirected to: "+redirectUrl.toDisplayString()+"\n");

}

void HttpWindow::downloadFile()
{
    if(lower->value()>=upper->value())
    {
        //If the user input their values in the incorrect order
        int value = lower->value();
        lower->setValue(upper->value());
        upper->setValue(value);
    }

    downloadButton->setEnabled(false);
    QString originalText =urlLineEdit->text();
    QString textWithoutNumberFirst ="";
    QString textWithoutNumberSecond ="";
    bool first =true;

    for(int j = 0;j<originalText.length();j++)
    {
        if((originalText[j] != '@')&&first)
        {
           textWithoutNumberFirst += originalText[j];
        }
        if(originalText[j] == '@')
        {
            first = false;
        }
        if((originalText[j] != '@')&&!first)
        {
           textWithoutNumberSecond += originalText[j];
        }
    }


    for(int i=lower->value();i<=upper->value();i++)
    {

        this->append(textWithoutNumberFirst +QString::number(i)+textWithoutNumberSecond);
    }

}

