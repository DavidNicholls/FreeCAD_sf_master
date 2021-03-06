# -*- coding: utf8 -*-

#***************************************************************************
#*                                                                         *
#*   Copyright (c) 2009 Yorik van Havre <yorik@uncreated.net>              *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU Lesser General Public License (GPL)     *
#*   as published by the Free Software Foundation; either version 2 of     *
#*   the License, or (at your option) any later version.                   *
#*   for detail see the LICENCE text file.                                 *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU Library General Public License for more details.                  *
#*                                                                         *
#*   You should have received a copy of the GNU Library General Public     *
#*   License along with this program; if not, write to the Free Software   *
#*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
#*   USA                                                                   *
#*                                                                         *
#***************************************************************************

"FreeCAD Draft Workbench - DWG importer/exporter"

if open.__module__ == '__builtin__':
    pythonopen = open # to distinguish python built-in open function from the one declared here

def open(filename):
    "called when freecad opens a file."
    dxf = convertToDxf(filename)
    import importDXF
    doc = importDXF.open(dxf)
    return doc

def insert(filename,docname):
    "called when freecad imports a file"
    dxf = convertToDxf(filemname)
    import importDXF
    doc = importDXF.insert(dxf,docname)
    return doc
    
def export(objectslist,filename):
    "called when freecad exports a file"
    import importDXF,os,tempfile
    outdir = tempfile.mkdtemp()
    dxf = outdir + os.sep + os.path.splitext(os.path.basename(filename))[0] + ".dxf"
    importDXF.export(objectslist,dxf)
    convertToDwg(dxf,filename)
    return filename

def getTeighaConverter():
    "finds the Teigha Converter executable"
    import os,platform
    teigha = None
    if platform.system() == "Linux":
        teigha = "/usr/bin/TeighaFileConverter"
    elif platform.system() == "Windows":
        odadir = "C:\Program Files\ODA"
        if os.path.exists(odadir):
            subdirs = os.walk(odadir).next()[1]
            for sub in subdirs:
                t = odadir + os.sep + sub + os.sep + "TeighaFileConverter.exe"
                if os.path.exists(t):
                    teigha = t
    if teigha:
        if os.path.exists(teigha):
            return teigha
    return None
    
def convertToDxf(dwgfilename):
    "converts a DWG file to DXF"
    import os,tempfile
    teigha = getTeighaConverter()
    indir = os.path.dirname(dwgfilename)
    outdir = tempfile.mkdtemp()
    basename = os.path.basename(dwgfilename)
    cmdline = teigha + ' "' + indir + '" "' + outdir + '" "ACAD2010" "DXF" "0" "1" "' + basename + '"'
    print "converting " + cmdline
    os.system(cmdline)
    return outdir + os.sep + os.path.splitext(basename)[0] + ".dxf"
    
def convertToDwg(dxffilename,dwgfilename):
    "converts a DXF file to DWG"
    import os
    teigha = getTeighaConverter()
    indir = os.path.dirname(dxffilename)
    outdir = os.path.dirname(dwgfilename)
    basename = os.path.basename(dxffilename)
    cmdline = teigha + ' "' + indir + '" "' + outdir + '" "ACAD2010" "DWG" "0" "1" "' + basename + '"'
    print "converting " + cmdline
    os.system(cmdline)
    return dwgfilename
