/****************************************************************************
** Meta object code from reading C++ file 'mW.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/sbEast/mW.h"
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
    QByteArrayData data[19];
    char stringdata[281];
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
QT_MOC_LITERAL(3, 31, 19),
QT_MOC_LITERAL(4, 51, 20),
QT_MOC_LITERAL(5, 72, 2),
QT_MOC_LITERAL(6, 75, 5),
QT_MOC_LITERAL(7, 81, 5),
QT_MOC_LITERAL(8, 87, 5),
QT_MOC_LITERAL(9, 93, 14),
QT_MOC_LITERAL(10, 108, 18),
QT_MOC_LITERAL(11, 127, 22),
QT_MOC_LITERAL(12, 150, 11),
QT_MOC_LITERAL(13, 162, 8),
QT_MOC_LITERAL(14, 171, 21),
QT_MOC_LITERAL(15, 193, 32),
QT_MOC_LITERAL(16, 226, 1),
QT_MOC_LITERAL(17, 228, 25),
QT_MOC_LITERAL(18, 254, 25)
    },
    "mainWindowClass\0mainTimerSlot\0\0"
    "statisticsTimerSlot\0dataInPlcChangedSlot\0"
    "ip\0table\0index\0value\0showServerSlot\0"
    "showPincer2DlgSlot\0showPincer2ConvDlgSlot\0"
    "qtAboutSlot\0stopSlot\0resetGraphicsViewZoom\0"
    "handShakeSbCraneStateChangedSlot\0s\0"
    "showScaleDlgTriggeredSlot\0"
    "showNewSbDlgTriggeredSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mainWindowClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a,
       3,    0,   75,    2, 0x0a,
       4,    4,   76,    2, 0x0a,
       9,    0,   85,    2, 0x0a,
      10,    0,   86,    2, 0x0a,
      11,    0,   87,    2, 0x0a,
      12,    0,   88,    2, 0x0a,
      13,    0,   89,    2, 0x0a,
      14,    0,   90,    2, 0x0a,
      15,    1,   91,    2, 0x0a,
      17,    0,   94,    2, 0x0a,
      18,    0,   95,    2, 0x0a,

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
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void mainWindowClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        mainWindowClass *_t = static_cast<mainWindowClass *>(_o);
        switch (_id) {
        case 0: _t->mainTimerSlot(); break;
        case 1: _t->statisticsTimerSlot(); break;
        case 2: _t->dataInPlcChangedSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QVariant(*)>(_a[4]))); break;
        case 3: _t->showServerSlot(); break;
        case 4: _t->showPincer2DlgSlot(); break;
        case 5: _t->showPincer2ConvDlgSlot(); break;
        case 6: _t->qtAboutSlot(); break;
        case 7: _t->stopSlot(); break;
        case 8: _t->resetGraphicsViewZoom(); break;
        case 9: _t->handShakeSbCraneStateChangedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->showScaleDlgTriggeredSlot(); break;
        case 11: _t->showNewSbDlgTriggeredSlot(); break;
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
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
