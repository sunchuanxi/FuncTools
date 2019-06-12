#ifndef ELPARTY_GLOBAL_H
#define ELPARTY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ELPARTY_LIBRARY)
#  define ELPARTYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ELPARTYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ELPARTY_GLOBAL_H
