#include <iostream>
#include <vtkMarchingCubes.h>
#include <vtkSTLWriter.h>
#include <vtkMetaImageReader.h>
#include <QString>

using namespace std;

int main() {
    QString imageName = "./temp/CTA20170410_LIUSHIFANG_labelmap.mhd";
    QString fileName = "./temp/CTA20170410_LIUSHIFANG_6.stl";
    int value = 6;
    vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    reader->SetFileName(imageName.toStdString().c_str());
    reader->Update();

    vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
    surface->SetInputConnection(reader->GetOutputPort());
    surface->ComputeNormalsOn();
    surface->SetValue(5, value);
    surface->Update();

    vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter->SetFileName(fileName.toStdString().c_str());
    stlWriter->SetInputConnection(surface->GetOutputPort());
    stlWriter->Write();
    return 0;
}