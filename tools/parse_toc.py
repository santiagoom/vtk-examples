import os
import shutil
import pyutils as u


def cpp_toc():
    # cpp_table = "../code/cpp/vtk-examples/InputandOutput.md"
    # cpp_table = "../code/cpp/vtk-examples/cpp-examples.md"
    cpp_table = "../code/cpp/vtk-examples/cpp-examples-zero.md"

    guide_path = "D:\\gitrepoe\\vtk-examples\\code\\cpp\\vtk-examples\\guide"
    target_path = ""
    with open(cpp_table, mode="r")as f:
        lines = f.readlines()

    levels = []
    for line in lines:
        parts = line.split()
        if (len(parts) > 1) and parts[0].startswith("#"):
            level_rate = len(parts[0])
            # if (level_rate <2):
            #     continue
            # print(level)
            # print(level[-1])
            flag_index = parts[-1].find("#")
            # print(flag_index)
            folder_name = parts[-1][flag_index + 1:-1]
            print("{},  {}".format(level_rate, folder_name))

            for i in range(len(levels) - level_rate + 1):
                levels.pop()
            levels.append(folder_name)
            path = "./"
            for i, level in enumerate(levels):
                path = os.path.join(path, level)
            print(path)
            target_path = path
            if not os.path.exists(target_path):
                os.makedirs(target_path)

        if (len(parts) > 2) and len(parts[1]) > 2 and parts[1][0] == "[":
            flag_left = parts[1].find("(")
            flag_right = parts[1].find(")")

            locate = parts[1][flag_left + 1:flag_right]
            # if locate.startswith("http"):
            # print(parts)

            filename = locate.split("/")[-1]
            filepath = locate.split("/")[-2]
            # print("{}, {}".format(filedir, filename))

            src_cxx = os.path.join(guide_path, filepath, "{}.cxx".format(filename))
            dst_cxx = os.path.join(target_path, "{}.cxx".format(filename))
            src_md = os.path.join(guide_path, filepath, "{}.md".format(filename))
            dst_md = os.path.join(target_path, "{}.md".format(filename))
            if os.path.isfile(src_cxx):
                try:
                    shutil.copyfile(src_cxx, dst_cxx)
                    shutil.copyfile(src_md, dst_md)
                except:
                    pass
                pass
            else:
                msg = "{}, {} with problems".format(filepath, filename)
                print(msg)
    pass


def py_toc():
    cpp_table = "../code/python/vtk-examples/python-examples-zero.md"

    guide_path = "D:\\gitrepoe\\vtk-examples\\code\\python\\vtk-examples\\guide"
    target_path = ""
    with open(cpp_table, mode="r", encoding="utf8")as f:
        lines = f.readlines()

    levels = []
    for line in lines:
        parts = line.split()
        if (len(parts) > 1) and parts[0].startswith("#"):
            level_rate = len(parts[0])
            flag_index = parts[-1].find("#")
            folder_name = parts[-1][flag_index + 1:-1]
            print("{},  {}".format(level_rate, folder_name))

            for i in range(len(levels) - level_rate + 1):
                levels.pop()
            levels.append(folder_name)
            path = "./"
            for i, level in enumerate(levels):
                path = os.path.join(path, level)
            print(path)
            target_path = path
            if not os.path.exists(target_path):
                os.makedirs(target_path)

        if (len(parts) > 2) and len(parts[1]) > 2 and parts[1][0] == "[":
            flag_left = parts[1].find("(")
            flag_right = parts[1].find(")")

            locate = parts[1][flag_left + 1:flag_right]
            # if not locate.startswith("http"):
            #     print(parts)

            filename = locate.split("/")[-1]
            filepath = locate.split("/")[-2]
            # print("{}, {}".format(filedir, filename))

            src_py = os.path.join(guide_path, filepath, "{}.py".format(filename))
            dst_py = os.path.join(target_path, "{}.py".format(filename))
            src_md = os.path.join(guide_path, filepath, "{}.md".format(filename))
            dst_md = os.path.join(target_path, "{}.md".format(filename))
            if os.path.isfile(src_py):
                try:
                    shutil.copyfile(src_py, dst_py)
                    shutil.copyfile(src_md, dst_md)
                    pass
                except:
                    pass
                pass
            else:
                msg = "{}, {} with problems".format(filepath, filename)
                print(src_py)
                print(msg)
    pass


def create_standard_CMakeLists():
    vtk_examples_path = "D:\\gitrepoe\\vtk-examples\\code\\cpp\\vtk-examples"
    cpp_examples_path = os.path.join(vtk_examples_path, "cpp-examples")
    CMakeLists_src = os.path.join(vtk_examples_path, "CMakeLists.txt")
    for root, dirs, files in os.walk(cpp_examples_path):
        CMakeLists_dst = os.path.join(root, "CMakeLists.txt")
        u.copy(CMakeLists_src, CMakeLists_dst)

    pass


if __name__ == "__main__":
    # cpp_toc()
    # py_toc()
    create_standard_CMakeLists()
    pass
