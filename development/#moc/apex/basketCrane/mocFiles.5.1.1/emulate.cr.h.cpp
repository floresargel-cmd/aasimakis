/****************************************************************************
** Meta object code from reading C++ file 'emulate.cr.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane/emulate.cr.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'emulate.cr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_craneEmulatorClass_t {
    QByteArrayData data[12];
    char stringdata[176];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_craneEmulatorClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_craneEmulatorClass_t qt_meta_stringdata_craneEmulatorClass = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 13),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 21),
QT_MOC_LITERAL(4, 56, 1),
QT_MOC_LITERAL(5, 58, 20),
QT_MOC_LITERAL(6, 79, 1),
QT_MOC_LITERAL(7, 81, 22),
QT_MOC_LITERAL(8, 104, 18),
QT_MOC_LITERAL(9, 123, 15),
QT_MOC_LITERAL(10, 139, 16),
QT_MOC_LITERAL(11, 156, 18)
    },
    "craneEmulatorClass\0mainTimerSlot\0\0"
    "onVelocitySpinBoxSlot\0v\0onStackerSpinBoxSlot\0"
    "t\0onDestackerSpinBoxSlot\0onAnimateTimerSlot\0"
    "onStepTimerSlot\0stackerTimerSlot\0"
    "destackerTimerSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_craneEmulatorClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08,
       3,    1,   55,    2, 0x08,
       5,    1,   58,    2, 0x08,
       7,    1,   61,    2, 0x08,
       8,    0,   64,    2, 0x08,
       9,    0,   65,    2, 0x08,
      10,    0,   66,    2, 0x08,
      11,    0,   67,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void craneEmulatorClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        craneEmulatorClass *_t = static_cast<craneEmulatorClass *>(_o);
        switch (_id) {
        case 0: _t->mainTimerSlot(); break;
        case 1: _t->onVelocitySpinBoxSlot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->onStackerSpinBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onDestackerSpinBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onAnimateTimerSlot(); break;
        case 5: _t->onStepTimerSlot(); break;
        case 6: _t->stackerTimerSlot(); break;
        case 7: _t->destackerTimerSlot(); break;
        default: ;
        }
    }
}

const QMetaObject craneEmulatorClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_craneEmulatorClass.data,
      qt_meta_data_craneEmulatorClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *craneEmulatorClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *craneEmulatorClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_craneEmulatorClass.stringdata))
        return static_cast<void*>(const_cast< craneEmulatorClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int craneEmulatorClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
