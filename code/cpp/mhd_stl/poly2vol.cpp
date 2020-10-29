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

using namespace std;
using namespace std::chrono;

int main() {
    auto start = high_resolution_clock::now();
    QString stlFileName = "./temp/mesh.stl";
    QString outputFileName = "./temp/mesh1.mhd";
    int dim[3] = {512, 512, 280};
    double spacing[3] = {0.398438, 0.398438, 0.398438};
    double origin[3] = {-59.8008, -275.301, 968};
    vtkSmartPointer<vtkMetaImageWriter> mhdWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
    vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();

    stlReader->SetFileName(stlFileName.toStdString().c_str());
    stlReader->Update();

    vtkSmartPointer<vtkPolyData> inputImage = vtkSmartPointer<vtkPolyData>::New();
    inputImage = stlReader->GetOutput();

    vtkSmartPointer<vtkXMLPolyDataWriter> polyDataWriter = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    polyDataWriter->SetInputConnection(stlReader->GetOutputPort());
    polyDataWriter->SetFileName("MyImage.vtp");
    polyDataWriter->SetCompressorTypeToNone();
    polyDataWriter->SetDataModeToAscii();
    polyDataWriter->Write();

    vtkSmartPointer<vtkImageData> whiteImage = vtkSmartPointer<vtkImageData>::New();
    double bounds[6];
    inputImage->GetBounds(bounds);

    whiteImage->SetSpacing(spacing);

    for (int i = 0; i < 3; ++i) {
        if (dim[i] < 1) {
            dim[i] = 1;
        }
    }

    whiteImage->SetDimensions(dim);
    whiteImage->SetExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1);
    whiteImage->SetOrigin(origin);
    whiteImage->AllocateScalars(VTK_FLOAT, 1);

    double inval = 255;
    double outval = 0;

//    vtkIdType cout = whiteImage->GetNumberOfPoints();
//    qDebug() << cout;
//    for (vtkIdType i = 0; i < cout; ++i) {
//        whiteImage->GetPointData()->GetScalars()->SetTuple1(i, inval);
//    }
    vtkIdType pointcount = whiteImage->GetNumberOfPoints();
//    qDebug() << pointcount;
    for (vtkIdType i = 0; i < pointcount; ++i) {
        whiteImage->GetPointData()->GetScalars()->SetTuple1(i, inval);
    }

    vtkSmartPointer<vtkPolyDataToImageStencil> pol2stenc = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
    pol2stenc->SetInputData(inputImage);
    pol2stenc->SetOutputOrigin(origin);
    pol2stenc->SetOutputSpacing(spacing);
    pol2stenc->SetOutputWholeExtent(whiteImage->GetExtent());
    pol2stenc->Update(0);

    vtkSmartPointer<vtkImageStencil> imageStencil = vtkSmartPointer<vtkImageStencil>::New();
    imageStencil->SetInputData(whiteImage);
    imageStencil->SetStencilConnection(pol2stenc->GetOutputPort());
    imageStencil->ReverseStencilOff();
    imageStencil->SetBackgroundValue(outval);
    imageStencil->Update();

    mhdWriter->SetFileName(outputFileName.toStdString().c_str());
    mhdWriter->SetInputConnection(imageStencil->GetOutputPort());
    mhdWriter->SetCompression(false);
    mhdWriter->Write();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Time taken by function: " << duration.count() << " seconds" << endl;
    return 0;
}