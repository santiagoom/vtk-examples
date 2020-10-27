#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkLandmarkTransform.h>
#include <vtkLine.h>
#include <vtkMatrix4x4.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkVertexGlyphFilter.h>

// For compatibility with new VTK generic data arrays
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif

namespace {
void AxesLines(vtkSmartPointer<vtkPolyData> linesPolyData);
}

int main(int, char*[])
{
  /*
  This demo creates a coordinate frame (+x, +y, +z) of vectors and a rotated,
  peturbed frame (+z, +y, -x) and aligns the rotated frame to the original as
  best as possible.
  */

  vtkNew<vtkNamedColors> colors;

  // An aid to orient the view of the created data.
  vtkNew<vtkPolyData> linesPolyData;
  AxesLines(linesPolyData);

  vtkNew<vtkPoints> sourcePoints;
  double sourcePoint1[3] = {1.0, 0.0, 0.0};
  sourcePoints->InsertNextPoint(sourcePoint1);
  double sourcePoint2[3] = {0.0, 1.0, 0.0};
  sourcePoints->InsertNextPoint(sourcePoint2);
  double sourcePoint3[3] = {0.0, 0.0, 1.0};
  sourcePoints->InsertNextPoint(sourcePoint3);

  vtkNew<vtkPoints> targetPoints;
  double targetPoint1[3] = {0.0, 0.0, 1.1};
  targetPoints->InsertNextPoint(targetPoint1);
  double targetPoint2[3] = {0.0, 1.02, 0.0};
  targetPoints->InsertNextPoint(targetPoint2);
  double targetPoint3[3] = {-1.11, 0.0, 0.0};
  targetPoints->InsertNextPoint(targetPoint3);

  // Setup the transform
  vtkNew<vtkLandmarkTransform> landmarkTransform;
  landmarkTransform->SetSourceLandmarks(sourcePoints);
  landmarkTransform->SetTargetLandmarks(targetPoints);
  landmarkTransform->SetModeToRigidBody();
  landmarkTransform->Update(); // should this be here?

  vtkNew<vtkPolyData> source;
  source->SetPoints(sourcePoints);

  vtkNew<vtkPolyData> target;
  target->SetPoints(targetPoints);

  vtkNew<vtkVertexGlyphFilter> sourceGlyphFilter;
  sourceGlyphFilter->SetInputData(source);
  sourceGlyphFilter->Update();

  vtkNew<vtkVertexGlyphFilter> targetGlyphFilter;
  targetGlyphFilter->SetInputData(target);
  targetGlyphFilter->Update();

  vtkNew<vtkTransformPolyDataFilter> transformFilter;
  transformFilter->SetInputConnection(sourceGlyphFilter->GetOutputPort());
  transformFilter->SetTransform(landmarkTransform);
  transformFilter->Update();

  // Display the transformation matrix that was computed
  vtkMatrix4x4* mat = landmarkTransform->GetMatrix();
  std::cout << "Matrix: " << *mat << std::endl;

  // Visualize
  vtkNew<vtkPolyDataMapper> sourceMapper;
  sourceMapper->SetInputConnection(sourceGlyphFilter->GetOutputPort());

  vtkNew<vtkActor> sourceActor;
  sourceActor->SetMapper(sourceMapper);
  sourceActor->GetProperty()->SetColor(colors->GetColor3d("Lime").GetData());
  sourceActor->GetProperty()->SetPointSize(5);

  vtkNew<vtkPolyDataMapper> targetMapper;
  targetMapper->SetInputConnection(targetGlyphFilter->GetOutputPort());

  vtkNew<vtkActor> targetActor;
  targetActor->SetMapper(targetMapper);
  targetActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
  targetActor->GetProperty()->SetPointSize(5);

  vtkNew<vtkPolyDataMapper> solutionMapper;
  solutionMapper->SetInputConnection(transformFilter->GetOutputPort());

  vtkNew<vtkActor> solutionActor;
  solutionActor->SetMapper(solutionMapper);
  solutionActor->GetProperty()->SetColor(colors->GetColor3d("Blue").GetData());
  solutionActor->GetProperty()->SetPointSize(5);

  vtkNew<vtkPolyDataMapper> axesMapper;
  axesMapper->SetInputData(linesPolyData);

  vtkNew<vtkActor> axesActor;
  axesActor->SetMapper(axesMapper);
  axesActor->GetProperty()->SetLineWidth(1);

  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindow->SetWindowName("LandmarkTransform");

  // Add the actor to the scene
  renderer->AddActor(sourceActor);
  renderer->AddActor(targetActor);
  renderer->AddActor(solutionActor);
  renderer->AddActor(axesActor);

  renderer->SetBackground(colors->GetColor3d("lamp_black").GetData());

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}

namespace {
void AxesLines(vtkSmartPointer<vtkPolyData> linesPolyData)
{
  // Create four points
  double origin[3] = {0.0, 0.0, 0.0};
  double p0[3] = {1.2, 0.0, 0.0};
  double p1[3] = {0.0, 1.2, 0.0};
  double p2[3] = {0.0, 0.0, 1.2};

  // Create a vtkPoints container and store the points in it
  vtkNew<vtkPoints> pts;
  pts->InsertNextPoint(origin);
  pts->InsertNextPoint(p0);
  pts->InsertNextPoint(p1);
  pts->InsertNextPoint(p2);

  // Add the points to the polydata container
  linesPolyData->SetPoints(pts);

  // Create the lines (between Origin and p0, ,p1, p2)
  vtkNew<vtkLine> line0;
  line0->GetPointIds()->SetId(0, 0);
  line0->GetPointIds()->SetId(1, 1);
  vtkNew<vtkLine> line1;
  line1->GetPointIds()->SetId(0, 0);
  line1->GetPointIds()->SetId(1, 2);
  vtkNew<vtkLine> line2;
  line2->GetPointIds()->SetId(0, 0);
  line2->GetPointIds()->SetId(1, 3);

  // Create a vtkCellArray container and store the lines in it
  vtkNew<vtkCellArray> lines;
  lines->InsertNextCell(line0);
  lines->InsertNextCell(line1);
  lines->InsertNextCell(line2);

  // Add the lines to the polydata container
  linesPolyData->SetLines(lines);

  vtkNew<vtkNamedColors> namedColors;

  // Create a vtkUnsignedCharArray container and store the colors in it
  vtkNew<vtkUnsignedCharArray> colors;
  colors->SetNumberOfComponents(3);
  colors->InsertNextTupleValue(namedColors->GetColor3ub("DarkRed").GetData());
  colors->InsertNextTupleValue(namedColors->GetColor3ub("DarkGreen").GetData());
  colors->InsertNextTupleValue(namedColors->GetColor3ub("SteelBlue").GetData());

  // Color the lines.
  linesPolyData->GetCellData()->SetScalars(colors);
}

} // namespace