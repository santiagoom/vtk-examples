//
// GenerateModelsFromLabels
//   Usage: GenerateModelsFromLabels InputVolume Startlabel Endlabel
//          where
//          InputVolume is a meta file containing a 3 volume of
//            discrete labels.
//          StartLabel is the first label to be processed
//          EndLabel is the last label to be processed
//          NOTE: There can be gaps in the labeling. If a label does
//          not exist in the volume, it will be skipped.
//
//
#include <vtkDiscreteMarchingCubes.h>
#include <vtkGeometryFilter.h>
#include <vtkImageAccumulate.h>
#include <vtkMaskFields.h>
#include <vtkMetaImageReader.h>
#include <vtkNew.h>
#include <vtkThreshold.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkXMLPolyDataWriter.h>

#include <sstream>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSTLWriter.h>

#include <QString>
#include <QDir>
#include <myUtils.h>

using namespace std;

int main(int argc, char *argv[]) {

    QString case_path = "F:\\data\\esdata\\esdata\\experiment\\CTA20140728_DENGXILIAN";
//    std::string casename = qPrintable(QFileInfo(case_path).completeBaseName());
//    std::string casedir = qPrintable(case_path);
    QString casename = QFileInfo(case_path).completeBaseName();
//    std::string casedir = qPrintable(case_path);

    QDir case_dir = QDir(case_path);
    case_dir.cdUp();

    QString parent_path = case_dir.absolutePath();

    qprint(parent_path);

    QString segment_type = "heart";

    QString stl_path = QString("%1\\%2\\%3\\").arg(parent_path).arg("stl").arg(casename);

    QDir stl_dir = QDir(stl_path);
    if (!stl_dir.exists()) {
        stl_dir.mkpath(stl_path);
    }
    QString filename;
    if (segment_type == "heart")
        filename = QString("%1\\%2\\heart_mask.mhd").arg(case_path).arg("step1");
    if (segment_type == "heart")
        filename = QString("%1\\%2\\heart_mask.mhd").arg(case_path).arg("step1");
//    std::string filename = "F:\\data\\esdata\\esdata\\volume_view\\stl_to_mhd\\CTA20140805_LINSHUWEN\\CTA20140805_LINSHUWEN_merge.mhd";
//    string filename = "E:\\data\\medicaldata\\vtk\\VTKWikiExamples-master\\VTKWikiExamples-master\\Testing\\Data\\frogtissue.mhd";
    if (filename.isEmpty())
        return 0;

    // Create all of the classes we will need
    vtkNew<vtkMetaImageReader> reader;
    vtkNew<vtkImageAccumulate> histogram;
    vtkNew<vtkDiscreteMarchingCubes> discreteCubes;
    vtkNew<vtkWindowedSincPolyDataFilter> smoother;
    vtkNew<vtkThreshold> selector;
    vtkNew<vtkMaskFields> scalarsOff;
    vtkNew<vtkGeometryFilter> geometry;
//    vtkNew<vtkXMLPolyDataWriter> writer;
    vtkNew<vtkSTLWriter> writer;

    // Define all of the variables
//    unsigned int startLabel = atoi(argv[2]);
//    unsigned int endLabel = atoi(argv[3]);
    unsigned int startLabel = 1;
    unsigned int endLabel = 7;
    std::string filePrefix = "Label";
    unsigned int smoothingIterations = 15;
    double passBand = 0.001;
    double featureAngle = 120.0;

    // Generate models from labels
    // 1) Read the meta file
    // 2) Generate a histogram of the labels
    // 3) Generate models from the labeled volume
    // 4) Smooth the models
    // 5) Output each model into a separate file

//    reader->SetFileName(argv[1]);
//    reader->SetFileName(filename.c_str());
    reader->SetFileName(filename.toLocal8Bit().constData());

    histogram->SetInputConnection(reader->GetOutputPort());
    histogram->SetComponentExtent(0, endLabel, 0, 0, 0, 0);
    histogram->SetComponentOrigin(0, 0, 0);
    histogram->SetComponentSpacing(1, 1, 1);
    histogram->Update();

    discreteCubes->SetInputConnection(reader->GetOutputPort());
    discreteCubes->GenerateValues(endLabel - startLabel + 1, startLabel,
                                  endLabel);

    smoother->SetInputConnection(discreteCubes->GetOutputPort());
    smoother->SetNumberOfIterations(smoothingIterations);
    smoother->BoundarySmoothingOff();
    smoother->FeatureEdgeSmoothingOff();
    smoother->SetFeatureAngle(featureAngle);
    smoother->SetPassBand(passBand);
    smoother->NonManifoldSmoothingOn();
    smoother->NormalizeCoordinatesOn();
    smoother->Update();

    selector->SetInputConnection(smoother->GetOutputPort());
    selector->SetInputArrayToProcess(0, 0, 0,
                                     vtkDataObject::FIELD_ASSOCIATION_CELLS,
                                     vtkDataSetAttributes::SCALARS);

    // Strip the scalars from the output
    scalarsOff->SetInputConnection(selector->GetOutputPort());
    scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA,
                                 vtkDataSetAttributes::SCALARS);
    scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA,
                                 vtkDataSetAttributes::SCALARS);

    geometry->SetInputConnection(scalarsOff->GetOutputPort());

    writer->SetInputConnection(geometry->GetOutputPort());

    for (unsigned int i = startLabel; i <= endLabel; i++) {
        // see if the label exists, if not skip it
        double frequency =
                histogram->GetOutput()->GetPointData()->GetScalars()->GetTuple1(i);
        if (frequency == 0.0) {
            continue;
        }

        // select the cells for a given label
        selector->ThresholdBetween(i, i);

        // output the polydata
        std::stringstream ss;
//        ss << filePrefix << i << ".vtp";
        ss << filePrefix << i << ".stl";
        std::cout << argv[0] << " writing " << ss.str() << std::endl;
        QString stl_filename;
        if (segment_type == "heart") {
//            CM_1_CTA20131112_ZHENGSUZHI_v1.stl
//            LA_2_CTA20131112_ZHENGSUZHI_v1.stl
//            LV_3_CTA20131112_ZHENGSUZHI_v1.stl
//            RA_4_CTA20131112_ZHENGSUZHI_v1.stl
//            RV_5_CTA20131112_ZHENGSUZHI_v1.stl
//            AT_6_CTA20131112_ZHENGSUZHI_v1.stl
//            PA_7_CTA20131112_ZHENGSUZHI_v1.stl
//            LAA_8_CTA20131112_ZHENGSUZHI_v1.stl

            if (i == 1)
                stl_filename = QString("%1\\CM_%2_%3_predictionv1.stl").arg(stl_path).arg(QString::number(i)).arg(
                        casename);
            if (i == 2)
                stl_filename = QString("%1\\LA_%2_%3_predictionv1.stl").arg(stl_path).arg(QString::number(i)).arg(
                        casename);
            if (i == 3)
                stl_filename = QString("%1\\LV_%2_%3_predictionv1.stl").arg(stl_path).arg(QString::number(i)).arg(
                        casename);
            if (i == 4)
                stl_filename = QString("%1\\RA_%2_%3_predictionv1.stl").arg(stl_path).arg(QString::number(i)).arg(
                        casename);
            if (i == 5)
                stl_filename = QString("%1\\RV_%2_%3_predictionv1.stl").arg(stl_path).arg(QString::number(i)).arg(
                        casename);
            if (i == 6)
                stl_filename = QString("%1\\AT_%2_%3_predictionv1.stl").arg(stl_path).arg(QString::number(i)).arg(
                        casename);
            if (i == 7)
                stl_filename = QString("%1\\PA_%2_%3_predictionv1.stl").arg(stl_path).arg(QString::number(i)).arg(
                        casename);
        }


        writer->SetFileName(stl_filename.toLocal8Bit().constData());
        writer->Write();
    }
    return EXIT_SUCCESS;
}
