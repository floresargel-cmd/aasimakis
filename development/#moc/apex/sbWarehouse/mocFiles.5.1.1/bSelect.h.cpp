/****************************************************************************
** Meta object code from reading C++ file 'bSelect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/sbWarehouse/bSelect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bSelect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cassetteSelectClass_t {
    QByteArrayData data[9];
    char stringdata[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_cassetteSelectClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_cassetteSelectClass_t qt_meta_stringdata_cassetteSelectClass = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 14),
QT_MOC_LITERAL(2, 35, 0),
QT_MOC_LITERAL(3, 36, 20),
QT_MOC_LITERAL(4, 57, 16),
QT_MOC_LITERAL(5, 74, 8),
QT_MOC_LITERAL(6, 83, 10),
QT_MOC_LITERAL(7, 94, 21),
QT_MOC_LITERAL(8, 116, 15)
    },
    "cassetteSelectClass\0updateListSlot\0\0"
    "listSelectionChanged\0QListWidgetItem*\0"
    "selected\0deselected\0filterTextChangedSlot\0"
    "addSelectedSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cassetteSelectClass[] = {

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
       1,    0,   34,    2, 0x0a,
       3,    2,   35,    2, 0x0a,
       7,    1,   40,    2, 0x0a,
       8,    0,   43,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 4,    5,    6,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

       0        // eod
};

void cassetteSelectClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cassetteSelectClass *_t = static_cast<cassetteSelectClass *>(_o);
        switch (_id) {
        case 0: _t->updateListSlot(); break;
        case 1: _t->listSelectionChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 2: _t->filterTextChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->addSelectedSlot(); break;
        default: ;
        }
    }
}

const QMetaObject cassetteSelectClass::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_cassetteSelectClass.data,
      qt_meta_data_cassetteSelectClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *cassetteSelectClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cassetteSelectClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cassetteSelectClass.stringdata))
        return static_cast<void*>(const_cast< cassetteSelectClass*>(this));
    return QDialog::qt_metacast(_clname);
}

int cassetteSelectClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
