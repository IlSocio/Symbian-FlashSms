

#ifndef __FlashSms_DOCUMENT_H__
#define __FlashSms_DOCUMENT_H__



// INCLUDES
#include <akndoc.h>


// FORWARD DECLARATIONS
class CFlashSmsAppUi;
class CEikApplication;



// CLASS DECLARATION

/**
* CFlashSmsDocument application class.
* An instance of class CFlashSmsDocument is the Document part of the AVKON
* application framework for the CFlashSmsApplication example application.
*/
class CFlashSmsDocument : public CAknDocument
    {
    public:  // Constructors and destructor


        /**
        * NewL.
        * Two-phased constructor.
        * Construct a CFlashSmsDocument for the AVKON application aApp.
        * Using two phase construction, and return a pointer to the created object.
        * @param aApp Application creating this document.
        * @return A pointer to the created instance of CFlashSmsDocument.
        */
        static CFlashSmsDocument* NewL( CEikApplication& aApp );

        /**
        * NewLC.
        * Two-phased constructor.
        * Construct a CFlashSmsDocument for the AVKON application aApp.
        * Using two phase construction, and return a pointer to the created object.
        * @param aApp Application creating this document.
        * @return A pointer to the created instance of CFlashSmsDocument.
        */
        static CFlashSmsDocument* NewLC( CEikApplication& aApp );

        /**
        * ~CFlashSmsDocument
        * Virtual Destructor.
        */
        virtual ~CFlashSmsDocument();


    public: // from CAknDocument

        /**
        * CreateAppUiL
        * From CEikDocument, CreateAppUiL.
        * Create a CFlashSmsAppUi object and return a pointer to it.
        * The object returned is owned by the Uikon framework.
        * @return Pointer to created instance of AppUi.
        */
        CEikAppUi* CreateAppUiL();

    private:  // Constructors

        /**
        * ConstructL
        * 2nd phase constructor.
        */
        void ConstructL();

        /**
        * CFlashSmsDocument.
        * C++ default constructor.
        * @param aApp Reference to Application class object
        */
        CFlashSmsDocument( CEikApplication& aApp );

    };


#endif // __FlashSms_DOCUMENT_H__


// End of File