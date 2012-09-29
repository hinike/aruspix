/////////////////////////////////////////////////////////////////////////////
// Name:        muslaidoutlayerelement.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include "musio.h"
#include "musdoc.h"
#include "muslaidoutlayerelement.h"

#include "musbarline.h"
#include "musbeam.h"
#include "musclef.h"
#include "musmensur.h"
#include "musneume.h"
#include "musnote.h"
#include "musrest.h"
#include "mussymbol.h"
#include "mustie.h"
#include "mustuplet.h"

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY( ArrayOfMusLaidOutLayerElements );

//----------------------------------------------------------------------------
// MusLaidOutLayerElement
//----------------------------------------------------------------------------

MusLaidOutLayerElement::MusLaidOutLayerElement( MusLayerElement *element ):
	MusLayoutObject()
{
    m_layerElement = element;
    m_layer = NULL;
    m_x_abs = 0;
    m_y_drawing = 0;
    m_in_layer_app = false;
}

MusLaidOutLayerElement::~MusLaidOutLayerElement()
{
    // If the is a parent layer and it is still active (that is not being deleted)
    // we remove the element from its list
    if ( m_layer && m_layer->IsActive() ) {
        wxLogDebug("Detaching the LaidOutLayerElement" );
        m_layer->m_elements.Detach( m_layer->m_elements.Index( *this ) );
    }
}

bool MusLaidOutLayerElement::Check()
{
    wxASSERT( m_layer && m_layerElement );
    return ( m_layer && m_layerElement && MusLayoutObject::Check());
}

void MusLaidOutLayerElement::Save( wxArrayPtrVoid params )
{
    // param 0: output stream
    MusFileOutputStream *output = (MusFileOutputStream*)params[0];         
    output->WriteLaidOutLayerElement( this );
}


void MusLaidOutLayerElement::Delete( wxArrayPtrVoid params )
{
    // param 0: the MusLayerElement we point to
    MusLayerElement *element = (MusLayerElement*)params[0];   

    if ( m_layerElement == element ) {
        //wxLogMessage( "YES" );
        delete this;
    }
}

void MusLaidOutLayerElement::CheckAndResetLayerElement( wxArrayPtrVoid params )
{
    // param 0: the MusDoc to check against
    MusDoc *doc = (MusDoc*)params[0];
    
    char uuidStr[37]; // bad fix
    uuid_unparse( *m_layerElement->GetUuid(), uuidStr ); 
    
    MusFunctor findElementUuid( &MusObject::FindWithUuid );
    MusLayerElement *element = dynamic_cast<MusLayerElement*>( doc->FindLogicalObject( &findElementUuid, *m_layerElement->GetUuid() ) );
    if ( element ) {
        m_layerElement = element;
    }
    else {
        m_layerElement = NULL;
        //wxLogDebug( "Element %s not found in the logical tree", uuidStr );
        delete this;
    }
}

void MusLaidOutLayerElement::UpdateXPosition( wxArrayPtrVoid params )
{
    // param 0: the MusLayerElement we point to
	int *current_x_shift = (int*)params[0];
    
    // reset the x position if we are starting a new layer
    if ( this->m_layer->m_elements.Index( *this ) == 0 ) {
        (*current_x_shift) = 0;
    }
    
    if ( !this->HasUpdatedBB() ) {
        // this is all we need for empty elements
        m_x_abs = (*current_x_shift);
        return;
    }
    
    if ( dynamic_cast<MusBeam*>(this->m_layerElement) ) {
        return;
    }
    
    if ( dynamic_cast<MusTie*>(this->m_layerElement) ) {
        return;
    }
    
    if ( dynamic_cast<MusTuplet*>(this->m_layerElement) ) {
        return;
    }
    
    int negative_offset = this->m_x_abs - this->m_contentBB_x1;
    this->m_x_abs = (*current_x_shift) + negative_offset;
    (*current_x_shift) += (this->m_contentBB_x2 - this->m_contentBB_x1) + this->m_layerElement->GetHorizontalSpacing();
}

int MusLaidOutLayerElement::GetElementNo() const
{
    wxASSERT_MSG( m_layer, "LaidOutLayer cannot be NULL");
    
    return m_layer->m_elements.Index( *this );
}

