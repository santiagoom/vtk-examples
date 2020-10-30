#include <vtkNew.h>
#include <vtkPLYWriter.h>
#include <vtkPolyData.h>
#include <vtkXMLPolyDataReader.h>

int main(int argc, char *argv[]) {
//    if (argc < 3) {
//        std::cerr << "Required arguments: input.vtp output.ply e.g. Bunny.vtp "
//                     "ConvertFile.ply"
//                  << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    std::string inputFileName = argv[1];
//    std::string outputFileName = argv[2];

    std::string inputFileName = "E:\\data\\medicaldata\\vtk\\vtp\\MyImage.vtp";
    std::string outputFileName = "E:\\data\\medicaldata\\vtk\\vtp\\MyImage.ply";

    vtkNew<vtkXMLPolyDataReader> reader;
    reader->SetFileName(inputFileName.c_str());
    reader->Update();

    vtkNew<vtkPLYWriter> writer;
    writer->SetFileName(outputFileName.c_str());
    writer->SetInputConnection(reader->GetOutputPort());
    writer->Update();

    return EXIT_SUCCESS;
}
