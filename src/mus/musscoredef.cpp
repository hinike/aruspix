/////////////////////////////////////////////////////////////////////////////
// Name:        musscoredef.cpp
// Author:      Laurent Pugin
// Created:     2013/11/08
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "musscoredef.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <typeinfo>

//----------------------------------------------------------------------------

#include "mus.h"
#include "musclef.h"
#include "musio.h"
#include "muskeysig.h"
#include "musmensur.h"

/**
 * Define the maximum levels of staffGrp within a scoreDef
 */
#define MAX_STAFFGRP_DEPTH 5

//----------------------------------------------------------------------------
// MusScoreOrStaffDefAttrInterface
//----------------------------------------------------------------------------

MusScoreOrStaffDefAttrInterface::MusScoreOrStaffDefAttrInterface()
{
    m_clef = NULL;
    m_keySig = NULL;
    m_mensur = NULL;
}

MusScoreOrStaffDefAttrInterface::~MusScoreOrStaffDefAttrInterface()
{
    if (m_clef) {
        delete m_clef;
    }
    if (m_keySig) {
        delete m_keySig;
    }
    if (m_mensur) {
        delete m_mensur;
    }
}

MusScoreOrStaffDefAttrInterface::MusScoreOrStaffDefAttrInterface( const MusScoreOrStaffDefAttrInterface& interface )
{
    m_clef = NULL;
    m_keySig = NULL;
    m_mensur = NULL;
    this->ReplaceClef( interface.m_clef );
    this->ReplaceKeySig( interface.m_keySig );
    this->ReplaceMensur( interface.m_mensur );
}

MusScoreOrStaffDefAttrInterface& MusScoreOrStaffDefAttrInterface::operator=( const MusScoreOrStaffDefAttrInterface& interface )
{
	if ( this != &interface ) // not self assignement
	{
        m_clef = NULL;
        m_keySig = NULL;
        m_mensur = NULL;
        this->ReplaceClef( interface.m_clef );
        this->ReplaceKeySig( interface.m_keySig );
        this->ReplaceMensur( interface.m_mensur );
	}
	return *this;
}

void MusScoreOrStaffDefAttrInterface::ReplaceClef( MusClef *newClef )
{
    if ( newClef ) {
        if (m_clef) {
            delete m_clef;
        }
        m_clef = new MusClef( *newClef );
    }
}

void MusScoreOrStaffDefAttrInterface::ReplaceKeySig( MusKeySig *newKeySig )
{
    if ( newKeySig ) {
        if (m_keySig) {
            delete m_keySig;
        }
        m_keySig = new MusKeySig( *newKeySig );
    }
}

void MusScoreOrStaffDefAttrInterface::ReplaceMensur( MusMensur *newMensur )
{
    if ( newMensur ) {
        if (m_mensur) {
            delete m_mensur;
        }
        m_mensur = new MusMensur( *newMensur );
    }
}

//----------------------------------------------------------------------------
// MusScoreDef
//----------------------------------------------------------------------------

MusScoreDef::MusScoreDef() :
	MusObject("scoredef-"), MusScoreOrStaffDefAttrInterface(), MusObjectListInterface()
{
}

MusScoreDef::~MusScoreDef()
{
    
}

void MusScoreDef::Clear()
{
    ReplaceClef(NULL);
    ReplaceKeySig(NULL);
    ReplaceMensur(NULL);
    ClearChildren();
}

void MusScoreDef::AddStaffGrp( MusStaffGrp *staffGrp )
{
    assert( m_children.empty() );
	staffGrp->SetParent( this );
	m_children.push_back( staffGrp );
    Modify();
}

void MusScoreDef::Replace( MusScoreDef *newScoreDef )
{
    ReplaceClef( newScoreDef->m_clef );
    ReplaceKeySig( newScoreDef->m_keySig );
    ReplaceMensur( newScoreDef->m_mensur );
    
    ArrayPtrVoid params;
	params.push_back( this );
    MusFunctor replaceStaffDefsInScoreDef( &MusObject::ReplaceStaffDefsInScoreDef );
    newScoreDef->Process( &replaceStaffDefsInScoreDef, params );
}

void MusScoreDef::Replace( MusStaffDef *newStaffDef )
{
    // first find the staffDef with the same @n
    MusStaffDef *staffDef = this->GetStaffDef( newStaffDef->GetStaffNo() );
    
    // if found, replace attributes
    if (staffDef) {
        staffDef->ReplaceClef( newStaffDef->GetClefAttr() );
        staffDef->ReplaceKeySig( newStaffDef->GetKeySigAttr() );
        staffDef->ReplaceMensur( newStaffDef->GetMensurAttr() );
    }
}

