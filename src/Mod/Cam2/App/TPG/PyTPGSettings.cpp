/***************************************************************************
 *   Copyright (c) 2012 Andrew Robinson <andrewjrobinson@gmail.com>        *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include <PreCompiled.h>
#ifndef _PreComp_
#endif

#include "PyTPGSettings.h"


/**
 * The python deallocator
 */
static void
PyTPGSettingDefinition_dealloc(cam_PyTPGSettings* self) {
    if (self->settings != NULL) {
        self->settings->release();
        self->settings = NULL;
    }
    self->ob_type->tp_free((PyObject*)self);
}

/**
 * The python allocator
 */
static PyObject *
PyTPGSettingDefinition_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    cam_PyTPGSettings *self;

    self = (cam_PyTPGSettings *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->settings = NULL; //new Cam::TPGSettings();
    }

    return (PyObject *)self;
}

/**
 * Python initialiser
 */
static int
PyTPGSettingDefinition_init(cam_PyTPGSettings *self, PyObject *args, PyObject *kwds) {

	self->settings = new Cam::TPGSettings();
    return 0;
}

/**
 * Add a command to the toolpath
 */
static PyObject *
PyTPGSettings_addOption (cam_PyTPGSettings* self, PyObject* args) {

    //Expects (<action>, <name>, <label>, <type>, <defaultvalue>, <unit>, <helptext>)

	const char *action;

    const char *name;
    const char *label;
    const char *type;
    const char *value;
    const char *unit;
    const char *helptext;

    if (!PyArg_ParseTuple(args, "sssssss", &action, &name, &label, &type, &value, &unit, &helptext)) {
        PyErr_SetString(PyExc_TypeError, "Expects '<action>, <name>, <label>, <type>, <defaultvalue>, <unit>, <helptext>' (7 strings)");
        return NULL;
    }

    Cam::TPGSettingDefinition *setting = new Cam::TPGSettingDefinition(name, label, type, value, unit, helptext);
    QString qaction = QString::fromAscii(action);
    self->settings->addSettingDefinition(qaction, setting);

    // release my copy of setting (TPGSettings object will grab its own copy)
    setting->release();

    //TODO: return the setting object here so user can add options.  Need to define a new PyTPGSettingDefinition type
    Py_RETURN_NONE;
}


/**
 * Method table for PyToolPath python type
 */
static PyMethodDef PyTPGSettingDefinition_methods[] = {
    {"addOption", (PyCFunction)PyTPGSettings_addOption, METH_VARARGS, "Add an option to this setting"},
//    {"getAction", (PyCFunction)PyTPGSettings_getAction, METH_NOARGS, "Get the selected action"},
//    {"setAction", (PyCFunction)PyTPGSettings_setAction, METH_VARARGS, "Set the selected action"},
//    {"getValue", (PyCFunction)PyTPGSettings_getValue, METH_VARARGS, "Get the value of a setting"},
//    {"setValue", (PyCFunction)PyTPGSettings_setValue, METH_VARARGS, "Set the value of a setting"},
//    {"addDefaults", (PyCFunction)PyTPGSettings_addDefaults, METH_NOARGS, "Adds any missing setting values (with their default value)"},
    {NULL}  /* Sentinel */
};

static PyTypeObject PyTPGSettingDefinitionType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "Cam.TPGSettingDefinition",       /*tp_name*/
    sizeof(cam_PyTPGSettingDefinition), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyTPGSettingDefinition_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,       /*tp_flags*/
    "Storage object for a collection of TPG Settings", /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyTPGSettingDefinition_methods,     /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyTPGSettingDefinition_init, /* tp_init */
    0,                         /* tp_alloc */
    PyTPGSettingDefinition_new,         /* tp_new */
};



// ----------------------------------------------------------------------------
// ----- TPGSettings ----------------------------------------------------------
// ----------------------------------------------------------------------------

// ----- Forward declarations -----

static void      PyTPGSettings_dealloc(cam_PyTPGSettings* self);
static PyObject *PyTPGSettings_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int       PyTPGSettings_init(cam_PyTPGSettings *self, PyObject *args, PyObject *kwds);
static PyObject *PyTPGSettings_addSettingDefinition (cam_PyTPGSettings* self, PyObject* args);
static PyObject *PyTPGSettings_getAction (cam_PyTPGSettings* self, PyObject* args);
static PyObject *PyTPGSettings_setAction (cam_PyTPGSettings* self, PyObject* args);
static PyObject *PyTPGSettings_getValue (cam_PyTPGSettings* self, PyObject* args);
static PyObject *PyTPGSettings_setValue (cam_PyTPGSettings* self, PyObject* args);
static PyObject *PyTPGSettings_addDefaults (cam_PyTPGSettings* self, PyObject* args);

