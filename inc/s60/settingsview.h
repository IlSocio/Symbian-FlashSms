


#ifndef __SETTINGS_VIEW_H__
#define __SETTINGS_VIEW_H__


// INCLUDES
#include <aknview.h>
#include <eiktxlbx.h>
#include "storage.h"
#include "csmshandler.h"


// FORWARD DECLARATIONS
class CSettingsContainer;
const TUid KViewSettingsId = { 2 };

// CLASS DECLARATION

/**
* CSettingsView view class.
* An instance of the Application View object for the MultiViews
* example application
*/
class CSettingsView: public CAknView
    {
    public:    // Constructors and destructor

        /**
        * NewL.
        * Two-phased constructor.
        * Construct a CSettingsView for the AVKON application aApp.
        * Using two phase construction,and return a pointer to the created object.
        * @return a pointer to the created instance of CSettingsView
        */
        static CSettingsView* NewL(CStorage& storage);

        /**
        * NewLC.
        * Two-phased constructor.
        * Construct a CSettingsView for the AVKON application aApp.
        * Using two phase construction,and return a pointer to the created object.
        * @return a pointer to the created instance of CSettingsView
        */
        static CSettingsView* NewLC(CStorage& storage);

        /**
        * ~CSettingsView.
        * Virtual Destructor.
        */
        virtual ~CSettingsView();


    public:// Functions from base classes from CAknView


        /**
        * Id
        * From CAknView, return Uid.
        * @return Uid Uid value
        */
        TUid Id() const;

        /**
        * HandleCommandL
        * From CAknView, takes care of command handling.
        * @param aCommand Command to be handled
        */
        void HandleCommandL( TInt aCommand );

        /**
        * HandleClientRectChange
        * From CAknView, handles client rectangle is changed.
        */
        void HandleClientRectChange();

        /**
        * DoActivateL
        * From CAknExView, activate an AknView.
        * @param aPrevViewId The id of the previous view
        * @param aCustomMessageId message identifier
        * @param aCustomMessage custom message provided when the view is changed
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );


        /**
        * DoDeactivate
        * From AknView, deactivate an AknView
        * Remove the container class instance from the App UI's stack and
        * deletes the instance
        */
        void DoDeactivate();

		void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane );
		TBool SendMessageL();
		TBool SendSingleMessageL();

    private:    // Constructors and destructor

        /**
        * CSettingsView.
        * C++ default constructor.
        */
        CSettingsView();

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL(CStorage& storage);


    private:    // Data

		CSmsHandler* iSms;
        /**
        * iContainer,container for this view
        * owned by CSettingsView object.
        */
        CSettingsContainer* iContainer;

        /** View Identifier **/
        TUid       iIdentifier;

		CStorage* iStorage;
       };


#endif // __MULTIVIEWS_VIEW2_H__


// End of File