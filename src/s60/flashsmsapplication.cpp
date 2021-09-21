

// INCLUDE FILES
#include "FlashSmsDocument.h"
#include "FlashSmsApplication.h"


// ========================= MEMBER FUNCTIONS ==================================

// -----------------------------------------------------------------------------
// CFlashSmsApplication::CreateDocumentL()
// Create an FlashSms document, and return a pointer to it
// -----------------------------------------------------------------------------
//
CApaDocument* CFlashSmsApplication::CreateDocumentL()
    {
    return( static_cast< CApaDocument* >( CFlashSmsDocument::NewL( *this ) ) );
    }

// -----------------------------------------------------------------------------
// CFlashSmsApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CFlashSmsApplication::AppDllUid() const
    {
    return KUidFlashSmsApp;
    }

// End of File