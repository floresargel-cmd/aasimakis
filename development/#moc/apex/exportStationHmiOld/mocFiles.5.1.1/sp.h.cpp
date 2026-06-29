/****************************************************************************
** Meta object code from reading C++ file 'sp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/exportStationHmiOld/sp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_layerClass_t {
    QByteArrayData data[8];
    char stringdata[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_layerClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_layerClass_t qt_meta_stringdata_layerClass = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 30),
QT_MOC_LITERAL(2, 42, 0),
QT_MOC_LITERAL(3, 43, 11),
QT_MOC_LITERAL(4, 55, 1),
QT_MOC_LITERAL(5, 57, 7),
QT_MOC_LITERAL(6, 65, 21),
QT_MOC_LITERAL(7, 87, 1)
    },
    "layerClass\0activeRadioButtonClickedSignal\0"
    "\0layerClass*\0b\0checked\0activeRadioButtonSlot\0"
    "v\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_layerClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       6,    1,   29,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    7,

       0        // eod
};

void layerClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        layerClass *_t = static_cast<layerClass *>(_o);
        switch (_id) {
        case 0: _t->activeRadioButtonClickedSignal((*reinterpret_cast< layerClass*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->activeRadioButtonSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< layerClass* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (layerClass::*_t)(layerClass * , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&layerClass::activeRadioButtonClickedSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject layerClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_layerClass.data,
      qt_meta_data_layerClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *layerClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *layerClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_layerClass.stringdata))
        return static_cast<void*>(const_cast< layerClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int layerClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void layerClass::activeRadioButtonClickedSignal(layerClass * _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_spacersClass_t {
    QByteArrayData data[5];
    char stringdata[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_spacersClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_spacersClass_t qt_meta_stringdata_spacersClass = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 22),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 33),
QT_MOC_LITERAL(4, 71, 11)
    },
    "spacersClass\0sendDataPushButtonSlot\0"
    "\0layerActiveRadioButtonClickedSlot\0"
    "layerClass*\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_spacersClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a,
       3,    2,   25,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Bool,    2,    2,

       0        // eod
};

void spacersClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        spacersClass *_t = static_cast<spacersClass *>(_o);
        switch (_id) {
        case 0: _t->sendDataPushButtonSlot(); break;
        case 1: _t->layerActiveRadioButtonClickedSlot((*reinterpret_cast< layerClass*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< layerClass* >(); break;
            }
            break;
        }
    }
}

const QMetaObject spacersClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_spacersClass.data,
      qt_meta_data_spacersClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *spacersClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *spacersClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_spacersClass.stringdata))
        return static_cast<void*>(const_cast< spacersClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int spacersClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
