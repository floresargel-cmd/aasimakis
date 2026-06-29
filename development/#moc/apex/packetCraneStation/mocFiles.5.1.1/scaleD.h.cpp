/****************************************************************************
** Meta object code from reading C++ file 'scaleD.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/packetCraneStation/scaleD.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scaleD.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_scaleDlgClass_t {
    QByteArrayData data[14];
    char stringdata[274];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_scaleDlgClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_scaleDlgClass_t qt_meta_stringdata_scaleDlgClass = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 28),
QT_MOC_LITERAL(2, 43, 0),
QT_MOC_LITERAL(3, 44, 26),
QT_MOC_LITERAL(4, 71, 13),
QT_MOC_LITERAL(5, 85, 23),
QT_MOC_LITERAL(6, 109, 18),
QT_MOC_LITERAL(7, 128, 29),
QT_MOC_LITERAL(8, 158, 23),
QT_MOC_LITERAL(9, 182, 13),
QT_MOC_LITERAL(10, 196, 23),
QT_MOC_LITERAL(11, 220, 17),
QT_MOC_LITERAL(12, 238, 28),
QT_MOC_LITERAL(13, 267, 5)
    },
    "scaleDlgClass\0currentCassetteChangedSignal\0"
    "\0currentWeightChangedSignal\0mainTimerSlot\0"
    "checkScaleZeroTimerSlot\0getWeightTimerSlot\0"
    "resetScaleActionTriggeredSlot\0"
    "readyReadSerialPortSlot\0readTimerSlot\0"
    "checkScaleIsOnTimerSlot\0errorOccurredSlot\0"
    "QSerialPort::SerialPortError\0error\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_scaleDlgClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x05,
       3,    0,   65,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       4,    0,   66,    2, 0x0a,
       5,    0,   67,    2, 0x0a,
       6,    0,   68,    2, 0x0a,
       7,    0,   69,    2, 0x0a,
       8,    0,   70,    2, 0x0a,
       9,    0,   71,    2, 0x0a,
      10,    0,   72,    2, 0x0a,
      11,    1,   73,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,

       0        // eod
};

void scaleDlgClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        scaleDlgClass *_t = static_cast<scaleDlgClass *>(_o);
        switch (_id) {
        case 0: _t->currentCassetteChangedSignal(); break;
        case 1: _t->currentWeightChangedSignal(); break;
        case 2: _t->mainTimerSlot(); break;
        case 3: _t->checkScaleZeroTimerSlot(); break;
        case 4: _t->getWeightTimerSlot(); break;
        case 5: _t->resetScaleActionTriggeredSlot(); break;
        case 6: _t->readyReadSerialPortSlot(); break;
        case 7: _t->readTimerSlot(); break;
        case 8: _t->checkScaleIsOnTimerSlot(); break;
        case 9: _t->errorOccurredSlot((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (scaleDlgClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&scaleDlgClass::currentCassetteChangedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (scaleDlgClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&scaleDlgClass::currentWeightChangedSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject scaleDlgClass::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_scaleDlgClass.data,
      qt_meta_data_scaleDlgClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *scaleDlgClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaleDlgClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_scaleDlgClass.stringdata))
        return static_cast<void*>(const_cast< scaleDlgClass*>(this));
    return QDialog::qt_metacast(_clname);
}

int scaleDlgClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void scaleDlgClass::currentCassetteChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void scaleDlgClass::currentWeightChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
