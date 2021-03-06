/////////////////////////////////////////////////////////////////////////////
// Name:        musio.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __MUS_IO_H__
#define __MUS_IO_H__

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/wfstream.h"

class MusDoc;
class MusPage;
class MusSystem;
class MusStaff;
class MusLayer;
class MusLayerElement;
// app
class MusLayerApp;
class MusLayerRdg;

//----------------------------------------------------------------------------
// MusFileOutputStream
//----------------------------------------------------------------------------

/** 
 * This class is a base class for file output stream classes.
 * It is not an abstract class but should not be instanciate directly.
 */ 
class MusFileOutputStream: public wxFileOutputStream
{
public:
    // constructors and destructors
    MusFileOutputStream( MusDoc *doc, wxString filename );
	MusFileOutputStream( MusDoc *doc, int fd );
    MusFileOutputStream() {};
	//MusFileOutputStream( MusDoc *file, wxFile *wxfile );
    virtual ~MusFileOutputStream();
    
    virtual bool ExportFile( ) { return true; }
    
    /** @name Writing element methods
     * The following methods actually write the elements.
     * They must be overriden in the child classes.
     * The children of the elements do not have to be writen from theses methods.
     * This actually happen in the Save method of the MusObject classes
     */
    ///@{
    virtual bool WriteDoc( MusDoc *doc ) { return true; };
    virtual bool WritePage( MusPage *page ) { return true; };
    virtual bool WriteSystem( MusSystem *system ) { return true; }; 
    virtual bool WriteStaff( MusStaff *staff ) { return true; };
    virtual bool WriteLayer( MusLayer *layer ) { return true; };
    virtual bool WriteLayerElement( MusLayerElement *element ) { return true; };
    // app
    virtual bool WriteLayerApp( MusLayerApp *app ) { return true; };
    virtual bool WriteLayerRdg( MusLayerRdg *rdg ) { return true; };
    ///@}

    /** @name Closing element methods
     * The following methods can be used to perform actions when the end of an element is reached.
     * It is usually not necessary and most of the following methods are not overriden.
     */    
    ///@{ 
    virtual bool EndPage( MusPage *page ) { return true; };
    virtual bool EndSystem( MusSystem *system ) { return true; };
    virtual bool EndStaff( MusStaff *staff ) { return true; };
    virtual bool EndLayer( MusLayer *layer ) { return true; };
    virtual bool EndLayerElement( MusLayerElement *element ) { return true; }; 
    // app
    virtual bool EndLayerApp( MusLayerApp *app ) { return true; };
    virtual bool EndLayerRdg( MusLayerRdg *rdg ) { return true; };
    ///@}
    
public:
    
protected:
    MusDoc *m_doc;
    
private:
    
};

//----------------------------------------------------------------------------
// MusFileInputStream
//----------------------------------------------------------------------------

/** 
 * This class is a base class for file input stream classes.
 * It is not an abstract class but should not be instanciate directly.
 */ 
class MusFileInputStream: public wxFileInputStream
{
public:
    // constructors and destructors
    MusFileInputStream( MusDoc *doc, wxString filename );
    MusFileInputStream( MusDoc *doc, int fr );
    MusFileInputStream() {};
    virtual ~MusFileInputStream();
    
    // read
    bool ImportFile( ) { return true; }
    
public:
    
protected:
    MusDoc *m_doc;
    
};

#endif
