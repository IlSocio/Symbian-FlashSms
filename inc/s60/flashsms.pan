/* Copyright (c) 2004, Nokia. All rights reserved */

#ifndef __FLASHSMS_PAN__
#define __FLASHSMS_PAN__


// MultiViews application panic codes 
enum TFlashSmsPanics 
    {
    EFlashSmsBasicUi = 1
    // add further panics here
    };

inline void Panic( TFlashSmsPanics aReason )
    {
    _LIT( applicationName,"FlashSms" );
    User::Panic( applicationName, aReason );
    }

#endif // __MULTIVIEWS_PAN__

// End of File