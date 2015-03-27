#ifndef BREPORT_GLOBAL_H
#define BREPORT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BREPORT_LIBRARY)
#  define BREPORTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BREPORTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BREPORTDLL_GLOBAL_H
