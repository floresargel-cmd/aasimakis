/****************************************************************************
** Meta object code from reading C++ file 'ap.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane/ap.h"
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
struct qt_meta_stringdata_allPosisionsClass_t {
    QByteArrayData data[10];
    char stringdata[166];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_allPosisionsClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_allPosisionsClass_t qt_meta_stringdata_allPosisionsClass = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 19),
QT_MOC_LITERAL(2, 38, 0),
QT_MOC_LITERAL(3, 39, 13),
QT_MOC_LITERAL(4, 53, 25),
QT_MOC_LITERAL(5, 79, 17),
QT_MOC_LITERAL(6, 97, 23),
QT_MOC_LITERAL(7, 121, 20),
QT_MOC_LITERAL(8, 142, 20),
QT_MOC_LITERAL(9, 163, 1)
    },
    "allPosisionsClass\0openGlChangedSignal\0"
    "\0mainTimerSlot\0inOvenUpdateTimeTimerSlot\0"
    "autoModeTimerSlot\0openGlObjectChangedSlot\0"
    "positionSelectedSlot\0basketPositionClass*\0"
    "b\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_allPosisionsClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    0,   45,    2, 0x0a,
       4,    0,   46,    2, 0x0a,
       5,    0,   47,    2, 0x0a,
       6,    0,   48,    2, 0x0a,
       7,    1,   49,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void allPosisionsClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        allPosisionsClass *_t = static_cast<allPosisionsClass *>(_o);
        switch (_id) {
        case 0: _t->openGlChangedSignal(); break;
        case 1: _t->mainTimerSlot(); break;
        case 2: _t->inOvenUpdateTimeTimerSlot(); break;
        case 3: _t->autoModeTimerSlot(); break;
        case 4: _t->openGlObjectChangedSlot(); break;
        case 5: _t->positionSelectedSlot((*reinterpret_cast< basketPositionClass*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
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
            typedef void (allPosisionsClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allPosisionsClass::openGlChangedSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject allPosisionsClass::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_allPosisionsClass.data,
      qt_meta_data_allPosisionsClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *allPosisionsClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *allPosisionsClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_allPosisionsClass.stringdata))
        return static_cast<void*>(const_cast< allPosisionsClass*>(this));
    return QObject::qt_metacast(_clname);
}

int allPosisionsClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void allPosisionsClass::openGlChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
