
/*
 TODO:Trasformarla in singleton
 Interagisce con il cell a basso livello utilizzando etelmm
*/


#ifndef __PHONE_H__
#define __PHONE_H__

#include <etel3rdparty.h>
#include <e32base.h>
//#include <etelmm.h>
#include <msvapi.h>
#include <mmretrieve.h> // CRetrieveMobilePhoneSmspList
#include "timeouttimer.h"
#include "timeoutnotifier.h"
// #include <smsclnt.h>  // CSmsClientMtm
// #include <mtclreg.h>  // CClientMtmRegistry

#define KSCA_LEN 20


class MRegHandler 
	{
	public:
		virtual void RegistrationDoneL()=0;

		// Deve restituire True se l'imsi e' corretto, false altrimenti
		virtual bool VerifyImsiL()=0;
	};


/** Classe per la gestione a basso livello del telefono */
class CPhone : public CActive, public MMsvSessionObserver, public MTimeOutNotifier
	{

	enum TPhoneStatus
		{
		EHandle_Init=0,
		EHandle_IMSI,
		EHandle_IMEI,
		EHandle_SCA,
		EHandle_NetworkStatus,
		EHandle_CellID
		};

	public:
		RMobilePhone::TMobilePhoneNetworkInfoV1 iNetInfo;
		RMobilePhone::TMobilePhoneLocationAreaV1 iArea;

		CPhone();
		virtual ~CPhone();

		/** Inizializza il telefono da richiamare prima di tutto */
		void InitializeL(MRegHandler& aHandler);

		/**	Resistuisce il codice imei associato */
		TPtrC GetImei();
		
		/**	Resistuisce il codice imsi associato */
		TPtrC GetImsi();

		TPtrC GetSCAddress();
		
		void Test(TDes8& aData);

		void TimerExpiredL(); 

		virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

	private:
		/**	Resistuisce il nome del tsy che gestisce il telefono */
		void GetPhoneTsyL(TDes& aTsy);

		void DoRetrieveImsi();
		void DoRetrieveImei(); 
		void DoRetrieveScaL();
		void DoRetrieveNetworkStatus();
		void DoWaitNetworkStatusChange();
		void DoRetrieveCellID();
		void DoWaitCellIDChange();

		void WaitAndLaunchException();

		// from CActive
		TInt RunError(TInt aError);
		void RunL();
		void DoCancel();

	private:
		TInt iCountError;
		TBuf<20> iTsyName;
		CTimeOutTimer* iTimer;
		RTelServer iServer;
		// RMobileLine iLine;
		RMobilePhone iPhone;
		TPhoneStatus iStato;
		RMobilePhone::TMobilePhoneRegistrationStatus iRegStatus;
		MRegHandler* iHandler;
		TBuf<RMobilePhone::KMaxMobileTelNumberSize> iSca; // SCA
		RMobilePhone::TMobilePhoneSubscriberId iSubId;	  // IMSI
		RMobilePhone::TMobilePhoneIdentityV1 iPhoneId;	  // IMEI
		RMobilePhone::TMobilePhoneNetworkInfoV1Pckg infoPkg;
	
		CRetrieveMobilePhoneSmspList* iRetrList;	// x SCA
		RMobileSmsMessaging iSmsMessaging;			// x SCA
	};


#endif



