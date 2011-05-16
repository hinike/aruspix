/////////////////////////////////////////////////////////////////////////////
// Name:        musstaff.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "musstaff.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "musstaff.h"
#include "muselement.h"
#include "mussymbol.h"
#include "musnote.h"
#include "musneume.h"
#include "musneumesymbol.h"
#include "muspage.h"
#include "musrc.h"

#include <math.h>

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY( ArrayOfMusElements );

// sorting function
int SortElements(MusElement **first, MusElement **second)
{
	if ( (*first)->xrel < (*second)->xrel )
		return -1;
	else if ( (*first)->xrel > (*second)->xrel )
		return 1;
	else 
		return 0;
}


//----------------------------------------------------------------------------
// MusStaff
//----------------------------------------------------------------------------

MusStaff::MusStaff():
	MusObject()
{
	Clear( );
}

MusStaff::MusStaff( const MusStaff& staff )
{
	nblement = staff.nblement;
	voix = staff.voix;
	noGrp = staff.voix;
	totGrp = staff.totGrp;
	noLigne = staff.noLigne;
	no = staff.no;
	armTyp = staff.armTyp;
	armNbr = staff.armNbr;
	notAnc = staff.notAnc;
	grise = staff.grise;
	invisible = staff.invisible;
	ecart = staff.ecart;
	vertBarre = staff.vertBarre;
	brace = staff.brace;
	pTaille = staff.pTaille;
	indent = staff.indent; 
	indentDroite = staff.indentDroite;
	portNbLine = staff.portNbLine;
	accol = staff.accol;
	accessoire = staff.accessoire;
	reserve = staff.reserve;
	yrel = staff.yrel;
	xrel = staff.xrel;

	for (int i = 0; i < (int)staff.m_elements.GetCount(); i++)
	{
		if ( staff.m_elements[i].IsNote() )
		{
			MusNote *nnote = new MusNote( *(MusNote*)&staff.m_elements[i] );
			this->m_elements.Add( nnote );
		}
		else if ( staff.m_elements[i].IsSymbol() )
		{
			MusSymbol *nsymbol = new MusSymbol( *(MusSymbol*)&staff.m_elements[i] );
			this->m_elements.Add( nsymbol );
		}
        else if ( staff.m_elements[i].IsNeume() )
		{
			MusNeume *nneume = new MusNeume( *(MusNeume*)&staff.m_elements[i] );
			this->m_elements.Add( nneume );
		}
		else if ( staff.m_elements[i].IsNeumeSymbol() )
		{
			MusNeumeSymbol *nneumesymbol = new MusNeumeSymbol( *(MusNeumeSymbol*)&staff.m_elements[i] );
			this->m_elements.Add( nneumesymbol );
		}
	}
}

MusStaff::~MusStaff()
{
}

void MusStaff::Clear()
{
	m_elements.Clear();
	nblement = 0;
	voix = 0;
	noGrp = 0;
	totGrp = 0;
	noLigne = 0;
	no = 0;
	armTyp = 0;
	armNbr = 0;
	notAnc = true;
	grise = false;
	invisible = false;
	ecart = 10;
	vertBarre = 0;
	brace = 0;
	pTaille = 0;
	indent = 0;
	indentDroite = false;
	portNbLine = 5;
	accol = 0;
	accessoire = 0;
	reserve = 0;
	yrel = 0;
	xrel = 0;
    
    //
    beamListPremier = NULL;
}

void MusStaff::CopyAttributes( MusStaff *nstaff )
{
	if ( !nstaff )
		return;

	nstaff->Clear();
	nstaff->nblement = nblement;
	nstaff->voix = voix;
	nstaff->noGrp = noGrp;
	nstaff->totGrp = totGrp;
	nstaff->noLigne = noLigne;
	nstaff->no = no;
	nstaff->armTyp = armTyp;
	nstaff->armNbr = armNbr;
	nstaff->notAnc = notAnc;
	nstaff->grise = grise;
	nstaff->invisible = invisible;
	nstaff->ecart = ecart;
	nstaff->vertBarre = vertBarre;
	nstaff->brace = brace;
	nstaff->pTaille = pTaille;
	nstaff->indent = indent;
	nstaff->indentDroite = indentDroite;
	nstaff->portNbLine = portNbLine;
	nstaff->accol = accol;
	nstaff->accessoire = accessoire;
	nstaff->reserve = reserve;
	nstaff->yrel = yrel;
	nstaff->xrel = xrel;
}

void MusStaff::CheckIntegrity()
{
	this->m_elements.Sort( SortElements );
	this->nblement = (int)this->m_elements.GetCount();

	MusElement *element;
	int i;
    for (i = 0; i < (int)nblement; i++) 
	{
		element = &m_elements[i];
		element->no = i;
	}
}

MusElement *MusStaff::GetFirst( )
{
	if ( m_elements.IsEmpty() )
		return NULL;
	return &m_elements[0];
}

MusElement *MusStaff::GetLast( )
{
	if ( m_elements.IsEmpty() )
		return NULL;
	int i = (int)m_elements.GetCount() - 1;
	return &m_elements[i];
}

