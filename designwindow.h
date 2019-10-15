#ifndef DESIGNWINDOW_H
#define DESIGNWINDOW_H

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
#include <QGridLayout>
#include <QMargins>
#include <QFile>
#include <QTextStream>
#include <QCheckBox>
#include <QFileDialog>
#include <QSpinBox>
#include <QTabWidget>


class designWindow : public QDialog
{
    Q_OBJECT
public:
    designWindow(QGridLayout *parentLayoutInner,QGridLayout *parentLayoutOuter, QTextEdit *parentReadingPage,QString parentBackColor,QString parentTextColor
                 ,int parentheigthvalue,int parentwidthvalue,int parentScrollBar,int parentTextSize, QString parentTextFamily,QString parentBackgroundPath
                 ,int parentBorderSize,QString parentBorderColor,QString parentBorderState,int parentLetterSpacing,int parentPaddingH,
                 int parentPaddingV);

private:
    QSlider *marginH;
    QSlider *marginV;

    QTabWidget *designTab;


    QPushButton *buttonTextColor;
    QPushButton *buttonBackColor;
    QPushButton *buttonFont;
    QPushButton *buttonBackgroundImage;
    QPushButton *buttonBackgroundColor;
    QPushButton *buttonBorderColor;


    QCheckBox *checkBoxScrollBar;
    QCheckBox *checkBoxBorder;

    QSpinBox *spinboxBorderSize;
    QSpinBox *spinboxSpacing;
    QSpinBox *spinboxPaddingH;
    QSpinBox *spinboxPaddingV;
    QSpinBox *spinboxMarginH;
    QSpinBox *spinboxMarginV;


    QLabel *labelMarginH;
    QLabel *labelMarginV;
    QLabel *labelPaddingH;
    QLabel *labelPaddingV;
    QLabel *labelTextColor;
    QLabel *labelBackColor;
    QLabel *labelBackground;
    QLabel *labelScrollBar;
    QLabel *labelBorder;
    QLabel *labelTextFont;
    QLabel *labelLetterSpacing;




    QTextEdit *readingPage;

    int paddingH;
    int paddingV;
    int heigthValueP;
    int widthValueP;
    int heigthValue;
    int widthValue;
    int marginleftright;
    int marginbottomtop;
    QString textColor;
    QString backColor;
    int scrollBar;
    int textSize;
    QString textFamily;
    QString backgroundPath;
    QString borderColor;
    int borderSize;
    QString borderStyle;
    QString borderState;
    int letterSpacing;



    QVBoxLayout *layoutSlider;
    QHBoxLayout * layoutBackground;
    QHBoxLayout *layoutBorder;
    QGridLayout *layoutRead;
    QGridLayout *layoutOuter;
    QHBoxLayout *layoutSpacing;


    QTimer *timerPadding1;
    QTimer *timerPadding2;

public slots:
    void changeMarginH(int value);
    void changeMarginV(int value);
    void changeScrollBar(int state);
    void changeBorder();
    void changeLetterSpacing();
    void changePaddingH(int value);
    void changePaddingV(int value);

    void activateBorder(int state);
    void openColorText();
    void openColorBack();
    void openColorBorder();
    void addSquareColor(QPushButton *buttonCurrent,QString colorCurrent);
    void openColorBackground();
    void openFont();
    void changeStyle();
    void saveConfig();
    void quit();
    void getBackgroundPath();


    void reject();
};

#endif // DESIGNWINDOW_H
