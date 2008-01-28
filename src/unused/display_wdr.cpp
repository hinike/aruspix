//------------------------------------------------------------------------------
// Source code generated by wxDesigner from file: display.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "display_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "display_wdr.h"

#include <wx/intl.h>

// Euro sign hack of the year
#if wxUSE_UNICODE
    #define __WDR_EURO__ wxT("\u20ac")
#else
    #if defined(__WXMAC__)
        #define __WDR_EURO__ wxT("\xdb")
    #elif defined(__WXMSW__)
        #define __WDR_EURO__ wxT("\x80")
    #else
        #define __WDR_EURO__ wxT("\xa4")
    #endif
#endif

// Implement window functions

wxSizer *WindowFunc3( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );
    item0->AddGrowableRow( 0 );

    wxSplitterWindow *item1 = new wxSplitterWindow( parent, ID3_SPLITTER1, wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item1, 0, wxGROW|wxALL, 0 );

    if (set_sizer)
    {
        parent->SetSizer( item0 );
        if (call_fit)
            item0->SetSizeHints( parent );
    }
    
    return item0;
}

// Implement menubar functions

wxMenuBar *MenuBarFunc3()
{
    wxMenuBar *item0 = new wxMenuBar;
    
    wxMenu* item1 = new wxMenu;
    item1->AppendSeparator();
    item1->Append( ID3_OPEN1, _("Open source &1"), _("Open the file of the source 1") );
    item1->Append( ID3_OPEN2, _("Open source &2"), _("Open the file of the source 2") );
    item0->Append( item1, _("commun1") );
    
    wxMenu* item2 = new wxMenu;
    item0->Append( item2, _("commun2") );
    
    wxMenu* item3 = new wxMenu;
    item0->Append( item3, _("&Tools") );
    
    return item0;
}

// Implement toolbar functions

void ToolBarFunc3( wxToolBar *parent )
{
    parent->SetMargins( 2, 2 );
    
    
    parent->Realize();
}

// Implement bitmap functions

wxBitmap BitmapsFunc3( size_t index )
{
    return wxNullBitmap;
}


// End of generated file