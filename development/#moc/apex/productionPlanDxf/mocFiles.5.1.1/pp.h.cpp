/****************************************************************************
** Meta object code from reading C++ file 'pp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/productionPlanDxf/pp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_profileProductionClass_t {
    QByteArrayData data[23];
    char stringdata[341];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_profileProductionClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_profileProductionClass_t qt_meta_stringdata_profileProductionClass = {
    {
QT_MOC_LITERAL(0, 0, 22),
QT_MOC_LITERAL(1, 23, 14),
QT_MOC_LITERAL(2, 38, 0),
QT_MOC_LITERAL(3, 39, 23),
QT_MOC_LITERAL(4, 63, 2),
QT_MOC_LITERAL(5, 66, 8),
QT_MOC_LITERAL(6, 75, 23),
QT_MOC_LITERAL(7, 99, 17),
QT_MOC_LITERAL(8, 117, 1),
QT_MOC_LITERAL(9, 119, 19),
QT_MOC_LITERAL(10, 139, 16),
QT_MOC_LITERAL(11, 156, 28),
QT_MOC_LITERAL(12, 185, 23),
QT_MOC_LITERAL(13, 209, 18),
QT_MOC_LITERAL(14, 228, 10),
QT_MOC_LITERAL(15, 239, 10),
QT_MOC_LITERAL(16, 250, 10),
QT_MOC_LITERAL(17, 261, 10),
QT_MOC_LITERAL(18, 272, 10),
QT_MOC_LITERAL(19, 283, 10),
QT_MOC_LITERAL(20, 294, 10),
QT_MOC_LITERAL(21, 305, 10),
QT_MOC_LITERAL(22, 316, 23)
    },
    "profileProductionClass\0selectedSignal\0"
    "\0profileProductionClass*\0pp\0selected\0"
    "productionChangedSignal\0lengthSpinBoxSlot\0"
    "v\0doubleLengthBoxSlot\0angleSpinBoxSlot\0"
    "interProfileSpaceSpinBoxSlot\0"
    "multiplicitySpinBoxSlot\0inCombCheckBoxSlot\0"
    "sp1BoxSlot\0sp2BoxSlot\0sp3BoxSlot\0"
    "sp4BoxSlot\0sp5BoxSlot\0sp6BoxSlot\0"
    "sp7BoxSlot\0sp8BoxSlot\0calculateSpacersBtnSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_profileProductionClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   99,    2, 0x05,
       6,    0,  104,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       7,    1,  105,    2, 0x0a,
       9,    1,  108,    2, 0x0a,
      10,    1,  111,    2, 0x0a,
      11,    1,  114,    2, 0x0a,
      12,    1,  117,    2, 0x0a,
      13,    1,  120,    2, 0x0a,
      14,    1,  123,    2, 0x0a,
      15,    1,  126,    2, 0x0a,
      16,    1,  129,    2, 0x0a,
      17,    1,  132,    2, 0x0a,
      18,    1,  135,    2, 0x0a,
      19,    1,  138,    2, 0x0a,
      20,    1,  141,    2, 0x0a,
      21,    1,  144,    2, 0x0a,
      22,    1,  147,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void profileProductionClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        profileProductionClass *_t = static_cast<profileProductionClass *>(_o);
        switch (_id) {
        case 0: _t->selectedSignal((*reinterpret_cast< profileProductionClass*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->productionChangedSignal(); break;
        case 2: _t->lengthSpinBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->doubleLengthBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->angleSpinBoxSlot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->interProfileSpaceSpinBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->multiplicitySpinBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->inCombCheckBoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->sp1BoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->sp2BoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->sp3BoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->sp4BoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->sp5BoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->sp6BoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->sp7BoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->sp8BoxSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->calculateSpacersBtnSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
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
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (profileProductionClass::*_t)(profileProductionClass * , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&profileProductionClass::selectedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (profileProductionClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&profileProductionClass::productionChangedSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject profileProductionClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_profileProductionClass.data,
      qt_meta_data_profileProductionClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *profileProductionClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *profileProductionClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_profileProductionClass.stringdata))
        return static_cast<void*>(const_cast< profileProductionClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int profileProductionClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void profileProductionClass::selectedSignal(profileProductionClass * _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void profileProductionClass::productionChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
