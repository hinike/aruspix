//
//  musslur.h
//  aruspix
//
//  Created by Rodolfo Zitellini on 26/06/12.
//  Copyright (c) 2012 com.aruspix.www. All rights reserved.
//

#ifndef __MUS_SLUR_H__
#define __MUS_SLUR_H__

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "musobject.h"
#include "muslayer.h"
#include "musnote.h"

//----------------------------------------------------------------------------
// MusBeam
//----------------------------------------------------------------------------

class MusSlur: public MusLayerElement
{
public:
    // constructors and destructors
    MusSlur();
    virtual ~MusSlur();
    
    virtual wxString MusClassName( ) { return "MusSlur"; };
    
    /**
     * Add an element (a note or a rest) to a beam.
     * Only MusNote or MusRest elements will be actually added to the beam.
     */
    void AddNote(MusLayerElement *element);
    
    /**
     * The array of notes or rests.
     * The beam object do not own the notes.
     */
    ArrayOfMusLayerElements m_notes; //
    
private:
    
public:
    
private:
    
};

#endif
