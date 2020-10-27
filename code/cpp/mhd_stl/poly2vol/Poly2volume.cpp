#include "Poly2volume.h"
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

Poly2volume::Poly2volume()
{

}

void Poly2volume::poly2volume(QString stlFileName, QString outputFileName, int dim[3], double spacing[3], double origin[3])
{
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

    for (int i=0; i<3; ++i)
    {
        if(dim[i]<1)
        {
            dim[i] = 1;
        }
    }

    whiteImage->SetDimensions(dim);
    whiteImage->SetExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
    whiteImage->SetOrigin(origin);
    whiteImage->AllocateScalars(VTK_FLOAT,1);

    double inval = 255;
    double outval = 0;

    vtkIdType cout = whiteImage->GetNumberOfPoints();
    for(vtkIdType i=0; i<cout; ++i)
    {
       whiteImage->GetPointData()->GetScalars()->SetTuple1(i,inval);
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
}
