// 
// Copyright (C) 2008 Pingtel Corp., certain elements licensed under a Contributor Agreement.  
// Contributors retain copyright to elements licensed under a Contributor Agreement.
// Licensed to the User under the LGPL license.
// 
//////////////////////////////////////////////////////////////////////////////

// SYSTEM INCLUDES
// APPLICATION INCLUDES
#include "alarm/AlarmRequestTask.h"
#include "net/XmlRpcRequest.h"
#include "os/OsSysLog.h"
#include "utl/UtlSListIterator.h"
#include "sipXecsService/SipXecsService.h"

// STATIC INITIALIZATIONS
const UtlContainableType AsynchAlarmMsg::TYPE = "AsynchAlarmMsg";
AlarmRequestTask* AlarmRequestTask::spAlarmRequestTask = NULL;
OsMutex           AlarmRequestTask::sLockMutex (OsMutex::Q_FIFO);

// DEFINES
#define RAISE_ALARM_METHOD             "Alarm.raiseAlarm"

// Misc
// CONSTANTS
// TYPEDEFS
// FORWARD DECLARATIONS


AlarmRequestTask::AlarmRequestTask()
{
   // pull Alarm Server address out of config file
   initXMLRPCsettings();
   
   // start the task which will listen for messages and send xmlrpc requests
   start();
}

AlarmRequestTask::~AlarmRequestTask()
{
   // Critical Section here
   OsLock lock( sLockMutex );
   
   // reset the static instance pointer to NULL
   spAlarmRequestTask = NULL;
}

AlarmRequestTask* AlarmRequestTask::getInstance()
{
    // Critical Section here
    OsLock lock( sLockMutex );

    // See if this is the first time through for this process
    if ( spAlarmRequestTask == NULL )
    {   // Create the singleton class for clients to use
       spAlarmRequestTask = new AlarmRequestTask();
    }

    return spAlarmRequestTask;
}


void AlarmRequestTask::raiseAlarm(const UtlString& alarmId, const UtlSList& alarmParams )
{
   OsSysLog::add(FAC_ALARM, PRI_INFO,
                 "AlarmRequestTask::raiseAlarm( %s, %s )", mLocalHostname.data(), alarmId.data() );
   AsynchAlarmMsg message( AsynchAlarmMsg::RAISE_ALARM, mLocalHostname, alarmId, alarmParams );
   postMessage( message );
}


// Parses the domain-config file and extracts the Alarm Server URL.  
// Returns OS_SUCCESS only if a valid URL was found 
OsStatus AlarmRequestTask::initXMLRPCsettings()
{
   OsStatus retval = OS_FAILED;

   UtlString  alarmServerUrl;

   OsConfigDb domainConfiguration;
   OsPath     domainConfigPath = SipXecsService::domainConfigPath();

   if (OS_SUCCESS == domainConfiguration.loadFromFile(domainConfigPath.data()))
   {
      domainConfiguration.get(SipXecsService::DomainDbKey::SIP_DOMAIN_NAME, mLocalHostname);
      domainConfiguration.get(SipXecsService::DomainDbKey::ALARM_SERVER_URL, alarmServerUrl);

      if (alarmServerUrl.isNull())
      {
         OsSysLog::add(FAC_ALARM, PRI_ERR, "Alarm::initXMLRPCsettings: failed to find Alarm Server URL in '%s'",
               domainConfigPath.data() );
      }
      else
      {
         // Construct the URL to the Alarm Server's XMLRPC server.
         Url alarmUrl(alarmServerUrl, Url::AddrSpec);
         if (alarmUrl.getScheme() != Url::HttpsUrlScheme)
         {
            OsSysLog::add(FAC_ALARM, PRI_ERR,
                  "Alarm::initXMLRPCsettings: badly formed alarm server url '%s'",
                  alarmServerUrl.data() );
         }
         else
         {
            mAlarmServerUrl = alarmUrl;
            mAlarmServerUrl.setPath("RPC2");
            retval = OS_SUCCESS;
         }
      }
   }
   else
   {
      OsSysLog::add(FAC_ALARM, PRI_ERR,
            "Alarm::initXMLRPCsettings: failed to load domain configuration from '%s'",
            domainConfigPath.data() );
   }

   return retval;
}


