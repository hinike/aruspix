/////////////////////////////////////////////////////////////////////////////
// Name:        supim.cpp
// Author:      Laurent Pugin
// Created:     2004
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifdef AX_SUPERIMPOSITION

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include <algorithm>
using std::min;
using std::max;

#include "supim.h"
#include "sup.h"
#include "supfile.h"

#include "app/axapp.h"
//#include "app/axframe.h"

// IMLIB
#include <im.h>
#include <im_image.h>
#include <im_convert.h>
#include <im_process.h>
#include <im_util.h>
#include <im_binfile.h>
#include <im_math_op.h>



//----------------------------------------------------------------------------
// SupImSrcWindow
//----------------------------------------------------------------------------

IMPLEMENT_CLASS(SupImSrcWindow, AxScrolledWindow)


BEGIN_EVENT_TABLE(SupImSrcWindow,AxScrolledWindow)
END_EVENT_TABLE()

SupImSrcWindow::SupImSrcWindow( AxImageController *parent, wxWindowID id,
    const wxPoint &position, const wxSize& size, long style ) :
    AxScrolledWindow( parent, id, position, size, style )
{
    m_circleCenter = wxPoint(0,0);
}

SupImSrcWindow::SupImSrcWindow()
{
}

SupImSrcWindow::~SupImSrcWindow()
{
}

void SupImSrcWindow::SetCirclePen( const wxPen *pen, int width )
{
    m_pen = *pen;
    m_pen.SetWidth( width );
}

void SupImSrcWindow::DrawCircle( )
{
    wxASSERT_MSG(m_bitmap,"Bitmap cannot be NULL");
    wxASSERT_MSG(m_bufferBitmap,"Buffer bitmap cannot be NULL");

    // calculate scroll position
    int scrollX, scrollY;
    this->GetViewStart(&scrollX,&scrollY);
    int unitX, unitY;
    this->GetScrollPixelsPerUnit(&unitX,&unitY);
    scrollX *= unitX;
    scrollY *= unitY;

    wxMemoryDC memDC;
    memDC.SelectObject(*m_bufferBitmap);
    
    wxMemoryDC bufDC;
    bufDC.SelectObject(*m_bitmap);
    bufDC.SetBrush( *wxTRANSPARENT_BRUSH );
    bufDC.SetPen( m_pen );

    int clientX, clientY;
    this->GetClientSize( &clientX, &clientY );

    bufDC.Blit(scrollX, scrollY, clientX, clientY,
        &memDC, scrollX, scrollY );

    memDC.SelectObject( wxNullBitmap );
    
    int radius = 40; 
    bufDC.DrawCircle( m_circleCenter.x, m_circleCenter.y, radius);

    wxClientDC dc(this);
    dc.Blit(0, 0, clientX, clientY,
        &bufDC, scrollX, scrollY);

    bufDC.SelectObject( wxNullBitmap );
}

void SupImSrcWindow::ScrollSource( double x, double y )
{
    // calculate scroll position
    int scrollX = (int)(x * m_scale);
    int scrollY = (int)(y * m_scale);

    int clientX, clientY;
    this->GetClientSize( &clientX, &clientY );

    m_circleCenter.x = scrollX;
    m_circleCenter.y = scrollY;

    scrollX -= clientX / 2;
    scrollY -= clientY / 2;
    
    int unitX, unitY;
    this->GetScrollPixelsPerUnit( &unitX,&unitY );

    if ( unitX )
        scrollX /= unitX;
    if ( unitY )
        scrollY /= unitY;

    scrollX = ( scrollX < 0 ) ? 0 : scrollX;
    scrollY = ( scrollY < 0 ) ? 0 : scrollY;

    this->Scroll( scrollX, scrollY );
}



//----------------------------------------------------------------------------
// SupImWindow
//----------------------------------------------------------------------------

IMPLEMENT_CLASS(SupImWindow, AxScrolledWindow)


BEGIN_EVENT_TABLE(SupImWindow,AxScrolledWindow)
    EVT_PAINT( SupImWindow::OnPaint )
    EVT_MOUSE_EVENTS( SupImWindow::OnMouse )
END_EVENT_TABLE()

SupImWindow::SupImWindow( AxImageController *parent, wxWindowID id,
    const wxPoint &position, const wxSize& size, long style ) :
    AxScrolledWindow( parent, id, position, size, style )
{
}

SupImWindow::SupImWindow()
{
}

SupImWindow::~SupImWindow()
{
}