MusElement *MusStaff::GetNext( MusElement *element )
{	
	//if ( !element || m_elements.IsEmpty() )//|| ( element->no >= (int)m_elements.GetCount() - 1 ) )
    // laurent: the third condition has been comment. Why?
    if ( !element || m_elements.IsEmpty() || ( element->no >= (int)m_elements.GetCount() - 1 ) )
		return NULL;

	if ( element->no >= (int)m_elements.GetCount() - 1) return NULL;
	else return &m_elements[element->no + 1];
}

MusElement *MusStaff::GetPrevious( MusElement *element )
{
	//if ( !element || m_elements.IsEmpty() ) //|| ( element->no <= 0 ) )
    // idem - we need the third condition to be false as well
    if ( !element || m_elements.IsEmpty() || ( element->no <= 0 ) )
		return NULL;
	
	if ( element->no > (int)m_elements.GetCount() - 1) return NULL;
	else return &m_elements[element->no - 1];
}

MusElement *MusStaff::GetAtPos( int x )
{
	MusElement *element = this->GetFirst();
	if ( !element )
		return NULL;

	//int xx = 0;
//	while (this->GetNext(element) && ((int)element->xrel < x) )
//		element = this->GetNext( element );

	int dif = x - element->xrel;
	while ( this->GetNext( element ) && (int)element->xrel < x ){
		element = this->GetNext( element );
		if ( (int)element->xrel > x && dif < (int)element->xrel - x )
			return this->GetPrevious( element );
		dif = x - element->xrel;
	}
	
	return element;
}

MusElement *MusStaff::Insert( MusElement *element )
{
	if ( !element ) return NULL;

	// copy element
	if ( element->IsSymbol() )
		element = new MusSymbol( *(MusSymbol*)element );
	else if ( element->IsNote() )
		element = new MusNote( *(MusNote*)element );
	else if ( element->IsNeume() )
		element = new MusNeume( *(MusNeume*)element );
	else if ( element->IsNeumeSymbol() )
		element = new MusNeumeSymbol( *(MusNeumeSymbol*)element );
//	else if ( element->IsNeume() ) 
//	{
	//	//copying a neume causes issues
//		element = new MusNeume( *(MusNeume*)element );
//		((MusNeume*)element)->n_pitches.resize(1);
//	}

	int idx = 0;
	MusElement *tmp = this->GetFirst();
	while ( tmp && (tmp->xrel < element->xrel) )
	{
		idx++;
		if ( this->GetNext( tmp ) )
			tmp = this->GetNext( tmp );
		else
			break;
	}

	if ( tmp &&  ((element->IsSymbol() && (((MusSymbol*)element)->flag == CLE))
		|| (element->IsNeumeSymbol() && ((((MusNeumeSymbol*)element)->getValue() == NEUME_SYMB_CLEF_C) || (((MusNeumeSymbol*)element)->getValue() == NEUME_SYMB_CLEF_F)))) )
		
		m_r->OnBeginEditionClef();

	m_elements.Insert( element, idx );
	this->CheckIntegrity();
	
	if ( (element->IsSymbol() && (((MusSymbol*)element)->flag == CLE))
		|| (element->IsNeumeSymbol() && ((((MusNeumeSymbol*)element)->getValue() == NEUME_SYMB_CLEF_C) || (((MusNeumeSymbol*)element)->getValue() == NEUME_SYMB_CLEF_F))) )
		
		m_r->OnEndEditionClef();

	if (m_r)
		m_r->DoRefresh();

	return element;
}

void MusStaff::Append( MusElement *element, int step )
{
	if ( !element ) return;
    
    // insert at the end of the staff with a random step
    MusElement *last = this->GetLast();
    if (last) {
        element->xrel += last->xrel + step;
    }
    else {
        element->xrel += step;
    }
	m_elements.Add( element );
	this->CheckIntegrity();
}


void MusStaff::Delete( MusElement *element )
{
	if ( !element ) return;

	if ( m_r ) // effacement
	{
		if ( (element->IsSymbol() && (((MusSymbol*)element)->flag == CLE))
			|| (element->IsNeumeSymbol() && ((((MusNeumeSymbol*)element)->getValue() == NEUME_SYMB_CLEF_C) || (((MusNeumeSymbol*)element)->getValue() == NEUME_SYMB_CLEF_F))) )
			
			m_r->OnBeginEditionClef();
	}
	
	m_elements.Detach( element->no );
	this->CheckIntegrity();

	if ( m_r )
	{
		if ( (element->IsSymbol() && (((MusSymbol*)element)->flag == CLE))
			|| (element->IsNeumeSymbol() && ((((MusNeumeSymbol*)element)->getValue() == NEUME_SYMB_CLEF_C) || (((MusNeumeSymbol*)element)->getValue() == NEUME_SYMB_CLEF_F))) )
			m_r->OnEndEditionClef();
		
		m_r->DoRefresh();
	}
	
	delete element;
}


