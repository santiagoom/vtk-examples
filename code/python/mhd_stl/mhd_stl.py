import vtk
import time


def mhd2stl():
    pass


# vtkSmartPointer<vtkMetaImageWriter> mhdWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
# vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
#
# stlReader->SetFileName(stlFileName.toStdString().c_str());
# stlReader->Update();
#
# vtkSmartPointer<vtkPolyData> inputImage = vtkSmartPointer<vtkPolyData>::New();
# inputImage = stlReader->GetOutput();
#
# vtkSmartPointer<vtkXMLPolyDataWriter> polyDataWriter = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
# polyDataWriter->SetInputConnection(stlReader->GetOutputPort());
# polyDataWriter->SetFileName("MyImage.vtp");
# polyDataWriter->SetCompressorTypeToNone();
# polyDataWriter->SetDataModeToAscii();
# polyDataWriter->Write();

# vtkSmartPointer<vtkImageData> whiteImage = vtkSmartPointer<vtkImageData>::New();
# double bounds[6];
# inputImage->GetBounds(bounds);
#
# whiteImage->SetSpacing(spacing);
#
# for (int i = 0; i < 3; ++i) {
#     if (dim[i] < 1) {
#         dim[i] = 1;
#     }
# }
#
# whiteImage->SetDimensions(dim);
# whiteImage->SetExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1);
# whiteImage->SetOrigin(origin);
# whiteImage->AllocateScalars(VTK_FLOAT, 1);
#
# double inval = 255;
# double outval = 0;
#
# vtkIdType cout = whiteImage->GetNumberOfPoints();
# for (vtkIdType i = 0; i < cout; ++i) {
#     whiteImage->GetPointData()->GetScalars()->SetTuple1(i, inval);
# }
def poly2vol(stlFileName, mhdFileName, dim, spacing, origin):
    tic = time.time()
    mhdWriter = vtk.vtkMetaImageWriter()
    stlReader = vtk.vtkSTLReader()
    stlReader.SetFileName(stlFileName)
    stlReader.Update()

    inputImage = stlReader.GetOutput()
    bounds = [0.0 for _ in range(6)]
    inputImage.GetBounds(bounds)

    whiteImage = vtk.vtkImageData()
    whiteImage.SetSpacing(spacing)
    whiteImage.SetDimensions(dim)
    whiteImage.SetExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1)
    whiteImage.SetOrigin(origin)
    whiteImage.AllocateScalars(10, 1)

    inval = 255.0
    outval = 0.0
    cout = whiteImage.GetNumberOfPoints()
    for i in range(cout):
        whiteImage.GetPointData().GetScalars().SetTuple1(i, inval)

    pol2stenc = vtk.vtkPolyDataToImageStencil()
    pol2stenc.SetInputData(inputImage)
    pol2stenc.SetOutputOrigin(origin)
    pol2stenc.SetOutputSpacing(spacing)
    pol2stenc.SetOutputWholeExtent(whiteImage.GetExtent())
    pol2stenc.Update(0)

    imageStencil = vtk.vtkImageStencil()
    imageStencil.SetInputData(whiteImage)
    imageStencil.SetStencilConnection(pol2stenc.GetOutputPort())
    imageStencil.ReverseStencilOff()
    imageStencil.SetBackgroundValue(outval)
    imageStencil.Update()

    mhdWriter.SetFileName(mhdFileName)
    mhdWriter.SetInputConnection(imageStencil.GetOutputPort())
    mhdWriter.SetCompression(False)
    mhdWriter.Write()

    toc = time.time()
    print("time: {}".format(toc - tic))
    pass


def run():
    stlFileName = "mesh.stl"
    mhdFileName = "mesh.mhd"
    dim = [512, 512, 347]
    spacing = [0.363281, 0.363281, 0.363281]
    origin = [-55.8184, -269.818, 1669]
    poly2vol(stlFileName, mhdFileName, dim, spacing, origin)
    pass


if __name__ == "__main__":
    run()
