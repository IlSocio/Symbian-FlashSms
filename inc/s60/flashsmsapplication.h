

#ifndef __FlashSms_APPLICATION_H__
#define __FlashSms_APPLICATION_H__


// INCLUDES
#include <aknapp.h>


// CONSTANTS
// Uid for the application, should match the one in the mmp file
const TUid KUidFlashSmsApp = { 0x1020DC97 };


// CLASS DECLARATION

/**
* An instance of CFlashSmsApplication is the application part of the AVKON
* application framework for the FlashSms example application
*/
class CFlashSmsApplication : public CAknApplication
    {

    public:  // from CAknApplication

        /**
        * AppDllUid
        * @return returns application's UID (KUidAknExMenu).
        */
        TUid AppDllUid() const;


    protected: // from CAknApplication


        /**
        * CreateDocumentL
        * Creates CFlashSmsDocument document object.
        * The returned pointer in not owned by the CFlashSmsApplication object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
    };

#endif // __FlashSms_APPLICATION_H__

// End of File
