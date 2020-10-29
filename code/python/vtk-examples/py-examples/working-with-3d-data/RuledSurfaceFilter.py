import vtk


def main():
    colors = vtk.vtkNamedColors()

    # Create a rendering window and renderer
    ren = vtk.vtkRenderer()
    renWin = vtk.vtkRenderWindow()
    renWin.AddRenderer(ren)

    # Create a renderwindowinteractor
    iren = vtk.vtkRenderWindowInteractor()
    iren.SetRenderWindow(renWin)

    # Create the points fot the lines.
    points = vtk.vtkPoints()
    points.InsertPoint(0, 0, 0, 1)
    points.InsertPoint(1, 1, 0, 0)
    points.InsertPoint(2, 0, 1, 0)
    points.InsertPoint(3, 1, 1, 1)

    # Create line1
    line1 = vtk.vtkLine()
    line1.GetPointIds().SetId(0, 0)
    line1.GetPointIds().SetId(1, 1)

    # Create line2
    line2 = vtk.vtkLine()
    line2.GetPointIds().SetId(0, 2)
    line2.GetPointIds().SetId(1, 3)

    # Create a cellArray containing the lines
    lines = vtk.vtkCellArray()
    lines.InsertNextCell(line1)
    lines.InsertNextCell(line2)

    # Create the vtkPolyData to contain the points and cellArray with the lines
    polydata = vtk.vtkPolyData()
    polydata.SetPoints(points)
    polydata.SetLines(lines)

    # Create the ruledSurfaceFilter from the polydata containing the lines
    ruledSurfaceFilter = vtk.vtkRuledSurfaceFilter()
    ruledSurfaceFilter.SetInputData(polydata)
    ruledSurfaceFilter.SetResolution(21, 21)
    ruledSurfaceFilter.SetRuledModeToResample()

    # Create the mapper with the ruledSurfaceFilter as input
    mapper = vtk.vtkPolyDataMapper()
    mapper.SetInputConnection(ruledSurfaceFilter.GetOutputPort())

    # Create the actor with the mapper
    actor = vtk.vtkActor()
    actor.SetMapper(mapper)
    actor.GetProperty().SetColor(colors.GetColor3d("Banana"))
    actor.GetProperty().SetSpecular(0.6)
    actor.GetProperty().SetSpecularPower(30)

    # Add the actor to the display
    ren.AddActor(actor)
    ren.SetBackground(colors.GetColor3d("SteelBlue"))
    renWin.SetWindowName('RuledSurfaceFilter')

    # Enable user interface interactor
    iren.Initialize()
    renWin.Render()
    # ren.GetActiveCamera().SetPosition(3.7, -0.5, -0.5)
    # ren.GetActiveCamera().SetFocalPoint(0.5, 0.5, 0.5)
    # ren.GetActiveCamera().SetViewUp(-0.3, 0.1, -1.0)
    ren.GetActiveCamera().Azimuth(60)
    ren.GetActiveCamera().Elevation(60)
    ren.ResetCamera()

    renWin.Render()
    iren.Start()


if __name__ == '__main__':
    main()
