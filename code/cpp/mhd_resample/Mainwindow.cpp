#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ESResample3D.h"
#include "ESGPUResample3D.h"
#include "imagereader.h"
#include "imageexporter.h"
#include <QFileDialog>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QTime>
#include <QCheckBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {

    m_inputFileName.clear();
    m_outputFileName.clear();
    m_originPath.clear();
    m_referencePath.clear();
    m_outputPath.clear();
//    m_originPath = "F:\\TianMinGT\\TianMinGT";
//    m_referencePath = "E:\\data\\medicaldata\\esdata\\TianMinGT-Vessel-WGT\\TianMinGT-Vessel-WGT";
//    m_outputPath = "F:\\casecpp\\outputall";

    ui->setupUi(this);
    setParametersDefault();
    connect(ui->m_importImage, SIGNAL(clicked()), this, SLOT(on_m_importImage_clicked()), Qt::UniqueConnection);
    connect(ui->m_outputImage, SIGNAL(clicked()), this, SLOT(on_m_outputImage_clicked()), Qt::UniqueConnection);
    connect(ui->m_mhdResampleGenerate, SIGNAL(clicked()), this, SLOT(on_m_mhdResampleGenerate_clicked()),
            Qt::UniqueConnection);
    connect(ui->m_originDir, SIGNAL(clicked()), this, SLOT(on_m_originDir_clicked), Qt::UniqueConnection);
    connect(ui->m_referenceDir, SIGNAL(clicked()), this, SLOT(on_m_referenceDir_clicked()), Qt::UniqueConnection);
    connect(ui->m_outpurDir, SIGNAL(clicked()), this, SLOT(on_m_outputDir_clicked()), Qt::UniqueConnection);
    connect(ui->m_batchGenerate, SIGNAL(clicked()), this, SLOT(on_m_batchGenerate_clicked()), Qt::UniqueConnection);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setParametersDefault() {
    ui->m_spaX->setText("1");
    ui->m_spaY->setText("1");
    ui->m_spaZ->setText("1");
    ui->m_linearInterpoltion->setChecked(true);
}

void MainWindow::on_m_importImage_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import file name"), "");
    m_inputFileName = fileName;
    qDebug() << fileName;
}

void MainWindow::on_m_outputImage_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file name"), "./", "*.mhd");
    m_outputFileName = fileName;
    qDebug() << fileName;
}

void MainWindow::on_m_mhdResampleGenerate_clicked() {
    QTime start;
    start.start();
    ui->m_message->clear();

    m_parameters.spacingX = ui->m_spaX->text().toFloat();
    m_parameters.spacingY = ui->m_spaY->text().toFloat();
    m_parameters.spacingZ = ui->m_spaZ->text().toFloat();

    qDebug() << m_parameters.resampleMethod;
    qDebug() << m_parameters.spacingX << m_parameters.spacingX << m_parameters.spacingX;

    QString originmhd = m_inputFileName;
    MiaImageReader r;
    r.setFileName(originmhd, MIA_AUTODETECT);
    r.reAllocOutputImage();
    MiaImageND *image = r.outputImage;

    ESResample3D resample;
    resample.setInputImage(image);
    ResampleMethod method;

    if (ui->m_linearInterpoltion->isChecked())
        ResampleMethod method = ResampleMethod::Mia_Resampe_LinearInterpoltion;
    else if (ui->m_nearestNeighbor->isChecked())
        ResampleMethod method = ResampleMethod::Mia_Resample_NearestNeighbor;
    else
        ResampleMethod method = ResampleMethod::Mia_Resampe_TriangleConvolution;

    resample.setCurrentResampleMethod(method);
    resample.setResampleSpacing(m_parameters.spacingX, m_parameters.spacingY, m_parameters.spacingZ);
    resample.reAllocOutputImage();
    resample.runOperation();

    MiaImageExporter e;
    e.setFileName(m_outputFileName, MIA_AUTODETECT);
    e.inputImage = resample.getOutputImage();
    e.runOperation();
    qDebug() << "case time cost: " << start.elapsed() << "ms";
    QString tip = "case time cost: " + QString::number(start.elapsed()) + " ms";
    ui->m_message->setText(tip);
}

void MainWindow::on_m_originDir_clicked() {
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Import file path"), "");
    m_originPath = filePath;
    qDebug() << filePath;
}

void MainWindow::on_m_referenceDir_clicked() {

    QString filePath = QFileDialog::getExistingDirectory(this, tr("Import file path"), "");
    m_referencePath = filePath;
    qDebug() << filePath;
}

void MainWindow::on_m_outpurDir_clicked() {
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Import file path"), "");
    m_outputPath = filePath;
    qDebug() << filePath;
}

void MainWindow::on_m_batchGenerate_clicked() {
    qDebug() << m_originPath;
    qDebug() << m_referencePath;
    qDebug() << m_outputPath;

    QDirIterator it(m_originPath, QStringList() << "step1", QDir::NoFilter, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QDir dir(it.next());
        QFileInfoList files = dir.entryInfoList();
        foreach(const QFileInfo &fi, files) {
            if (fi.isFile() && (fi.fileName() == "origin.mhd")) {
                QString path = fi.absoluteFilePath();
                QStringList parts = path.split("/");
                QString caseName = parts.at(3);
                QString filename = fi.baseName();
                QString referenceName = m_referencePath + "\\" + caseName + "\\levelset.mhd";
                QString outputDir = m_outputPath + "\\" + caseName;
                QString outputName = m_outputPath + "\\" + caseName + "\\origin_resampled.mhd";

                if(QFile(outputName).exists())
                    continue;
                if (!QDir(outputDir).exists())
                    QDir().mkdir(outputDir);
                qDebug() << path;
                qDebug() << referenceName;
                qDebug() << outputName;

                QTime start;
                start.start();

                MiaImageReader originImage;
                originImage.setFileName(path, MIA_AUTODETECT);
                originImage.reAllocOutputImage();
                MiaImageND *imageOrigin = originImage.outputImage;

                MiaImageReader referenceImage;
                referenceImage.setFileName(referenceName, MIA_AUTODETECT);
                referenceImage.reAllocOutputImage();
                MiaImageND *imageReference = referenceImage.outputImage;

                ESResample3D resample;
                resample.setInputImage(imageOrigin);

                ResampleMethod method = ResampleMethod::Mia_Resampe_LinearInterpoltion;

                resample.setCurrentResampleMethod(method);
                resample.setResampleSpacing(imageReference->getSpacing()[0], imageReference->getSpacing()[1],
                                            imageReference->getSpacing()[2]);
                resample.reAllocOutputImage();
                resample.runOperation();

                MiaImageND *imageOutput = resample.getOutputImage();

                int *referenceDim = imageReference->getDimSize();
                int *outputDim = imageOutput->getDimSize();

                qDebug() << "referenceDim: " << referenceDim[0] << referenceDim[1] << referenceDim[2];
                qDebug() << "outputDim: " << outputDim[0] << outputDim[1] << outputDim[2];

                MiaImageExporter e;
                e.setFileName(outputName, MIA_AUTODETECT);
                e.inputImage = imageOutput;
                e.runOperation();

                delete imageOrigin;
                delete imageReference;

                qDebug() << "case time cost: " << start.elapsed() << "ms";
                QString tip = "case time cost: " + QString::number(start.elapsed()) + " ms";
                ui->m_message->setText(tip);
            }
        }
    }
}

