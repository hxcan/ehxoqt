#ifndef EHXO_QT_GLOBAL_H
#define EHXO_QT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EHXO_QT)

#if defined(EHXO_QT_EXPORT)
#else
#define EHXO_QT_EXPORT Q_DECL_EXPORT
#endif

#else
#define EHXO_QT_EXPORT Q_DECL_IMPORT
#endif

#endif // XY_GLOBAL_H
