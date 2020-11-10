#include "mhd2stl.h"
#include <vtkMarchingCubes.h>
#include <vtkSTLWriter.h>
#include <vtkMetaImageReader.h>

Mhd2stl::Mhd2stl(QObject *parent) : QObject(parent) {

}

void Mhd2stl::ImageFromMhd2Stl(QString imageName, QString fileName, float value) {
    vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    reader->SetFileName(imageName.toStdString().c_str());
    reader->Update();

    vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
    surface->SetInputConnection(reader->GetOutputPort());
    surface->ComputeNormalsOn();
    surface->SetValue(0, value);
    surface->Update();

    vtkSmartPointer<vtkSTLWriter> stlWriter =
            vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter->SetFileName(fileName.toStdString().c_str());
    stlWriter->SetInputConnection(surface->GetOutputPort());
    stlWriter->Write();
}
