/****************************************************************************
** Meta object code from reading C++ file 'mW.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/exportStation/mW.h"
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
    QByteArrayData data[15];
    char stringdata[314];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_mainWindowClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_mainWindowClass_t qt_meta_stringdata_mainWindowClass = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 11),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 44),
QT_MOC_LITERAL(4, 74, 49),
QT_MOC_LITERAL(5, 124, 56),
QT_MOC_LITERAL(6, 181, 44),
QT_MOC_LITERAL(7, 226, 20),
QT_MOC_LITERAL(8, 247, 2),
QT_MOC_LITERAL(9, 250, 5),
QT_MOC_LITERAL(10, 256, 5),
QT_MOC_LITERAL(11, 262, 5),
QT_MOC_LITERAL(12, 268, 17),
QT_MOC_LITERAL(13, 286, 14),
QT_MOC_LITERAL(14, 301, 11)
    },
    "mainWindowClass\0onMainTimer\0\0"
    "onImportStationDefineNewBasketPushButtonSlot\0"
    "onImportStationDefineNewScrapBasketPushButtonSlot\0"
    "onImportStationBasketIsReadyForImportationPushButtonSlot\0"
    "onImportStationIsWithoutBasketPushButtonSlot\0"
    "signalFromTuxPlcSlot\0ip\0table\0index\0"
    "value\0signalFromPlcSlot\0showServerSlot\0"
    "qtAboutSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mainWindowClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x0a,
       3,    0,   60,    2, 0x0a,
       4,    0,   61,    2, 0x0a,
       5,    0,   62,    2, 0x0a,
       6,    0,   63,    2, 0x0a,
       7,    4,   64,    2, 0x0a,
      12,    3,   73,    2, 0x0a,
      13,    0,   80,    2, 0x0a,
      14,    0,   81,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::QVariant,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,    9,   10,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void mainWindowClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        mainWindowClass *_t = static_cast<mainWindowClass *>(_o);
        switch (_id) {
        case 0: _t->onMainTimer(); break;
        case 1: _t->onImportStationDefineNewBasketPushButtonSlot(); break;
        case 2: _t->onImportStationDefineNewScrapBasketPushButtonSlot(); break;
        case 3: _t->onImportStationBasketIsReadyForImportationPushButtonSlot(); break;
        case 4: _t->onImportStationIsWithoutBasketPushButtonSlot(); break;
        case 5: _t->signalFromTuxPlcSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QVariant(*)>(_a[4]))); break;
        case 6: _t->signalFromPlcSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->showServerSlot(); break;
        case 8: _t->qtAboutSlot(); break;
        default: ;
        }
    }
}

const QMetaObject mainWindowClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_mainWindowClass.data,
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
    return QWidget::qt_metacast(_clname);
}

int mainWindowClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
