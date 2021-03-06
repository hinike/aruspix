/////////////////////////////////////////////////////////////////////////////
// Name:        mussymbol.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include "mussymbol.h"


//----------------------------------------------------------------------------
// MusSymbol
//----------------------------------------------------------------------------

MusSymbol::MusSymbol():
	MusLayerElement(), MusPositionInterface()
{
    Init( SYMBOL_UNDEFINED );
}

MusSymbol::MusSymbol( SymbolType type ):
	MusLayerElement(), MusPositionInterface()
{
    Init( type );
}

MusSymbol::~MusSymbol()
{
}

bool MusSymbol::operator==( MusObject& other )
{
    MusSymbol *otherSymbol = dynamic_cast<MusSymbol*>( &other );
    if ( !otherSymbol ) {
        return false;
    }
    if ( this->m_type != otherSymbol->m_type ) {
        return false;
    }
    if ( this->m_dot != otherSymbol->m_dot ) {
        return false;
    }
    if ( this->m_accid != otherSymbol->m_accid ) {
        return false;
    }
    if ( !this->HasIdenticalPositionInterface( otherSymbol ) ) {
        return false;
    }
    return true;
}

void MusSymbol::Init( SymbolType type )
{
    m_type = type; 
    // DOT
    m_dot = 0;
    // ACCID
    m_accid = 0;
}


void MusSymbol::SetValue( int value, int flag ) 
{
    Init( SYMBOL_UNDEFINED );
    if ( flag & 1 ) // control is down - alteration, double sharp or double flat
    {
		switch (value)
        {
            case ('S'): value = 'X'; break;
            case ('F'): value = 'D'; break;
        }
    }
    
    switch (value)
    {
            
		case ('S'): this->m_type = SYMBOL_ACCID; this->m_accid = ACCID_SHARP; break;
		case ('F'): this->m_type = SYMBOL_ACCID; this->m_accid = ACCID_FLAT; break;
		case ('N'): this->m_type = SYMBOL_ACCID; this->m_accid = ACCID_NATURAL; break;
		case ('X'): this->m_type = SYMBOL_ACCID; this->m_accid = ACCID_DOUBLE_SHARP; break;
		case ('D'): this->m_type = SYMBOL_ACCID; this->m_accid = ACCID_DOUBLE_FLAT; break;
            
		case ('.'): this->m_type = SYMBOL_DOT; this->m_dot = 0; break;
            
        case ('C'): this->m_type = SYMBOL_CUSTOS; break;
    }
}
