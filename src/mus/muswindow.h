/////////////////////////////////////////////////////////////////////////////
// Name:        muswindow.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __MUS_WINDOW_H__
#define __MUS_WINDOW_H__

#ifdef __GNUG__
    #pragma interface "muswindow.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "musrc.h"

#include "mus_wdr.h"
#include "app/axundo.h"

class MusToolPanel;
class MusKeyboardEditor;

// kind of operation, maybe used in subclasses 
enum
{
	MUS_NONE = 0,
	MUS_NOTE_INSERT,
	MUS_NOTE_DEL,
	MUS_NOTE_PITCH,
	MUS_NOTE_VALUE,
	MUS_NOTE_STEM,
	MUS_NOTE_LIGATURE,
	MUS_NOTE_COLORATION,
	MUS_SYMBOL_INSERT,
	MUS_SYMBOL_DEL,
	MUS_ELEMENT_MOVE
};

// levels of undo
// save either staff, page or file depending on operation
enum
{
	MUS_UNDO_STAFF = 0,
	MUS_UNDO_PAGE,
	MUS_UNDO_FILE
};

//----------------------------------------------------------------------------
// MusWindow
//----------------------------------------------------------------------------

class MusWindow: public wxScrolledWindow, public MusRC, public AxUndo
{
public:
    // constructors and destructors
    MusWindow();
    MusWindow( wxWindow *parent, wxWindowID id = -1,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxScrolledWindowStyle, bool center = true );
	virtual ~MusWindow();
    
    // edition
    virtual void DoRefresh() { Refresh(); }
    virtual void DoResize() { Resize(); }
    virtual void DoLyricCursor( int x, int y, AxDC *dc, wxString lyric );
    virtual void DoReset();

	// copy paste
	void Copy();
	void Cut();
	void Paste();
	bool CanCopy() { return (m_currentElement != NULL); }
	bool CanCut() { return (m_currentElement != NULL); }
	bool CanPaste() { return (m_bufferElement != NULL); };
    
	// zoom
	void Zoom( bool zoomIn );
	bool CanZoom( bool zoomIn );
	void SetZoom( int percent );
    int GetZoom( ) { return 100 * m_zoomNum / m_zoomDen; };
	
	// undo
	virtual void Load( AxUndoFile *undoPtr );
    virtual void Store( AxUndoFile *undoPtr );

	// sync scroll
	virtual void OnSyncScroll( int x, int y ) {} // called when scroll synchronzation is needed

	void InitDC( wxDC *dc);

    bool CanGoto();
	void Goto( );
	// scroll
	void UpdateScroll(); // update scroll position if moving keys

	// set the tool panel ptr
	void SetToolPanel( MusToolPanel *toolpanel );
	// get the current tool
	int GetToolType( );
	void SetToolType( int type );
	void UpdatePen();
    
    void ToggleEditorMode();
	void SetEditorMode( MusEditorMode insert );
    
	/**
		Redimensionne la fenetre en fonction de la taille du parent et du zoom de la taille de la page
	  */
	void Resize( );
	/**
     Met a jour la table des fontes en fonction de nTailleFont et du zoom
	 Doit etre appelee apres changement de zoom ou de definition de page
     */
    
	/** indique si la page doit etre centree asi elle n'occupe pas toute la fenetre */
	bool m_center;

	MusElement *m_newElement;
	MusElement *m_bufferElement;
	MusElement *m_lastEditedElement;

	// keep elements statically - m_newElement will point on them
	MusNote m_note;
	MusSymbol m_symbol;
	MusNeume m_neume;
	MusNeumeSymbol m_neumesymbol;
	MusNeumeElement m_neumeeelement;

	int m_insertx;
	int m_insertcode;
	int m_insertoct;
	int m_dragging_x;
	int m_dragging_y_offset;
	bool m_has_been_dragged;
	MusToolPanel *m_toolpanel;
	int m_lyricCursor;
    
    // zoom
    int m_zoomNum;
    int m_zoomDen;
	
    static int s_flats[];
    static int s_sharps[];
    
private:
	void SyncToolPanel(); // synchronize tool dlg (m_currentElement, mode ... )

	bool KeyboardEntry(wxKeyEvent &event);
    void LyricEntry( wxKeyEvent &event );
    
    void OnSize( wxSizeEvent &event );
    void OnPaint( wxPaintEvent &event );
	void OnMouseMotion( wxMouseEvent &event );
	void OnMouseLeftDown( wxMouseEvent &event );
	void OnMouseLeftUp( wxMouseEvent &event );
	void OnMouseDClick( wxMouseEvent &event );
	void OnMouseLeave( wxMouseEvent &event );
	void OnKeyUp( wxKeyEvent &event );
	void OnKeyDown( wxKeyEvent &event );
    void OnChar( wxKeyEvent &event );
	void OnPopupMenuNote( wxCommandEvent &event );
	void OnPopupMenuSymbole( wxCommandEvent &event );
    void OnMidiInput( wxCommandEvent &event );

    void SharedEditOnKeyDown(wxKeyEvent &event);
    void NeumeEditOnKeyDown(wxKeyEvent &event);
    void NeumeInsertOnKeyDown(wxKeyEvent &event);
    void MensuralEditOnKeyDown(wxKeyEvent &event);
    void MensuralInsertOnKeyDown(wxKeyEvent &event);
	void NewMeiNeumeElement(MusElement *element);
    
	wxWindow* parentwindow;
	
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(MusWindow)
};

//----------------------------------------------------------------------------
// NewNeumeDlg
//----------------------------------------------------------------------------

class NewNeumeDlg: public wxDialog
{
public:
    // constructors and destructors
    NewNeumeDlg( wxWindow *parent, wxWindowID id, const wxString &title,
			  const wxPoint& pos = wxDefaultPosition,
			  const wxSize& size = wxDefaultSize,
			  long style = wxDEFAULT_DIALOG_STYLE );
	
    wxComboBox* GetCbNeumeType() { return (wxComboBox*) FindWindow( ID7_NEUME_TYPE ); }
	wxRadioBox* GetRbVariant()  { return (wxRadioBox*) FindWindow( ID7_NEUME_VARIANT ); }
	wxSpinCtrl* GetScNbPitches()  { return (wxSpinCtrl*) FindWindow( ID7_NB_PITCHES ); }
	wxCheckBox* GetCxInclinatum() { return (wxCheckBox*) FindWindow( ID7_INCLINATUM ); }
	wxCheckBox* GetCxQuilisma() { return (wxCheckBox*) FindWindow( ID7_QUILISMA ); }
	
	void OnNeumeTypeChange(wxCommandEvent& event);
    
public:
	static int s_neumetype;
	static int s_variant;
	static int s_nbpitches;
	static bool s_inclinatum;
	static bool s_quilisma;
    
private:
	
	
private:
    DECLARE_EVENT_TABLE()
};



#endif
