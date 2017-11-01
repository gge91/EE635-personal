#include "laucodecwidget.h"

LAUCodecWidget::LAUCodecWidget(QStringList strings, QWidget *parent) : QWidget(parent), files(strings)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0,0,0,0);

    QListWidget *widget = new QListWidget();
    this->layout()->addWidget(widget);

    if (files.isEmpty()){
        QSettings settings;
        QString directory = settings.value(QString("LAUImageWidget::lastDirectory"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();;
        files = QFileDialog::getOpenFileNames(NULL, QString("Select files to compress..."), directory, QString());
        if (files.isEmpty() == false) {
            settings.setValue(QString("LAUImageWidget::lastDirectory"), QFileInfo(files.first()).absolutePath());
        }
    }
    widget->addItems(files);
}

LAUCodecWidget::~LAUCodecWidget()
{

}

bool LAUCodecWidget::save(QString filename)
{
    uint imHeight;
    uint widthIm = 1024; //width of line for tiff
    if (filename.isEmpty()){
        QSettings settings;
        QString directory = settings.value(QString("LAUImageWidget::lastDirectory"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();;
        filename = QFileDialog::getSaveFileName(NULL, QString("Select save file..."), directory, QString("*.lau"));
        if (filename.isEmpty() == false) {
            settings.setValue(QString("LAUImageWidget::lastDirectory"), QFileInfo(filename).absolutePath());
        } else{
            return(false);
        }
    }

    if (filename.toLower().endsWith(".lau") == false){
        filename.append(QString(".lau"));
    }
    QByteArray ba = filename.toLocal8Bit();
    const char *charfname = ba.data();

    TIFF *output = TIFFOpen(charfname,"w");
    for (int i = 0; i< files.length(); i++){
        //int checkdir = TIFFSetDirectory(output,i); //set directory

        QFile data1(files.at(i)); //create qfile w/ filelist
        QFileInfo fileInfo(data1);
        QString filename(fileInfo.fileName());
        QByteArray fileba = filename.toLocal8Bit();
        char* filenameTIFF = fileba.data();
        uint32 check = data1.size(); //see how large file is
        imHeight = (check/widthIm)+1;

        TIFFSetField(output, TIFFTAG_DOCUMENTNAME, filenameTIFF);//set tifftags for current directory
        TIFFSetField(output, TIFFTAG_IMAGEWIDTH, widthIm);
        TIFFSetField(output, TIFFTAG_IMAGELENGTH, imHeight);
        TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(output, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
        TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(output, TIFFTAG_COMPRESSION, 5);
        TIFFSetField(output, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(output, widthIm));

        data1.open(QIODevice::ReadOnly); //file open
        int image_s;
        for (uint32 j = 0; j<= imHeight+2; j++){
            QByteArray blob = data1.read(widthIm); //read datastream into bytearray for writing to tiff
            image_s = TIFFWriteScanline(output,(unsigned char*)(blob.data()),j,0);
        }
        data1.close();
        TIFFWriteDirectory(output);
    }
    TIFFClose(output);
    return(true);
}

bool LAUCodecWidget::load(QString filename)
{
    if (filename.isEmpty()){
        QSettings settings;
        QString directory = settings.value(QString("LAUImageWidget::lastDirectory"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();;
        filename = QFileDialog::getOpenFileName(NULL, QString("Select file to open..."), directory, QString("*.lau"));
        if (filename.isEmpty() == false) {
            settings.setValue(QString("LAUImageWidget::lastDirectory"), QFileInfo(filename).absolutePath());
        } else{
            return(false);
        }
    }
    QByteArray ba = filename.toLocal8Bit();
    const char *charfname = ba.data();

    TIFF *input = TIFFOpen(charfname,"r");
    bool moreDir = true;
    char* fName;
    uint widthIm;
    uint imHeight;
    uint isBW;
    uint isChar;
    if (input){
        while (moreDir){
            TIFFGetField(input, TIFFTAG_DOCUMENTNAME, &fName);
            TIFFGetField(input, TIFFTAG_IMAGEWIDTH, &widthIm);
            TIFFGetField(input, TIFFTAG_IMAGELENGTH, &imHeight);
            TIFFGetField(input, TIFFTAG_SAMPLESPERPIXEL, &isBW);
            TIFFGetField(input, TIFFTAG_BITSPERSAMPLE, &isChar);

            QDir dirHandler = QDir::current();
            dirHandler.setFilter(QDir::Files|QDir::NoDotAndDotDot);
            QFileInfoList dirList = dirHandler.entryInfoList();
            bool overwrite = true;
            for(int i = 0; i<dirList.size(); i++){
                QFileInfo fileInfo = dirList.at(i);
                if (fileInfo.fileName() == fName){
                    QMessageBox warnMsg;
                    char textBuff[50];
                    sprintf_s(textBuff,"File %s already exists.",fName);
                    warnMsg.setText(textBuff);
                    warnMsg.setInformativeText("Do you want to overwrite?");
                    warnMsg.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
                    warnMsg.setDefaultButton(QMessageBox::Yes);
                    int ret = warnMsg.exec();
                    switch (ret) {
                    case QMessageBox::Yes:
                        break;
                    case QMessageBox::No:
                        overwrite = false;
                        break;
                    default:
                        break;
                    }
                    break;
                }
            }
            if(overwrite==false){
                moreDir = TIFFReadDirectory(input);
                continue;
            }
            QFile data1(fName); //create qfile w/ filelist
            data1.open(QIODevice::WriteOnly); //file open
            char placehold = 0;
            QByteArray blob(widthIm,placehold);
            for(uint i = 0; i<=imHeight; i++){
                TIFFReadScanline(input,(unsigned char*)(blob.data()),i,0);
                data1.write(blob);
            }
            data1.close();
            moreDir = TIFFReadDirectory(input);
        }
    }
    TIFFClose(input);

    // LOAD FILE AND THEN SAVE THE CONTENTS TO DISK IN THE SAME DIRECTORY AS THE SOURCE FILE
    // LOAD FILE AND THEN SAVE THE CONTENTS TO DISK IN THE SAME DIRECTORY AS THE SOURCE FILE
    // LOAD FILE AND THEN SAVE THE CONTENTS TO DISK IN THE SAME DIRECTORY AS THE SOURCE FILE

    return(true);
}
