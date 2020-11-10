#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

struct parameters{
//    enum ResampleMethod { Mia_Resampe_LinearInterpoltion, Mia_Resample_NearestNeighbor,
//                                                          Mia_Resampe_TriangleConvolution};
    QString resampleMethod = "LinearInterpoltion";
    float spacingX = 0.0f;
    float spacingY = 0.0f;
    float spacingZ = 0.0f;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setParametersDefault();
    void on_m_importImage_clicked();
    void on_m_outputImage_clicked();
    void on_m_mhdResampleGenerate_clicked();
    void on_m_originDir_clicked();
    void on_m_referenceDir_clicked();
    void on_m_outpurDir_clicked();
    void on_m_batchGenerate_clicked();
private:

    Ui::MainWindow *ui;

    QString m_inputFileName;
    QString m_outputFileName;
    QString m_originPath;
    QString m_referencePath;
    QString m_outputPath;
    parameters m_parameters;
};

#endif // MAINWINDOW_H