void MusScoreDef::FilterList()
{
    // We want to keep only staffDef
    ListOfMusObjects::iterator iter = m_list.begin();
    
    while ( iter != m_list.end()) {
        MusStaffDef *currentStaffDef = dynamic_cast<MusStaffDef*>(*iter);
        if ( !currentStaffDef )
        {
            iter = m_list.erase( iter );
        } else {
            iter++;
        }
    }
}


MusStaffDef *MusScoreDef::GetStaffDef( int n )
{
    MusStaffDef *staffDef = NULL;
    
    this->ResetList( this );
    ListOfMusObjects::iterator iter;
    int i;
    for (iter = m_list.begin(), i = 0; iter != m_list.end(); ++iter, i++)
    {
        staffDef = dynamic_cast<MusStaffDef*>(*iter);
        if (staffDef && (staffDef->GetStaffNo() == n) ) {
            return staffDef;
        }
    }
    
    return staffDef;
}


void MusScoreDef::SetRedraw( bool clef, bool keysig, bool mensur )
{
    ArrayPtrVoid params;
	params.push_back( &clef );
    params.push_back( &keysig );
	params.push_back( &mensur );
    MusFunctor setStaffDefDraw( &MusObject::SetStaffDefDraw );
    this->Process( &setStaffDefDraw, params );
}

//----------------------------------------------------------------------------
// MusStaffGrp
//----------------------------------------------------------------------------

MusStaffGrp::MusStaffGrp() :
    MusObject(), MusObjectListInterface()
{
    m_symbol = STAFFGRP_NONE;
    m_barthru = false;
}

MusStaffGrp::~MusStaffGrp()
{
}

void MusStaffGrp::AddStaffDef( MusStaffDef *staffDef )
{
	staffDef->SetParent( this );
	m_children.push_back( staffDef );
    Modify();
}

void MusStaffGrp::AddStaffGrp( MusStaffGrp *staffGrp )
{
	staffGrp->SetParent( this );
	m_children.push_back( staffGrp );
    Modify();
}

int MusStaffGrp::Save( ArrayPtrVoid params )
{
    // param 0: output stream
    MusFileOutputStream *output = (MusFileOutputStream*)params[0];
    if (!output->WriteStaffGrp( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
    
}


void MusStaffGrp::FilterList()
{
    // We want to keep only staffDef
    ListOfMusObjects::iterator iter = m_list.begin();
    
    while ( iter != m_list.end()) {
        MusStaffDef *currentStaffDef = dynamic_cast<MusStaffDef*>(*iter);
        if ( !currentStaffDef )
        {
            iter = m_list.erase( iter );
        } else {
            iter++;
        }
    }
}

//----------------------------------------------------------------------------
// MusStaffDef
//----------------------------------------------------------------------------

MusStaffDef::MusStaffDef() :
    MusObject(), MusScoreOrStaffDefAttrInterface()
{
}

MusStaffDef::~MusStaffDef()
{
}

int MusStaffDef::Save( ArrayPtrVoid params )
{
    // param 0: output stream
    MusFileOutputStream *output = (MusFileOutputStream*)params[0];
    if (!output->WriteStaffDef( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
    
}

//----------------------------------------------------------------------------
// MusStaffDef functor methods
//----------------------------------------------------------------------------

int MusStaffDef::ReplaceStaffDefsInScoreDef( ArrayPtrVoid params )
{
    // param 0: the scoreDef
    MusScoreDef *scoreDef = (MusScoreDef*)params[0];
    
    scoreDef->Replace( this );
    
    return FUNCTOR_CONTINUE;
}

int MusStaffDef::SetStaffDefDraw( ArrayPtrVoid params )
{
    // param 0: bool clef flag
    // param 1: bool keysig flag
    // param 2: bool the mensur flag
    bool *clef = (bool*)params[0];
    bool *keysig = (bool*)params[1];
    bool *mensur = (bool*)params[2];
    
    if ( (*clef) ) {
        this->SetDrawClef( true );
    }
    if ( (*keysig) ) {
        this->SetDrawKeySig( true );
    }
    if ( (*mensur) ) {
        this->SetDrawMensur( true );
    }
    
    return FUNCTOR_CONTINUE;
}
