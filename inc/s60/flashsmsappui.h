

#ifndef __FlashSms_APPUI_H__
#define __FlashSms_APPUI_H__


// INCLUDES
#include <aknViewAppUi.h>
#include <akntabgrp.h>
#include <aknnavide.h>
#include "Storage.h"
#include "Phone.h"
#include "TimeOutNotifier.h"


// CLASS DECLARATION

/**
* CFlashSmsAppUi application UI class.
* An instance of class CFlashSmsAppUi is the UserInterface part of the AVKON
* application framework for the FlashSms example application
*/
class CFlashSmsAppUi : public CAknViewAppUi, public MTimeOutNotifier, public MRegHandler
    {

    public: // Constructors and destructor

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL();

    public: // New functions from CAknAppUi

        /**
		* HandleCommandL
        * Takes care of command handling.
        * @param aCommand Command to be handled.
        */
        void HandleCommandL( TInt aCommand );
		
		void SaveSettingsL();

        /**
        * ~CAknExNoteAppUi
        * Virtual Destructor.
        */     
        virtual ~CFlashSmsAppUi();

		// TimeOutNotifier
        void TimerExpiredL();

		
		void RegistrationDoneL();

		// Deve restituire True se l'imsi e' corretto, false altrimenti
		bool VerifyImsiL();

	private:    // From MEikMenuObserver
				/** 
				* From MEikMenuObserver
				* @param aResourceId menu pane resource ID
				* @param aMenuPane menu pane pointer
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

        virtual TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType);

    private: // Data
		CAknNavigationDecorator*        iDecoratedTabGroup;	// Own
		//CPhone*							iPhone;				// Own
		CStorage*						iStorage;			// Own
		CPhone* iPhone;
    };


#endif // __FlashSms_APPUI_H__

// End of File



















