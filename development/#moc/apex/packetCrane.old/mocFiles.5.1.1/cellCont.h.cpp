/****************************************************************************
** Meta object code from reading C++ file 'cellCont.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/packetCrane.old/cellCont.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cellCont.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cellContentsClass_t {
    QByteArrayData data[14];
    char stringdata[311];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_cellContentsClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_cellContentsClass_t qt_meta_stringdata_cellContentsClass = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 21),
QT_MOC_LITERAL(2, 40, 0),
QT_MOC_LITERAL(3, 41, 4),
QT_MOC_LITERAL(4, 46, 26),
QT_MOC_LITERAL(5, 73, 4),
QT_MOC_LITERAL(6, 78, 30),
QT_MOC_LITERAL(7, 109, 5),
QT_MOC_LITERAL(8, 115, 38),
QT_MOC_LITERAL(9, 154, 7),
QT_MOC_LITERAL(10, 162, 39),
QT_MOC_LITERAL(11, 202, 46),
QT_MOC_LITERAL(12, 249, 46),
QT_MOC_LITERAL(13, 296, 13)
    },
    "cellContentsClass\0cellDataChangedSignal\0"
    "\0cell\0onPacketSpinBoxChangedSlot\0pack\0"
    "onLockedCheckButtonClickedSlot\0state\0"
    "onCassetteInCellRadioButtonClickedSlot\0"
    "checked\0onCassetteOnCraneRadioButtonClickedSlot\0"
    "onCassetteOnWestConveyorRadioButtonClickedSlot\0"
    "onCassetteOnEastConveyorRadioButtonClickedSlot\0"
    "onSaveBtnSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cellContentsClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       4,    1,   57,    2, 0x0a,
       6,    1,   60,    2, 0x0a,
       8,    1,   63,    2, 0x0a,
      10,    1,   66,    2, 0x0a,
      11,    1,   69,    2, 0x0a,
      12,    1,   72,    2, 0x0a,
      13,    0,   75,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,

       0        // eod
};

void cellContentsClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cellContentsClass *_t = static_cast<cellContentsClass *>(_o);
        switch (_id) {
        case 0: _t->cellDataChangedSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onPacketSpinBoxChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onLockedCheckButtonClickedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onCassetteInCellRadioButtonClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onCassetteOnCraneRadioButtonClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onCassetteOnWestConveyorRadioButtonClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->onCassetteOnEastConveyorRadioButtonClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->onSaveBtnSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (cellContentsClass::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cellContentsClass::cellDataChangedSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject cellContentsClass::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_cellContentsClass.data,
      qt_meta_data_cellContentsClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *cellContentsClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cellContentsClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cellContentsClass.stringdata))
        return static_cast<void*>(const_cast< cellContentsClass*>(this));
    return QDialog::qt_metacast(_clname);
}

int cellContentsClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void cellContentsClass::cellDataChangedSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
