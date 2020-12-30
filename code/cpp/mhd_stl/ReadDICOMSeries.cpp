//
// Created by admin on 2020/12/28.
//

# include <iostream>
# include <vector>

// some standard vtk headers
#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
// headers needed for this example
#include <vtkActor2D.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkInteractorStyleImage.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkMetaImageWriter.h>

#include <vtkUtils.h>
#include <QString>



using namespace std;

class Solution {

};

int main() {
    QString dimcomFolder = "F:\\data\\esdata\\esdata\\experiment\\CTA20140728_DENGXILIAN\\CTA";
    QString mhdFileame = "CTA20140728_DENGXILIAN_ssss1.mhd";
    vtkNew<vtkDICOMImageReader> reader;
    reader->SetDirectoryName(dimcomFolder.toLocal8Bit());
    reader->Update();

    vtkSmartPointer<vtkMetaImageWriter> mhdWriter = vtkSmartPointer<vtkMetaImageWriter>::New();

    mhdWriter->SetInputConnection(reader->GetOutputPort());
    mhdWriter->SetFileName(mhdFileame.toLocal8Bit());
//    mhdWriter->
//    mhdWriter->SetRAWFileName(filePathRaw.c_str());
    mhdWriter->SetCompression(false);
    mhdWriter->Write();

    return 0;
}