/*#ifdef __WINS__
	_LIT(KEmulatorImei, "123456789012345");
#endif*/

#include <f32file.h>
//#include <etelmm.h>
#include <Commdb.h>
#include "Phone.h"
#include <gsmuetel.h>
#include <gsmumsg.h>
#include <gsmubuf.h>
#include <smuthdr.h>
#include <txtfmlyr.h>
#include <txtrich.h>
#include <es_sock.h>
#include <smsuaddr.h>
#include <smsustrm.h>
#include <mtclreg.h>
#include <SMSCLNT.h>
//#include <etel3rdparty.h>

//#include <stdio.h>



CPhone::CPhone() : CActive(EPriorityStandard), infoPkg(iNetInfo)
	{
	CActiveScheduler::Add( this );
	iStato = EHandle_Init;
	}


CPhone::~CPhone() 
	{
	if (iTimer)
		delete iTimer;
	if (iRetrList) 
		delete iRetrList;

#ifndef __WINS__
	iSmsMessaging.Close();
#endif

	iPhone.Close();
	if (iTsyName.Length()>0) 
		iServer.UnloadPhoneModule(iTsyName);
	iServer.Close();
	}


TInt CPhone::RunError(TInt aError) 
	{
	return aError;
	}


void CPhone::InitializeL(MRegHandler& aHandler)
	{
	iHandler = &aHandler;

	User::LeaveIfError(iServer.Connect());

	GetPhoneTsyL(iTsyName);	
	User::LeaveIfError(iServer.LoadPhoneModule(iTsyName));

	// Get the details for the first (and only) phone.
	RTelServer::TPhoneInfo info;
	User::LeaveIfError(iServer.GetPhoneInfo(0, info));

	// Open the phone.
	User::LeaveIfError(iPhone.Open(iServer, info.iName));

#ifdef __WINS__
	iStato = EHandle_IMEI;
	iPhoneId.iSerialNumber = _L("357093007205366");
	iSubId = _L("IMSI2_890");
#else
	iStato = EHandle_Init;
	// Open the sms Messaging
	User::LeaveIfError(iSmsMessaging.Open( iPhone ));
#endif

	iTimer = CTimeOutTimer::NewL(EPriorityStandard, *this);
	iCountError = 0;
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();

	}



/// Metodi per invio delle richieste...
void CPhone::DoRetrieveImsi() 
	{
	iSubId.Zero();
	iPhone.GetSubscriberId(iStatus, iSubId);
	SetActive();
	}


void CPhone::DoRetrieveImei() 
	{
	iPhoneId.iSerialNumber.Zero();
	iPhone.GetPhoneId(iStatus, iPhoneId);
	SetActive();
	}


void CPhone::DoRetrieveScaL() 
	{
	if (iRetrList) delete iRetrList;
	iRetrList = CRetrieveMobilePhoneSmspList::NewL(iSmsMessaging);
	iRetrList->Start(iStatus);
	SetActive();
	}


void CPhone::DoRetrieveNetworkStatus() 
	{
	iRegStatus = RMobilePhone::ERegistrationUnknown;
	iPhone.GetNetworkRegistrationStatus(iStatus, iRegStatus);
	SetActive();
	}


void CPhone::DoWaitNetworkStatusChange() 
	{
	iPhone.NotifyNetworkRegistrationStatusChange(iStatus, iRegStatus);
	SetActive();
	}


void CPhone::DoRetrieveCellID() 
	{
	iNetInfo.iCountryCode.Zero();
	iPhone.GetCurrentNetwork(iStatus, infoPkg, iArea);
	SetActive();
	}


void CPhone::DoWaitCellIDChange() 
	{
	iPhone.NotifyCurrentNetworkChange(iStatus, infoPkg, iArea);
	SetActive();
	}


// Attende 5 secondi e poi richiama il metodo per gestire l'errore
void CPhone::WaitAndLaunchException() 
	{
	iCountError++;
	TTime time;
	time.HomeTime();

	// Adds <aNbSeconds> seconds to the time
	TTimeIntervalSeconds timeIntervalSeconds(5);
	time += timeIntervalSeconds;

	// Wait for timer
	iTimer->At(time);
	}


