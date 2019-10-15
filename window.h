#ifndef WINDOW_H
#define WINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLCDNumber>
#include <QSlider>
#include <QMessageBox>
#include <QInputDialog>
#include <QFontDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPainter>
#include <QTextDocument>
#include <QColorDialog>
#include <QTimer>
#include <QStyleOption>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include "qsmoothscrollarea.h"




class window : public QMainWindow
{
    Q_OBJECT
public:
    window();
    void valueSetter(QString *valueToSet,QString defaultValue,QString line);
    void valueSetter(int *valueToSet,int defaultValue,QString line);


    public slots:


    void getFilePath();
    void getBackgroundPath();
    void setBackgroundImage(QString file);
    void openDownloadTab();

    void addReadingPage();

    void replaceHtml(QString *html);
    void cleanAllFileInADir();
    void openAndReadFile();
    void openAndReadFile(QString fileToOpen);
    void openAndCleanFile();
    void changeChapter();







    void openDesign();

    void saveConfig();
    void readConfig();
    void loadCustomCss();

    void changeStyle();
    void changeMargin();
    void changeScrollBar(int state);

    void paintEvent(QPaintEvent *);
    void keyPressEvent ( QKeyEvent * event );
    private:


    QGridLayout *layoutOuter;
    QGridLayout *layoutInner;



    QAction *quitAction;
    QAction *downloadAction;
    QAction *checkUpdateAction;
    QAction *openFileAction;
    QAction *openFileCleanAction;
    QAction *cleanAllAction;


    QAction *customCSSAction;
    QAction *designAction;


    QString backColor;
    QString textColor;
    int textSize;
    int heigthvalue;
    int widthvalue;
    int scrollBar;
    QString outline;
    QString textFamily;
    int borderSize;
    QString borderColor;
    QString borderState;
    QString borderStyle;
    int letterSpacing;
    int paddingH;
    int paddingV;
    int realPaddingH;
    int realPaddingV;


    QPushButton *buttonGetHtml;

    QWidget *centralWidget;


    QWidget *background;
    QWidget *pageDownload;

    QSmoothScrollArea *readingPage;
    QString filePath;
    QString dirNovelPath;
    QString backgroundPath;


    QLineEdit *html;







};

#endif // WINDOW_H