// ----- Data structures instances -----

static PyMethodDef PyTPGSettings_methods[] = {
    {"addSettingDefinition", (PyCFunction)PyTPGSettings_addSettingDefinition, METH_VARARGS, "Add a setting to this settings collection"},
    {"getAction", (PyCFunction)PyTPGSettings_getAction, METH_NOARGS, "Get the selected action"},
    {"setAction", (PyCFunction)PyTPGSettings_setAction, METH_VARARGS, "Set the selected action"},
    {"getValue", (PyCFunction)PyTPGSettings_getValue, METH_VARARGS, "Get the value of a setting"},
    {"setValue", (PyCFunction)PyTPGSettings_setValue, METH_VARARGS, "Set the value of a setting"},
    {"addDefaults", (PyCFunction)PyTPGSettings_addDefaults, METH_NOARGS, "Adds any missing setting values (with their default value)"},
//    {"clone", (PyCFunction)PyTPGSettings_clone, METH_NOARGS, "Clones the settings collection"},
    {NULL}  /* Sentinel */
};

static PyTypeObject PyTPGSettingsType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "Cam.TPGSettings",       /*tp_name*/
    sizeof(cam_PyTPGSettings), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyTPGSettings_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,    /*tp_getattro*/ //PyTPGSettings_getattro
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,       /*tp_flags*/
    "Storage object for a collection of TPG Settings", /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyTPGSettings_methods,     /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyTPGSettings_init, /* tp_init */
    0,                         /* tp_alloc */
    PyTPGSettings_new,         /* tp_new */
};

// ----- Function implementations ----

/**
 * The python deallocator
 */
static void
PyTPGSettings_dealloc(cam_PyTPGSettings* self) {
    if (self->settings != NULL) {
        self->settings->release();
        self->settings = NULL;
    }
    self->ob_type->tp_free((PyObject*)self);
}

/**
 * The python allocator
 */
static PyObject *
PyTPGSettings_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    cam_PyTPGSettings *self;

    self = (cam_PyTPGSettings *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->settings = NULL; //new Cam::TPGSettings();
    }

    return (PyObject *)self;
}

/**
 * Python initialiser
 */
static int
PyTPGSettings_init(cam_PyTPGSettings *self, PyObject *args, PyObject *kwds) {

	// extract parameter
	PyObject *copy = NULL;
	if (!PyArg_ParseTuple(args, "|O", &copy)) {
		PyErr_SetString(PyExc_TypeError, "Expects an optional TPGSettings object (to clone)");
		return -1;
	}

	if (copy == NULL) {
		self->settings = new Cam::TPGSettings();
		return 0;
	} else {
		if (PyCamTPGSettings_Check(copy)) {
			cam_PyTPGSettings *pcopy = (cam_PyTPGSettings*) copy;
			if (pcopy->settings != NULL) {
				self->settings = pcopy->settings->clone();
			} else {
				PyErr_SetString(PyExc_Exception, "Underlying data-structure not set.");
				return -1;
			}
		} else {
			PyErr_SetString(PyExc_TypeError, "Expects an optional TPGSettings object (to clone)");
			return -1;
		}
	}
}

///**
// * Python Attribute getter
// */
//static PyObject *
//PyTPGSettings_getattro(cam_PyTPGSettings *self, PyObject *name) {
//
//    // get default attributes (such as methods)
//    PyObject *tmp;
//    if (!(tmp = PyObject_GenericGetAttr((PyObject *) self, name))) {
//        if (!PyErr_ExceptionMatches(PyExc_AttributeError))
//            return NULL;
//        PyErr_Clear();
//    } else
//        return tmp;
//
//    // otherwise try my own attributes
////    char* attrname = PyString_AsString(name);
////    QString *tmpstr = self->settings->getSettings()(attrname);
////    if (tmpstr != NULL)
////        return PyString_FromString(((const char*)(tmpstr->toStdString().c_str())));
//    Py_RETURN_NONE;
//}

/**
 * Add a command to the toolpath
 */
