/****************************************************************************
** Meta object code from reading C++ file 'expWdg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane3/expWdg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'expWdg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_exportStationWidgetClass_t {
    QByteArrayData data[19];
    char stringdata[322];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_exportStationWidgetClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_exportStationWidgetClass_t qt_meta_stringdata_exportStationWidgetClass = {
    {
QT_MOC_LITERAL(0, 0, 24),
QT_MOC_LITERAL(1, 25, 17),
QT_MOC_LITERAL(2, 43, 0),
QT_MOC_LITERAL(3, 44, 15),
QT_MOC_LITERAL(4, 60, 9),
QT_MOC_LITERAL(5, 70, 15),
QT_MOC_LITERAL(6, 86, 20),
QT_MOC_LITERAL(7, 107, 17),
QT_MOC_LITERAL(8, 125, 31),
QT_MOC_LITERAL(9, 157, 16),
QT_MOC_LITERAL(10, 174, 8),
QT_MOC_LITERAL(11, 183, 10),
QT_MOC_LITERAL(12, 194, 22),
QT_MOC_LITERAL(13, 217, 24),
QT_MOC_LITERAL(14, 242, 23),
QT_MOC_LITERAL(15, 266, 26),
QT_MOC_LITERAL(16, 293, 11),
QT_MOC_LITERAL(17, 305, 13),
QT_MOC_LITERAL(18, 319, 1)
    },
    "exportStationWidgetClass\0listChangedSignal\0"
    "\0setToolBoxClass\0QToolBox*\0parrentToolBox_\0"
    "parrentToolBoxIndex_\0dbUpdateTimerSlot\0"
    "exportsListSelectionChangedSlot\0"
    "QListWidgetItem*\0selected\0deselected\0"
    "basketUpBtnClickedSlot\0basketDownBtnClickedSlot\0"
    "basketAddBtnClickedSlot\0"
    "basketRemoveBtnClickedSlot\0resizeEvent\0"
    "QResizeEvent*\0e\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_exportStationWidgetClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    2,   60,    2, 0x0a,
       7,    0,   65,    2, 0x0a,
       8,    2,   66,    2, 0x0a,
      12,    0,   71,    2, 0x0a,
      13,    0,   72,    2, 0x0a,
      14,    0,   73,    2, 0x0a,
      15,    0,   74,    2, 0x0a,
      16,    1,   75,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,

       0        // eod
};

void exportStationWidgetClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        exportStationWidgetClass *_t = static_cast<exportStationWidgetClass *>(_o);
        switch (_id) {
        case 0: _t->listChangedSignal(); break;
        case 1: _t->setToolBoxClass((*reinterpret_cast< QToolBox*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->dbUpdateTimerSlot(); break;
        case 3: _t->exportsListSelectionChangedSlot((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 4: _t->basketUpBtnClickedSlot(); break;
        case 5: _t->basketDownBtnClickedSlot(); break;
        case 6: _t->basketAddBtnClickedSlot(); break;
        case 7: _t->basketRemoveBtnClickedSlot(); break;
        case 8: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (exportStationWidgetClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&exportStationWidgetClass::listChangedSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject exportStationWidgetClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_exportStationWidgetClass.data,
      qt_meta_data_exportStationWidgetClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *exportStationWidgetClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *exportStationWidgetClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_exportStationWidgetClass.stringdata))
        return static_cast<void*>(const_cast< exportStationWidgetClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int exportStationWidgetClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void exportStationWidgetClass::listChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
