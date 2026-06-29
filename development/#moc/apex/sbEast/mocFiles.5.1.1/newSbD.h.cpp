/****************************************************************************
** Meta object code from reading C++ file 'newSbD.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/sbEast/newSbD.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'newSbD.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_newSbPosClass_t {
    QByteArrayData data[6];
    char stringdata[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_newSbPosClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_newSbPosClass_t qt_meta_stringdata_newSbPosClass = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 18),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 25),
QT_MOC_LITERAL(4, 60, 3),
QT_MOC_LITERAL(5, 64, 29)
    },
    "newSbPosClass\0saveBtnClickedSlot\0\0"
    "sbSpinBoxValueChangedSlot\0val\0"
    "ticketSpinBoxValueChangedSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_newSbPosClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a,
       3,    1,   30,    2, 0x0a,
       5,    1,   33,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void newSbPosClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        newSbPosClass *_t = static_cast<newSbPosClass *>(_o);
        switch (_id) {
        case 0: _t->saveBtnClickedSlot(); break;
        case 1: _t->sbSpinBoxValueChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->ticketSpinBoxValueChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject newSbPosClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_newSbPosClass.data,
      qt_meta_data_newSbPosClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *newSbPosClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *newSbPosClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_newSbPosClass.stringdata))
        return static_cast<void*>(const_cast< newSbPosClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int newSbPosClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
struct qt_meta_stringdata_newSbDlgClass_t {
    QByteArrayData data[1];
    char stringdata[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_newSbDlgClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_newSbDlgClass_t qt_meta_stringdata_newSbDlgClass = {
    {
QT_MOC_LITERAL(0, 0, 13)
    },
    "newSbDlgClass\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_newSbDlgClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void newSbDlgClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject newSbDlgClass::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_newSbDlgClass.data,
      qt_meta_data_newSbDlgClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *newSbDlgClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *newSbDlgClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_newSbDlgClass.stringdata))
        return static_cast<void*>(const_cast< newSbDlgClass*>(this));
    return QDialog::qt_metacast(_clname);
}

int newSbDlgClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
