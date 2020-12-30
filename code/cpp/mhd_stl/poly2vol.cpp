#include <iostream>
#include <vtkMarchingCubes.h>
#include <vtkSTLWriter.h>
#include <vtkMetaImageReader.h>
#include <QString>
#include <QFile>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkMetaImageReader.h>
#include <vtkMetaImageWriter.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkImageData.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageHistogram.h>
#include <vtkImageStencil.h>
#include <vtkPointData.h>
#include <chrono>
#include <vtkUtils.h>

using namespace std;
using namespace std::chrono;

typedef VTKUtils vu;

int main() {
    auto start = high_resolution_clock::now();
//    QString stlFileName = "E:\\data\\medicaldata\\other\\yli\\TM_DXL_CTA20140728_DENGXILIAN\\TM_DXL\\step3\\mesh.stl";
//    QString outputFileName = "E:\\data\\medicaldata\\other\\yli\\TM_DXL_CTA20140728_DENGXILIAN\\TM_DXL\\step3\\mesh.mhd";
    QString stlFileName = "F:\\data\\esdata\\esdata\\experiment\\CTA20160328_YANGXIUZHI\\temp_csf\\step3\\mesh.stl";
    QString outputFileName = "E:\\data\\medicaldata\\other\\yli\\tianming\\CTA20160328_YANGXIUZHI_mesh.mhd";
//    QString outputFileName1 = "E:\\data\\medicaldata\\other\\yli\\tianming\\_CM_001_1.mhd";
//    int dim[3] = {512, 512, 345};
//    double spacing[3] = {0.371, 0.371, 0.371};
//    double origin[3] = {-60.4394, -94.8145, 1816.75};
//    int dim[3] = {512, 512, 280};
//    double spacing[3] = {0.376, 0.376, 0.5};
//    double origin[3] = {-76.6870, -96.0620, 1814.25};
//    int dim[3] = {512, 512, 280};
//    double spacing[3] = {0.376, 0.376, 0.5};
//    double origin[3] = {-76.6870, -96.0620, 1814.25};




    QString mhd_meta = "F:\\data\\esdata\\esdata\\experiment\\CTA20160328_YANGXIUZHI\\temp_csf\\step1\\resampled.mhd";
    QList<QString> keys = {"Offset", "ElementSpacing", "DimSize"};
    QMap<QString, QList<QVariant >> results = vu::getMhdMeta(mhd_meta, keys);
//        traverse_map(results);

    int dim[3] = {results["DimSize"][0].toInt(), results["DimSize"][1].toInt(), results["DimSize"][2].toInt()};
    double origin[3] = {results["Offset"][0].toDouble(), results["Offset"][1].toDouble(),
                        results["Offset"][2].toDouble()};
    double spacing[3] = {results["ElementSpacing"][0].toDouble(), results["ElementSpacing"][1].toDouble(),
                         results["ElementSpacing"][2].toDouble()};


    vu::poly2vol(stlFileName, outputFileName, dim, origin, spacing);


//    vtkSmartPointer<vtkMetaImageWriter> mhdWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
//    vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
//
//    stlReader->SetFileName(stlFileName.toStdString().c_str());
//    stlReader->Update();
//
//    vtkSmartPointer<vtkPolyData> inputImage = vtkSmartPointer<vtkPolyData>::New();
//    inputImage = stlReader->GetOutput();
//
////    vtkSmartPointer<vtkXMLPolyDataWriter> polyDataWriter = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
////    polyDataWriter->SetInputConnection(stlReader->GetOutputPort());
////    polyDataWriter->SetFileName("MyImage.vtp");
////    polyDataWriter->SetCompressorTypeToNone();
////    polyDataWriter->SetDataModeToAscii();
////    polyDataWriter->Write();
//
//    vtkSmartPointer<vtkImageData> whiteImage = vtkSmartPointer<vtkImageData>::New();
//    double bounds[6];
//    inputImage->GetBounds(bounds);
//
//    whiteImage->SetSpacing(spacing);
//
//    for (int i = 0; i < 3; ++i) {
//        if (dim[i] < 1) {
//            dim[i] = 1;
//        }
//    }
//
//    whiteImage->SetDimensions(dim);
//    whiteImage->SetExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1);
//    whiteImage->SetOrigin(origin);
//    whiteImage->AllocateScalars(VTK_FLOAT, 1);
//
//    double inval = 255;
//    double outval = 0;
//
////    vtkIdType cout = whiteImage->GetNumberOfPoints();
////    qDebug() << cout;
////    for (vtkIdType i = 0; i < cout; ++i) {
////        whiteImage->GetPointData()->GetScalars()->SetTuple1(i, inval);
////    }
//    vtkIdType pointcount = whiteImage->GetNumberOfPoints();
////    qDebug() << pointcount;
//    for (vtkIdType i = 0; i < pointcount; ++i) {
//        whiteImage->GetPointData()->GetScalars()->SetTuple1(i, inval);
//    }
//
//    vtkSmartPointer<vtkPolyDataToImageStencil> pol2stenc = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
//    pol2stenc->SetInputData(inputImage);
//    pol2stenc->SetOutputOrigin(origin);
//    pol2stenc->SetOutputSpacing(spacing);
//    pol2stenc->SetOutputWholeExtent(whiteImage->GetExtent());
//    pol2stenc->Update(0);
//
//    vtkSmartPointer<vtkImageStencil> imageStencil = vtkSmartPointer<vtkImageStencil>::New();
//    imageStencil->SetInputData(whiteImage);
//    imageStencil->SetStencilConnection(pol2stenc->GetOutputPort());
//    imageStencil->ReverseStencilOff();
//    imageStencil->SetBackgroundValue(outval);
//    imageStencil->Update();
//
//    mhdWriter->SetFileName(outputFileName.toStdString().c_str());
//    mhdWriter->SetInputConnection(imageStencil->GetOutputPort());
//    mhdWriter->SetCompression(false);
//    mhdWriter->Write();
//
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " seconds" << std::endl;
    return 0;
}