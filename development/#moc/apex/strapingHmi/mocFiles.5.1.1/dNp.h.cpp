/****************************************************************************
** Meta object code from reading C++ file 'dNp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/strapingHmi/dNp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dNp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_defineNewPacketClass_t {
    QByteArrayData data[18];
    char stringdata[394];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_defineNewPacketClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_defineNewPacketClass_t qt_meta_stringdata_defineNewPacketClass = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 21),
QT_MOC_LITERAL(2, 43, 0),
QT_MOC_LITERAL(3, 44, 5),
QT_MOC_LITERAL(4, 50, 23),
QT_MOC_LITERAL(5, 74, 22),
QT_MOC_LITERAL(6, 97, 23),
QT_MOC_LITERAL(7, 121, 29),
QT_MOC_LITERAL(8, 151, 21),
QT_MOC_LITERAL(9, 173, 21),
QT_MOC_LITERAL(10, 195, 21),
QT_MOC_LITERAL(11, 217, 21),
QT_MOC_LITERAL(12, 239, 21),
QT_MOC_LITERAL(13, 261, 21),
QT_MOC_LITERAL(14, 283, 31),
QT_MOC_LITERAL(15, 315, 29),
QT_MOC_LITERAL(16, 345, 30),
QT_MOC_LITERAL(17, 376, 16)
    },
    "defineNewPacketClass\0packetCodeSpinBoxSlot\0"
    "\0value\0packetLengthSpinBoxSlot\0"
    "packetWidthSpinBoxSlot\0packetHeightSpinBoxSlot\0"
    "packetToWareHouseCheckBoxSlot\0"
    "woods2RadioButtonSlot\0woods3RadioButtonSlot\0"
    "woods4RadioButtonSlot\0woods5RadioButtonSlot\0"
    "woods6RadioButtonSlot\0woods7RadioButtonSlot\0"
    "shortTermStorageRadioButtonSlot\0"
    "midTermStorageRadioButtonSlot\0"
    "longTermStorageRadioButtonSlot\0"
    "okPushbuttonSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_defineNewPacketClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x0a,
       4,    1,   92,    2, 0x0a,
       5,    1,   95,    2, 0x0a,
       6,    1,   98,    2, 0x0a,
       7,    0,  101,    2, 0x0a,
       8,    0,  102,    2, 0x0a,
       9,    0,  103,    2, 0x0a,
      10,    0,  104,    2, 0x0a,
      11,    0,  105,    2, 0x0a,
      12,    0,  106,    2, 0x0a,
      13,    0,  107,    2, 0x0a,
      14,    0,  108,    2, 0x0a,
      15,    0,  109,    2, 0x0a,
      16,    0,  110,    2, 0x0a,
      17,    0,  111,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void defineNewPacketClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        defineNewPacketClass *_t = static_cast<defineNewPacketClass *>(_o);
        switch (_id) {
        case 0: _t->packetCodeSpinBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->packetLengthSpinBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->packetWidthSpinBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->packetHeightSpinBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->packetToWareHouseCheckBoxSlot(); break;
        case 5: _t->woods2RadioButtonSlot(); break;
        case 6: _t->woods3RadioButtonSlot(); break;
        case 7: _t->woods4RadioButtonSlot(); break;
        case 8: _t->woods5RadioButtonSlot(); break;
        case 9: _t->woods6RadioButtonSlot(); break;
        case 10: _t->woods7RadioButtonSlot(); break;
        case 11: _t->shortTermStorageRadioButtonSlot(); break;
        case 12: _t->midTermStorageRadioButtonSlot(); break;
        case 13: _t->longTermStorageRadioButtonSlot(); break;
        case 14: _t->okPushbuttonSlot(); break;
        default: ;
        }
    }
}

const QMetaObject defineNewPacketClass::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_defineNewPacketClass.data,
      qt_meta_data_defineNewPacketClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *defineNewPacketClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *defineNewPacketClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_defineNewPacketClass.stringdata))
        return static_cast<void*>(const_cast< defineNewPacketClass*>(this));
    return QDialog::qt_metacast(_clname);
}

int defineNewPacketClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
