#include "window.h"
#include "httpwindow.h"
#include "designwindow.h"
#include "qsmoothscrollarea.h"





window::window()
{

    /* Setting up the main window of the application and creating the menu
     *
     *
     *
     */


    centralWidget = new QWidget(this);



    //setting the widget for the main Window
    setCentralWidget(centralWidget);


    QMenu *menuFile = menuBar()->addMenu("&File");

    downloadAction = new QAction("&Download",this);
    menuFile->addAction(downloadAction);
    openFileAction = new QAction("&Open a File",this);
    menuFile->addAction(openFileAction);
    openFileCleanAction = new QAction("Open and Clean File",this);
    menuFile->addAction(openFileCleanAction);
    cleanAllAction = new QAction("Clean all files in the current directory",this);
    menuFile->addAction(cleanAllAction);
    quitAction = new QAction("&Quit",this);
    menuFile->addAction(quitAction);
    /*checkUpdateAction = new QAction("&Check Update",this);
    menuFile->addAction(checkUpdateAction);*/




    QMenu *menuSetting = menuBar()->addMenu("&Setting");



    designAction = new QAction("Design",this);
    menuSetting->addAction(designAction);
    customCSSAction = new QAction("Load Custom CSS",this);
    menuSetting->addAction(customCSSAction);

    QMenu *menuHelp = menuBar()->addMenu("&Help");




    QObject::connect(quitAction,SIGNAL(triggered()),qApp,SLOT(quit()));
    QObject::connect(downloadAction,SIGNAL(triggered()),this,SLOT(openDownloadTab()));

    QObject::connect(openFileAction,SIGNAL(triggered()),this,SLOT(openAndReadFile()));
    QObject::connect(openFileCleanAction,SIGNAL(triggered()),this,SLOT(openAndCleanFile()));
    QObject::connect(cleanAllAction,SIGNAL(triggered()),this,SLOT(cleanAllFileInADir()));


    QObject::connect(customCSSAction,SIGNAL(triggered()),this,SLOT(loadCustomCss()));
    QObject::connect(designAction,SIGNAL(triggered()),this,SLOT(openDesign()));

    //QKeyEvent *e = new QKeyEvent(QEvent::KeyPress,Qt::Key::Key_Right,Qt::NoModifier);
    //QObject::connect(e,SIGNAL(),this,SLOT(changeChapter()))


    addReadingPage();

    readConfig();


    readingPage->setFontPointSize(textSize);


    changeScrollBar(scrollBar);
    setBackgroundImage(backgroundPath);










}

void window::changeMargin()
{

    //set the size of the margin (we need to wait for the window to be shown this is why there is a timer)
    QRect geometryOut = layoutOuter->contentsRect();

    double widthOut = geometryOut.width();
    double heigthOut = geometryOut.height();

if(widthOut>=0 && heigthOut>=0)
{

    double newvalueOut1 = (heigthOut*valueV)/200;
    double newvalueOut2 = (widthOut*valueH)/200;

    layoutOuter->setMargin(0);
    layoutInner->setContentsMargins(int(newvalueOut2),int(newvalueOut1),int(newvalueOut2),int(newvalueOut1));


    QRect geometryIn = layoutInner->contentsRect();

    double widthIn = geometryIn.width();
    double heigthIn = geometryIn.height();
    if(widthIn>=0 && heigthIn>=0)
    {



    double newvalueIn1 = (paddingH*widthIn)/200;
    double newvalueIn2 = (paddingV*heigthIn)/200;
    realPaddingH = int(newvalueIn1);
    realPaddingV = int(newvalueIn2);

    }
    else
    {
         QTimer::singleShot(200, this, SLOT(changeMargin()));
    }

    changeStyle();







}
else
{
        QTimer::singleShot(200, this, SLOT(changeMargin()));
}
}
void window::changeScrollBar(int state)
{
if(state==0)
    readingPage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
else
    readingPage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void window::addReadingPage()
{

    /* Adding the different layer in the main window
     * layoutOuter is the main layout inside the centralWidget it contains background
     * layoutInner is the front most layout it contains the readingPage
     * In term of parent-child:(with parent being on the left)
     * centralWidget -> layoutOuter -> background -> layoutInner -> readingPage
     *
     */
    layoutOuter = new QGridLayout(centralWidget);

    background = new QWidget();

    layoutOuter->addWidget(background);

    layoutInner = new QGridLayout(background);



    readingPage = new QSmoothScrollArea();


    readingPage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    readingPage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    readingPage->setReadOnly(true);
    readingPage->setHtml("This is the debug text used in case there is no text opened");


    layoutInner->addWidget(readingPage);




}

void window::cleanAllFileInADir()
{
    if(QDir(QFileInfo(filePath).absolutePath()).dirName() == "Raw")
    {
        QString pathToClean = QFileInfo(QFileInfo(filePath).absolutePath()).absolutePath()+"/Clean";
        if(!QDir(pathToClean).exists())
        {

            QDir().mkdir(pathToClean);
        }


        QString pathwithoutname = QFileInfo(filePath).absolutePath()+"/";
        QDir directory(QFileInfo(filePath).absoluteDir());
        QStringList fileindir = directory.entryList(QDir::Files);
        foreach(QString filename, fileindir)
        {
            QFile file(pathwithoutname+filename);
            file.open(QFile::ReadOnly | QFile::Text);
            QTextStream stream(&file);

            QString html = stream.readAll();
            file.close();

            QFile file2(pathToClean+"/"+filename);
            if (file2.open(QIODevice::ReadWrite))
            {

                QString cleanHtml;
                cleanHtml = regexParagraph(html);

                QTextStream stream(&file2);
                stream << cleanHtml;
                file2.close();
            }

        }
        QMessageBox::information(this,"Clean","All files in the directory have been cleaned");

    }
    else
    {
        QMessageBox::warning(this,"Invalid directory","The directory is invalid, open a file in a valid directory named Raw");
    }
}
void window::openAndReadFile()
{
    getFilePath();


    QFile file(filePath);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    QString html = stream.readAll();
    file.close();
    stream.setCodec("UTF-8");


    readingPage->setHtml(html.toLocal8Bit());



}
void window::openAndReadFile(QString fileToOpen)
{
    filePath = fileToOpen;


    QFile file(filePath);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    QString html = stream.readAll();
    file.close();
    stream.setCodec("UTF-8");


    readingPage->setHtml(html.toLocal8Bit());

}
void window::openAndCleanFile()
{
    getFilePath();
    if(QDir(QFileInfo(filePath).absolutePath()).dirName() == "Raw")
    {
        QFile file(filePath);
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        QString html = stream.readAll();

        file.close();

        QString cleanHtml;
        cleanHtml = regexParagraph(html);


        //absolutepath two time to get into the dir of the novel from the dir raw

        QString pathToClean = QFileInfo(QFileInfo(filePath).absolutePath()).absolutePath()+"/Clean";
        if(!QDir(pathToClean).exists())
        {
            QDir().mkdir(pathToClean);
        }
        QString filename = QFileInfo(filePath).fileName();
        QFile file2(pathToClean+"/"+filename);
        if (file2.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file2);
            stream << cleanHtml;
            openAndReadFile(pathToClean+"/"+filename);
        }
    }
    else
    {
        QMessageBox::warning(this,"Invalid directory","The directory is invalid, open a file in a valid directory named Raw");
    }
}

