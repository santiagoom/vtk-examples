//
// Created by admin on 2020/11/1.
//

#ifndef VTK_CPP_MYVTKUTILS_H
#define VTK_CPP_MYVTKUTILS_H
#include <vtkNew.h>
#include <vtkSimplePointsWriter.h>
#include <vtkSphereSource.h>



class myVtkUtils {

};




using namespace std;



int myvtkSimplePointsWriter(vtkNew<vtkPolyData> &obh, string filename);



#endif //VTK_CPP_MYVTKUTILS_H
