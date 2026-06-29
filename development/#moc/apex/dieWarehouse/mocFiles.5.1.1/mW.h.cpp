/****************************************************************************
** Meta object code from reading C++ file 'mW.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/dieWarehouse/mW.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mW.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_mainWindowClass_t {
    QByteArrayData data[22];
    char stringdata[346];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_mainWindowClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_mainWindowClass_t qt_meta_stringdata_mainWindowClass = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 13),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 25),
QT_MOC_LITERAL(4, 57, 20),
QT_MOC_LITERAL(5, 78, 2),
QT_MOC_LITERAL(6, 81, 5),
QT_MOC_LITERAL(7, 87, 5),
QT_MOC_LITERAL(8, 93, 5),
QT_MOC_LITERAL(9, 99, 14),
QT_MOC_LITERAL(10, 114, 22),
QT_MOC_LITERAL(11, 137, 28),
QT_MOC_LITERAL(12, 166, 19),
QT_MOC_LITERAL(13, 186, 11),
QT_MOC_LITERAL(14, 198, 18),
QT_MOC_LITERAL(15, 217, 8),
QT_MOC_LITERAL(16, 226, 18),
QT_MOC_LITERAL(17, 245, 20),
QT_MOC_LITERAL(18, 266, 21),
QT_MOC_LITERAL(19, 288, 28),
QT_MOC_LITERAL(20, 317, 25),
QT_MOC_LITERAL(21, 343, 1)
    },
    "mainWindowClass\0mainTimerSlot\0\0"
    "onBackupDatabaseTimerSlot\0"
    "dataInPlcChangedSlot\0ip\0table\0index\0"
    "value\0showServerSlot\0createReportActionSlot\0"
    "userDatabaseBackupActionSlot\0"
    "showUsageDialogSlot\0qtAboutSlot\0"
    "openWorkStationDlg\0stopSlot\0"
    "backupFinishedSlot\0QProcess::ExitStatus\0"
    "resetGraphicsViewZoom\0"
    "hmiDlgBasicButtonClickedSlot\0"
    "handShakeStateChangedSlot\0s\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mainWindowClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a,
       3,    0,   85,    2, 0x0a,
       4,    4,   86,    2, 0x0a,
       9,    0,   95,    2, 0x0a,
      10,    0,   96,    2, 0x0a,
      11,    0,   97,    2, 0x0a,
      12,    0,   98,    2, 0x0a,
      13,    0,   99,    2, 0x0a,
      14,    0,  100,    2, 0x0a,
      15,    0,  101,    2, 0x0a,
      16,    2,  102,    2, 0x0a,
      18,    0,  107,    2, 0x0a,
      19,    0,  108,    2, 0x0a,
      20,    1,  109,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::QVariant,    5,    6,    7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 17,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   21,

       0        // eod
};

void mainWindowClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        mainWindowClass *_t = static_cast<mainWindowClass *>(_o);
        switch (_id) {
        case 0: _t->mainTimerSlot(); break;
        case 1: _t->onBackupDatabaseTimerSlot(); break;
        case 2: _t->dataInPlcChangedSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QVariant(*)>(_a[4]))); break;
        case 3: _t->showServerSlot(); break;
        case 4: _t->createReportActionSlot(); break;
        case 5: _t->userDatabaseBackupActionSlot(); break;
        case 6: _t->showUsageDialogSlot(); break;
        case 7: _t->qtAboutSlot(); break;
        case 8: _t->openWorkStationDlg(); break;
        case 9: _t->stopSlot(); break;
        case 10: _t->backupFinishedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 11: _t->resetGraphicsViewZoom(); break;
        case 12: _t->hmiDlgBasicButtonClickedSlot(); break;
        case 13: _t->handShakeStateChangedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject mainWindowClass::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_mainWindowClass.data,
      qt_meta_data_mainWindowClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *mainWindowClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mainWindowClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mainWindowClass.stringdata))
        return static_cast<void*>(const_cast< mainWindowClass*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int mainWindowClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
