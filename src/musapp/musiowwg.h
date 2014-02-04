/////////////////////////////////////////////////////////////////////////////
// Name:        musiowwg.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __MUS_IOWWG_H__
#define __MUS_IOWWG_H__

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/wfstream.h"

#include "vrv/doc.h"
#include "vrv/beam.h"
#include "vrv/layer.h"


#include "muswwg.h"

class vrv::Staff;
class vrv::Layer;
class vrv::Beam;

//----------------------------------------------------------------------------
// MusWWGElement
//----------------------------------------------------------------------------

/**
 * This class represents the original Wolfgang element structures.
 * It is a parent class of the MusWWGInput and MusWWGOutput classes.
 * It is used to read/write original Wolfgang elements (notes or symbols).
*/
class MusWWGElement
{
public:
    // constructors and destructors
    MusWWGElement() {};
    virtual ~MusWWGElement() {};
    
    void WWGInitElement();
    
protected:
    // WWG Page members
    char noMasqueFixe;
    char noMasqueVar;
    unsigned char reserve;
    char defin;
    
    unsigned short noGrp;
    unsigned short totGrp;
    unsigned char armTyp;
    unsigned char armNbr;
    unsigned char vertBarre;
    unsigned char brace;
    unsigned char portNbLine;
    unsigned char accol;
    unsigned char accessoire;
    unsigned char notAnc;
    
    
    // WWG Note members
    char sil;
    unsigned char val;
    char inv_val;
    unsigned char note_point;
    char stop_rel;
    unsigned char acc;
    char accInvis;
    char q_auto;
    char queue;
    char stacc;
    char oblique;
    char queue_lig;
    char chord;
    char fchord;
    char lat;
    char haste;
    unsigned char tetenot;
    unsigned char typStac;
    
    // WWG Symbol members
    unsigned char flag;
    unsigned char calte;
    unsigned char carStyle;
    unsigned char carOrient;
    unsigned char fonte;
    unsigned char s_lie_l;
    char symbol_point;
    unsigned short l_ptch;
    
    // WWG Element members
    char TYPE;
    char liaison;
    char dliai;
    char fliai;
    char lie_up;
    char rel;
    char drel;
    char frel;
    unsigned char oct;
    unsigned char dimin;
    unsigned char grp;
    unsigned char _shport;
    char ligat;
    char ElemInvisible;
    char pointInvisible;
    char existDebord;
    char fligat;
    unsigned char notschowgrp;
    char cone;
    unsigned char liaisonPointil;
    unsigned char reserve1;
    unsigned char reserve2;
    unsigned char ottava;
    unsigned short durNum;
    unsigned short durDen;
    unsigned short offset;
    unsigned int xrel;
    int dec_y;
    void* pdebord;
    unsigned short debordCode;
    unsigned short debordSize;
	unsigned short code;
    
    // WWG staff
    unsigned short ecarts[1024];
    
};

//----------------------------------------------------------------------------
// MusWWGOutput
//----------------------------------------------------------------------------

/**
 * This class is a file output stream for writing Wolfgang WWG files.
*/
class MusWWGOutput: public wxFileOutputStream, public MusWWGElement
{
public:
    // constructors and destructors
    MusWWGOutput( vrv::Doc *doc, std::string filename );
    virtual ~MusWWGOutput();
    
    bool ExportFile( );
	bool WriteFileHeader( const MusWWGData *header );
	bool WriteParametersMidi( const MusWWGData *midi );
	bool WriteParameters2( const MusWWGData *param );
	bool WriteFonts( const MusWWGData *fonts );
	bool WriteSeparator( );
	bool WritePage( const vrv::Page *page );
	bool WriteLayer( const vrv::Layer *layer, int staffNo );

	bool WritePagination( const MusWWGData *pagination );
	bool WriteHeader( const MusWWGData *header);
	bool WriteFooter( const MusWWGData *footer);
    
private:

    bool WriteNote( );
    bool WriteSymbol( );
    bool WriteElementAttr( );
    bool WriteDebord( );

    unsigned short uint16;
	signed short int16;
	unsigned int uint32;
	signed int int32;
	std::string m_filename;
    vrv::System *m_current_system; // the current system we are writing
    vrv::Staff *m_current_staff; // the current staff we are writing

protected:
    vrv::Doc *m_doc;
    MusWWGData m_wwgData;
};


//----------------------------------------------------------------------------
// MusWWGInput
//----------------------------------------------------------------------------

/**
 * This class is a file input stream for reading Wolfgang WWG files.
*/
class MusWWGInput: public wxFileInputStream, public MusWWGElement
{
public:
    // constructors and destructors
    MusWWGInput( vrv::Doc *doc, std::string filename);
    virtual ~MusWWGInput();
    
    bool ImportFile( );
	bool ReadFileHeader( MusWWGData *header );
	bool ReadParametersMidi( MusWWGData *midi );
	bool ReadParameters2( MusWWGData *param );
	bool ReadFonts( MusWWGData *fonts );
	bool ReadSeparator( );
	bool ReadPage( vrv::Page *page );
	bool ReadStaff( vrv::Staff *staff, vrv::Layer *layer, int staffNo );
	bool ReadNote( vrv::Layer *layer );
	bool ReadSymbol( vrv::Layer *layer );
	bool ReadElementAttr( );
	bool ReadDebord( );
	bool ReadPagination( MusWWGData *pagination );
	bool ReadHeader( MusWWGData *header);
	bool ReadFooter( MusWWGData *footer);
    
private:
    unsigned short uint16;
	signed short int16;
	unsigned int uint32;
	signed int int32;
    // 
    //ArrayOfMusSymbols m_lyrics;
    // for reading system from 2.0.0
    unsigned short m_noLigne;
    char m_indent;
    char m_indentDroite;
    
    //
    vrv::Beam *m_currentBeam;
    bool m_isLastNoteInBeam;
    
protected:
    vrv::Doc *m_doc;
    MusWWGData m_wwgData;
};


#endif
