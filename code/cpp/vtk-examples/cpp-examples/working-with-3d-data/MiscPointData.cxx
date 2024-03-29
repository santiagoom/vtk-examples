#include <string>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkMath.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkXMLPolyDataReader.h>

int main(int argc, char* argv[])
{
  // Parse command line arguments
  if (argc != 2)
  {
    std::cout << "Required arguments: Filename" << std::endl;
    return EXIT_FAILURE;
  }

  // Get filename from command line
  std::string filename = argv[1]; // first command line argument

  // Read the file
  vtkNew<vtkXMLPolyDataReader> reader;
  std::cout << "Reading " << filename << std::endl;
  reader->SetFileName(filename.c_str());
  reader->Update();

  // Extract the polydata
  auto polydata = reader->GetOutput();

  // Get the number of points in the polydata
  vtkIdType idNumPointsInFile = polydata->GetNumberOfPoints();

  // Add distances to each point
  vtkNew<vtkFloatArray> distances;
  distances->SetNumberOfComponents(1);
  distances->SetName("Distances");

  for (vtkIdType i = 0; i < idNumPointsInFile; i++)
  {
    distances->InsertNextValue(vtkMath::Random(0.0, 1.0));
  }

  polydata->GetPointData()->AddArray(distances);

  return EXIT_SUCCESS;
}