// Callback per gestire il Recovery degli Errori 
void CPhone::TimerExpiredL() 
	{
	switch (iStato) 
		{
		case EHandle_IMSI:		// Tenta una nuova lettura del codice IMSI
			{
			DoRetrieveImsi();
			break;
			}
		case EHandle_IMEI:		// Tenta una nuova lettura del codice IMEI
			{
			DoRetrieveImei();
			break;
			}
		case EHandle_SCA:		// Tenta una nuova lettura del SCA
			{
			DoRetrieveScaL();
			break;
			}
		case EHandle_NetworkStatus:	// Tenta un nuovo Retrieve o Wait del Network Status
			{
			if (iRegStatus == RMobilePhone::ERegistrationUnknown)
				DoRetrieveNetworkStatus();
			else
				DoWaitNetworkStatusChange();
			break;
			}
		case EHandle_CellID:
			{
			if (iNetInfo.iCountryCode.Length() == 0)
				DoRetrieveCellID();
			else
				DoWaitCellIDChange();
			break;
			}

		default:
			{ // Non si verifica mai
			break;
			}
		}
	}


void CPhone::RunL() 
	{

	if ( iStatus.Int() != KErrNone )
	{
		WaitAndLaunchException();	// Gestisce l'errore tra 5 secondi
		return;
	}

	switch (iStato) 
		{
		case EHandle_Init:
			{			
			DoRetrieveImei();
			iStato = EHandle_IMEI;
			break;
			}
		case EHandle_IMEI:
			{
			TInt i=0;
			while ((i<iPhoneId.iSerialNumber.Length()) && (iPhoneId.iSerialNumber[i]>='0') && (iPhoneId.iSerialNumber[i]<='9')) i++;
			if (i<iPhoneId.iSerialNumber.Length()) 
			{
				WaitAndLaunchException();	// Gestisce l'errore tra 5 secondi
				return;
			}

			// IMEI LETTO
			// CallBack per fare in modo che se la sim non e' cambiata venga mostrata
			// la gui, senza attendere la presenza della rete
			iHandler->VerifyImsiL();
			break;
			}
	
		}
	}



void CPhone::DoCancel()
	{
	switch (iStato) 
		{
		case EHandle_Init:
		break;
		case EHandle_IMSI:
			iPhone.CancelAsyncRequest(EMobilePhoneGetSubscriberId);
		break;
		case EHandle_IMEI:
			iPhone.CancelAsyncRequest(EMobilePhoneGetPhoneId);
		break;
		case EHandle_SCA:
//			iRetrList->CancelReq()
		break;
		case EHandle_NetworkStatus:
			if (iRegStatus == RMobilePhone::ERegistrationUnknown)
				iPhone.CancelAsyncRequest(EMobilePhoneGetNetworkRegistrationStatus);
			else
				iPhone.CancelAsyncRequest(EMobilePhoneNotifyNetworkRegistrationStatusChange);
		break;
		case EHandle_CellID:
			// In alternativa e' possibile controllare Handler
			if (iNetInfo.iCountryCode.Length() == 0)
				iPhone.CancelAsyncRequest(EMobilePhoneGetCurrentNetwork);
			else
				iPhone.CancelAsyncRequest(EMobilePhoneNotifyCurrentNetworkChange);
		break;
		}

	}


TPtrC CPhone::GetImei() 
	{
	return TPtrC(iPhoneId.iSerialNumber);
	}

		
TPtrC CPhone::GetImsi()
	{
	return TPtrC(iSubId);
	}


void CPhone::HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {}


TPtrC CPhone::GetSCAddress()
	{
	return TPtrC(iSca);
	}



void CPhone::GetPhoneTsyL(TDes& aTsy)
	{
	iServer.GetTsyName(0, aTsy);

//	A quanto pare va in crash su UIQ (NO, ANDAVA IN CRASH PERCHE' ERA COMPILATO CON SDK SERIES60)
	CCommsDatabase* db = CCommsDatabase::NewL(EDatabaseTypeUnspecified);
	CleanupStack::PushL(db);
	TUint32 modemId = 0;
	db->GetGlobalSettingL(TPtrC(MODEM_PHONE_SERVICES_SMS), modemId);
	CCommsDbTableView* const view = db->OpenViewMatchingUintLC(TPtrC(MODEM), TPtrC(COMMDB_ID), modemId);
	TInt err = view->GotoFirstRecord();
	User::LeaveIfError(err);
	view->ReadTextL(TPtrC(MODEM_TSY_NAME), aTsy);
	CleanupStack::PopAndDestroy(view);
	CleanupStack::PopAndDestroy(db);/**/
	}



