/*************************************************************************
*
* ShenZhen EscopeTech Incoperated CONFIDENTIAL
* ________________________________________________________________________
*
* [2020] - Present ShenZhen EscopeTech Incoperated
* All Rights Reserved.
*
* NOTICE: All information contained herein is, and remains
* the property of ShenZhen EscopeTech Incoperated and its suppliers,
* if any. The intellectual and technical concepts contained
* herein are proprietary to ShenZhen EscopeTech Incoperated
* and its suppliers and may be covered by P.R.C. and Foreign Patents,
* patents in process, and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from ShenZhen EscopeTech Incoperated.
*
*************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QDirIterator>
#include "qdebug.h"
#include <QMessageBox>

#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkMetaImageReader.h"
#include "vtkMetaImageWriter.h"
#include "vtkSTLReader.h"
#include "vtkSTLWriter.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkImageData.h"
#include "vtkPolyDataToImageStencil.h"
#include "vtkImageHistogram.h"
#include "vtkImageStencil.h"
#include "vtkPointData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageCast.h"
#include "vtkMarchingCubes.h"
#include "vtkSTLWriter.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkCleanPolyData.h"
#include "vtkAppendPolyData.h"
#include "vtkReverseSense.h"
#include "vtkImageConnectivityFilter.h"

#include "imagereader.h"
#include "imageexporter.h"
#include "miaimagend.h"
#include "ESPreDefine.h"
#include "Poly2volume.h"
#include "ESImageND.h"
#include "mhd2stl.h"
#include "Geometry.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_inputPath.clear();
    m_outputPath.clear();
    m_fileName.clear();
    m_smoothImage = false;
    m_imageReader = new MiaImageReader(1);
    m_exporter = new MiaImageExporter(1);

    ui->setupUi(this);
    QIcon icon;
    icon.addFile(":/icons/escope_1256_567.png",QSize(128,32));
    setWindowIcon(icon);
    setConParametersDefaultValue();
    QFile file(":/qss/psblack.css");
    if (file.open(QFile::ReadOnly))
    {
        QString qss = file.readAll();//QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        setPalette(QPalette(QColor(paletteColor)));
        setStyleSheet(qss);
        file.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    DELETEPOINTER(m_exporter);
    DELETEPOINTER(m_imageReader);
}

void MainWindow::on_m_importImage_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this,tr("Import file path"),"");
    m_inputPath = filePath;
    QStringList fileList;
    QFileInfo fileinfo;
    QString fileName;
    m_fileName.clear();
    QDirIterator it(m_inputPath, QStringList() << "*.stl", QDir::Files | QDir::NoSymLinks | QDir::Readable);
    while (it.hasNext())
    {
        it.next();
        fileList.append(it.filePath());
        fileinfo = QFileInfo(it.filePath());
        fileName = fileinfo.baseName();
        m_fileName.append(fileName);
//        qDebug()<<"fileName: "<<fileName<<m_inputPath;
    }
}

void MainWindow::on_m_outputDir_clicked()
{
    QString outportPath = QFileDialog::getExistingDirectory(this,tr("Disp images import file path"),"");
    m_outputPath = outportPath;
}

void MainWindow::on_m_stl2mhd_clicked()
{
    if (m_parameters.dimension == 3)
    {
        qDebug()<<"Start Image processing.";
        double spacing[3];
        spacing[0] = m_parameters.spacingX;
        spacing[1] = m_parameters.spacingY;
        spacing[2] = m_parameters.spacingZ;

        int dim[3];
        dim[0] = m_parameters.dimensionX;
        dim[1] = m_parameters.dimensionY;
        dim[2] = m_parameters.dimensionZ;

        double center[3];
        center[0] = m_parameters.centerX;
        center[1] = m_parameters.centerY;
        center[2] = m_parameters.centerZ;

        double origin[3];
        for (int i =0; i<3; ++i)
        {
            origin[i] = center[i] - 0.5*dim[i]*spacing[i];
        }

        for(int i=0; i<m_fileName.size();++i)
        {
            QString inputFile, outputFile;
            inputFile = m_inputPath +"/"+m_fileName[i]+".stl";
            outputFile = m_outputPath +"/"+m_fileName[i]+".mhd";
            Poly2volume poly2vol;
            poly2vol.poly2volume(inputFile, outputFile, dim, spacing, origin);
        }
        qDebug()<<"Finished image written";
    }
    else
    {
        QMessageBox::critical(this,"Warning","Input is not 3D images",QMessageBox::Yes);
        return;
    }
}

void MainWindow::on_m_smoothChoice_activated(int index)
{
    if (index == 1)
    {
        m_smoothImage = true;
    }
    else
    {
        m_smoothImage = false;
    }
}

void MainWindow::on_m_combineImage_clicked()
{
    QString strs;
    QStringList fileList, outputName;
    QStringList strPathList = QFileDialog::getOpenFileNames(this,tr("Choose input images"),tr("/home"),tr("Images(*.mhd);;All files (*.*);;"));
            qDebug()<<"+++++++0";
    if(!strPathList.isEmpty())
    {
        MiaImageND *pImage;
        qDebug()<<"template image file: "<<strPathList[0];
        m_imageReader->setFileName(strPathList[0],MIA_MHD);
        m_imageReader->reAllocOutputImage();
        pImage = m_imageReader->outputImage;
        int width = pImage->getWidth();
        int height = pImage->getHeight();
        int depth = pImage->getDepth();
        MiaImageND *newImage = new MiaImageND(width, height,depth,0,pImage->getType());
        newImage->setOffset(pImage->getOffset());
        newImage->setSpacing(pImage->getSpacing());
        float *pNewImage = (float*)newImage->getDataPtr();
        for (int i = 0; i < strPathList.size(); i++)
        {
            QString strPath = strPathList[i];
            qDebug()<<"Path = "<<strPath;
            MiaImageND *currImage;
            m_imageReader->setFileName(strPathList[i],MIA_AUTODETECT);
            m_imageReader->reAllocOutputImage();
            currImage = m_imageReader->outputImage;
            float *pCurrImage = (float *) currImage->getDataPtr();

            for (int k = 0; k < depth; k++)
            {
                for (int j=0; j<height; j++)
                {
                    for (int i=0; i<width; i++)
                    {
                        int index = k*width*height+j*width + i;
                        if (pCurrImage[index]> 0.0)
                        {
                            pNewImage[index] = 255;
                        }
                    }
                }
            }
        }

        QFileInfo file = QFileInfo(strPathList[0]);
        QString fileName = file.baseName();
        QString filePath = file.path();
//        outputName.append(fileName);
//        strs.append(fileName);
//        strs += "\n";
        QString outputFileName = filePath + "/" + fileName + "Combine.mhd";
        m_exporter->setFileName(outputFileName,MIA_AUTODETECT);
        m_exporter->inputImage = newImage;
        m_exporter->runOperation();
        qDebug()<<"Finished image written.";
    }
}

void MainWindow::on_m_confirm_clicked()
{
    m_parameters.dimension = ui->m_dimension->text().toInt();
    m_parameters.centerX = ui->m_centX->text().toFloat();
    m_parameters.centerY = ui->m_centY->text().toFloat();
    m_parameters.centerZ = ui->m_centZ->text().toFloat();
    m_parameters.dimensionX = ui->m_dimX->text().toFloat();
    m_parameters.dimensionY = ui->m_dimY->text().toFloat();
    m_parameters.dimensionZ = ui->m_dimZ->text().toFloat();
    m_parameters.spacingX = ui->m_spaX->text().toFloat();
    m_parameters.spacingY = ui->m_spaY->text().toFloat();
    m_parameters.spacingZ = ui->m_spaZ->text().toFloat();
    qDebug()<<"Parameters are: "<< m_parameters.dimension << m_parameters.centerX <<m_parameters.centerY\
           <<m_parameters.centerZ<<m_parameters.dimensionX<<m_parameters.dimensionY<<m_parameters.dimensionZ\
           <<m_parameters.spacingX<<m_parameters.spacingY<<m_parameters.spacingZ;
}

void MainWindow::on_m_reset_clicked()
{
    setConParametersDefaultValue();
    qDebug()<<"Reset current selection.";
}

void MainWindow::setConParametersDefaultValue()
{
    ui->m_dimension->setText("3");
    ui->m_centX->setText("0");
    ui->m_centY->setText("0");
    ui->m_centZ->setText("0");
    ui->m_dimX->setText("256");
    ui->m_dimY->setText("256");
    ui->m_dimZ->setText("256");
    ui->m_spaX->setText("1");
    ui->m_spaY->setText("1");
    ui->m_spaZ->setText("1");
}

ESImageND* MainWindow::mhdImageFillHoles(ESImageND*image, int dist)
{
    if (image)
    {
        int width = image->getWidth();
        int height = image->getHeight();
        int depth = image->getDepth();
        float *pImage = reinterpret_cast<float*>(image->getDataPtr());
        ESImageND* newImage = new ESImageND(width,height,depth,0,image->getType());
        float *pNewImage = reinterpret_cast<float*>(newImage->getDataPtr());

        for (int k = 0; k < depth; k++)
        {
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    int index = width*height*k + width*j + i;
                    float epsilon = 0.000001f;
                    float numPositive = 0.0f;
                    numPositive = (pImage[index+1]+pImage[index-1]+pImage[index-width]+pImage[index+width]+pImage[index-width*height]+pImage[index+width*height])/255.0f;
                    if (pImage[index] < epsilon && numPositive > 4.0f)
                    {
                        pNewImage[index] = 255.0f;
                    }
                    else
                    {
                       pNewImage[index] = pImage[index];
                    }
                }
            }
        }
        return newImage;
    }
    else
    {
        qDebug()<<"Please inport a mhd image.";
        return false;
    }
}

void MainWindow::on_m_convertMhd2Stl_clicked()
{
    QString imageName = QFileDialog::getOpenFileName(this,tr("Open a mhd image"),"",tr("MHD File (*.mhd)"));
    QString imageFilePath, fileName;
    QFileInfo fi;
    fi = QFileInfo(imageName);
    fileName = fi.baseName();
    imageFilePath = fi.absolutePath();
    ESImageND *image = loadESImage(imageName);
    ESImageND *newImage = new ESImageND(image->getWidth(),image->getHeight(),image->getDepth(),0,ESIM_float);
    newImage->copyGeometryInfoFrom(image);

    float* pImage = reinterpret_cast<float*>(image->getDataPtr());
    float* pNewImage = reinterpret_cast<float*>(newImage->getDataPtr());
    float epsilon = 0.001f;
    for (int i = 0; i < 8; i++)
    {
        QString proFileName = imageFilePath + "/" + fileName +"_process.mhd";
        QString outputFileName = imageFilePath + "/" + fileName +"_"+ QString::number(i)+".stl";
        for (int j = 0; j < image->getImageSize(); j++)
        {
            if (abs(pImage[j]-i) < epsilon)
            {
                pNewImage[j] = i;
            }
        }
        writeESImage(proFileName,newImage);
        Mhd2stl::ImageFromMhd2Stl(proFileName, outputFileName,i);
    }

}

ESImageND* MainWindow::loadESImage(QString fileName)
{
    m_imageReader->setFileName(fileName,MIA_AUTODETECT);
    m_imageReader->reAllocOutputImage();
    return m_imageReader->m_ESOutputImage;
}

void MainWindow::writeESImage(QString fileName, ESImageND*image)
{
    m_exporter->setFileName(fileName,MIA_AUTODETECT);
    m_exporter->m_ESInputImage = image;
    m_exporter->runOperation();
    m_exporter->m_ESInputImage = nullptr;
}

void MainWindow::ExtractParts(QString inputImage, float value, int minPoints)
{
    QString imageFilePath, fileName;
    QFileInfo fi;
    fi = QFileInfo(inputImage);
    fileName = fi.baseName();
    imageFilePath = fi.absolutePath();
    QString outputFile = imageFilePath + "/" + fileName + ".stl";

    vtkSmartPointer<vtkMetaImageReader> mhdReader = vtkSmartPointer<vtkMetaImageReader>::New();
    mhdReader->SetFileName(inputImage.toStdString().c_str());
    mhdReader->Update();

    vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
    surface->SetInputConnection(mhdReader->GetOutputPort());
    surface->ComputeNormalsOn();
    surface->SetValue(0,value);
    surface->Update();

    vtkPolyDataConnectivityFilter *extractor = vtkPolyDataConnectivityFilter::New();
    vtkCleanPolyData *cleaner = vtkCleanPolyData::New();
    extractor->SetInputConnection(surface->GetOutputPort());
    extractor->SetExtractionModeToSpecifiedRegions();
    extractor->AddSpecifiedRegion(0);
    extractor->Update();
    extractor->DeleteSpecifiedRegion(0);
    int regions = extractor->GetNumberOfExtractedRegions();
    vtkIdTypeArray *regionSizes = extractor->GetRegionSizes();
    std::vector<int> validRegions;

    for (int i = 0; i < regions; i++) {
        if (regionSizes->GetValue(i) > minPoints)
            validRegions.push_back(i);
    }

    int lastSelectedRegion = 0;
    vtkSmartPointer<vtkPolyData> allRegion = vtkSmartPointer<vtkPolyData>::New();

    for (auto it = validRegions.begin(); it != validRegions.end(); ++it)
    {
        int i = *it;
        extractor->DeleteSpecifiedRegion(lastSelectedRegion);
        extractor->AddSpecifiedRegion(i);
        lastSelectedRegion = i;

        cleaner->SetInputConnection(extractor->GetOutputPort());
        cleaner->Update();

        vtkSmartPointer<vtkPolyData> region = vtkSmartPointer<vtkPolyData>::New();
        region->DeepCopy(cleaner->GetOutput());

        if (i == 0)
        {
            allRegion->DeepCopy(cleaner->GetOutput());
        }
        vtkSmartPointer<vtkAppendPolyData> appendFilter =
                vtkSmartPointer<vtkAppendPolyData>::New();
        appendFilter->AddInputData(cleaner->GetOutput());
        appendFilter->AddInputData(allRegion);
        appendFilter->Update();

        allRegion->DeepCopy(appendFilter->GetOutput());
    }
    vtkSmartPointer<vtkSTLWriter> stlWriter =
            vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter->SetFileName(outputFile.toStdString().c_str());
    stlWriter->SetInputData(allRegion);
    stlWriter->Write();
}

void MainWindow::ExtractPolyParts(vtkPolyData* polyParts, QString inputImage, float value, int minPoints)
{
    QString imageFilePath, fileName;
    QFileInfo fi;
    fi = QFileInfo(inputImage);
    fileName = fi.baseName();
    imageFilePath = fi.absolutePath();
    QString outputFile = imageFilePath + "/" + fileName + ".stl";

    vtkSmartPointer<vtkMetaImageReader> mhdReader = vtkSmartPointer<vtkMetaImageReader>::New();
    mhdReader->SetFileName(inputImage.toStdString().c_str());
    mhdReader->Update();

    vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
    surface->SetInputConnection(mhdReader->GetOutputPort());
    surface->ComputeNormalsOn();
    surface->SetValue(0,value);
    surface->Update();

    vtkPolyDataConnectivityFilter *extractor = vtkPolyDataConnectivityFilter::New();
    vtkCleanPolyData *cleaner = vtkCleanPolyData::New();
    extractor->SetInputConnection(surface->GetOutputPort());
    extractor->SetExtractionModeToSpecifiedRegions();
    extractor->AddSpecifiedRegion(0);
    extractor->Update();
    extractor->DeleteSpecifiedRegion(0);
    int regions = extractor->GetNumberOfExtractedRegions();
    vtkIdTypeArray *regionSizes = extractor->GetRegionSizes();
    std::vector<int> validRegions;

    for (int i = 0; i < regions; i++) {
        if (regionSizes->GetValue(i) > minPoints)
            validRegions.push_back(i);
    }

    int lastSelectedRegion = 0;
    for (auto it = validRegions.begin(); it != validRegions.end(); ++it)
    {
        int i = *it;
        extractor->DeleteSpecifiedRegion(lastSelectedRegion);
        extractor->AddSpecifiedRegion(i);
        lastSelectedRegion = i;

        cleaner->SetInputConnection(extractor->GetOutputPort());
        cleaner->Update();
//        vtkSmartPointer<vtkPolyData> region = vtkSmartPointer<vtkPolyData>::New();
//        region->DeepCopy(cleaner->GetOutput());

        vtkSmartPointer<vtkAppendPolyData> appendFilter =
                vtkSmartPointer<vtkAppendPolyData>::New();
        appendFilter->AddInputData(cleaner->GetOutput());
        appendFilter->AddInputData(polyParts);
        appendFilter->Update();

        polyParts->DeepCopy(appendFilter->GetOutput());
    }
    vtkSmartPointer<vtkSTLWriter> stlWriter =
            vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter->SetFileName(outputFile.toStdString().c_str());
    stlWriter->SetInputData(polyParts);
    stlWriter->Write();
}

void MainWindow::polyData2volume(QString imageFileName, vtkPolyData* polyParts)
{
    QString imageFilePath, fileName;
    QFileInfo fi;
    fi = QFileInfo(imageFileName);
    fileName = fi.baseName();
    imageFilePath = fi.absolutePath();

    ESImageND* image = loadESImage(imageFileName);
    vtkSmartPointer<vtkImageData> whiteImage = vtkSmartPointer<vtkImageData>::New();

    double origin[3], spacing[3];
    for (int i = 0; i < 3; i++)
    {
        origin[i] = image->getOffset()[i];
        spacing[i] = image->getSpacing()[i];
    }

    whiteImage->SetSpacing(spacing);
    whiteImage->SetDimensions(image->getDimSize());
    whiteImage->SetExtent(0,image->getWidth()-1,0,image->getHeight()-1,0,image->getDepth()-1);
    whiteImage->SetOrigin(origin);
    whiteImage->AllocateScalars(VTK_FLOAT,1);

    double inval = 255;
    double outval = 0;

    vtkIdType cout = whiteImage->GetNumberOfPoints();
    for(vtkIdType i=0; i<cout; ++i)
    {
       whiteImage->GetPointData()->GetScalars()->SetTuple1(i,inval);
    }

    vtkSmartPointer<vtkMetaImageWriter> mhdWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
    vtkSmartPointer<vtkPolyDataToImageStencil> pol2stenc = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
    pol2stenc->SetInputData(polyParts);
    pol2stenc->SetOutputOrigin(origin);
    pol2stenc->SetOutputSpacing(spacing);
    pol2stenc->SetOutputWholeExtent(whiteImage->GetExtent());
    pol2stenc->Update(0);

    QString outputFileName1 = imageFilePath + "/" +fileName +QString::number(0) +"_1New.mhd";
    mhdWriter->SetFileName(outputFileName1.toStdString().c_str());
    mhdWriter->SetInputConnection(pol2stenc->GetOutputPort());
    mhdWriter->SetCompression(false);
    mhdWriter->Write();

    vtkSmartPointer<vtkImageStencil> imageStencil = vtkSmartPointer<vtkImageStencil>::New();
    imageStencil->SetInputData(whiteImage);
    imageStencil->SetStencilConnection(pol2stenc->GetOutputPort());
    imageStencil->ReverseStencilOff();
    imageStencil->SetBackgroundValue(outval);
    imageStencil->Update();

    QString outputFileName2 = imageFilePath + "/" +fileName +QString::number(0) +"_2New.mhd";
    mhdWriter->SetFileName(outputFileName2.toStdString().c_str());
    mhdWriter->SetInputConnection(imageStencil->GetOutputPort());
    mhdWriter->SetCompression(false);
    mhdWriter->Write();
}

void MainWindow::removeSmallParts(QString imagePathName)
{
    QString imageFilePath, fileName, normFile;
    QFileInfo fi;
    fi = QFileInfo(imagePathName);
    fileName = fi.baseName();
    imageFilePath = fi.absolutePath();
    normFile = imageFilePath + "/" + fileName +"_Nor.mhd";
    vtkSmartPointer<vtkMetaImageReader> mhdReader = vtkSmartPointer<vtkMetaImageReader>::New();
    mhdReader->SetFileName(normFile.toStdString().c_str());
    mhdReader->Update();

//    int dims[3];
//    mhdReader->GetOutput()->GetDimensions(dims);

//    for (int k = 0; k < dims[2]; ++k)
//    {
//        for (int j = 0; j < dims[1]; ++j)
//        {
//            for (int i = 0; i < dims[0]; ++i)
//            {
//                float* pixel = static_cast<float*>(mhdReader->GetOutput()->GetScalarPointer(i,j,k));
//                if (pixel[0] > 1.0f)
//                {
//                    pixel[0] = 0.0f;
//                }
//                else
//                {
//                    pixel[0] = 1.0f;
//                }

//            }
//        }
//    }
//    vtkSmartPointer<vtkMetaImageWriter> mhdWriter1 = vtkSmartPointer<vtkMetaImageWriter>::New();
//    QString outputFileName1 = imageFilePath + "/" +fileName +QString::number(0) +"_0Con.mhd";
//    mhdWriter1->SetFileName(outputFileName1.toStdString().c_str());
//    mhdWriter1->SetInputData(mhdReader->GetOutput());
//    mhdWriter1->SetCompression(false);
//    mhdWriter1->Write();

    Point3d spacing, origin, bound, pos;
    int extent[6];
    mhdReader->GetOutput()->GetExtent(extent);
    mhdReader->GetOutput()->GetSpacing(spacing.V());
    mhdReader->GetOutput()->GetOrigin(origin.V());
    bound[0] = extent[1] - extent[0];
    bound[1] = extent[3] - extent[2];
    bound[2] = extent[5] - extent[4];
    pos[0] = origin[0] + spacing[0] * bound[0] / 2;
    pos[1] = origin[1] + spacing[1] * bound[1] / 2;
    pos[2] = origin[2] + spacing[2] * bound[2] / 2;

    vtkNew<vtkPoints> points;
    points->InsertNextPoint(pos.V());
    vtkNew<vtkPolyData> pointsPolydata;
    pointsPolydata->SetPoints(points);

    vtkSmartPointer<vtkImageConnectivityFilter> con = vtkSmartPointer<vtkImageConnectivityFilter>::New();
//    con->SetInputConnection(mhdReader->GetOutputPort());
    con->SetInputData(mhdReader->GetOutput());
//    con->SetSeedData(pointsPolydata);
//    con->SetExtractionModeToSeededRegions();
//    con->SetInputData(mhdReader->GetOutput());
    con->SetSizeRange(1000,VTK_ID_MAX);
//    con->SetLabelModeToSizeRank();
//    con->SetExtractionModeToLargestRegion();
    con->SetExtractionMode(0);
    con->Update();

    vtkSmartPointer<vtkMetaImageWriter> mhdWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
    QString outputFileName = imageFilePath + "/" +fileName +QString::number(0) +"_Con.mhd";
    mhdWriter->SetFileName(outputFileName.toStdString().c_str());
    mhdWriter->SetInputConnection(con->GetOutputPort());
    mhdWriter->SetCompression(false);
    mhdWriter->Write();
}

void MainWindow::on_m_extractParts_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this,tr("Import file path"),"");
    QStringList fileList;
    QFileInfo fileinfo;
    QString fileName, outDir;
    QList<QString> fileNames;
    fileNames.clear();
    outDir = filePath + "/RemoveSmallParts";
    QDir dir(outDir);
    if (!dir.exists())
    {
        dir.mkpath(outDir);
    }
    else
    {
        qDebug() << outDir <<" is existing";
    }
    QDirIterator it(filePath, QStringList() << "*.mhd", QDir::Files | QDir::NoSymLinks | QDir::Readable);
    while (it.hasNext())
    {
        it.next();
        fileList.append(it.filePath());
        fileinfo = QFileInfo(it.filePath());
        fileName = fileinfo.baseName();
        fileNames.append(fileName);
    }
    Point3i point1 = Point3i(128, 128, 128);
//    Point3i point1 = Point3i(55, 75, 155);
//    Point3i point2 = Point3i(128,128,128);
//    Point3i point3 = Point3i(155,115,70);
    QList<Point3i> points;
    points.append(point1);
//    points.append(point2);
//    points.append(point3);
    for (int i = 0; i < fileNames.size(); ++i)
    {
        QString fileName = filePath + "/" + fileNames[i]+".mhd";
        QString outFileName = outDir + "/Nor" + fileNames[i]+".mhd";
        ESImageND* image = loadESImage(fileName);
        normalizeImage(image);
        regionGrowth(image,points,outFileName);
        DELETEPOINTER(image);
    }
}

void MainWindow::normalizeImage(ESImageND* image)
{
    float* pImage = reinterpret_cast<float *>(image->getDataPtr());
    int count = 0;
    float maxValue = 0.0f, minValue=255.0f;
    for (int i = 0; i < image->getImageSize(); ++i)
    {
        if(pImage[i] > maxValue)
        {
            maxValue = pImage[i];
        }
        else if (pImage[i] < minValue)
        {
            minValue = pImage[i];
        }
    }
    for (int i = 0; i < image->getImageSize(); ++i)
    {
        if(pImage[i] > 0.5*(maxValue+minValue))
        {
            pImage[i] = 0.0f;
        }
        else
        {
            pImage[i] = 1.0f;
            count++;
        }
    }
    qDebug()<<"Positive value cells :"<<count;

    int breakCells = 0;
    for (int k = 0; k < image->getDepth(); ++k)
    {
        for (int j = 0; j < image->getHeight(); ++j)
        {
            for (int i = 0; i < image->getWidth(); ++i)
            {
                int index = k*image->getHeight()*image->getWidth() + j*image->getWidth() + i;
                int count = 0;
                int total = 0;
                if (pImage[index] > 0.5f)
                {
                    for (int x = -3; x < 4; ++x)
                    {
                        for (int y = -3; y < 4; ++y)
                        {
                            for (int z = -3; z < 4; ++z)
                            {
                                if (sqrt(x*x+y*y+z*z) <= 3.0f)
                                {
                                    int curX = x + i;
                                    int curY = y + j;
                                    int curZ = z + k;
                                    if(curX > -1 && curX < image->getWidth()
                                            &&curY > -1 && curY < image->getHeight()
                                            &&curZ > -1 && curZ < image->getDepth())
                                    {
                                        int curIndex = curZ*image->getHeight()*image->getWidth() + curY*image->getWidth() + curX;
                                        if (pImage[curIndex] > 0.0f)
                                        {
                                            count++;
                                            total++;
                                        }
                                        else
                                        {
                                            total++;
                                        }
                                    }
                                }

                            }
                        }
                    }
                    float ratio = float(count)/float(total);
                    if (ratio < 0.3)
                    {
                       pImage[index] = 0.0f;
                       breakCells ++;
                    }
                }
            }
        }
    }
    qDebug()<<"Break cells number: "<<breakCells;
}

void MainWindow::regionGrowth(ESImageND*image, QList<Point3i> points, QString fileName)
{
    int width = image->getWidth();
    int height = image->getHeight();
    int depth = image->getDepth();
    int sliceSize = width*height;

    ESImageND* outputImage = new ESImageND(width,height,depth,1,ESIM_float);
    outputImage->copyGeometryInfoFrom(image);
    float* pImage = reinterpret_cast<float*>(image->getDataPtr());
    float* pOutputImage = reinterpret_cast<float *>(outputImage->getDataPtr());

    int num = 0;
    for (int i = 0; i < image->getImageSize(); ++i)
    {
        pOutputImage[i] = 0.0f;
        if (pImage[i] > 0.5f)
        {
            num++;
        }
    }
    qDebug()<<"Read positive value cells :"<<num;

    QList<Point3i> seeds;
    for (int i = 0; i < points.size(); ++i)
    {
        seeds.append(points[i]);
    }
    int Its = 0;
    float threshould = 0.01f;
    while (seeds.size())
    {
        Its ++;

        const int x = seeds[0].X();
        const int y = seeds[0].Y();
        const int z = seeds[0].Z();
        const int seedIndex = z*sliceSize + y*width + x;
        pOutputImage[seedIndex] = 1.0f;

        for (int k = -1; k < 2; ++k)
        {
            for (int j = -1; j < 2; ++j)
            {
                for (int i = -1; i < 2; ++i)
                {
                    const int curX = x + i;
                    const int curY = y + j;
                    const int curZ = z + k;
                    if (curX > -1 && curX < width
                            &&curY > -1 && curY < height
                            &&curZ > -1 && curZ < depth)
                    {
                        const int index = curZ*sliceSize + curY*width + curX;
                        bool inRange = abs(pImage[index] - pImage[seedIndex]) < threshould;
                        if ((pOutputImage[index] < 0.01f) && inRange)
                        {
                            pOutputImage[index] = 1.0f;
                            seeds.append(Point3i(curX,curY,curZ));
//                            qDebug()<<"Point : "<<Its<<pImage[index]<<x<<y<<z<<"results: "<<curX<<curY<<curZ;
                        }
                    }
                }
            }
        }
        seeds.pop_front();
    }
    qDebug()<<"+++++++++++++++"<<"Its N0."<<Its;
    writeESImage(fileName,image);
    DELETEPOINTER(outputImage);
}
