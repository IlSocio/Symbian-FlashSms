


#ifndef __SETTINGS_CONTAINER_H__
#define __SETTINGS_CONTAINER_H__


// INCLUDES
#include <aknview.h>
#include <eiktxlbx.h>
#include "storage.h"


_LIT(KTAB, "\t");


// CLASS DECLARATION

/**
* CSettingsContainer container control class.
* An instance of CSettingsContainer contains the view drawn to the screen
* for CMultiViewsView2
*/

class CSettingsContainer : public CCoeControl
    {
    public:  // Constructors and destructor

        /**
        * ConstructL
        * 2nd phase constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL( const TRect& aRect, TFlashSmsSettings& settings );

		~CSettingsContainer();

		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
        /**
        * Draw
        * Draw this CMultiViewsContainer1 to the screen.
        * @param aRect the rectangle of this view that needs updating
        */
        void Draw( const TRect& aRect ) const;

        /**
        * CountComponentControls
        * @return number of controls inside this container.
        */
        TInt CountComponentControls() const;

        /**
        * ComponentControl
        * @param Index number of the control.
        * @return Pointer of controls inside this container.
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

		void SizeChanged();

	private:
		TFlashSmsSettings* iSettings;

	public:
        CEikEdwin* iEdwin;
    };

#endif // __MULTIVIEWS_CONTAINER2_H__

// End of File