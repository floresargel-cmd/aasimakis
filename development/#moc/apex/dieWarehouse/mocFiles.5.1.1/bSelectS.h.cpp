/****************************************************************************
** Meta object code from reading C++ file 'bSelectS.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/dieWarehouse/bSelectS.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bSelectS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_basketSelectForScheduleClass_t {
    QByteArrayData data[8];
    char stringdata[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_basketSelectForScheduleClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_basketSelectForScheduleClass_t qt_meta_stringdata_basketSelectForScheduleClass = {
    {
QT_MOC_LITERAL(0, 0, 28),
QT_MOC_LITERAL(1, 29, 25),
QT_MOC_LITERAL(2, 55, 0),
QT_MOC_LITERAL(3, 56, 16),
QT_MOC_LITERAL(4, 73, 8),
QT_MOC_LITERAL(5, 82, 10),
QT_MOC_LITERAL(6, 93, 22),
QT_MOC_LITERAL(7, 116, 25)
    },
    "basketSelectForScheduleClass\0"
    "shiftListSelectionChanged\0\0QListWidgetItem*\0"
    "selected\0deselected\0addCassettesButtonSlot\0"
    "addAllCassettesButtonSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_basketSelectForScheduleClass[] = {

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
       1,    2,   29,    2, 0x0a,
       6,    0,   34,    2, 0x0a,
       7,    0,   35,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void basketSelectForScheduleClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        basketSelectForScheduleClass *_t = static_cast<basketSelectForScheduleClass *>(_o);
        switch (_id) {
        case 0: _t->shiftListSelectionChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 1: _t->addCassettesButtonSlot(); break;
        case 2: _t->addAllCassettesButtonSlot(); break;
        default: ;
        }
    }
}

const QMetaObject basketSelectForScheduleClass::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_basketSelectForScheduleClass.data,
      qt_meta_data_basketSelectForScheduleClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *basketSelectForScheduleClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *basketSelectForScheduleClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_basketSelectForScheduleClass.stringdata))
        return static_cast<void*>(const_cast< basketSelectForScheduleClass*>(this));
    return QDialog::qt_metacast(_clname);
}

int basketSelectForScheduleClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
