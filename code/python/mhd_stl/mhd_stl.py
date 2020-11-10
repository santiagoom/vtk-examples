import vtk
import time


def mhd2stl():
    # QString imageName = "E:\\data\\medicaldata\\esdata\\tianming\\";
    # QString fileName = "CTA20170410_LIUSHIFANG_4.stl";
    # int value = 4;
    # vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    # reader->SetFileName(imageName.toStdString().c_str());
    # reader->Update();
    #
    # vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
    # surface->SetInputConnection(reader->GetOutputPort());
    # surface->ComputeNormalsOn();
    # surface->SetValue(0, value);
    # surface->Update();
    #
    # vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
    # stlWriter->SetFileName(fileName.toStdString().c_str());
    # stlWriter->SetInputConnection(surface->GetOutputPort());
    # stlWriter->Write();
    # return 0;

    imageName = "E:\\data\\medicaldata\\esdata\\tianming\\CTA20140805_LINSHUWEN_labelmap.mhd"
    fileName = "E:\\data\\medicaldata\\esdata\\tianming\\CTA20140805_LINSHUWEN_labelmap_4.stl"
    value = 4
    reader = vtk.vtkMetaImageReader()
    reader.SetFileName(imageName)
    reader.Update()

    surface = vtk.vtkMarchingCubes()
    surface.SetInputConnection(reader.GetOutputPort())
    surface.ComputeNormalsOn()
    surface.SetValue(0, value)
    surface.Update()

    stlWriter = vtk.vtkSTLWriter()
    stlWriter.SetFileName(fileName)
    stlWriter.SetInputConnection(surface.GetOutputPort())
    stlWriter.Write()
    pass


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
    # poly2vol
    stlFileName = "E:\\data\\medicaldata\\other\\yli\\TM_DXL\\TM_DXL\\step3\\mesh.stl"
    mhdFileName = "E:\\data\\medicaldata\\other\\yli\\TM_DXL\\TM_DXL\\step3\\mesh.mhd"
    dim = [512, 512, 347]
    spacing = [0.363281, 0.363281, 0.363281]
    origin = [-55.8184, -269.818, 1669]
    poly2vol(stlFileName, mhdFileName, dim, spacing, origin)

    # mhd2stl
    mhd2stl()
    pass


if __name__ == "__main__":
    run()