QString window::regexParagraph(QString html)
{
    html.replace(QRegularExpression("(<head>(?:.|\n|\r)+?</head>)"),"");
    QString html2 = html;
    QString html3 ="";
    int i =0;
    int j =0;

    //parsing the the file to keep only the text between p
    //html2.replace(QRegularExpression("(<p>(.+?)</p>)"),"");
    html2.replace(QRegularExpression("<p.*>(.+)</p>"),"");
    while(i<html.length())
    {
        if(html[i] == html2[j])
        {
            i++;
            j++;
        }
        else {
            html3+= html[i];
            i++;
        }
    }
    return html3;



}

void window::changeChapter()
{
    //get all files name in the dir and load next chapter
    QString pathwithoutname = QFileInfo(filePath).absolutePath()+"/";
    QDir directory(QFileInfo(filePath).absoluteDir());
    QStringList fileindir = directory.entryList(QDir::Files);


    if(fileindir.indexOf(QFileInfo(filePath).fileName())+1< fileindir.length())
    {
        openAndReadFile(pathwithoutname + fileindir[fileindir.indexOf(QFileInfo(filePath).fileName())+1]);
    }
    else {
        QMessageBox::information(this,"No more chapter","No more chapter");
    }

}





void window::openDesign()
{


    designWindow win(layoutInner,layoutOuter,readingPage,backColor,textColor,valueV,valueH,scrollBar,textSize,textFamily,backgroundPath
                     ,borderSize,borderColor,borderState,letterSpacing,paddingH,paddingV);
    win.exec();
    win.setParent(this);

    win.setAttribute(Qt::WA_DeleteOnClose);
    connect(&win,SIGNAL(destroyed()),this,SLOT(readConfig()));
}

void window::openDownloadTab()
{



    HttpWindow win;
    win.exec();



}


void window::getFilePath()
{
    QString filePathHolder = QFileDialog::getOpenFileName(this,"Open a file",""," (*)");
    if(!filePathHolder.isEmpty())
    {
        filePath = filePathHolder;
        QDir(filePath);
    }



}
void window::getBackgroundPath()
{
    backgroundPath = QFileDialog::getOpenFileName(this,"Open a file",""," (*)");
}
void window::setBackgroundImage(QString file)
{
    if(file[0] != '#')
    {
        background->setObjectName("background");
        background->setStyleSheet("QWidget#background {"
                                        "border-image: url("+file+")"
                                            "}");
    }
    else {
        background->setObjectName("background");
        background->setStyleSheet("QWidget#background {"
                                        "background-color:"+file+";"
                                            "}");
    }

}