// Dans la direction indiquee (sens), cavale sur tout element qui n'est pas un
// symbol, de la nature indiquee (flg). Retourne le ptr si succes, ou 
// l'element de depart; le ptr succ est vrai si symb trouve. 

MusElement *MusStaff::no_note ( MusElement *chk, unsigned int sens, unsigned int flg, int *succ)
/*
	sens:	0, vers arriere; 1 avant --
	flg:	symbol->flag a chercher --
	*succ	test succes de recherche:0,echec 
*/
{	MusElement *temoin = chk;

	*succ = OFF;
	if (chk == NULL)
		return (chk);

	int i = m_elements.Index( *chk, (sens==AR) );
	if ( i == wxNOT_FOUND )
		return (chk);

	while ( (chk->TYPE != SYMB || ((MusSymbol*)chk)->flag != flg) && (chk->TYPE != NEUME_SYMB || ((MusNeumeSymbol*)chk)->getValue() != flg) )
	{
		if (sens==AR)
		{	
			if (i < 1) break;
			i--;
			chk = &m_elements[i];
		}
		else
		{	if (i >= (int)m_elements.GetCount() - 1 ) break;
			i++;
			chk = &m_elements[i];
		}

	}	

	if (*succ == 0)
	{	
		if ( (chk->TYPE == SYMB && ((MusSymbol*)chk)->flag == flg) || (chk->TYPE == NEUME_SYMB && ((MusNeumeSymbol*)chk)->getValue() == flg) )
			*succ = ON;
	}

	return (*succ ? chk : temoin);
}

// mlf == 1 wwg to mlf
// mlf == -1 mfl to wwg

int MusStaff::getOctCl ( MusElement *test, char *cle_id, int mlf )
{
	int succ=0;

	if (test)
	{	if ( (test->TYPE == SYMB && ((MusSymbol*)test)->flag == CLE) || (test->TYPE == NEUME_SYMB && ((((MusNeumeSymbol*)test)->getType() == NEUME_SYMB_CLEF_C) || ((MusNeumeSymbol*)test)->getType() == NEUME_SYMB_CLEF_F)) )
			succ = 1;
		else if ( (test->TYPE != SYMB || ((MusSymbol*)test)->flag != CLE) && (test->TYPE != NEUME_SYMB || ((((MusNeumeSymbol*)test)->getType() != NEUME_SYMB_CLEF_C) && ((MusNeumeSymbol*)test)->getType() != NEUME_SYMB_CLEF_F)) )
		{	
			test = no_note(test,AR,CLE,&succ);
			// LP mlf-> pas de recherche en avant si aucune cle trouvee
			if ((mlf != 1) && (succ == 0))
				test = no_note(test,AV,CLE,&succ);
			// LP
			if (succ == 0) { //this may need correction --Jamie
				test = no_note(test,AR,NEUME_SYMB_CLEF_C,&succ);
				if ((mlf != 1) && (succ == 0))
					test = no_note(test,AV,NEUME_SYMB_CLEF_C,&succ);
			}
			if (succ == 0) {
				test = no_note(test,AR,NEUME_SYMB_CLEF_F,&succ);
				if ((mlf != 1) && (succ == 0))
					test = no_note(test,AR,NEUME_SYMB_CLEF_F,&succ);
			}
		}
	}

	*cle_id = UT1;	/* initialiser */

	if (succ)
	{
		// LP -> succ == ON si n'importe quelle clef trouvee
		if ( mlf != 1)
		{
			if (((MusSymbol*)test)->code == FA4 || ((MusSymbol*)test)->code == FA3
			 || ((MusSymbol*)test)->code == FA5 || ((MusSymbol*)test)->code == UT4
			 || ((MusSymbol*)test)->code == UT5 || ((MusSymbol*)test)->code == SOLva
			 || ((MusNeumeSymbol*)test)->getValue() == nF1 || ((MusNeumeSymbol*)test)->getValue() == nF2
			 || ((MusNeumeSymbol*)test)->getValue() == nC1) //what does this represent? do we need neumatic clefs here? --Jamie
						succ = ON;
			else succ = OFF;
		}
		// LP
		*cle_id = ((test->TYPE == SYMB) ? (((MusSymbol*)test)->code) : (((MusNeumeSymbol*)test)->getValue()));
	}

	return (succ);
}


// alternateur de position d'octave 
void MusStaff::getOctDec (int ft, int _ot, int rupt, int *oct)
{
	if (ft>0)	// ordre DIESE 
		if (rupt % 2 == 0)	// sens: au premier appel; puis on alterne...
			// test a revoir (mauvais, ne marche pas si rupt est impair) 
			*oct -=1;
		else *oct = _ot;
	else
		if (rupt % 2 == 0)	*oct +=1;
		else *oct -= 1;
	return;
}


