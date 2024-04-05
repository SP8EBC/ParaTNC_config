#include "stdafx.h"
#include "DiagnosticDescription.h"

std::vector<DDD> vDiagnosticDescription_DidDefs;

void DiagnosticDescription_Init()
{
	DDD def;

	def.id = 0x1000U;
	//def.didDescription =_T("MASTER_TIME");
	_tcscpy_s(def.didNameOnList, DIAGNOSTICDESCRIPTION_DID_NAME_ON_LIST_LN, _T("MASTER_TIME"));
	_tcscpy_s(def.didName, DIAGNOSTICDESCRIPTION_DID_NAME_LN, _T("Controller Master Time"));
	_tcscpy_s(def.didDescription, DIAGNOSTICDESCRIPTION_DID_DESCRIPTION_LN, _T("Uptime since power up"));

	def.first.size = DIDRESPONSE_DATASIZE_INT32;
	_tcscpy_s(def.first.name, DIAGNOSTICDESCRIPTION_DATA_NAME_LN, _T("master_time"));

	vDiagnosticDescription_DidDefs.push_back(def);


	def.id = 0x1001U;
	//def.didDescription =_T("MASTER_TIME");
	_tcscpy_s(def.didNameOnList, DIAGNOSTICDESCRIPTION_DID_NAME_ON_LIST_LN, _T("RX_10MIN_COUNTER"));
	_tcscpy_s(def.didName, DIAGNOSTICDESCRIPTION_DID_NAME_LN, _T("Receive packets counter"));
	_tcscpy_s(def.didDescription, DIAGNOSTICDESCRIPTION_DID_DESCRIPTION_LN, _T("How many packets have been received from RF network in 10 minutes. Counter is reset after each telemetry values frame."));

	def.first.size = DIDRESPONSE_DATASIZE_INT32;
	_tcscpy_s(def.first.name, DIAGNOSTICDESCRIPTION_DATA_NAME_LN, _T("rx10min"));

	vDiagnosticDescription_DidDefs.push_back(def);


	def.id = 0x9999U;
	//def.didDescription =_T("MASTER_TIME");
	_tcscpy_s(def.didNameOnList, DIAGNOSTICDESCRIPTION_DID_NAME_ON_LIST_LN, _T("TEST_NOEXIST"));
	_tcscpy_s(def.didName, DIAGNOSTICDESCRIPTION_DID_NAME_LN, _T("Test DID which doesn't exist"));
	_tcscpy_s(def.didDescription, DIAGNOSTICDESCRIPTION_DID_DESCRIPTION_LN, _T("To check if this app correctly handles NRC."));

	def.first.size = DIDRESPONSE_DATASIZE_INT32;
	_tcscpy_s(def.first.name, DIAGNOSTICDESCRIPTION_DATA_NAME_LN, _T("rx10min"));

	vDiagnosticDescription_DidDefs.push_back(def);
}
