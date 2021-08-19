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

#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkDataSetSurfaceFilter.h>

using namespace std;


vtkSmartPointer<vtkPolyData> ScaledPolyData(vtkPolyData *pd, double *scaled) {
    vtkSmartPointer<vtkPolyData> outputPD = nullptr;
    vtkSmartPointer<vtkTransform> tf = vtkSmartPointer<vtkTransform>::New();
    double *center = pd->GetCenter();
    tf->Translate(center[0], center[1], center[2]);
    tf->Scale(scaled);
    tf->Translate(-center[0], -center[1], -center[2]);
    vtkSmartPointer<vtkTransformPolyDataFilter> tfPDFilter =
            vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    tfPDFilter->SetInputData(pd);
    tfPDFilter->SetTransform(tf);
    tfPDFilter->Update();
    outputPD = tfPDFilter->GetOutput();
    tfPDFilter->GetOutputPort();
//    return outputPD;
    return outputPD;
}

vtkSmartPointer<vtkTransformPolyDataFilter> ScaledPolyData1(vtkPolyData *pd, double *scaled) {
    vtkSmartPointer<vtkPolyData> outputPD = nullptr;
    vtkSmartPointer<vtkTransform> tf = vtkSmartPointer<vtkTransform>::New();
    double *center = pd->GetCenter();
    tf->Translate(center[0], center[1], center[2]);
    tf->Scale(scaled);
    tf->Translate(-center[0], -center[1], -center[2]);
    vtkSmartPointer<vtkTransformPolyDataFilter> tfPDFilter =
            vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    tfPDFilter->SetInputData(pd);
    tfPDFilter->SetTransform(tf);
    tfPDFilter->Update();
    outputPD = tfPDFilter->GetOutput();
    tfPDFilter->GetOutputPort();
//    return outputPD;
    return tfPDFilter;
}


int main() {
//    string stl = "";
//    QString stl = "F:\\data\\esdata\\esdata\\experiment\\stl\\CTA20140728_DENGXILIAN\\AT_6_CTA20140728_DENGXILIAN_predictionv1.stl";
//    QString stlout = "F:\\data\\esdata\\esdata\\experiment\\stl\\CTA20140728_DENGXILIAN\\AT_6_CTA20140728_DENGXILIAN_predictionv1_0.9.stl";
//    QString filename = "LA_2_CTA20140728_DENGXILIAN_predictionv1";
//    QString filename = "LV_3_CTA20140728_DENGXILIAN_predictionv1";
//    QString filename = "RA_4_CTA20140728_DENGXILIAN_predictionv1";
    QString filename = "RV_5_CTA20140728_DENGXILIAN_predictionv1";
//    QString mhd_meta = QString("%1\\%2\\step1\\origin.mhd").arg(mhd_meta_path).arg(caseInfo.fileName());

//    QString stl = "F:\\data\\esdata\\esdata\\experiment\\stl\\CTA20140728_DENGXILIAN\\AT_6_CTA20140728_DENGXILIAN_predictionv1.stl";
//    QString stlout = "F:\\data\\esdata\\esdata\\experiment\\stl\\CTA20140728_DENGXILIAN\\AT_6_CTA20140728_DENGXILIAN_predictionv1_0.9.stl";

//    QString stl = QString("F:\\data\\esdata\\esdata\\experiment\\stl\\CTA20140728_DENGXILIAN\\lv\\%1.stl").arg(filename);
//    QString stlout = QString("F:\\data\\esdata\\esdata\\experiment\\stl\\CTA20140728_DENGXILIAN\\lv\\%1_0.9.stl").arg(
//            filename);
//    QString stlout1 = QString("F:\\data\\esdata\\esdata\\experiment\\stl\\CTA20140728_DENGXILIAN\\lv\\%1_1.1.stl").arg(
//            filename);

    QString stl = QString("F:\\data\\esdata\\esdata\\experiment\\stl\\CTA20140728_DENGXILIAN\\%1.stl").arg(filename);
    QString stlout = QString("F:\\data\\esdata\\esdata\\experiment\\stl\\CTA20140728_DENGXILIAN\\lv2\\%1_0.9.stl").arg(
            filename);
    QString stlout1 = QString("F:\\data\\esdata\\esdata\\experiment\\stl\\CTA20140728_DENGXILIAN\\lv2\\%1_1.1.stl").arg(
            filename);

    vtkSmartPointer<vtkMetaImageWriter> mhdWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
    vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
    stlReader->SetFileName(stl.toStdString().c_str());
    stlReader->Update();
    vtkSmartPointer<vtkPolyData> inputImage = vtkSmartPointer<vtkPolyData>::New();
    inputImage = stlReader->GetOutput();


//    double scaled[3] = {0.9, 0.9, 0.9};
//    double scaled1[3] = {1.1, 1.1, 1.1};
    double scaled[3] = {0.75, 0.75, 0.75};
    double scaled1[3] = {1.25, 1.25, 1.25};
//    vtkSmartPointer<vtkPolyData> output = ScaledPolyData(inputImage, scaled);

    vtkSmartPointer<vtkTransformPolyDataFilter> tfPDFilter = ScaledPolyData1(inputImage, scaled);
    vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter->SetFileName(stlout.toStdString().c_str());
    stlWriter->SetInputConnection(tfPDFilter->GetOutputPort());
    stlWriter->Write();

    vtkSmartPointer<vtkTransformPolyDataFilter> tfPDFilter1 = ScaledPolyData1(inputImage, scaled1);
    vtkSmartPointer<vtkSTLWriter> stlWriter1 = vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter1->SetFileName(stlout1.toStdString().c_str());
    stlWriter1->SetInputConnection(tfPDFilter1->GetOutputPort());
    stlWriter1->Write();

    return 0;
}