static PyObject *
PyTPGSettings_addSettingDefinition (cam_PyTPGSettings* self, PyObject* args) {

    //Expects (<action>, <name>, <label>, <type>, <defaultvalue>, <unit>, <helptext>)

	const char *action;

    const char *name;
    const char *label;
    const char *type;
    const char *value;
    const char *unit;
    const char *helptext;

    if (!PyArg_ParseTuple(args, "sssssss", &action, &name, &label, &type, &value, &unit, &helptext)) {
        PyErr_SetString(PyExc_TypeError, "Expects '<action>, <name>, <label>, <type>, <defaultvalue>, <unit>, <helptext>' (7 strings)");
        return NULL;
    }

    Cam::TPGSettingDefinition *setting = new Cam::TPGSettingDefinition(name, label, type, value, unit, helptext);
    QString qaction = QString::fromAscii(action);
    self->settings->addSettingDefinition(qaction, setting);

    // release my copy of setting (TPGSettings object will grab its own copy)
    setting->release();

    //TODO: return the setting object here so user can add options.  Need to define a new PyTPGSettingDefinition type
    Py_RETURN_NONE;
}

/**
 * Get selected action
 */
static PyObject *
PyTPGSettings_getAction (cam_PyTPGSettings* self, PyObject* args) {

	if (self->settings != NULL) {
		QString qaction = self->settings->getAction();
		return PyString_FromString(qaction.toAscii().constData());
	}

	PyErr_SetString(PyExc_Exception, "Underlying data-structure not set.");
	return NULL;
}

/**
 * Set selected action
 */
static PyObject *
PyTPGSettings_setAction (cam_PyTPGSettings* self, PyObject* args) {

	const char *action = NULL;

    if (!PyArg_ParseTuple(args, "s", &action)) {
		PyErr_SetString(PyExc_TypeError, "Expects '<action>' (1 string)");
		return NULL;
	}

    if (self->settings != NULL) {
    	QString qaction = QString::fromAscii(action);
    	bool result = self->settings->setAction(qaction);
		return PyBool_FromLong(result?1:0);
	}

	PyErr_SetString(PyExc_Exception, "Underlying data-structure not set.");
	return NULL;
}

/**
 * Get the value of a setting
 */
static PyObject *
PyTPGSettings_getValue (cam_PyTPGSettings* self, PyObject* args) {

	const char *action = NULL;
    const char *name = NULL;

    if (!PyArg_ParseTuple(args, "s|s", &action, &name)) {
		PyErr_SetString(PyExc_TypeError, "Expects '[<action>, ]<name>' (1 or 2 strings)");
		return NULL;
	}


    // get the value
    if (self->settings != NULL) {
        QString value;
		if (name == NULL) {
			QString qname = QString::fromAscii(action);
			value = self->settings->getValue(qname);
		} else {
			QString qname = QString::fromAscii(name);
			QString qaction = QString::fromAscii(action);
			value = self->settings->getValue(qaction, qname);
		}

	    // pack the value for return to python
	    return PyString_FromString(value.toAscii().constData());
    }

	PyErr_SetString(PyExc_Exception, "Underlying data-structure not set.");
	return NULL;
}

/**
 * Set the value of a setting
 */
static PyObject *
PyTPGSettings_setValue (cam_PyTPGSettings* self, PyObject* args) {

	const char *var1 = NULL;
    const char *var2 = NULL;
    const char *var3 = NULL;

    if (!PyArg_ParseTuple(args, "ss|s", &var1, &var2, &var3)) {
		PyErr_SetString(PyExc_TypeError, "Expects '[<action>, ]<name>, <value>' (2 or 3 strings)");
		return NULL;
	}

    // update the settings
    if (self->settings != NULL) {
    	bool result;
		if (var3 == NULL) {
			QString qname = QString::fromAscii(var1);
			QString qvalue = QString::fromAscii(var2);
			result = self->settings->setValue(qname, qvalue);
		} else {
			QString qaction = QString::fromAscii(var1);
			QString qname = QString::fromAscii(var2);
			QString qvalue = QString::fromAscii(var3);
			result = self->settings->setValue(qaction, qname, qvalue);
		}
		return PyBool_FromLong(result?1:0);
    }

	PyErr_SetString(PyExc_Exception, "Underlying data-structure not set.");
	return NULL;
}

/**
 * Adds any missing setting values (with their default value)
 */
static PyObject *
PyTPGSettings_addDefaults (cam_PyTPGSettings* self, PyObject* args) {

	if (self->settings != NULL)
		self->settings->addDefaults();

    Py_RETURN_NONE;
}

///**
// * Clones the settings collection
// */
//static PyObject *
//PyTPGSettings_clone (cam_PyTPGSettings* self, PyObject* args) {
//
//	if (self->settings != NULL) {
//		Cam::TPGSettings* sett = self->settings->clone();
//		PyObject result = PyObject_New(, &PyTPGSettingsType);
//	}
//
//	PyErr_SetString(PyExc_Exception, "Underlying data-structure not set.");
//	return NULL;
//}

/**
 * Method table for PyToolPath python type
 */
