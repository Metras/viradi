/****************************************************************************
** Meta object code from reading C++ file 'specbox.h'
**
** Created: Mon Jul 19 20:48:53 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "specbox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'specbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SpecBox[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      29,    8,    8,    8, 0x0a,
      48,    8,    8,    8, 0x0a,
      68,   66,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SpecBox[] = {
    "SpecBox\0\0setRefLevel(double)\0"
    "setdBscale(double)\0setMarker(double)\0"
    "n\0setMarker(QwtDoublePoint)\0"
};

const QMetaObject SpecBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SpecBox,
      qt_meta_data_SpecBox, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SpecBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SpecBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SpecBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SpecBox))
        return static_cast<void*>(const_cast< SpecBox*>(this));
    return QWidget::qt_metacast(_clname);
}

int SpecBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setRefLevel((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: setdBscale((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: setMarker((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: setMarker((*reinterpret_cast< QwtDoublePoint(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