void MusStaff::updat_pscle (int i, MusElement *chk)
// int i;		nbre de cles sur cette ligne
{	
	//int size=0;

	//if (!modEfface)		// on n'est pas en train d'effacer
	{	if (i < MAXCLE)
		{	m_r->kPos[this->no].posx[i] = chk->xrel;
			m_r->kPos[this->no].dec[i] = chk->dec_y;
			m_r->kPos[this->no].compte++;
			//if (m_r->kPos[this->no].compte > 1)
			//	bsort ((unsigned int)m_r->kPos[this->no].compte, poscle_comp, poscle_swap);
			// on passe a shell les pointeurs vers fonctions specifiques
		}
	}
	/*else		// mise a jour de struct poscle
	{
		i=0;
		while(i<m_r->kPos[this->no].compte && m_r->kPos[this->no].posx[i] < chk->xrel)
			i++;
			// i contient position de cle (index correct)
		size = MAXCLE - (++i);
		memmove ((char *)&m_r->kPos[this->no].posx[i-1], (char *)&m_r->kPos[this->no].posx[i], size*sizeof(int));
		memmove ((char *)&m_r->kPos[this->no].dec[i-1], (char *)&m_r->kPos[this->no].dec[i], size*sizeof(int));
		m_r->kPos[this->no].compte--;	
	}*/
	return;
}

static char armatKey[] = {F5,F2,F6,F3,F7,F4,F8};
int MusStaff::armatDisp ( AxDC *dc )
{
	wxASSERT_MSG( dc , "DC cannot be NULL");

	if ( !Check() )
		return 0;

	/* calculer xrel, c et oct
	 * y_note calcule le decalage en fonction du code de hauteur c et du
	 *	decalage eventuel implique par l'oct courante et la clef. Ce
	 *	decalage est retourne par testcle() qui presuppose l'existence de
	 *	la table poscle */
	int step, oct;
	unsigned int xrl;
	MusElement *chk;
	int dec;
	int pos=1, fact, i, c, clid=UT1, rupture=-1;	// rupture, 1e pos (in array) ou
		// un shift d'octave est necessaire; ensuite, fonctionne par modulo
		// pour pairs (pour DIESE) et impair (pour BEMOL)
	int _oct;


	step = m_r->_pas*8;
	xrl = step + (this->indent? this->indent*10 : 0);

	if (this->notAnc)
		xrl += m_r->_pas;

	step = m_r->largAlter[this->pTaille][0];

	dec = this->testcle(xrl);	// clef courante

	chk = this->GetFirst();
	if (chk && m_r->kPos[ this->no].compte )
	{
		chk = this->no_note ( chk,AV, CLE, &pos);
		if (chk != NULL && chk->xrel < xrl && pos)
			this->getOctCl (chk,(char *)&clid);
	}

	if (this->armTyp==DIESE) 	// sens de parcourt de l'array 
	{	pos = 0; fact = 1;	}
	else
	{	pos = 6; fact = -1;	}

	oct = 0;		// default

	if (clid==FA4 || clid==FA5 || clid==FA3 || clid==UT5)
		oct = pos ? -2 : -1;

	else
	{	if (pos && (clid==UT4 || clid==UT3 || clid==UT2 || clid==SOLva))
			oct = -1;

		else if (!pos && (clid==SOL1 || clid==SOL2))
			oct = 1;
	}

	if (!pos)	// c'est des DIESES
	{	if (clid == UT4)
			rupture = 2;
		else if (clid == UT3 || clid < FA3 || clid == SOLva || clid == FA5)
								// ut3, sol1-2, fa4, solva 
			rupture = 4;
	}
	else
	{	if (clid == FA3 || clid == UT5)
			rupture = 2;
		else if (clid == UT2)
			rupture = 4;
	}
	_oct = oct;

	for (i = 0; i < this->armNbr; i++, pos += fact, xrl+=step)
	{
		c = armatKey [pos];
		if (i%2 && this->armTyp == BEMOL) oct += 1;
		else oct = _oct; 
		if (clid == UT1 && i==1) oct = _oct;
// correctif pour BEMOLS, clef Ut1, descente apres SI initial. Controler??? 
		if (rupture==i)
		{	this->getOctDec (fact,_oct,rupture, &oct); rupture = i+1;	}

		//if (!modMetafile || in (xrl, drawRect.left, drawRect.right) && in (this->yrel, drawRect.top, drawRect.bottom+_portee[pTaille]))
			((MusSymbol*)chk)->dess_symb ( dc,xrl,this->y_note(c,dec, oct),ALTER,this->armTyp , this);
	}
	return xrl;

}


void MusStaff::place_clef (  AxDC *dc )
{
	wxASSERT_MSG( dc , "DC cannot be NULL");

	if ( !Check() )
		return;

	MusElement *pelement = NULL;
	int j;

	for(j = 0; j < (int)this->nblement; j++)
	{
		pelement = &this->m_elements[j];
		if ((pelement->TYPE==SYMB && (((MusSymbol*)pelement)->flag==CLE
			|| (((MusSymbol*)pelement)->flag==BARRE && ((MusSymbol*)pelement)->code != CTRL_L)))
			|| (pelement->TYPE==NEUME_SYMB && ((((MusNeumeSymbol*)pelement)->getType()==NEUME_SYMB_CLEF_C) || (((MusNeumeSymbol*)pelement)->getType()==NEUME_SYMB_CLEF_F)))
			)
		{
			pelement->Init( m_r );
			pelement->Draw( dc, this );
			//((MusSymbol*)pelement)->rd_symb ( dc, this);
		}
	}
	return;
}



