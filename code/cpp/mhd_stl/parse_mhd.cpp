#include <iostream>
#include <vtkMarchingCubes.h>
#include <vtkSTLWriter.h>
#include <vtkMetaImageReader.h>
#include <QString>
#include <myUtils.h>

using namespace std;

int main() {
//    QString imageName = QFileDialog::getOpenFileName(this, tr("Open a mhd image"), "", tr("MHD File (*.mhd)"));
//    QString imageFilePath, fileName;
//    QFileInfo fi;
//    fi = QFileInfo(imageName);
//    fileName = fi.baseName();
//    imageFilePath = fi.absolutePath();
//    ESImageND *image = loadESImage(imageName);
//    ESImageND *newImage = new ESImageND(image->getWidth(), image->getHeight(), image->getDepth(), 0, ESIM_float);
//    newImage->copyGeometryInfoFrom(image);
//
//    float *pImage = reinterpret_cast<float *>(image->getDataPtr());
//    float *pNewImage = reinterpret_cast<float *>(newImage->getDataPtr());
//    float epsilon = 0.001f;
//    for (int i = 0; i < 8; i++) {
//        QString proFileName = imageFilePath + "/" + fileName + "_process.mhd";
//        QString outputFileName = imageFilePath + "/" + fileName + "_" + QString::number(i) + ".stl";
//        for (int j = 0; j < image->getImageSize(); j++) {
//            if (abs(pImage[j] - i) < epsilon) {
//                pNewImage[j] = i;
//            }
//        }
//        writeESImage(proFileName, newImage);
//        Mhd2stl::ImageFromMhd2Stl(proFileName, outputFileName, i);
//    }
//    QString imageName = "./temp/CTA20170410_LIUSHIFANG_labelmap.mhd";
    QString imageName = "E:\\data\\medicaldata\\esdata\\tianming\\CTA20140805_LINSHUWEN_labelmap.mhd";
    QString fileName = "E:\\data\\medicaldata\\esdata\\tianming\\CTA20140805_LINSHUWEN_labelmap_4.stl";
    int value = 4;
    vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    reader->SetFileName(imageName.toStdString().c_str());
    reader->Update();
    print(reader->GetHeaderSize());

//    vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
//    surface->SetInputConnection(reader->GetOutputPort());
//    surface->ComputeNormalsOn();
//    surface->SetValue(0, value);
//    surface->Update();
//
//    vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
//    stlWriter->SetFileName(fileName.toStdString().c_str());
//    stlWriter->SetInputConnection(surface->GetOutputPort());
//    stlWriter->Write();
    return 0;
}