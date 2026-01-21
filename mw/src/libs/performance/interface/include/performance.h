#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#ifdef PERFORMANCE
 #include <QDateTime>
 #include <iostream>
 #define TRACE(m) std::cerr << QDateTime::currentMSecsSinceEpoch() << " " << m << "\n";
#else
 #define TRACE(m)
#endif

#endif
