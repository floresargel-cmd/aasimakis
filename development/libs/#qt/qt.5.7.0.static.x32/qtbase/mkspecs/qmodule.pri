QT_BUILD_PARTS += libs tools
QT_QCONFIG_PATH = 

host_build {
    QT_CPU_FEATURES.i386 =  sse sse2
} else {
    QT_CPU_FEATURES.i386 =  sse sse2
}
QT_COORD_TYPE += double
CONFIG += precompile_header pcre debug compile_examples msvc_mp sse2 sse3 ssse3 sse4_1 sse4_2 avx avx2 largefile
LIBS           += -LR:/development/libs/qt/qt.5.7.0.static.x32/__/mysql-5.7.14-win32/lib -LR:/development/libs/qt/qt.5.7.0.static.x32/__/oracleSdk12.1.0.2.0/sdk/lib/msvc -LR:/development/libs/qt/qt.5.7.0.static.x32/__/oracleSdk12.1.0.2.0/sdk/lib/msvc/vc12
INCLUDEPATH    += R:/development/libs/qt/qt.5.7.0.static.x32/__/mysql-5.7.14-win32/include R:/development/libs/qt/qt.5.7.0.static.x32/__/oracleSdk12.1.0.2.0/sdk/include
sql-drivers    += mysql odbc oci sqlite
styles         += windows fusion windowsxp windowsvista
