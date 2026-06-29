/****************************************************************************
** Meta object code from reading C++ file 'cr.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane1/cr.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_craneClass_t {
    QByteArrayData data[9];
    char stringdata[213];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_craneClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_craneClass_t qt_meta_stringdata_craneClass = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 32),
QT_MOC_LITERAL(2, 44, 0),
QT_MOC_LITERAL(3, 45, 13),
QT_MOC_LITERAL(4, 59, 45),
QT_MOC_LITERAL(5, 105, 46),
QT_MOC_LITERAL(6, 152, 19),
QT_MOC_LITERAL(7, 172, 22),
QT_MOC_LITERAL(8, 195, 16)
    },
    "craneClass\0scrapBasketPositionChangedSignal\0"
    "\0basketTableId\0"
    "missionFromAfterStackerPositionFinishedSignal\0"
    "missionFromAfterStacker2PositionFinishedSignal\0"
    "openGlChangedSignal\0activeMissionTimerSlot\0"
    "animateTimerSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_craneClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x05,
       4,    0,   47,    2, 0x05,
       5,    0,   48,    2, 0x05,
       6,    0,   49,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       7,    0,   50,    2, 0x0a,
       8,    0,   51,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void craneClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        craneClass *_t = static_cast<craneClass *>(_o);
        switch (_id) {
        case 0: _t->scrapBasketPositionChangedSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->missionFromAfterStackerPositionFinishedSignal(); break;
        case 2: _t->missionFromAfterStacker2PositionFinishedSignal(); break;
        case 3: _t->openGlChangedSignal(); break;
        case 4: _t->activeMissionTimerSlot(); break;
        case 5: _t->animateTimerSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (craneClass::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&craneClass::scrapBasketPositionChangedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (craneClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&craneClass::missionFromAfterStackerPositionFinishedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (craneClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&craneClass::missionFromAfterStacker2PositionFinishedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (craneClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&craneClass::openGlChangedSignal)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject craneClass::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_craneClass.data,
      qt_meta_data_craneClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *craneClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *craneClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_craneClass.stringdata))
        return static_cast<void*>(const_cast< craneClass*>(this));
    return QObject::qt_metacast(_clname);
}

int craneClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void craneClass::scrapBasketPositionChangedSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void craneClass::missionFromAfterStackerPositionFinishedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void craneClass::missionFromAfterStacker2PositionFinishedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void craneClass::openGlChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
