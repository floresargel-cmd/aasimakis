/****************************************************************************
** Meta object code from reading C++ file 'ovS.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../legacy/ovS.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ovS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ovenStackClass_t {
    QByteArrayData data[9];
    char stringdata[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ovenStackClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ovenStackClass_t qt_meta_stringdata_ovenStackClass = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 28),
QT_MOC_LITERAL(2, 44, 0),
QT_MOC_LITERAL(3, 45, 23),
QT_MOC_LITERAL(4, 69, 28),
QT_MOC_LITERAL(5, 98, 31),
QT_MOC_LITERAL(6, 130, 29),
QT_MOC_LITERAL(7, 160, 26),
QT_MOC_LITERAL(8, 187, 26)
    },
    "ovenStackClass\0mouseEnterProfileGroupSignal\0"
    "\0profileProductionClass*\0"
    "mouseLeaveProfileGroupSignal\0"
    "productionSequenceChangedSignal\0"
    "productionSequenceChangedSlot\0"
    "mouseEnterProfileGroupSlot\0"
    "mouseLeaveProfileGroupSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ovenStackClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x05,
       4,    1,   47,    2, 0x05,
       5,    0,   50,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       6,    0,   51,    2, 0x0a,
       7,    1,   52,    2, 0x0a,
       8,    1,   55,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void ovenStackClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ovenStackClass *_t = static_cast<ovenStackClass *>(_o);
        switch (_id) {
        case 0: _t->mouseEnterProfileGroupSignal((*reinterpret_cast< profileProductionClass*(*)>(_a[1]))); break;
        case 1: _t->mouseLeaveProfileGroupSignal((*reinterpret_cast< profileProductionClass*(*)>(_a[1]))); break;
        case 2: _t->productionSequenceChangedSignal(); break;
        case 3: _t->productionSequenceChangedSlot(); break;
        case 4: _t->mouseEnterProfileGroupSlot((*reinterpret_cast< profileProductionClass*(*)>(_a[1]))); break;
        case 5: _t->mouseLeaveProfileGroupSlot((*reinterpret_cast< profileProductionClass*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< profileProductionClass* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< profileProductionClass* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< profileProductionClass* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< profileProductionClass* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ovenStackClass::*_t)(profileProductionClass * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ovenStackClass::mouseEnterProfileGroupSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (ovenStackClass::*_t)(profileProductionClass * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ovenStackClass::mouseLeaveProfileGroupSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (ovenStackClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ovenStackClass::productionSequenceChangedSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject ovenStackClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ovenStackClass.data,
      qt_meta_data_ovenStackClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *ovenStackClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ovenStackClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ovenStackClass.stringdata))
        return static_cast<void*>(const_cast< ovenStackClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int ovenStackClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void ovenStackClass::mouseEnterProfileGroupSignal(profileProductionClass * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ovenStackClass::mouseLeaveProfileGroupSignal(profileProductionClass * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ovenStackClass::productionSequenceChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
