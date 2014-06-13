/***************************************************************************
 *   Copyright (c) 2012 Luke Parry    (l.parry@warwick.ac.uk)              *
 *   Copyright (c) 2013 Andrew Robinson <andrewjrobinson@gmail.com>        *
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

#ifndef CAM_MACHININGSESSION_H
#define CAM_MACHININGSESSION_H

//#define CamExport
#include <PreCompiled.h>

#include <App/DocumentObject.h>
#include <App/PropertyContainer.h>
#include <App/PropertyLinks.h>
#include <App/PropertyFile.h>
#include <App/FeaturePython.h>

#include <boost/signals.hpp>

#include "TPGFeature.h"

typedef boost::signals::connection Connection;

namespace Cam
{
/**
 * The Cam Machining Session collects all the of the TPG features into this parent object
 * 
 */
class CamExport MachiningSession : public App::DocumentObject
{
    PROPERTY_HEADER(Cam::MachiningSession);

public:
    MachiningSession(); ///Constructor
    virtual ~MachiningSession(); ///Destructor

    /// This is only called on the creation of a new document object (must be made to intialise hierarchy
    void initialise();

    ///SLOTS
    void onDelete(const App::DocumentObject &docObj);

    /// Property
    App::PropertyLinkList TPGList;				// Stores the list of TPG's that are used

    /// recalculate the Feature
    App::DocumentObjectExecReturn *execute(void);
    short mustExecute() const; // Check if recalculation is needed

    /// returns the type name of the ViewProvider
    const char* getViewProviderName(void) const {
        return "CamGui::ViewProviderMachiningSession";
    }

    // from base class
    // virtual PyObject *getPyObject(void);
    virtual unsigned int getMemSize(void) const;
    virtual void Save(Base::Writer &/*writer*/) const;
    virtual void Restore(Base::XMLReader &/*reader*/);

    void addTPG(TPGFeature *tpg);

    void updateTPG();

protected:
    ///Connections
    Connection delObjConnection;

protected:
    /// get called by the container when a property has changed
    //     virtual void onChanged(const App::Property* /*prop*/);
    virtual void onSettingDocument();

private:

};


} //namespace Cam

#endif // CAM_MACHININGSESSION_H
