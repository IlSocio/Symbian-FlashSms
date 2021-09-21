

// INCLUDE FILES
#include <e32std.h>
#include <avkon.hrh>
#include <aknnavi.h>
#include <akntitle.h> 
#include <eikapp.h>		// Application
#include <AknQueryDialog.h> 
#include <AknNoteWrappers.h> 
#include <cntdb.h>
#include <bautils.h>

#include "base64.h"
#include "FlashSms.rsg"
#include "FlashSms.hrh"
#include "FlashSms.pan"
#include "FlashSmsAppUi.h"
#include "CreditsView.h"
#include "SplashView.h"
#include "AboutView.h"
#include "SettingsView.h"
#include "Storage.h"



#define InfoMsg(R_RSC)  {CAknInformationNote* dialog = new ( ELeave ) CAknInformationNote(ETrue); \
						TBuf<64> aDes; \
    					iCoeEnv->ReadResource( aDes, R_RSC ); \
						dialog->ExecuteLD( aDes );}

#define ErrorMsg(R_RSC) {CAknErrorNote* dialog = new ( ELeave ) CAknErrorNote(ETrue); \
						TBuf<64> aDes; \
						iCoeEnv->ReadResource( aDes, R_RSC ); \
						dialog->ExecuteLD( aDes );}

#define WarnMsg(R_RSC)  {CAknWarningNote* dialog = new ( ELeave ) CAknWarningNote(ETrue); \
						TBuf<64> aDes; \
    					iCoeEnv->ReadResource( aDes, R_RSC ); \
						dialog->ExecuteLD( aDes );}





// ========================= MEMBER FUNCTIONS ==================================
// -----------------------------------------------------------------------------
// CFlashSmsAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFlashSmsAppUi::ConstructL()
    {
	BaseConstructL(EAknEnableSkin);

	TParse exeFile;
	exeFile.Set( Application()->AppFullName(), NULL, NULL );

	iStorage = CStorage::NewL( iEikonEnv->FsSession(), exeFile.Drive() );
	
	
	TFileName fname = exeFile.Drive();
	fname.Append( _L("\\system\\apps\\flashsms\\firstboot.dat") );
	if ( BaflUtils::FileExists(iEikonEnv->FsSession(), fname ) ) 
	{
		InfoMsg(R_FIRST_BOOT);
		iEikonEnv->FsSession().Delete(fname);	
	}
	
	// Lettura Settings
	TInt ris = KErrNone;
	TRAPD(error, ris = iStorage->LoadSettingsL() );
	if (error != KErrNone)	// Il file di configurazione ha un formato non valido
	{
		ris = KErrGeneral;		
	}

//	CEikonEnv::QueryWinL(_L("3"), _L(""));	
	if (ris != KErrNone)	// Il file di configurazione non esiste...
	{
		iStorage->ResetSettingsL();
//		iEikonEnv->ReadResourceL(iStorage->iSettings.message, R_WARNING_SMS );
		iStorage->SaveSettingsL();
	}
		
//****************
	iPhone = new (ELeave) CPhone();
    iPhone->InitializeL(*this);
	// Show tabs for main views from resources
    CEikStatusPane *sp = StatusPane();

    CAknTabGroup* tabGroup = NULL;
    // Fetch pointer to the default navi pane control
    CAknNavigationControlContainer* iNaviPane = static_cast<CAknNavigationControlContainer*> 
		( sp->ControlL( TUid::Uid( EEikStatusPaneUidNavi )  ) );/**/
	
/*    iDecoratedTabGroup = iNaviPane->ResourceDecorator();
    if ( iDecoratedTabGroup )
    {
        tabGroup = static_cast<CAknTabGroup*> 
			( iDecoratedTabGroup->DecoratedControl() );
    }*/

	CCreditsView* iCreditsView1 = CCreditsView::NewLC();
	AddViewL( iCreditsView1 );
	CleanupStack::Pop();

	CAboutView* iAppView1 = CAboutView::NewLC(*iStorage);
    AddViewL( iAppView1 );
	CleanupStack::Pop();
	
	CSettingsView* iAppView2 = CSettingsView::NewLC(*iStorage);
	AddViewL( iAppView2 );
	CleanupStack::Pop();
	
    SetDefaultViewL( *iAppView1 );
    if (  tabGroup  )
	{
        ActivateLocalViewL(  TUid::Uid(  tabGroup->ActiveTabId()  )  );
	}
    else  // just in case
	{
        ActivateLocalViewL(  iAppView1->Id()  );
	}

}



void CFlashSmsAppUi::RegistrationDoneL() 
{
}


