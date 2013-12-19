/////////////////////////////////////////////////////////////////////////////
// Name:        musmeasure.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __MUS_MEASURE_H__
#define __MUS_MEASURE_H__

#include "musobject.h"

#include "musaligner.h"
#include "musbarline.h"

class MusStaff;

//----------------------------------------------------------------------------
// MusMeasure
//----------------------------------------------------------------------------

/**
 * This class represents a measure in a page-based score (MusDoc).
 * A MusMeasure is contained in a MusStaff.
 * It contains MusLayer objects.
 * For internally simplication of processing, unmeasure music is contained in one single measure object
 */
class MusMeasure: public MusDocObject
{
    
public:
    // constructors and destructors
    MusMeasure( bool measuredMusic = true, int logMeasureNb = -1 );
    virtual ~MusMeasure();
    
    virtual std::string MusClassName( ) { return "MusMeasure"; };
    
    /**
     * Return true if measured music (otherwise we have fakes measures)
     */
    bool IsMeasuredMusic() { return m_measuredMusic; };
    
    void Clear();
    
	void AddStaff( MusStaff *staff );
	
	int GetStaffCount() const { return (int)m_children.size(); };
    
    /**
     * Return the index position of the measure in its system parent
     */
    int GetMeasureNo() const { return MusObject::GetIdx(); };
    
    /**
     * @name Set and get the left and right barline types
     */
    ///@{
    BarlineType GetLeftBarlineType() const { return m_leftBarline.m_barlineType; };
    void SetLeftBarlineType( BarlineType type ) { m_leftBarline.m_barlineType = type; };
    BarlineType GetRightBarlineType() const { return m_rightBarline.m_barlineType; };
    void SetRightBarlineType( BarlineType type ) { m_rightBarline.m_barlineType = type; };
    ///@}
    
    /**
     * @name Set and get the barlines. 
     * Careful - the barlines are owned by the measure and will be destroy by it.
     * This method should be used only for acessing them (e.g., when drawing) and 
     * not for creating other measure objects.
     */
    ///@{
    MusBarline *GetLeftBarline() { return &m_leftBarline; };
    MusBarline *GetRightBarline() { return &m_rightBarline; };
    ///@}
       
    // functors
    virtual int Save( ArrayPtrVoid params );
    
	void CopyAttributes( MusMeasure *measure ); // copy all attributes but none of the elements
	//void ClearElements( MusDC *dc , MusElement *start = NULL );
    
	MusStaff *GetFirst( );
	MusStaff *GetLast( );
	MusStaff *GetNext( MusStaff *layer );
	MusStaff *GetPrevious( MusStaff *layer );
    MusStaff *GetStaffWithIdx( int staffIdx );
    
    MusStaff *GetStaffWithNo( int staffNo );
    
    int GetXRel( );
    
    int GetXRelRight( );
    
    /**
     * Align the content of a system.
     */
    virtual int Align( ArrayPtrVoid params );
    
    /**
     * Correct the X alignment once the the content of a system has been aligned and laid out.
     * Special case that redirects the functor to the MusMeasureAligner.
     */
    virtual int IntegrateBoundingBoxXShift( ArrayPtrVoid params );
    
    /**
     * Set the position of the MusAlignment.
     * Special case that redirects the functor to the MusMeasureAligner.
     */
    virtual int SetAligmentXPos( ArrayPtrVoid params );
    
    /**
     * Align the measures by adjusting the m_x_rel position looking at the MusMeasureAligner.
     * This method also moves the end position of the measure according to the barline width.
     */
    virtual int AlignMeasures( ArrayPtrVoid params );
    
    /**
     * Justify the X positions
     * Special case that redirects the functor to the MusMeasureAligner.
     */
    virtual int JustifyX( ArrayPtrVoid params );
        
public:
    /** The logical staff */
    int m_logMeasureNb;
	/**
     * The X absolute position of the measure for facsimile (transcription) encodings.
     * This is the top left position of the measure.
     */
    int m_x_abs;
    /**
     * The X relative position of the measure.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_x_rel;
	/**
     * The X drawing position of the measure.
     * It is re-computed everytime the measure is drawn and it is not stored in the file.
     */
    int m_x_drawing;
    
private:
    bool m_measuredMusic;
    
    MusMeasureAligner m_measureAligner;
    
    /**
     * @name The measure barlines (left and right) used when drawing
     */
    ///@{
    MusBarline m_leftBarline;
    MusBarline m_rightBarline;
    ///@}
};


#endif
