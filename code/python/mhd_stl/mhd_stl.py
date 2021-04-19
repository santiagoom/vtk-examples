import vtk
import time
import numpy as np
import SimpleITK as sitk
import os


def getMhdMeta(mhd_info_path, keys):
    with open(mhd_info_path, mode="r") as f:
        lines = f.readlines()

    infos = dict()
    for line in lines:
        parts = line.split()
        if parts[0] == "Offset":
            Offsets = []
            Offsets.append(parts[2])
            Offsets.append(parts[3])
            Offsets.append(parts[4])
            infos["Offset"] = Offsets
            pass
        if parts[0] == "ElementSpacing":
            Spacinges = []
            Spacinges.append(parts[2])
            Spacinges.append(parts[3])
            Spacinges.append(parts[4])
            infos["ElementSpacing"] = Spacinges
        if parts[0] == "DimSize":
            Dims = []
            Dims.append(parts[2])
            Dims.append(parts[3])
            Dims.append(parts[4])
            infos["DimSize"] = Dims

    results = dict()
    for key in keys:
        results[key] = infos[key]
    return results


def mhd2stl():
    # QString imageName = "E:/data/medicaldata/esdata/tianming/";
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

    imageName = "F:/data/esdata/yanli/LIU YONG QIONG_tm/LIU YONG QIONG_tm/LIU YONG QIONG/LIU YONG QIONG/LH_10_grayvalues_from_txt.mhd"
    fileName = "F:/data/esdata/yanli/LIU YONG QIONG_tm/LIU YONG QIONG_tm/LIU YONG QIONG/LIU YONG QIONG/LH_10_grayvalues_from_txt.stl"
    value = 255
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

    inval = 1.0
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


def mask2vol_single():
    tic = time.time()

    filename = "LH_10_grayvalues"
    mask_path = "F:/data/esdata/yanli/LIU YONG QIONG_tm/LIU YONG QIONG_tm/LIU YONG QIONG/LIU YONG QIONG/{}.txt".format(
        filename)
    output_path = "F:/data/esdata/yanli/LIU YONG QIONG_tm/LIU YONG QIONG_tm/LIU YONG QIONG/LIU YONG QIONG/"
    mhd_info_path = "F:/data/esdata/yanli/LIU YONG QIONG_tm/LIU YONG QIONG_tm/LIU YONG QIONG/LIU YONG QIONG/origin.mhd"
    keys = ["Offset", "ElementSpacing", "DimSize"]
    mhd_infos = getMhdMeta(mhd_info_path, keys)

    Offset = np.array([mhd_infos["Offset"][0], mhd_infos["Offset"][1], mhd_infos["Offset"][2]],
                      dtype=np.float32).tolist()
    ElementSpacing = np.array(
        [mhd_infos["ElementSpacing"][0], mhd_infos["ElementSpacing"][1], mhd_infos["ElementSpacing"][2]],
        dtype=np.float32).tolist()
    Dims = np.array(
        [mhd_infos["DimSize"][2], mhd_infos["DimSize"][0], mhd_infos["DimSize"][1]],
        dtype=np.int)

    arrays = np.zeros(Dims, dtype=np.float32)

    with open(mask_path, mode="r") as f:
        lines = f.readlines()

    for i, line in enumerate(lines[:]):
        parts = line.strip().split()
        x = float(parts[0][:-1])
        y = float(parts[1][:-1])
        z = float(parts[2][:-1])

        pos_x = round((x - Offset[0]) / ElementSpacing[0])
        pos_y = round((y - Offset[1]) / ElementSpacing[1])
        pos_z = round((z - Offset[2]) / ElementSpacing[2])

        arrays[pos_z, pos_y, pos_x] = 1

    volume = sitk.GetImageFromArray(arrays)
    volume.SetOrigin(Offset)
    volume.SetSpacing(ElementSpacing)
    sitk.WriteImage(volume, "{}/{}_from_txt.mhd".format(output_path, filename))
    toc = time.time()
    intervial = toc - tic

    print("time cost: {:.2f}".format(intervial))

    pass


def run():
    # poly2vol
    # stlFileName = "F:/data/esdata/yanli/LIU YONG QIONG_tm/LIU YONG QIONG_tm/LIU YONG QIONG/LIU YONG QIONG/_LH_10_001.stl"
    # mhdFileName = "F:/data/esdata/yanli/LIU YONG QIONG_tm/LIU YONG QIONG_tm/LIU YONG QIONG/LIU YONG QIONG/_LH_10_001.mhd"
    # mhd_info_path = "F:/data/esdata/yanli/LIU YONG QIONG_tm/LIU YONG QIONG_tm/LIU YONG QIONG/LIU YONG QIONG/origin.mhd"

    stlFileName = "C:/Users/admin/Downloads/surfaces.stl"
    mhdFileName = "C:/Users/admin/Downloads/surfaces_xbai.mhd"
    mhd_info_path = "C:/Users/admin/Downloads/surfaces.mhd"



    keys = ["Offset", "ElementSpacing", "DimSize"]
    mhd_infos = getMhdMeta(mhd_info_path, keys)

    Offset = np.array([mhd_infos["Offset"][0], mhd_infos["Offset"][1], mhd_infos["Offset"][2]],
                      dtype=np.float32).tolist()
    ElementSpacing = np.array(
        [mhd_infos["ElementSpacing"][0], mhd_infos["ElementSpacing"][1], mhd_infos["ElementSpacing"][2]],
        dtype=np.float32).tolist()
    Dims = np.array(
        [mhd_infos["DimSize"][0], mhd_infos["DimSize"][1], mhd_infos["DimSize"][2]],
        dtype=np.int).tolist()

    poly2vol(stlFileName, mhdFileName, Dims, ElementSpacing, Offset)

    # mhd2stl
    # mhd2stl()

    # mask2vol
    # mask2vol_single()

    pass


if __name__ == "__main__":
    run()
