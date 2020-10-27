#ifndef POLY2VOLUME_H
#define POLY2VOLUME_H
#include "ESAuxiliary_Global.h"

#include <QObject>

class QString;
class ESAUXILIARYSHARED_EXPORT Poly2volume: public QObject
{
    Q_OBJECT
public:
    Poly2volume();

    static void poly2volume(QString stlFileName, QString outputFileName, int dim[3], double spacing[3], double origin[3]);
};

#endif // POLY2VOLUME_H
