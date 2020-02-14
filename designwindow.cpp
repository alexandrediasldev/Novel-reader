#include "designwindow.h"

designWindow::designWindow(QGridLayout *parentLayoutInner,QGridLayout *parentLayoutOuter, QTextEdit *parentReadingPage, QString parentBackColor,QString parentTextColor
                           ,int parentheigthvalue,int parentwidthvalue,int parentScrollBar,int parentTextSize, QString parentTextFamily,QString parentBackgroundPath
                           ,int parentBorderSize,QString parentBorderColor,QString parentBorderState,int parentLetterSpacing,int parentPaddingH,
                           int parentPaddingV)
{

layoutInner =parentLayoutInner;
layoutOuter = parentLayoutOuter;
readingPage =parentReadingPage;
textColor = parentTextColor;
backColor =parentBackColor;
scrollBar =parentScrollBar;
textSize =parentTextSize;
textFamily =parentTextFamily;
backgroundPath = parentBackgroundPath;
borderSize = parentBorderSize;
borderColor = parentBorderColor;
borderState =parentBorderState;
letterSpacing =parentLetterSpacing;
paddingH = parentPaddingH;
paddingV = parentPaddingV;
heigthValue =parentheigthvalue;
widthValue= parentwidthvalue;

designTab = new QTabWidget(this);

QWidget *mainTab = new QWidget();
QWidget *colorTab = new QWidget();
QWidget *generalTab = new QWidget();
QWidget *advancedTab = new QWidget();

setupGeneralTab(generalTab);
setupAdvancedTab(advancedTab);


QObject::connect(spinboxMarginH,SIGNAL(valueChanged(int)),this,SLOT(changeMarginH(int)));
QObject::connect(spinboxMarginV,SIGNAL(valueChanged(int)),this,SLOT(changeMarginV(int)));
QObject::connect(marginH,SIGNAL(sliderReleased()),this,SLOT(saveConfig()));
QObject::connect(marginV,SIGNAL(sliderReleased()),this,SLOT(saveConfig()));
QObject::connect(buttonBackColor,SIGNAL(clicked()),this,SLOT(openColorBack()));
QObject::connect(buttonTextColor,SIGNAL(clicked()),this,SLOT(openColorText()));
QObject::connect(checkBoxScrollBar,SIGNAL(stateChanged(int)),this,SLOT(changeScrollBar(int)));
QObject::connect(buttonFont,SIGNAL(clicked()),this,SLOT(openFont()));
QObject::connect(buttonBackgroundImage,SIGNAL(clicked()),this,SLOT(getBackgroundPath()));
QObject::connect(buttonBackgroundColor,SIGNAL(clicked()),this,SLOT(openColorBackground()));
QObject::connect(buttonBorderColor,SIGNAL(clicked()),this,SLOT(openColorBorder()));
QObject::connect(spinboxBorderSize,SIGNAL(valueChanged(int)),this,SLOT(changeBorder()));
QObject::connect(checkBoxBorder,SIGNAL(stateChanged(int)),this,SLOT(activateBorder(int)));
QObject::connect(spinboxSpacing,SIGNAL(valueChanged(int)),this,SLOT(changeLetterSpacing()));
QObject::connect(spinboxPaddingH,SIGNAL(valueChanged(int)),this,SLOT(changePaddingH(int)));
QObject::connect(spinboxPaddingV,SIGNAL(valueChanged(int)),this,SLOT(changePaddingV(int)));

designTab->addTab(generalTab,"General Settings");
designTab->addTab(advancedTab,"Advanced Settings");
designTab->setGeometry(generalTab->rect());


timerPadding1 = new QTimer();
timerPadding2 = new QTimer();


}
void designWindow::setupGeneralTab(QWidget *generaltab)
{
    /*GeneralTab*/
    QVBoxLayout *layoutTabGeneral = new QVBoxLayout(generaltab);

    /*Font*/

    labelTextFont = new QLabel("Font:");
    layoutTabGeneral->addWidget(labelTextFont);

    buttonFont = new QPushButton();
    buttonFont->setText(textFamily);
    layoutTabGeneral->addWidget(buttonFont);

    /*Color*/

    buttonTextColor = new QPushButton(textColor);
    buttonBackColor = new QPushButton(backColor);
    addSquareColor(buttonTextColor,textColor);
    addSquareColor(buttonBackColor,backColor);

    labelBackColor = new QLabel("Change primary background color:");
    labelTextColor = new QLabel("Change text color:");

    layoutTabGeneral->addWidget(labelTextColor);
    layoutTabGeneral->addWidget(buttonTextColor);
    layoutTabGeneral->addWidget(labelBackColor);
    layoutTabGeneral->addWidget(buttonBackColor);


    /*Secondary Background*/

    labelBackground = new QLabel("Secondary background style:");
    layoutTabGeneral->addWidget(labelBackground);


    layoutBackground = new QHBoxLayout();

    buttonBackgroundImage = new QPushButton();
    buttonBackgroundImage->setText("Choose a secondary background Image");
    buttonBackgroundColor = new QPushButton();
    buttonBackgroundColor->setText("Choose a secondary background Color");

    layoutBackground->addWidget(buttonBackgroundColor);
    layoutBackground->addWidget(buttonBackgroundImage);

    layoutTabGeneral->addLayout(layoutBackground);
}
void designWindow::setupAdvancedTab(QWidget *advancedTab)
{

    /*AdvancedTab*/
    QVBoxLayout *layoutTabAdvanced = new QVBoxLayout(advancedTab);
    /*Margin*/
    marginH = new QSlider(Qt::Horizontal);
    marginV = new QSlider(Qt::Horizontal);

    spinboxMarginH = new QSpinBox();
    spinboxMarginV = new QSpinBox();

    layoutSlider = new QVBoxLayout();
    layoutTabAdvanced->addLayout(layoutSlider);

    labelMarginH = new QLabel("Horizontal margin:");
    labelMarginV = new QLabel("Vertical margin:");


    layoutSlider->addWidget(labelMarginH);
    layoutSlider->addWidget(spinboxMarginH);
    layoutSlider->addWidget(labelMarginV);
    layoutSlider->addWidget(spinboxMarginV);

    spinboxMarginH->setRange(0,50);
    spinboxMarginV->setRange(0,50);



    spinboxMarginV->setValue(heigthValue);


    spinboxMarginH->setValue(widthValue);


    spinboxPaddingH = new QSpinBox();
    spinboxPaddingV = new QSpinBox();
    spinboxPaddingH->setRange(0,30);
    spinboxPaddingV->setRange(0,30);
    spinboxPaddingH->setValue(paddingH);
    spinboxPaddingV->setValue(paddingV);


    changePaddingHV(paddingH,paddingV);

    layoutSlider->addWidget(spinboxPaddingH);
    layoutSlider->addWidget(spinboxPaddingV);

    //set the value from last session

    QRect geometry = layoutOuter->contentsRect();
    double width = geometry.width();
    double height = geometry.height();

    double newvalue1 = (height*heigthValue)/200;
    double newvalue2 = (width*widthValue)/200;

    layoutInner->setContentsMargins(int(newvalue2),int(newvalue1),int(newvalue2),int(newvalue1));

    marginbottomtop = int(newvalue1);
    marginleftright = int(newvalue2);


    /*Letter Spacing*/

    labelLetterSpacing = new QLabel("Letter spacing(%):");
    spinboxSpacing = new QSpinBox();

    spinboxSpacing->setRange(100,1000);
    spinboxSpacing->setValue(letterSpacing);

    layoutSpacing = new QHBoxLayout();
    layoutSpacing->addWidget(labelLetterSpacing);
    layoutSpacing->addWidget(spinboxSpacing);
    layoutTabAdvanced->addLayout(layoutSpacing);

    /*ScrollBar*/
    checkBoxScrollBar = new QCheckBox("Show Scroll Bar");
    checkBoxScrollBar->setChecked(scrollBar);

    labelScrollBar = new QLabel("Scrollbar policy:");
    layoutTabAdvanced->addWidget(labelScrollBar);

    layoutTabAdvanced->addWidget(checkBoxScrollBar);



    /*Border*/


    labelBorder = new QLabel("Border style:");
    layoutTabAdvanced->addWidget(labelBorder);

    layoutBorder= new QHBoxLayout();

    checkBoxBorder = new QCheckBox();

    spinboxBorderSize = new QSpinBox();
    spinboxBorderSize->setRange(0,20);
    spinboxBorderSize->setValue(borderSize);

    buttonBorderColor = new QPushButton();
    buttonBorderColor->setText("Choose a border Color");
    addSquareColor(buttonBorderColor,borderColor);
    if(borderState=="on")
    {
        checkBoxBorder->setChecked(true);
        activateBorder(1);
    }
    else
    {
        checkBoxBorder->setChecked(false);
        activateBorder(0);
    }

    layoutBorder->addWidget(checkBoxBorder,1);
    layoutBorder->addWidget(spinboxBorderSize,1);
    layoutBorder->addWidget(buttonBorderColor,20);

    layoutTabAdvanced->addLayout(layoutBorder);
}
void designWindow::changeMarginH(int value)
{

    //Useless (maybe)
    //widthValueP=0;
    //heigthValueP=0;
    //changeStyle();


    widthValue=value;
    //set the horizontal size of the reading layout based on the size of the window and the value of the slider
    QRect geometry = layoutOuter->contentsRect();
    double width = geometry.width();


    double newvalue = (width*value)/200;
    if(width > 0)
    {
    layoutInner->setContentsMargins(int(newvalue),marginbottomtop,int(newvalue),marginbottomtop);
    marginleftright = int(newvalue);
    }

    if(!timerPadding1->isSingleShot())
    {
        timerPadding1->stop();
        timerPadding1->singleShot(100,[=]() {changePaddingH(spinboxPaddingH->value());});
    }


    if(!timerPadding2->isSingleShot())
    {
        timerPadding2->stop();
        timerPadding2->singleShot(200,[=]() {changePaddingV(spinboxPaddingV->value());});
    }









}
void designWindow::changeMarginV(int value)
{
    //Useless (maybe)
    //widthValueP=0;
    //heigthValueP=0;
    //changeStyle();

    heigthValue =value;
    //set the vertical size of the reading/inner layout based on the size of the window and the value of the slider
    QRect geometry = layoutOuter->contentsRect();

    double height = geometry.height();
    double newheight = (height*value)/200;
    if(height > 0)
    {
    layoutInner->setContentsMargins(marginleftright,int(newheight),marginleftright,int(newheight));
    marginbottomtop = int(newheight);
    }

    if(!timerPadding1->isSingleShot())
    {
        timerPadding1->stop();
        timerPadding1->singleShot(100,[=]() {changePaddingH(spinboxPaddingH->value());});
    }


    if(!timerPadding2->isSingleShot())
    {
        timerPadding2->stop();
        timerPadding2->singleShot(200,[=]() {changePaddingV(spinboxPaddingV->value());});
    }









}
void designWindow::changePaddingH(int value)
{



    paddingH=value;
    //set the horizontal size of the reading layout based on the size of the window and the value of the slider
    QRect geometry = layoutInner->contentsRect();
    double width = geometry.width();
    if(width<0)
    {
        return;
    }





    double newvalue = (width*value)/200;
    widthValueP = int(newvalue);



    changeStyle();

}
void designWindow::changePaddingV(int value)
{
     paddingV=value;
    //set the vertical size of the reading layout based on the size of the window and the value of the slider
    QRect geometry = layoutInner->contentsRect();
     double heigth = geometry.height();
     if(heigth<0)
     {
         return;
     }


    double newvalue = (heigth*value)/200;


    heigthValueP = int(newvalue);

    changeStyle();

}
void designWindow::changePaddingHV(int valueH,int valueV)
{
    paddingH= valueH;
    paddingV= valueV;
   //set the vertical size of the reading layout based on the size of the window and the value of the slider
    QRect geometry = layoutInner->contentsRect();
    double heigth = geometry.height();
    double width = geometry.width();
    if(heigth<0)
    {
        return;
    }
    if(width<0)
    {
        return;
    }



    double newvalueH = (width*valueH)/200;
    widthValueP = int(newvalueH);
    double newvalueV = (heigth*valueV)/200;
    heigthValueP = int(newvalueV);
    //QMessageBox::information(this,QString::number(widthValueP),QString::number(heigthValueP));

}
void designWindow::changeScrollBar(int state)
{
readingPage->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
if(state==0)
{

    readingPage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollBar =0;
}
else {

    readingPage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollBar=1;
}
saveConfig();
}
void designWindow::changeLetterSpacing()
{

    letterSpacing = spinboxSpacing->value();
    QFont curFont = readingPage->font();
    curFont.setLetterSpacing(QFont::PercentageSpacing, letterSpacing);
    readingPage->setFont(curFont);

    changeStyle();
}
void designWindow::activateBorder(int state)
{
    if(state ==0)
    {
        borderStyle= "none";
        borderState="off";
        buttonBorderColor->setEnabled(false);
        spinboxBorderSize->setEnabled(false);
    }
    else
    {
        buttonBorderColor->setEnabled(true);
        spinboxBorderSize->setEnabled(true);
        borderStyle = QString::number(borderSize) +"px solid "+ borderColor;
        borderState="on";
    }
    changeStyle();
}
void designWindow::changeBorder()
{
    borderSize = spinboxBorderSize->value();
    borderStyle = QString::number(borderSize) +"px solid "+ borderColor;
    changeStyle();

}
void designWindow::saveConfig()
{
    QString filename = "config.txt";
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << backColor <<endl;
            stream << textColor <<endl;
            stream << QString::number(heigthValue) <<endl;
            stream << QString::number(widthValue) << endl;
            stream << scrollBar << endl;
            stream << QString::number(textSize) << endl;
            stream << textFamily << endl;
            stream << backgroundPath << endl;
            stream << borderSize << endl;
            stream << borderColor << endl;
            stream <<borderState << endl;
            stream <<QString::number(letterSpacing)<<endl;
            stream <<QString::number(paddingH)<<endl;
            stream <<QString::number(paddingV)<<endl;



            file.close();

        }

}
void designWindow::quit()
{
    this->quit();
}
void designWindow::getBackgroundPath()
{
    backgroundPath = QFileDialog::getOpenFileName(this,"Open a file",""," (*)");
    layoutInner->parentWidget()->setObjectName("background");
    layoutInner->parentWidget()->setStyleSheet("QWidget#background {"
                                    "border-image: url("+backgroundPath+")"
                                        "}");
    buttonBackgroundColor->setText("Choose a background Color");
    buttonBackgroundColor->setIcon(QIcon());


    saveConfig();
}
void designWindow::addSquareColor(QPushButton *buttonCurrent,QString colorCurrent)
{
    QPixmap pixmap(100,100);
    pixmap.fill(QColor(colorCurrent));
    QIcon redIcon(pixmap);

    buttonCurrent->setText(colorCurrent);
    buttonCurrent->setIcon(redIcon);
}
void designWindow::openColorBorder()
{
        QColor fontColor = QColorDialog::getColor(Qt::white, this);

        borderColor = fontColor.name();
        borderStyle = QString::number(borderSize) +"px solid "+ borderColor;

        addSquareColor(buttonBorderColor,borderColor);




        changeStyle();
}
void designWindow::openColorText()
{
        QColor fontColor = QColorDialog::getColor(Qt::white, this);

        textColor = fontColor.name();
        addSquareColor(buttonTextColor,textColor);


        changeStyle();

}
void designWindow::openColorBack()
{
        QColor fontColor = QColorDialog::getColor(Qt::white, this);


        backColor = fontColor.name();
        addSquareColor(buttonBackColor,backColor);


        changeStyle();

}
void designWindow::openColorBackground()
{
        QColor backgroundColor = QColorDialog::getColor(Qt::white, this);


        backgroundPath =backgroundColor.name();
        addSquareColor(buttonBackgroundColor,backgroundPath);



        layoutInner->parentWidget()->setObjectName("background");
        layoutInner->parentWidget()->setStyleSheet("QWidget#background {"
                                        "background-color: "+backgroundPath+";"
                                            "}");
        buttonBackgroundImage->setText("Choose a background Image");



        saveConfig();

}
void designWindow::openFont()
{
    //font choice for the reading font
    bool ok = false;

    QFont fontSelected = QFontDialog::getFont(&ok,readingPage->font(),this,"Choose a font");




    if(ok)
    {
        textSize =fontSelected.pointSize();
        textFamily = fontSelected.family();
        buttonFont->setText(fontSelected.family());

    }
    changeStyle();
}
void designWindow::changeStyle()
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
                               +QString::number(heigthValueP)+
                               "px;"
                               "padding-bottom: "
                               +QString::number(heigthValueP)+
                               "px;"
                               "padding-left: "
                               +QString::number(widthValueP)+
                               "px;"
                               "padding-right: "
                               +QString::number(widthValueP)+
                               "px;"


                            );

    saveConfig();

}



void designWindow::reject()
{
    timerPadding1->stop();
    timerPadding2->stop();
    changeStyle();
    QDialog::reject();



}

