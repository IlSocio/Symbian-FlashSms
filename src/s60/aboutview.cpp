

// INCLUDE FILES
#include <aknviewappui.h>
#include <aknconsts.h>
#include <FlashSms.rsg>

#include "AboutView.h"
#include "AboutContainer.h"
#include "FlashSms.hrh"
#include <eikmenup.h>
#include <akntitle.h> 
#include <aknquerydialog.h> 
#include "base64.h"
// ========================= MEMBER FUNCTIONS ==================================

// -----------------------------------------------------------------------------
// CAboutView::CAboutView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CAboutView::CAboutView()
    {
    // No implementation required
    }


// -----------------------------------------------------------------------------
// CAboutView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAboutView* CAboutView::NewL(CStorage& storage)
    {
    CAboutView* self = CAboutView::NewLC(storage);
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CAboutView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAboutView* CAboutView::NewLC(CStorage& storage)
    {
    CAboutView* self = new ( ELeave ) CAboutView();
    CleanupStack::PushL( self );
    self->ConstructL(storage);
    return self;
    }

// -----------------------------------------------------------------------------
// CAboutView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAboutView::ConstructL(CStorage& storage)
    {
    BaseConstructL( R_FLASHSMS_VIEW1 );
	iStorage = &storage;
    }

// -----------------------------------------------------------------------------
// CAboutView::~CAboutView()
// Destructor.
// -----------------------------------------------------------------------------
//
CAboutView::~CAboutView()
    {
    if ( iContainer )
	{
        AppUi()->RemoveFromStack( iContainer );
	}
    delete iContainer;
    }

// -----------------------------------------------------------------------------
// CAboutView::Id()
// Returns View's ID.
// -----------------------------------------------------------------------------
//
TUid CAboutView::Id() const
    {
    return KViewAboutId;
    }

// -----------------------------------------------------------------------------
// CAboutView::DoActivateL()
// Activate an FlashSms1
// -----------------------------------------------------------------------------
//
void CAboutView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                    TUid /*aCustomMessageId*/,
                                    const TDesC8& /*aCustomMessage*/)
    {
	iContainer = new ( ELeave ) CAboutContainer();
	iContainer->SetMopParent(this);
    iContainer->ConstructL( ClientRect() );
    AppUi()->AddToStackL( iContainer );

	// Imposta il titolo
	CEikStatusPane* sp=iEikonEnv->AppUiFactory()->StatusPane();
	CAknTitlePane* tp=(CAknTitlePane*)sp->ControlL(TUid::Uid(EEikStatusPaneUidTitle));
	HBufC* string = CCoeEnv::Static()->AllocReadResourceL(R_ABOUT);
	tp->SetText( string );
    }

// -----------------------------------------------------------------------------
// CAboutView::DoDeactivate()
// DeActivate an FlashSms1
// -----------------------------------------------------------------------------
//
void CAboutView::DoDeactivate()
    {

    if ( iContainer )
        {
        // Remove View1's container form controllStack
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CAknExPopListAppUi::DynInitMenuPaneL()
// This function is called by the EIKON framework just before it displays
// a menu pane. Its default implementation is empty, and by overriding it,
// the application can set the state of menu items dynamically according
// to the state of application data.
// -----------------------------------------------------------------------------
//
void CAboutView::DynInitMenuPaneL( TInt aResourceId,
										CEikMenuPane* aMenuPane )
	{
	if ((aResourceId == R_FLASHSMS_MENU1) &&
	    ( iStorage->iSettings.IsValidRegCodeL() ))
		{
		// Nasconde il menu Donate
		aMenuPane->DeleteMenuItem(EDonate);
		}
	}
// -----------------------------------------------------------------------------
// CAboutView::HandleCommandL()
// Takes care of Command handling.
// -----------------------------------------------------------------------------
//
void CAboutView::HandleCommandL( TInt aCommand )
    {
	switch (aCommand)
        {
		case EDonate:
			{			
			CBase64CodecBase* base64 = new (ELeave) CBase64CodecBase();
			CleanupStack::PushL(base64);
			TBuf<10> regReq64;

			HBufC* regReq = iStorage->iSettings.GetRegRequestCodeLC();

// Visualizza il codice corretto

// RegReq: w5j6khM2
// 2Rs@tYHq
/*
		TPtr ptr = regReq->Des();
		ptr.Zero();
		ptr.Append( TChar(195) ); // 195
		ptr.Append( TChar(152) ); // 250
		ptr.Append( TChar(250) ); // 146
		ptr.Append( TChar(146) ); // 19
 		ptr.Append( TChar(19) );  // 54
		ptr.Append( TChar(54) );  

		HBufC* good = iStorage->iSettings.GetRegRequestCodeLC( regReq->Des() );		
		regReq->Des().Copy(good->Des());
		/**/

			base64->Encode( *regReq, regReq64 );
			CleanupStack::PopAndDestroy(); // RegRequestCode bin
//			regReq64.Zero();
//			regReq64.Copy( _L("w5j6khM2") );
			TBuf<10> regCode64;
			CAknMultiLineDataQueryDialog* dlg = CAknMultiLineDataQueryDialog::NewL(regReq64, regCode64);

			if (dlg->ExecuteLD(R_DONATE_DIALOG))
			{
				iStorage->iSettings.regCode.Zero();
				base64->Decode(regCode64, iStorage->iSettings.regCode);
			}
			CleanupStack::PopAndDestroy(); // CBase64

			if ( iStorage->iSettings.IsValidRegCodeL() ) 
			{
				iStorage->SaveSettingsL();
				iStorage->LoadSettingsL();
			}
			break;					   
			}
		default:
			{
			AppUi()->HandleCommandL( aCommand );
			}
        }
    }


// -----------------------------------------------------------------------------
// CAknExPopListView2::HandleClientRectChange()
// Handles Clients Rect
// -----------------------------------------------------------------------------
//
void CAboutView::HandleClientRectChange()
	{	
    if ( iContainer )
		{
        iContainer->SetRect( ClientRect() );
		}
	}