// Forse il metodo non viene richiamato quando fallisce il rilevamento...
bool CFlashSmsAppUi::VerifyImsiL() 
	{
	// TODO: Verifica cosa accade se fallisce...
	//CEikonEnv::QueryWinL(_L("VERIFY IMSI/IMEI"), _L(""));
	if (iPhone->GetImei().Length() < 5) 
	{
		Exit();
	}

	// Se il campo imei di guardian e' vuoto imposta imei
	// Questa associazione avviene soltanto qui
	if (iStorage->iSettings.imei.Length() < 5) 
	{
		InfoMsg(R_FIRST_BOOT);
		// Messaggio di info su come supportare il progetto
		// e limitazioni relative a questa versione
		iStorage->iSettings.imei = iPhone->GetImei();
		iStorage->SaveSettingsL();
	}

	// Se i 2 imei non coincidono esce
	if (iStorage->iSettings.imei != iPhone->GetImei()) 
	{
		// Si verifica quando a cell. acceso viene inserita una mmc con guardian
#ifdef  __WINS__
		iStorage->ResetSettingsL();
#else 
		Exit();
#endif
	}
	
	return ETrue;
	}	




void CFlashSmsAppUi::TimerExpiredL() 
{
	Exit();
}


// -----------------------------------------------------------------------------
// CAknExPopListAppUi::DynInitMenuPaneL()
// This function is called by the EIKON framework just before it displays
// a menu pane. Its default implementation is empty, and by overriding it,
// the application can set the state of menu items dynamically according
// to the state of application data.
// -----------------------------------------------------------------------------
//
void CFlashSmsAppUi::DynInitMenuPaneL( TInt /*aResourceId*/,
										  CEikMenuPane* /*aMenuPane*/ )
	{
    // No implementation required
	}


// Per la corretta gestione dei Tab e delle relative viste
TKeyResponse CFlashSmsAppUi::HandleKeyEventL( const TKeyEvent& aKeyEvent,
												 TEventCode /*aType*/ )
	{
	if (iDecoratedTabGroup == NULL) return EKeyWasNotConsumed;

/*    CAknTabGroup* tabGroup = static_cast<CAknTabGroup*> 
		( iDecoratedTabGroup->DecoratedControl() );
    
    if ( tabGroup == NULL )
    {
        return EKeyWasNotConsumed;
    }
	
    TInt active = tabGroup->ActiveTabIndex();
    TInt count = tabGroup->TabCount();
	
    switch ( aKeyEvent.iCode )
	{
        // Left Arrow pressed
	case EKeyLeftArrow:
		if ( active > 0 )
		{
			active--;
			tabGroup->SetActiveTabByIndex( active );
			ActivateLocalViewL(  TUid::Uid(  tabGroup->ActiveTabId() ) );
		}
		break;
		
        // Right Arrow pressed
	case EKeyRightArrow:
		if( ( active + 1 ) < count )
		{
			active++;
			tabGroup->SetActiveTabByIndex( active );
			ActivateLocalViewL(  TUid::Uid(  tabGroup->ActiveTabId() ) );
		}
		break;
		
	default:		
		//iTabGroup->OfferKeyEventL(aKeyEvent, aType);
		return EKeyWasNotConsumed;
		break;
	}*/

    return EKeyWasConsumed;
    }


// -----------------------------------------------------------------------------
// CFlashSmsAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CFlashSmsAppUi::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
		case ENewSms:
			{
	        ActivateLocalViewL( KViewSettingsId );
			break;
			}
		case ECredits:
			{
	        ActivateLocalViewL( KViewCreditsId );
			break;
			}
/* Non fa piu' cosi' xche' potrebbe darsi che l'invio e' ancora in corso
		case ESend:
			{
	        ActivateLocalViewL( KViewAboutId );
			break;
			}*/
        case EAknSoftkeyBack:
			{
	        ActivateLocalViewL( KViewAboutId );
			break;
			}
        case EEikCmdExit:
        case EAknSoftkeyExit:
			{
			CAknQueryDialog* dlg = CAknQueryDialog::NewL();
			if ( dlg->ExecuteLD(R_REALLY_EXIT) )
				{
				CSplashView* iAppView1 = CSplashView::NewLC(*this);
				AddViewL( iAppView1 );
				CleanupStack::Pop();
		        ActivateLocalViewL( KView0Id );
				}				
            break;
			}
        default:
//            Panic( EFlashSmsBasicUi );
            break;
        }
    }


CFlashSmsAppUi::~CFlashSmsAppUi() 
	{
	delete iStorage;
	if (iDecoratedTabGroup)
		delete iDecoratedTabGroup;
	if (iPhone)
		delete iPhone;
	}

// End of File