void CPhone::Test(TDes8&)
	{
	
//	NotifyNetworkRegistrationStatusChange
//	TMobilePhoneRegistrationStatus
//		ERegisteredOnHomeNetwork  
//		ERegisteredRoaming

//	iPhone.GetCurrentNetwork();

/*__FLOG(_T8("Line1"));
		RMobileBroadcastMessaging::TGsmBroadcastMessageData iGsmMsgdata;
		RMobileBroadcastMessaging iBroadcastMsg;
		iBroadcastMsg.Open(iPhone);
        RMobileBroadcastMessaging::TMobileBroadcastAttributesV1 iAttrInfo;
        TPckg<RMobileBroadcastMessaging::TMobileBroadcastAttributesV1> iDes(iAttrInfo);       
        //Wait for the CBM
		TRequestStatus iReqStatus;
__FLOG(_T8("Line2"));
		RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter aSett;
		iBroadcastMsg.GetFilterSetting(aSett);
__FLOG_1(_T8("Filter:%d"), aSett);
		iBroadcastMsg.SetFilterSetting(iReqStatus, RMobileBroadcastMessaging::EBroadcastAcceptAll);
        iBroadcastMsg.ReceiveMessage(iReqStatus,iGsmMsgdata,iDes);
__FLOG(_T8("Line3"));
        User::WaitForRequest(iReqStatus);
__FLOG_1(_T8("RIS:%d"), iReqStatus.Int());
		if(iReqStatus.Int()==KErrNone){//show the CBM retrieved
                //first write the 88 bytes of CBm into a file
                //I am doing this as I will be using C
                //code to decode the data
                RFs fs;
                fs.Connect();
                RFile file;
                TBuf<32> aFileName = _L("C:\\log.txt");
                fs.Delete(aFileName);
                file.Replace(fs,aFileName,EFileWrite);
                file.Write(iGsmMsgdata);
                file.Close();
                fs.Close();

                //here starts the decoding code
                //Credits : Vikram K.
                FILE* fp;
                fp = fopen("c:\\log.txt","rb");
                char locationString[94];
                char cbuf;
                int char_cnt=0;
                unsigned int bb = 0;
                // 8-bit to 7-bit conversion
                unsigned char ur,curr,prev = 0;
                int cnt = 0;
                for(cnt = 0;cnt <6;cnt++)
                        fread(&cbuf,1,1,fp);
                while(fread(&cbuf,1,1,fp)){       
                        unsigned char aa = (1 << (7 - bb%7)) - 1;
                        ur = cbuf & aa;
                        ur = (ur << (bb)) | prev;
                        curr = cbuf & (0xff ^ aa);
                        curr = curr >> (7 - bb);                       
                        prev = curr;
                        if(ur == 0xd)
                        {
                                break;
                        }
               
                        locationString[char_cnt] = ur;

                        bb = ++bb % 7;               

                        char_cnt++;
                        if(bb==0)
                        {       
                                locationString[char_cnt++] = prev;
                                prev =0;
                        }
                }

                locationString[char_cnt] = '\0';
                fclose(fp);
                //decoding ends here now just
                //convert the C string to TBuf (Symbian format)
                int len=0;
                while(locationString[len] != NULL)  
                        len++;
                // Create empty descriptor
                HBufC* nameHeap = HBufC::NewLC(len);
                TPtr namePtr(nameHeap->Des());
                // Copy contents
                for(int i=0; i<len; i++)  
                namePtr.Append((TChar)locationString[i]);
                //now you have the location string
                //do whatever you want here..

                // Pop descriptor from cleanup stack
                CleanupStack::PopAndDestroy( nameHeap); */
/*		}

	RMobilePhone::TLineInfo info;
	RMobileLine::TCaps caps;

	iPhone.GetLineInfo(0, info);
	iLine.Open(iPhone, info.iName);

	//CTest* test = new (ELeave) CTest(iLine);
	//test->HookL(iLine);

__FLOG(_T8("New Call"));
	RMobileCall call;
	
	TBuf<40> callname;
	TInt risc = call.OpenNewCall(iLine, callname);
__FLOG_1(_T8("New Call Ris:%d"), risc);

__FLOG(_T8("Dial Call"));
	TBufC<20> number;

	RMobileCall::TMobileCallStatus iStatoLinea;

	number.Des().Copy(_L("0112217479"));
	call.Dial(number);

	call.Close();
	iLine.Close();*/
	}


