/****************************************************************************
** Meta object code from reading C++ file 'sbWarehouse.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane2ExportStation/sbWarehouse.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sbWarehouse.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sbWarehouseClass_t {
    QByteArrayData data[7];
    char stringdata[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_sbWarehouseClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_sbWarehouseClass_t qt_meta_stringdata_sbWarehouseClass = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 20),
QT_MOC_LITERAL(2, 38, 0),
QT_MOC_LITERAL(3, 39, 1),
QT_MOC_LITERAL(4, 41, 24),
QT_MOC_LITERAL(5, 66, 19),
QT_MOC_LITERAL(6, 86, 23)
    },
    "sbWarehouseClass\0sbSpinBoxChangedSlot\0"
    "\0b\0ticketSpinBoxChangedSlot\0"
    "okButtonClicledSlot\0readyReadSerialPortSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sbWarehouseClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a,
       4,    1,   37,    2, 0x0a,
       5,    0,   40,    2, 0x0a,
       6,    0,   41,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void sbWarehouseClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        sbWarehouseClass *_t = static_cast<sbWarehouseClass *>(_o);
        switch (_id) {
        case 0: _t->sbSpinBoxChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->ticketSpinBoxChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->okButtonClicledSlot(); break;
        case 3: _t->readyReadSerialPortSlot(); break;
        default: ;
        }
    }
}

const QMetaObject sbWarehouseClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sbWarehouseClass.data,
      qt_meta_data_sbWarehouseClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *sbWarehouseClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sbWarehouseClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sbWarehouseClass.stringdata))
        return static_cast<void*>(const_cast< sbWarehouseClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int sbWarehouseClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
