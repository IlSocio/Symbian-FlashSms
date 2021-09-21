

// INCLUDE FILES
#include "FlashSmsAppUi.h"
#include "FlashSmsDocument.h"


// ========================= MEMBER FUNCTIONS ==================================


// -----------------------------------------------------------------------------
// CFlashSmsDocument::CFlashSmsDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CFlashSmsDocument::CFlashSmsDocument( CEikApplication& aApp ) :
                                          CAknDocument( aApp )
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CFlashSmsDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFlashSmsDocument* CFlashSmsDocument::NewL( CEikApplication& aApp )
    {
    CFlashSmsDocument* self = NewLC( aApp );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CFlashSmsDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFlashSmsDocument* CFlashSmsDocument::NewLC( CEikApplication& aApp )
    {
    CFlashSmsDocument* self = new ( ELeave ) CFlashSmsDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CFlashSmsDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFlashSmsDocument::ConstructL()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CFlashSmsDocument::~CFlashSmsDocument()
// Destructor.
// -----------------------------------------------------------------------------
//
CFlashSmsDocument::~CFlashSmsDocument()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CFlashSmsDocument::CreateAppUiL()
// Create the application user interface, and return a pointer to it
// -----------------------------------------------------------------------------
//
CEikAppUi* CFlashSmsDocument::CreateAppUiL()
    {
    return( static_cast<CEikAppUi*>( new ( ELeave ) CFlashSmsAppUi ) );
    }

// End of File