void MusStaff::DrawStaffLines( AxDC *dc, int i )
{
	wxASSERT_MSG( dc , "DC cannot be NULL");
	if ( !Check() )
		return;
        
    if (this->invisible)
        return;

	if ( i == -1 )
		i = m_r->m_page->m_staves.Index( *this );
	if ( i == wxNOT_FOUND)
		return;

	int j, x1, x2, yy;

	yy = (int)m_r->kPos[i].yp;
    if ( portNbLine == 1 )
		yy  -= m_r->_interl[ pTaille ]*2;
    else if ( portNbLine == 4 )
		yy  -= m_r->_interl[ pTaille ];
	yy = this->yrel - m_r->_portee[ pTaille ];

	x1 = this->indent ? this->indent*10 : 0;
	x2 = m_r->m_pageMaxX;


	dc->SetPen( m_r->m_currentColour, m_r->ToRendererX( m_p->EpLignesPortee ), wxSOLID );
    dc->SetBrush( m_r->m_currentColour , wxSOLID );
    dc->StartGraphic( "staff", wxString::Format("s_%d", this->no) );

	x1 =  m_r->ToRendererX (x1);
	x2 =  m_r->ToRendererX (x2);

	for(j = 0;j < this->portNbLine; j++)
	{
		dc->DrawLine( x1 , m_r->ToRendererY ( yy ) , x2 , m_r->ToRendererY ( yy ) );
		yy -= m_r->_interl[pTaille];
	}
    
    dc->EndGraphic();
    dc->ResetPen( );
    dc->ResetBrush( );
	return;
}



void MusStaff::DrawStaff( AxDC *dc, int i )
{
	wxASSERT_MSG( dc , "DC cannot be NULL");
	if ( !Check() )
		return;

	if ( i == -1 )
		i = m_r->m_page->m_staves.Index( *this );
	if ( i == wxNOT_FOUND)
		return;

	m_r->kPos[i].compte = 0; // mettre � zero le compteur de cles

	MusElement *pelement = NULL;
	int j;

	for(j = 0; j < (int)this->nblement; j++)
	{
		pelement = &this->m_elements[j];
		pelement->Init( m_r );
		pelement->Draw( dc, this );
	}

}

int MusStaff::y_neume (int note, int dec_clef, int oct)
{
	static int notes[] = {1,2,3,4,5,6,7};
	int y_int;
	int *pnote, i;
	pnote = &notes[0] - 1;
	
	y_int = ((dec_clef + oct*7) - 17 ) *m_r->_espace[pTaille];
	if (portNbLine > 4)
		y_int -= ((portNbLine - 4) * 2) *m_r->_espace[pTaille];
	
	for (i=0; i<(signed)sizeof(notes); i++)
		if (*(pnote+i)==note)
			return(y_int += (i*m_r->_espace[pTaille]));
	return 0;
}

int MusStaff::y_note (int code, int dec_clef, int oct)
{	static int touches[] = {F1,F2,F3,F4,F5,F6,F7,F8,F9,F10};
	int y_int;
	int *ptouche, i;
	ptouche=&touches[0];

	y_int = ((dec_clef + oct*7) - 17 ) *m_r->_espace[pTaille];
	if (portNbLine > 5)
		y_int -= ((portNbLine - 5) * 2) *m_r->_espace[pTaille];

	/* exprime distance separant yrel de
	position 1e Si, corrigee par dec_clef et oct. Elle est additionnee
	ensuite, donc elle doit etre NEGATIVE si plus bas que yrel */
	for (i=0; i<(signed)sizeof(touches); i++)
		if (*(ptouche+i)== code)
			return(y_int += (i*m_r->_espace[pTaille]));
	return 0;
}


// a partir d'un y, trouve la hauteur d'une note exprimee en code touche et
// octave. Retourne code clavier, et situe l'octave. 

int MusStaff::trouveCodNote (int y_n, int x_pos, int *octave)
{	static int touches[] = {F2,F3,F4,F5,F6,F7,F8};
	int y_dec, yb, plafond;
	int degres, octaves, position, code, decalOct;
	char cle_id=0;

	if ( !m_r )
		return 0;

	// calculer position du do central en fonction clef
	y_n += (int)m_r->v4_unit[pTaille];

	yb = this->yrel - m_r->_portee[pTaille]*2; // UT1 default 
	yb += (testcle (x_pos)) *m_r->_espace[pTaille];	// UT1 reel

	plafond = yb + 8 * m_r->_octave[pTaille];
	if (y_n > plafond)
		y_n = plafond;

	MusElement *pelement = this->GetAtPos( x_pos );
	if ( this->GetPrevious( pelement ) )
		pelement = this->GetPrevious( pelement );

	decalOct = getOctCl (pelement, &cle_id);
	yb -= (4 + decalOct) * m_r->_octave[pTaille];	// UT, note la plus grave

	y_dec = y_n - yb;	// decalage par rapport a UT le plus grave

	if (y_dec< 0)
		y_dec = 0;

	degres = y_dec / m_r->_espace[pTaille];	// ecart en degres (F2..F8) par rapport a UT1
	octaves = degres / 7;
	position = degres % 7;

	code = touches[position];
	*octave = octaves /*- OCTBIT*/ - decalOct; // LP remove OCTBIT : oct 0 � 7

	return (code);
}