void SupImWindow::SynchronizeScroll( int x, int y )
{

    wxASSERT_MSG( m_imControlPtr,"Image controller cannot be NULL" );
    wxClassInfo *info;
    info = m_imControlPtr->GetClassInfo();
    wxASSERT_MSG( info,"Class info cannot be NULL" );
    wxASSERT_MSG( info->IsKindOf( CLASSINFO( SupImController ) ),
        "Controller must be a SupImController");

    SupImController *controller = (SupImController*)m_imControlPtr;

    // calculate scroll position
    int scrollX, scrollY;
    this->GetViewStart(&scrollX,&scrollY);
    int unitX, unitY;
    this->GetScrollPixelsPerUnit(&unitX,&unitY);
    scrollX *= unitX;
    scrollY *= unitY;

    double absX = (double)(x + scrollX) / m_scale;
    double absY = (double)(y + scrollY) / m_scale;
        
    controller->ScrollSources( absX, absY );
    this->SetFocus();
}

void SupImWindow::OnPaint(wxPaintEvent &event)
{
    if (m_bitmap && m_bitmap->IsOk() ) {
        wxMemoryDC dc;
        dc.SelectObject(*m_bitmap);
        
        // draw points
        SupImController *controller = (SupImController*)m_imControlPtr;
        
        if ( controller->m_supFilePtr->m_hasManualPoints1  && !controller->m_supFilePtr->IsSuperimposed() ) {
            
            wxPen pen;
            pen.SetWidth( max( ToZoomedRender( 4 ), 1 ) );
            imPoint *points;
            if ( controller->GetId() == ID2_CONTROLLER1) {
                points = controller->m_supFilePtr->m_points1;
                pen.SetColour( *wxGREEN );
            }
            else {
                points = controller->m_supFilePtr->m_points2; 
                pen.SetColour( *wxRED );
            }
            
            int radius = ToZoomedRender( 10 );
            dc.SetBrush( *wxTRANSPARENT_BRUSH );
            dc.SetPen( pen  );
            dc.DrawCircle( ToZoomedRender( controller->ToRender( points[0] ) ), radius );
            dc.DrawCircle( ToZoomedRender( controller->ToRender( points[1] ) ), radius );
            dc.DrawCircle( ToZoomedRender( controller->ToRender( points[2] ) ), radius );
            dc.DrawCircle( ToZoomedRender( controller->ToRender( points[3] ) ), radius );
            dc.DrawLine( ToZoomedRender( controller->ToRender( points[0] ) ), ToZoomedRender( controller->ToRender( points[1] ) ) );
            dc.DrawLine( ToZoomedRender( controller->ToRender( points[1] ) ), ToZoomedRender( controller->ToRender( points[3] ) ) );
            dc.DrawLine( ToZoomedRender( controller->ToRender( points[3] ) ), ToZoomedRender( controller->ToRender( points[2] ) ) );  
            dc.DrawLine( ToZoomedRender( controller->ToRender( points[2] ) ), ToZoomedRender( controller->ToRender( points[0] ) ) );
            dc.SetBackground( wxNullBrush );
            //dc.DrawPolygon( 4, controller->m_supFilePtr->m_points1 );
            
        }
    }
    event.Skip();
    
}


void SupImWindow::OnMouse(wxMouseEvent &event)
{

    wxASSERT_MSG( m_imControlPtr,"Image controller cannot be NULL" );
    wxClassInfo *info;
    info = m_imControlPtr->GetClassInfo();
    wxASSERT_MSG( info,"Class info cannot be NULL" );
    wxASSERT_MSG( info->IsKindOf( CLASSINFO( SupImController ) ),
        "Controller must be a SupImController");

    SupImController *controller = (SupImController*)m_imControlPtr;


    // enter and leaving window
    if (event.GetEventType() == wxEVT_LEAVE_WINDOW)
    {
        this->SetCursor(*wxSTANDARD_CURSOR);
        event.Skip();
        return;
    }

    if (event.GetEventType() == wxEVT_ENTER_WINDOW)
    {
        if (event.m_leftDown)
            this->SetCursor( wxCURSOR_HAND );
    }

    // checking bitmap and position
    if (!m_bitmap || ( event.m_x > m_bitmap->GetWidth() ) || (event.m_y > m_bitmap->GetHeight() ) )
    {
        event.Skip();
        return;
    }


    if (event.m_middleDown)
    {
        if (!event.Dragging() && !event.m_wheelRotation)
        {
            this->SynchronizeScroll( event.m_x, event.m_y );
            wxGetApp().Yield();
            controller->DrawCircles( );
            this->SetFocus();
            event.Skip();
        }
        return;
    }
    if (event.GetEventType() == wxEVT_MIDDLE_UP)
    {        
        controller->DrawCircles( true );
        this->SetFocus();
    }

    // left up and down
    if (event.GetEventType() == wxEVT_LEFT_UP)
    {
        this->SetCursor( *wxSTANDARD_CURSOR );
    }

    if (event.GetEventType() == wxEVT_LEFT_DOWN)
    {
        this->SetCursor( wxCURSOR_HAND );
        this->SynchronizeScroll( event.m_x, event.m_y );
    }

    // dragging with left down
    if ( event.Dragging() && event.m_leftDown)
    {
        this->SynchronizeScroll( event.m_x, event.m_y );
    }
    event.Skip();
}

