#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkMarchingCubes.h>
#include <vtkNew.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkStructuredPointsReader.h>

#include <vtkNamedColors.h>
int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0]
              << " InputFile(.vtk) Threshold e.g. brain.vtk 50 1" << std::endl;
    return EXIT_FAILURE;
  }
  const char* fileName = argv[1];
  float threshold = atof(argv[2]);
  int extractLargest = 1;
  if (argc == 4)
  {
    extractLargest = atoi(argv[3]);
  }

  vtkNew<vtkNamedColors> colors;

  // Load data
  vtkNew<vtkStructuredPointsReader> reader;
  reader->SetFileName(fileName);

  // Create a 3D model using marching cubes
  vtkNew<vtkMarchingCubes> mc;
  mc->SetInputConnection(reader->GetOutputPort());
  mc->ComputeNormalsOn();
  mc->ComputeGradientsOn();
  mc->SetValue(0, threshold); // second value acts as threshold

  // To remain largest region
  vtkNew<vtkPolyDataConnectivityFilter> confilter;
  confilter->SetInputConnection(mc->GetOutputPort());
  confilter->SetExtractionModeToLargestRegion();

  // Create a mapper
  vtkNew<vtkPolyDataMapper> mapper;
  if (extractLargest)
  {
    mapper->SetInputConnection(confilter->GetOutputPort());
  }
  else
  {
    mapper->SetInputConnection(mc->GetOutputPort());
  }

  mapper->ScalarVisibilityOff(); // utilize actor's property I set

  // Visualize
  vtkNew<vtkActor> actor;
  actor->GetProperty()->SetColor(colors->GetColor3d("NavajoWhite").GetData());
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(actor);
  renderer->SetBackground(colors->GetColor3d("Burlywood").GetData());
  renderer->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);
  renderer->GetActiveCamera()->SetPosition(0.0, 1.0, 0.0);
  renderer->GetActiveCamera()->SetFocalPoint(0.0, 0.0, 0.0);
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Azimuth(30.0);
  renderer->GetActiveCamera()->Elevation(30.0);
  vtkNew<vtkRenderWindow> renwin;
  renwin->AddRenderer(renderer);
  renwin->SetWindowName("ExtractLargestIsosurface");

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renwin);
  renwin->Render();
  iren->Initialize();
  iren->Start();

  return EXIT_SUCCESS;
}
