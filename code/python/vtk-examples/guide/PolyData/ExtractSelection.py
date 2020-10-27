'''
converted from:
 - http://www.vtk.org/Wiki/VTK/Examples/Python/PolyData/ExtractSelectionCells
'''

import vtk


def main():
    colors = vtk.vtkNamedColors()

    # colors.SetColor('leftBkg', [0.6, 0.5, 0.4, 1.0])
    # colors.SetColor('centreBkg', [0.3, 0.1, 0.4, 1.0])
    # colors.SetColor('rightBkg', [0.4, 0.5, 0.6, 1.0])

    pointSource = vtk.vtkPointSource()
    pointSource.SetNumberOfPoints(50)
    pointSource.Update()

    print('There are %s input points\n' % pointSource.GetOutput().GetNumberOfPoints())

    ids = vtk.vtkIdTypeArray()
    ids.SetNumberOfComponents(1)

    # Set values
    i = 10
    while i < 20:
        ids.InsertNextValue(i)
        i += 1

    selectionNode = vtk.vtkSelectionNode()
    selectionNode.SetFieldType(1)  # POINT
    #  CELL_DATA = 0
    #  POINT_DATA = 1
    #  FIELD_DATA = 2
    #  VERTEX_DATA = 3
    #  EDGE_DATA = 4

    selectionNode.SetContentType(4)  # INDICES
    # SELECTIONS = 0
    # GLOBALIDS = 1
    # PEDIGREEIDS = 2
    # VALUES = 3
    # INDICES = 4
    # FRUSTUM = 5
    # LOCATIONS = 6
    # THRESHOLDS = 7
    # BLOCKS = 8
    selectionNode.SetSelectionList(ids)

    selection = vtk.vtkSelection()
    selection.AddNode(selectionNode)

    extractSelection = vtk.vtkExtractSelection()

    extractSelection.SetInputConnection(0, pointSource.GetOutputPort())
    extractSelection.SetInputData(1, selection)
    extractSelection.Update()

    # In selection
    selected = vtk.vtkUnstructuredGrid()
    selected.ShallowCopy(extractSelection.GetOutput())

    print('There are %s points in the selection' % selected.GetNumberOfPoints())
    print('There are %s cells in the selection' % selected.GetNumberOfCells())

    # Get points that are NOT in the selection
    # invert the selection
    selectionNode.GetProperties().Set(vtk.vtkSelectionNode.INVERSE(), 1)
    extractSelection.Update()

    notSelected = vtk.vtkUnstructuredGrid()
    notSelected.ShallowCopy(extractSelection.GetOutput())

    print('There are %s points NOT in the selection' % notSelected.GetNumberOfPoints())
    print('There are %s cells NOT in the selection' % notSelected.GetNumberOfCells())

    inputMapper = vtk.vtkDataSetMapper()
    inputMapper.SetInputConnection(pointSource.GetOutputPort())
    inputActor = vtk.vtkActor()
    inputActor.SetMapper(inputMapper)
    inputActor.GetProperty().SetColor(colors.GetColor3d('MidnightBlue'))
    inputActor.GetProperty().SetPointSize(5)

    selectedMapper = vtk.vtkDataSetMapper()
    selectedMapper.SetInputData(selected)
    selectedActor = vtk.vtkActor()
    selectedActor.SetMapper(selectedMapper)
    selectedActor.GetProperty().SetColor(colors.GetColor3d('MidnightBlue'))
    selectedActor.GetProperty().SetPointSize(5)

    notSelectedMapper = vtk.vtkDataSetMapper()
    notSelectedMapper.SetInputData(notSelected)
    notSelectedActor = vtk.vtkActor()
    notSelectedActor.SetMapper(notSelectedMapper)
    notSelectedActor.GetProperty().SetColor(colors.GetColor3d('MidnightBlue'))
    notSelectedActor.GetProperty().SetPointSize(5)

    # There will be one render window
    renderWindow = vtk.vtkRenderWindow()
    renderWindow.SetSize(900, 300)
    renderWindow.SetWindowName('ExtractSelection')

    # And one interactor
    interactor = vtk.vtkRenderWindowInteractor()
    interactor.SetRenderWindow(renderWindow)

    # Define viewport ranges
    # (xmin, ymin, xmax, ymax)
    leftViewport = [0.0, 0.0, 0.33, 1.0]
    centerViewport = [0.33, 0.0, 0.66, 1.0]
    rightViewport = [0.66, 0.0, 1.0, 1.0]

    # Create a camera for all renderers
    camera = vtk.vtkCamera()

    # Setup the renderers
    leftRenderer = vtk.vtkRenderer()
    renderWindow.AddRenderer(leftRenderer)
    leftRenderer.SetViewport(leftViewport)
    leftRenderer.SetBackground(colors.GetColor3d('BurlyWood'))
    leftRenderer.SetActiveCamera(camera)

    centerRenderer = vtk.vtkRenderer()
    renderWindow.AddRenderer(centerRenderer)
    centerRenderer.SetViewport(centerViewport)
    centerRenderer.SetBackground(colors.GetColor3d('orchid_dark'))
    centerRenderer.SetActiveCamera(camera)

    rightRenderer = vtk.vtkRenderer()
    renderWindow.AddRenderer(rightRenderer)
    rightRenderer.SetViewport(rightViewport)
    rightRenderer.SetBackground(colors.GetColor3d('CornflowerBlue'))
    rightRenderer.SetActiveCamera(camera)

    leftRenderer.AddActor(inputActor)
    centerRenderer.AddActor(selectedActor)
    rightRenderer.AddActor(notSelectedActor)

    leftRenderer.ResetCamera()

    renderWindow.Render()
    interactor.Start()


if __name__ == '__main__':
    main()
