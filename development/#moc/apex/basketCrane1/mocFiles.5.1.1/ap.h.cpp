/****************************************************************************
** Meta object code from reading C++ file 'ap.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane1/ap.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_allPositionsClass_t {
    QByteArrayData data[13];
    char stringdata[230];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_allPositionsClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_allPositionsClass_t qt_meta_stringdata_allPositionsClass = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 19),
QT_MOC_LITERAL(2, 38, 0),
QT_MOC_LITERAL(3, 39, 30),
QT_MOC_LITERAL(4, 70, 7),
QT_MOC_LITERAL(5, 78, 13),
QT_MOC_LITERAL(6, 92, 25),
QT_MOC_LITERAL(7, 118, 17),
QT_MOC_LITERAL(8, 136, 24),
QT_MOC_LITERAL(9, 161, 23),
QT_MOC_LITERAL(10, 185, 20),
QT_MOC_LITERAL(11, 206, 20),
QT_MOC_LITERAL(12, 227, 1)
    },
    "allPositionsClass\0openGlChangedSignal\0"
    "\0return160TrackingStatusChanged\0message\0"
    "mainTimerSlot\0inOvenUpdateTimeTimerSlot\0"
    "autoModeTimerSlot\0checkOvenUnloadPositions\0"
    "openGlObjectChangedSlot\0positionSelectedSlot\0"
    "basketPositionClass*\0b\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_allPositionsClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x05,
       3,    1,   55,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       5,    0,   58,    2, 0x0a,
       6,    0,   59,    2, 0x0a,
       7,    0,   60,    2, 0x0a,
       8,    0,   61,    2, 0x0a,
       9,    0,   62,    2, 0x0a,
      10,    1,   63,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

void allPositionsClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        allPositionsClass *_t = static_cast<allPositionsClass *>(_o);
        switch (_id) {
        case 0: _t->openGlChangedSignal(); break;
        case 1: _t->return160TrackingStatusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->mainTimerSlot(); break;
        case 3: _t->inOvenUpdateTimeTimerSlot(); break;
        case 4: _t->autoModeTimerSlot(); break;
        case 5: _t->checkOvenUnloadPositions(); break;
        case 6: _t->openGlObjectChangedSlot(); break;
        case 7: _t->positionSelectedSlot((*reinterpret_cast< basketPositionClass*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< basketPositionClass* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (allPositionsClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allPositionsClass::openGlChangedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (allPositionsClass::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allPositionsClass::return160TrackingStatusChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject allPositionsClass::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_allPositionsClass.data,
      qt_meta_data_allPositionsClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *allPositionsClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *allPositionsClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_allPositionsClass.stringdata))
        return static_cast<void*>(const_cast< allPositionsClass*>(this));
    return QObject::qt_metacast(_clname);
}

int allPositionsClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void allPositionsClass::openGlChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void allPositionsClass::return160TrackingStatusChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
