#ifndef MHD2STL_H
#define MHD2STL_H
#include "ESAuxiliary_Global.h"

#include <QObject>

class ESAUXILIARYSHARED_EXPORT Mhd2stl : public QObject
{
    Q_OBJECT
public:
    static void ImageFromMhd2Stl(QString imageName, QString fileName, float value);

    explicit Mhd2stl(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MHD2STL_H