UtlBoolean AlarmRequestTask::handleMessage( OsMsg& rMsg )
{
   UtlBoolean handled = FALSE;
   XmlRpcRequest* pXmlRpcRequestToSend = 0;
   AsynchAlarmMsg* pAlarmMsg = dynamic_cast <AsynchAlarmMsg*> ( &rMsg );
   
   UtlString localHostname       = pAlarmMsg->getLocalHostname();
   UtlString alarmId             = pAlarmMsg->getAlarmId();
   UtlSListIterator iterator(pAlarmMsg->getAlarmParams());
   UtlString* pObject;
   UtlSList  alarmParams;
   while ( (pObject = dynamic_cast<UtlString*>(iterator())))
   {
      alarmParams.append(pObject);
   }
   
   switch ( rMsg.getMsgType() )
   {
   case OsMsg::OS_EVENT:
      switch( rMsg.getMsgSubType() )
      {
      case AsynchAlarmMsg::RAISE_ALARM:
         pXmlRpcRequestToSend = new XmlRpcRequest( mAlarmServerUrl, RAISE_ALARM_METHOD );
         pXmlRpcRequestToSend->addParam( &localHostname );
         pXmlRpcRequestToSend->addParam( &alarmId );
         pXmlRpcRequestToSend->addParam( &alarmParams );
         break;
         
      default:
         OsSysLog::add(FAC_ALARM, PRI_CRIT,
                       "AlarmRequestTask::handleMessage: received unknown sub-type: %d",
                       rMsg.getMsgSubType() );
         break;
      }
      handled = TRUE;
      break;
      
   default:
      OsSysLog::add(FAC_ALARM, PRI_CRIT,
                    "AlarmRequestTask::handleMessage: '%s' unhandled message type %d.%d",
                    mName.data(), rMsg.getMsgType(), rMsg.getMsgSubType());
      break;
   }

   if( pXmlRpcRequestToSend )
   {
      XmlRpcResponse response1;
      if (!pXmlRpcRequestToSend->execute(response1)) // blocks; returns false for any fault
      {
         // The XMLRPC request failed.
         int faultCode;
         UtlString faultString;
         response1.getFault(&faultCode, faultString);         
         OsSysLog::add(FAC_ALARM, PRI_CRIT, "Alarm.raiseAlarm failed, fault %d : %s",
                       faultCode, faultString.data());
         // since we could not send the alarm, log it here
         faultString = "Failed to report alarm ";
         faultString.append(alarmId.data());
         UtlSListIterator iterator(alarmParams);
         while ( (pObject = dynamic_cast<UtlString*>(iterator())))
         {
            faultString.append(" ");
            faultString.append(*pObject);
         }
         OsSysLog::add(FAC_ALARM, PRI_CRIT, faultString);
      }
   }
   delete pXmlRpcRequestToSend;
   return handled;
}

//////////////////////////////////////////////////////////////////////////////
AsynchAlarmMsg::AsynchAlarmMsg(EventSubType eventSubType,
                               const UtlString& localHostname,
                               const UtlString& alarmId,
                               const UtlSList& alarmParams) :
   OsMsg( OS_EVENT, eventSubType ),
   mLocalHostname( localHostname ),
   mAlarmId( alarmId )
{
   UtlSListIterator iterator(alarmParams);
   UtlString* pObject;
   mAlarmParams.removeAll();
   while ( (pObject = dynamic_cast<UtlString*>(iterator())))
   {
      mAlarmParams.append(new UtlString(*pObject));
   }
}
         
// deep copy of alarm and parameters
AsynchAlarmMsg::AsynchAlarmMsg( const AsynchAlarmMsg& rhs) :
   OsMsg( OS_EVENT, rhs.getMsgSubType() ),
   mLocalHostname( rhs.getLocalHostname() ),
   mAlarmId( rhs.getAlarmId() )
{
   UtlSListIterator iterator(rhs.getAlarmParams());
   UtlString* pObject;
   mAlarmParams.removeAll();
   while ( (pObject = dynamic_cast<UtlString*>(iterator())))
   {
      mAlarmParams.append(new UtlString(*pObject));
   }
}

AsynchAlarmMsg::~AsynchAlarmMsg()
{
   mAlarmParams.destroyAll();
}

OsMsg* AsynchAlarmMsg::createCopy( void ) const
{
   return new AsynchAlarmMsg( *this );
}

const UtlString& AsynchAlarmMsg::getLocalHostname( void ) const
{
   return mLocalHostname;
}

const UtlString& AsynchAlarmMsg::getAlarmId( void ) const
{
   return mAlarmId;
}

const UtlSList& AsynchAlarmMsg::getAlarmParams( void ) const
{
   return mAlarmParams;
}