AxPoint CalcPositionAfterRotation( AxPoint point , float rot_alpha, AxPoint center)
{
    int distCenterX = (point.x - center.x);
    int distCenterY = (point.y - center.y);
    // pythagore, distance entre le point d'origine et le centre
    int distCenter = (int)sqrt( pow( (double)distCenterX, 2 ) + pow( (double)distCenterY, 2 ) );
	
	// angle d'origine entre l'axe x et la droite passant par le point et le centre
    float alpha = atan ( (float)distCenterX / (float)(distCenterY) );
    
    AxPoint new_p = center;
    int new_distCenterX, new_distCenterY;

    new_distCenterX = ( (int)( sin( alpha - rot_alpha ) * distCenter ) );
	new_p.x += new_distCenterX;

    new_distCenterY = ( (int)( cos( alpha - rot_alpha ) * distCenter ) );
	new_p.y += new_distCenterY;

    return new_p;
}

/**
  x1 y1 = point de depart
  x2 y2 = point d'arrivee
  up = liaison vers le haut
  heigth = hauteur de la liaison ( � plat )
  **/
void MusStaff::DrawSlur( AxDC *dc, int x1, int y1, int x2, int y2, bool up, int height )
{

    dc->SetPen( m_r->m_currentColour, 1, wxSOLID );
    dc->SetBrush( m_r->m_currentColour, wxSOLID );


    int distX = x1 - x2;
    int distY = y1 - y2;
    // pythagore, distance entre les points de depart et d'arrivee
    int dist = (int)sqrt( pow( (double)distX, 2 ) + pow( (double)distY, 2 ) );

	// angle
    float alpha2 = float( distY ) / float( distX );
    alpha2 = atan( alpha2 );
	AxPoint orig(0,0);

	int step = dist / 10; // espace entre les points
	int nbpoints = dist / step;
	dist += 2*step; // ajout d'un pas de chaque cote, supprime � l'affichage
	if ( nbpoints <= 2)
		nbpoints = 3;
	else if ( !(nbpoints % 2) ) // nombre impair de points
		nbpoints++;

	int i,j, k;
	double a, b;
	a = dist / 2; // largeur de l'ellipse
	int nbp2 = nbpoints/2;
	AxPoint *points = new AxPoint[2*nbpoints]; // buffer double - aller retour

	// aller
	b = 100; // hauteur de l'ellipse
	points[nbp2].x = (int)a; // point central
	points[nbp2].y = (int)b;
	for(i = 0, j = nbpoints - 1, k = 1; i < nbp2; i++, j--, k++) // calcul de l'ellipse
	{
		points[i].x = k*step;
		points[i+nbp2+1].y = (int)sqrt( (1 - pow((double)points[i].x,2) / pow(a,2)) * pow(b,2) );
		points[j].x = dist - points[i].x;
		points[j-nbp2-1].y = points[i+nbp2+1].y;		
	}
	int dec_y = points[0].y; // decalage y � cause des 2 pas ajoutes
	for(i = 0; i < nbpoints; i++)
	{
		points[i] = CalcPositionAfterRotation( points[i], alpha2, orig ); // rotation		
		points[i].x = m_r->ToRendererX( points[i].x + x1 - 1*step ); // transposition
		points[i].y = m_r->ToRendererY( points[i].y + y1 - dec_y );
	}
	dc->DrawSpline( nbpoints, points );

	// retour idem
	b = 90;	
	points[nbp2+nbpoints].x = (int)a;
	points[nbp2+nbpoints].y = (int)b;
	for(i = nbpoints, j = 2*nbpoints - 1, k = 1; i < nbp2+nbpoints; i++, j--, k++)
	{	
		points[j].x = k*step;
		points[i+nbp2+1].y = (int)sqrt( (1 - pow((double)points[j].x,2) / pow(a,2)) * pow(b,2) );
		points[i].x = dist - points[j].x;
		points[j-nbp2-1].y = points[i+nbp2+1].y;	
	}
	dec_y = points[nbpoints].y;

	for(i = nbpoints; i < 2*nbpoints; i++)
	{
		points[i] = CalcPositionAfterRotation( points[i], alpha2, orig );
		points[i].x = m_r->ToRendererX( points[i].x + x1 - 1*step );
		points[i].y = m_r->ToRendererY( points[i].y + y1 - dec_y );
	}
	dc->DrawSpline( nbpoints, points+nbpoints );

	// remplissage ?

    dc->ResetPen( );
    dc->ResetBrush( );

	delete[] points;

}

