//
// Created by admin on 2020/11/1.
//

#include "myVtkUtils.h"



#include <vtkNew.h>
#include <vtkSimplePointsWriter.h>
#include <vtkSphereSource.h>

using namespace std;

int myvtkSimplePointsWriter(vtkNew<vtkPolyData> &obj, string filename) {
    cout << "aa" << endl;
    return 0;
//    vtkNew<vtkSphereSource> sphereSource;
//    sphereSource->Update();
//
//    vtkNew<vtkSimplePointsWriter> writer;
//    writer->SetFileName("SimplePointsWriter_test.xyz");
//    writer->SetInputConnection(sphereSource->GetOutputPort());
//    writer->Write();

}