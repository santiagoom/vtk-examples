#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vtkSmartPointer.h>
#include "Geometry.h"

namespace Ui {
    class MainWindow;
}

class MiaImageReader;

class MiaImageND;

class MiaImageExporter;

class ESImageND;

class vtkPolyData;

struct conParameters {
    int dimension = 3;
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;
    float dimensionX = 0.0f;
    float dimensionY = 0.0f;
    float dimensionZ = 0.0f;
    float spacingX = 0.0f;
    float spacingY = 0.0f;
    float spacingZ = 0.0f;
};

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:

    void on_m_importImage_clicked();

    void on_m_outputDir_clicked();

    void on_m_stl2mhd_clicked();

    void on_m_smoothChoice_activated(int index);

    void on_m_combineImage_clicked();

    void on_m_confirm_clicked();

    void on_m_reset_clicked();

    void setConParametersDefaultValue();

    ESImageND *mhdImageFillHoles(ESImageND *image, int dist);

    ESImageND *loadESImage(QString fileName);

    void ExtractParts(QString inputImage, float value, int minPoints);

    void ExtractPolyParts(vtkPolyData *polyParts, QString inputImage, float value, int minPoints);

    void on_m_convertMhd2Stl_clicked();

    void writeESImage(QString fileName, ESImageND *image);

    void on_m_extractParts_clicked();

    void polyData2volume(QString imageFileName, vtkPolyData *polyParts);

    void removeSmallParts(QString imagePathName);

    void normalizeImage(ESImageND *image);

    void regionGrowth(ESImageND *image, QList<Point3i> points, QString fileName);

    void on_m_radius_editingFinished();

    void smoothImage(ESImageND *image);

    void on_m_ratio_editingFinished();

private:
    Ui::MainWindow *ui;

    QString m_inputPath;
    QString m_outputPath;
    QList<QString> m_fileName;
    float m_radius = 2.0f;
    float m_ratio = 0.3f;
    bool m_smoothImage;

    MiaImageReader *m_imageReader;
    MiaImageExporter *m_exporter;

    conParameters m_parameters;
};

#endif // MAINWINDOW_H