// Gets the y coordinate of the previous lyric. If lyric is NULL, it will return the y coordinate of the first lyric 
// in the stave. If there are no lyrics in the Stave -1 is returned.
int MusStaff::GetLyricPos( MusSymbol *lyric )
{
	MusSymbol *tmp;
	if ( !lyric ){
		if ( !( tmp = GetFirstLyric() ) )
			return -1;
		return tmp->dec_y;
	}
	
	if ( !( tmp = GetPreviousLyric( lyric ) ) )
		return -1;
	return tmp->dec_y;
}

MusSymbol *MusStaff::GetPreviousLyric( MusSymbol *lyric )
{
	if ( !lyric || m_elements.IsEmpty() || !lyric->m_note_ptr || lyric->m_note_ptr->no < 0 )
		return NULL;
	
	// If there are other lyrics attached to the note that lyric is attached to...
	if ( (int)lyric->m_note_ptr->m_lyrics.GetCount() > 1 ){
		bool check = false; // Keeps track if we have past the pointer to this element in m_lyrics
		for ( int i = (int)lyric->m_note_ptr->m_lyrics.GetCount() - 1; i >= 0; i-- ){
			MusSymbol *previousLyric = &lyric->m_note_ptr->m_lyrics[i];
			if ( check ) return previousLyric;
			if ( previousLyric == lyric ) check = true;
		}
	}
	// Check previous note in staff for lyric
	int no = lyric->m_note_ptr->no - 1;
	while ( no >= 0 ){
		if ( m_elements[ no ].IsNote() ){
			for ( int i = (int) ((MusNote*)&m_elements[ no ])->m_lyrics.GetCount() - 1; i >= 0 ; i-- ){
				MusSymbol *previousLyric = &((MusNote*)&m_elements[ no ])->m_lyrics[i];
				if ( previousLyric ) return previousLyric;
			}
		}
		no--;
	}
	return NULL;
}

MusSymbol *MusStaff::GetNextLyric( MusSymbol *lyric )
{	
	if ( !lyric || m_elements.IsEmpty() || !lyric->m_note_ptr || lyric->m_note_ptr->no > (int)m_elements.GetCount() - 1 )
		return NULL;
	
	// If there are other lyrics attached to the note that lyric is attached to...
	if ( (int)lyric->m_note_ptr->m_lyrics.GetCount() > 1 ){
		bool check = false; // Keeps track if we have past the pointer to this element in m_lyrics
		for ( int i = 0; i < (int)lyric->m_note_ptr->m_lyrics.GetCount(); i++ ){
			MusSymbol *nextLyric = &lyric->m_note_ptr->m_lyrics[i];
			if ( check ) 
				return nextLyric;
			if ( nextLyric == lyric ) 
				check = true;
		}
	}
	// Check next note in staff for lyric
	int no = lyric->m_note_ptr->no + 1;
	while ( no < (int)m_elements.GetCount() ){
		if ( m_elements[ no ].IsNote() ){
			for ( int i = 0; i < (int) ((MusNote*)&m_elements[ no ])->m_lyrics.GetCount(); i++ ){
				MusSymbol *nextLyric = &((MusNote*)&m_elements[ no ])->m_lyrics[i];
				if ( nextLyric )
					return nextLyric;
			}
		}
		no++;
	}
	return NULL;
}

MusSymbol *MusStaff::GetFirstLyric( )
{
	if ( m_elements.IsEmpty() )
		return NULL;
	int no = 0;
	while ( no < (int)m_elements.GetCount() ){
		if ( m_elements[ no ].IsNote() ){
			for ( int i = 0; i < (int) ((MusNote*)&m_elements[ no ])->m_lyrics.GetCount(); i++ ){
				MusSymbol *lyric = &((MusNote*)&m_elements[ no ])->m_lyrics[i];
				if ( lyric )
					return lyric;
			}
		}
		no++;
	}
	return NULL;	
}

MusSymbol *MusStaff::GetLastLyric( )
{
	if ( m_elements.IsEmpty() )
		return NULL;
	int no = (int)m_elements.GetCount() - 1;
	while ( no >= 0 ){
		if ( m_elements[ no ].IsNote() ) {
			for ( int i = (int) ((MusNote*)&m_elements[ no ])->m_lyrics.GetCount() - 1; i >= 0 ; i-- ){
				MusSymbol *lyric = &((MusNote*)&m_elements[ no ])->m_lyrics[i];
				if ( lyric )
					return lyric;
			}
		}
		no--;
	}
	return NULL;
}

MusSymbol *MusStaff::GetLyricAtPos( int x )
{
	MusSymbol *lyric = this->GetFirstLyric();
	if ( !lyric )
		return NULL;
	
	//int xx = 0;
	int dif = x - lyric->xrel;
	while ( this->GetNextLyric( lyric ) && (int)lyric->xrel < x ){
		lyric = this->GetNextLyric( lyric );
		if ( (int)lyric->xrel > x && dif < (int)lyric->xrel - x )
			return this->GetPreviousLyric( lyric );
		dif = x - lyric->xrel;
	}
		
	return lyric;
}