//----------------------------------------------------------------------------
// SupImController
//----------------------------------------------------------------------------

IMPLEMENT_CLASS(SupImController,AxImageController)


BEGIN_EVENT_TABLE(SupImController,AxImageController)
END_EVENT_TABLE()

SupImController::SupImController( wxWindow *parent, wxWindowID id,
    const wxPoint &position, const wxSize& size, long style , int flags) :
    AxImageController( parent, id, position, size, style, flags )
{
    m_redIm = NULL;
    m_greenIm = NULL;
    m_imControl1Ptr = NULL;
    m_imControl2Ptr = NULL;
    m_viewSrc1Ptr = NULL;
    m_viewSrc2Ptr = NULL;
    m_supFilePtr = NULL;

    m_redBrightness = 0;
    m_redContrast = 0;
    m_greenBrightness = 0;
    m_greenContrast = 0;
}

SupImController::SupImController()
{
}

SupImController::~SupImController()
{
    if ( m_redIm ) 
		imImageDestroy( m_redIm );
    if ( m_greenIm ) 
		imImageDestroy( m_greenIm );
}

void SupImController::SetControllers( AxImageController *controller1, AxImageController *controller2 )
{
    m_imControl1Ptr = controller1;
    m_imControl2Ptr = controller2;
}

void SupImController::SetViews( SupImSrcWindow *view1, SupImSrcWindow *view2 )
{
    m_viewSrc1Ptr = view1;
    m_viewSrc2Ptr = view2;
}

void SupImController::ResetImage( AxImage image )
{
    AxImageController::ResetImage( image );
	
	if ( !m_imControl1Ptr || !m_imControl2Ptr )
		return;

    wxGetApp().AxBeginBusyCursor();

    imImage *im1 = GetImImage( this, ( IM_RGB ) );

    if ( m_redIm ) 
		imImageDestroy( m_redIm );
    m_redIm = NULL;
    if ( m_greenIm ) 
		imImageDestroy( m_greenIm );
    m_greenIm = NULL;

    // memorisation des 2 buffers
    m_redIm = imImageCreate( im1->width, im1->height, IM_GRAY, IM_BYTE );
    memcpy( m_redIm->data[0], im1->data[0], m_redIm->size );
    m_greenIm = imImageCreate( im1->width, im1->height, IM_GRAY, IM_BYTE );
    memcpy( m_greenIm->data[0], im1->data[1], m_greenIm->size );
   
    imImageDestroy( im1 );

    wxGetApp().AxEndBusyCursor();

}

void SupImController::UpdateBrightness( )
{
    if (! m_redIm || ! m_greenIm ) 
        return;
		
	if ( !m_imControl1Ptr || !m_imControl2Ptr )
		return;

    wxASSERT_MSG(m_viewPtr,"View cannot be NULL");

    wxGetApp().AxBeginBusyCursor();
    
    int w = m_redIm->width;
    int h = m_redIm->height;
    // ajustement de brightness
    imImage *im1 = imImageCreate( w, h, IM_RGB, IM_BYTE );
    imImage *r_buf = imImageDuplicate( m_redIm );
    imImage *g_buf = imImageDuplicate( m_greenIm );
    imImage *imTmp = imImageCreate( w, h, IM_GRAY, IM_BYTE );

    float param[2] = { 0, 0 }; // %

    if ( (m_greenBrightness != 0)  || (m_greenContrast != 0))
    {   
        param[0] = 5.0 * (float)m_greenBrightness;
        param[1] = 5.0 * (float)m_greenContrast;
        imProcessToneGamut( r_buf , imTmp, IM_GAMUT_BRIGHTCONT, param);
        imProcessBitwiseOp( r_buf, g_buf, r_buf, IM_BIT_OR ); // valeurs communes doivent rester � 100%
        imProcessBitwiseOp( imTmp, r_buf, r_buf, IM_BIT_AND ); // AND entre valeurs communes et brightness ajuste
    }
    if ( (m_redBrightness != 0)  || (m_redContrast != 0))
    {        
        param[0] = 5.0 * (float)m_redBrightness;
        param[1] = 5.0 * (float)m_redContrast;
        imProcessToneGamut( g_buf , imTmp, IM_GAMUT_BRIGHTCONT, param);
        imProcessBitwiseOp( g_buf, r_buf, g_buf, IM_BIT_OR ); // valeurs communes doivent rester � 100%
        imProcessBitwiseOp( imTmp, g_buf, g_buf, IM_BIT_AND ); // AND entre valeurs communes et brightness ajuste
    }
    imProcessBitwiseOp( r_buf, g_buf, imTmp, IM_BIT_AND );

    memcpy( im1->data[0], r_buf->data[0], r_buf->size );
    memcpy( im1->data[1], g_buf->data[0], g_buf->size );
    memcpy( im1->data[2], imTmp->data[0], imTmp->size );

    imImageDestroy( r_buf );
    imImageDestroy( g_buf );
    imImageDestroy( imTmp );


    SetImImage( im1, this );

    imImageDestroy( im1 );
    m_viewPtr->UpdateView();
    wxGetApp().AxEndBusyCursor();
}

