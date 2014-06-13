/***************************************************************************
 *   Copyright (c) 2012 Luke Parry    (l.parry@warwick.ac.uk)              *
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

#include <Gui/Command.h>

#include "TaskDlgEditMachiningSession.h"
#include "../ViewProviderMachiningSession.h"

using namespace CamGui;


//**************************************************************************
//**************************************************************************
// TaskDialog
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TaskDlgEditMachiningSession::TaskDlgEditMachiningSession(ViewProviderMachiningSession *camFeatView)
    : TaskDialog(),camFeatView(camFeatView)
{
    assert(camFeatView);
}

TaskDlgEditMachiningSession::~TaskDlgEditMachiningSession()
{

}

//==== calls from the TaskView ===============================================================

void TaskDlgEditMachiningSession::open()
{

}

void TaskDlgEditMachiningSession::clicked(int)
{
    
}

bool TaskDlgEditMachiningSession::accept()
{
    return true;
}

bool TaskDlgEditMachiningSession::reject()
{
    std::string document = getDocumentName(); // needed because resetEdit() deletes this instance
    Gui::Command::doCommand(Gui::Command::Gui,"Gui.getDocument('%s').resetEdit()", document.c_str());
    Gui::Command::doCommand(Gui::Command::Doc,"App.getDocument('%s').recompute()", document.c_str());

    return true;
}

void TaskDlgEditMachiningSession::helpRequested()
{

}

#include "TaskDialog/moc_TaskDlgEditMachiningSession.cpp"