void window::saveConfig()
{
    QString filename = "config.txt";
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            for (int i =0;i<80;i++) {
               stream << " " << endl;
            }



        }
}
void window::loadCustomCss()
{
QString fileCssPath = QFileDialog::getOpenFileName(this,"Open a file",""," (*)");
QFile file(fileCssPath);
file.open(QFile::ReadOnly);
QString StyleSheet = QLatin1String(file.readAll());

readingPage->setStyleSheet(StyleSheet);

}
void window::readConfig()
{
    /*Read the config.txt file if it does not exist create a blank file
     *read the value inside the file and save them, if they do not exist
     *use the default value instead
     *
     */
    int index=0;
    QFile inputFile("config.txt");
    if(!inputFile.exists())
    {
        saveConfig();
    }
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();

          if(index==0)
                valueSetter(&backColor,"#ffffff",line);
          if(index==1) 
              valueSetter(&textColor,"#000000",line);
          if(index==2)
              valueSetter(&valueV,0,line);
          if(index==3)
              valueSetter(&valueH,0,line);
          if(index==4)
              valueSetter(&scrollBar,0,line);
          if(index==5)
              valueSetter(&textSize,18,line);
          if(index==6)
              valueSetter(&textFamily,"Arial",line);
          if(index==7)
              valueSetter(&backgroundPath,"COLOR",line);
          if(index==8)
              valueSetter(&borderSize,0,line);
          if(index==9)
              valueSetter(&borderColor,"#000000",line);
          if(index==10)
              valueSetter(&borderState,"off",line);
          if(index ==11)
              valueSetter(&letterSpacing,1,line);
          if(index==12)
              valueSetter(&paddingH,0,line);
          if(index==13)
              valueSetter(&paddingV,0,line);

          index++;

       }
       inputFile.close();

    }

    //non style sheet set up
    if(borderState=="off")
    {
        borderStyle="none";
    }
    else {
        borderStyle =QString::number(borderSize)+
                "px solid "
                +borderColor;
    }

    QFont curFont = readingPage->font();
    curFont.setLetterSpacing(QFont::PercentageSpacing, letterSpacing);
    readingPage->setFont(curFont);

    //changeMargin to set realPadding then it call changeStyle
    changeMargin();


}
void window::valueSetter(int *valueToSet,int defaultValue,QString line)
{
    if(line=='\n'||line==' ')
        *valueToSet= defaultValue;

    else {
        *valueToSet= line.toInt();
    }
}
void window::valueSetter(QString *valueToSet,QString defaultValue,QString line)
{
    if(line=='\n'||line==' ')
        *valueToSet= defaultValue;

    else {
        *valueToSet= line;
    }
}
void window::changeStyle()
{
    QRect geometry = readingPage->contentsRect();
    double width = geometry.width();
    double heigth = geometry.height();
    //check if padding has been set already, apply padding if yes
    if(realPaddingH> int(width) || realPaddingH < 0 ||realPaddingV> int(heigth) || realPaddingV < 0 )
    {
    //QMessageBox::information(this,"yes","yes");

    readingPage->setStyleSheet("background-color: "
                               +backColor+
                               ";"
                               "color: "
                                +textColor+
                                ";"
                               "font-size:"
                               +QString::number(textSize)+
                               "pt;"
                               "font-family:"
                               +textFamily+
                               ";"
                               "outline: 0px;"
                               "outline: none;"
                               "outline-style: none;"
                               "border: "
                                +borderStyle+
                                  ";"

                            );

    }
    else
    {



    readingPage->setStyleSheet("background-color: "
                               +backColor+
                               ";"
                               "color: "
                                +textColor+
                                ";"
                               "font-size:"
                               +QString::number(textSize)+
                               "pt;"
                               "font-family:"
                               +textFamily+
                               ";"
                               "outline: 0px;"
                               "outline: none;"
                               "outline-style: none;"
                               "border: "
                                +borderStyle+
                                  ";"
                                  "padding-top: "
                                  +QString::number(realPaddingV)+
                                  "px;"
                                  "padding-bottom: "
                                  +QString::number(realPaddingV)+
                                  "px;"
                                  "padding-left: "
                                  +QString::number(realPaddingH)+
                                  "px;"
                                  "padding-right: "
                                  +QString::number(realPaddingH)+
                                  "px;"
                               );
}


}


void window::replaceHtml(QString *html)
{
    html->replace(QRegularExpression("(<head.*>(?:.|\n|\r)+?</head>)"),"");
    html->replace(QRegularExpression("(<header.*>(?:.|\n|\r)+?</header>)"),"");
    html->replace(QRegularExpression("(<footer.*>(?:.|\n|\r)+?</footer>)"),"");
    html->replace(QRegularExpression("(<link.*>(?:.|\n|\r)+?</link>)"),"");
    html->replace(QRegularExpression("(<script.*>(?:.|\n|\r)+?</script>)"),"");
    html->replace("â€“","–");
    html->replace("â€","”");
    html->replace("â€¦","…");
    html->replace("â€™","’");
    html->replace("â€œ","“");
    html->replace("ã€Œ","「");
    html->replace("ã€","」");

}


void window::paintEvent(QPaintEvent *)
{
    //custom paint event needed for the style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
void window::keyPressEvent ( QKeyEvent * event )
{
     if(event->key() == Qt::Key_N) // à chager par une auter touche
        changeChapter();


}