void MusStaff::DeleteLyric( MusSymbol *symbol )
{
	if ( !symbol ) return;
	
	
	if ( m_r ) // effacement
	{
		if ( symbol->IsSymbol() && (((MusSymbol*)symbol)->IsLyric()) )
			m_r->OnBeginEditionClef();
	}
	
	MusNote *note = symbol->m_note_ptr;
	for ( int i = 0; i < (int)note->m_lyrics.GetCount(); i++ ){
		MusSymbol *lyric = &note->m_lyrics[i];
		if ( symbol == lyric )
			note->m_lyrics.Detach(i);
	}
	
	this->CheckIntegrity();
	
	if ( m_r )
	{
		if ( symbol->IsSymbol() && (((MusSymbol*)symbol)->IsLyric()) )
			m_r->OnEndEditionClef();
		m_r->DoRefresh();
	}
	
	delete symbol;
}

MusNote *MusStaff::GetNextNote( MusSymbol * lyric )
{
	if ( !lyric || m_elements.IsEmpty() || !lyric->m_note_ptr || lyric->m_note_ptr->no >= (int)m_elements.GetCount() - 1 )
		return NULL;
	
	int no = lyric->m_note_ptr->no + 1;
	for ( int i = no; i < (int)m_elements.GetCount(); i++ ){
		MusElement *element = &m_elements[i];
		if ( element->IsNote() && ((MusNote*)element)->sil == _NOT )
			return (MusNote*)element; 
	}
	return NULL;
}

MusNote *MusStaff::GetPreviousNote( MusSymbol * lyric )
{
	if ( !lyric || m_elements.IsEmpty() || !lyric->m_note_ptr || lyric->m_note_ptr->no <= 0 )
		return NULL;
	
	int no = lyric->m_note_ptr->no - 1;
	for ( int i = no; i >= 0; i-- ){
		MusElement *element = &m_elements[i];
		if ( element->IsNote() && ((MusNote*)element)->sil == _NOT )
			return (MusNote*)element; 
	}
	return NULL;
}

//Switches the note association of lyric from oldNote to newNote and modifies the two notes accordingly
//bool beginning: indicates if we want to add the lyric to beginning or end of the lyric array in newNote 
//		true = beginning of array
//		false = end of array
void MusStaff::SwitchLyricNoteAssociation( MusSymbol *lyric, MusNote *oldNote, MusNote* newNote, bool beginning )
{
	if ( !lyric || !oldNote || !newNote )
		return;
	
	lyric->m_note_ptr = newNote;
	if ( beginning )
		newNote->m_lyrics.Insert( lyric, 0 );
	else
		newNote->m_lyrics.Insert( lyric, newNote->m_lyrics.GetCount() );
	
	for ( int i = 0; i < (int)oldNote->m_lyrics.GetCount(); i++ ){
		MusSymbol *element = &oldNote->m_lyrics[i];
		if ( element == lyric ){
			oldNote->m_lyrics.Detach(i);
			break;
		}			
	}
}

void MusStaff::AdjustLyricLineHeight( int delta ) 
{
	for ( int i = 0; i < (int)m_elements.GetCount(); i++ ){
		MusElement *element = &m_elements[i];
		if ( element->IsNote() ){
			for ( int j = 0; j < (int)((MusNote*)element)->m_lyrics.GetCount(); j++ ){
				MusSymbol *lyric = &((MusNote*)element)->m_lyrics[j];
				lyric->dec_y += delta;
			}
		}
	}
}

// functors for MusStaff

void MusStaff::CopyElements( wxArrayPtrVoid params )
{
    // param 0: MusStaff
    MusStaff *staff = (MusStaff*)params[0];

	MusElement *element = staff->GetLast();
    int x_last = 0;
    if (element) {
        x_last = element->xrel;
    }
	int i;
    for (i = 0; i < (int)nblement; i++) 
	{
		if ( m_elements[i].IsNote() )
		{
			MusNote *nnote = new MusNote( *(MusNote*)&m_elements[i] );
            nnote->xrel += x_last;
			staff->m_elements.Add( nnote );
		}
		else
		{
			MusSymbol *nsymbol = new MusSymbol( *(MusSymbol*)&m_elements[i] );
            nsymbol->xrel += x_last;
			staff->m_elements.Add( nsymbol );
		}
	}
    staff->CheckIntegrity();
}


void MusStaff::GetMaxXY( wxArrayPtrVoid params )
{
    // param 0: int
    int *max_x = (int*)params[0];
    int *max_y = (int*)params[1]; // !!this is given in term of staff line space

	MusElement *element = this->GetLast();
    if (element) {
        int last_max = element->xrel;
        if (!element->IsSymbol() || (((MusSymbol*)element)->flag != BARRE)) {
            last_max += 35; // abirtrary margin;
        }
        if ((*max_x) < last_max) {
            *max_x = last_max;
        }
    }
    (*max_y) += this->ecart;
}