void SupImController::ScrollSources( double x, double y )
{
	if ( !m_imControl1Ptr || !m_imControl2Ptr )
		return;

    wxASSERT_MSG( m_viewSrc1Ptr, "View 1 cannot be NULL");
    wxASSERT_MSG( m_viewSrc2Ptr, "View 2 cannot be NULL");

    m_viewSrc1Ptr->ScrollSource( x, y );
    m_viewSrc2Ptr->ScrollSource( x, y );
}

void SupImController::DrawCircles( bool clear )
{
	if ( !m_imControl1Ptr || !m_imControl2Ptr )
		return;

    wxASSERT_MSG( m_viewSrc1Ptr, "View 1 cannot be NULL");
    wxASSERT_MSG( m_viewSrc2Ptr, "View 2 cannot be NULL");

    if ( !clear )
    {
        m_viewSrc1Ptr->DrawCircle( );
        m_viewSrc2Ptr->DrawCircle( );
    }
    else
    {
        m_viewSrc1Ptr->RedrawBuffer( );
        m_viewSrc2Ptr->RedrawBuffer( );
    }
}

imPoint SupImController::ToLogical( wxPoint p )
{
    wxASSERT( this->IsOk() );
    
    return imPoint( p.x, this->GetHeight() - p.y );
}

wxPoint SupImController::ToRender( imPoint p )
{
    wxASSERT( this->IsOk() );
    
    return wxPoint( p.x, this->GetHeight() - p.y );

}


void SupImController::CloseDraggingSelection(wxPoint start, wxPoint end)
{
    wxASSERT( m_supFilePtr );
    
    if ( m_supFilePtr->IsSuperimposed() ) {
        return;
    }
    
    imPoint *points;
    if ( this->GetId() == ID2_CONTROLLER1) {
        points = m_supFilePtr->m_points1;
    }
    else {
        points = m_supFilePtr->m_points2; 
    }

    if (( end.x < this->GetWidth() / 2) && ( end.y < this->GetHeight() / 2 )) {
        //wxLogDebug("top left");
        points[1] = ToLogical(end);
    }
    else if (( end.x > this->GetWidth() / 2) && ( end.y < this->GetHeight() / 2 )) {
        //wxLogDebug("top right");
        points[3] = ToLogical(end);
    }
    else if (( end.x < this->GetWidth() / 2) && ( end.y > this->GetHeight() / 2 )) {
        //wxLogDebug("bottom left");
        points[0] = ToLogical(end);
    }
    else if (( end.x > this->GetWidth() / 2) && ( end.y > this->GetHeight() / 2 )) {
        //wxLogDebug("bottom right");
        points[2] = ToLogical(end);
    }
    m_viewPtr->UpdateViewFast();
}

void SupImController::SetInitialPoints()
{
    wxASSERT( m_supFilePtr );

    imPoint *points;
    if ( this->GetId() == ID2_CONTROLLER1) {
        points = m_supFilePtr->m_points1;
    }
    else {
        points = m_supFilePtr->m_points2; 
    }
    
    if ( !this->Ok() || !this->HasFilename() )
        return;
    
    int margin = 40;
    points[0] = imPoint( margin, margin );
    points[1] = imPoint( margin, this->GetHeight() - margin );
    points[2] = imPoint( GetWidth() - margin, margin );
    points[3] = imPoint( GetWidth() - margin, this->GetHeight() - margin );
}





#endif // AX_SUPERIMPOSITION

