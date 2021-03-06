/////////////////////////////////////////////////////////////////////////////
// Name:        musmensur.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __MUS_MENSUR_H__
#define __MUS_MENSUR_H__

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "muslayerelement.h"
#include "musdef.h"

//----------------------------------------------------------------------------
// MusMensur
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <mensur> element. 
 */
class MusMensur: public MusLayerElement
{
public:
    // constructors and destructors
    MusMensur();
    virtual ~MusMensur();
    
    /**
     * Comparison operator. 
     * Check if the MusLayerElement if a MusMensur and compare attributes
     */
    virtual bool operator==(MusObject& other);
    
    virtual wxString MusClassName( ) { return "MusMensur"; };
    
    /**
     * Set the value for the mensur.
     */
	virtual void SetValue( int value, int flag = 0 );
    
private:
    
public:
    /** Indicates the number of dots with the sign (max 1 for now). */
    unsigned char m_dot;
    /** Indicates the use of a meter symbol (C or C-cut) instead of a signature.
     * This is not available in the MEI <mensur> element (only in <staffdef>).
     * It was kept here because available in Wolfgang
     */
    MeterSymb m_meterSymb;
    /** Indicates the numerator of the duration ratio. */
    int m_num;
    /** Indicates the denominator of the duration ratio. */
    int m_numBase;
    /** Indicates if the sign is reversed. */
    bool m_reversed;
    /** Indicates the sign of the mensuration signature. */
    MensurSign m_sign;    
    /** Indicates the number of slashes with the sign (max 1 for now). */
    unsigned char m_slash;
    
    /** 
     * Static member for setting a value from a controller.
     * Used for example in SetValue
     */
    static int s_num;
    /** 
     * Static member for setting a value from a controller.
     * Used for examle in SetValue.
     */
    static int s_numBase;

private:
    
};


#endif
