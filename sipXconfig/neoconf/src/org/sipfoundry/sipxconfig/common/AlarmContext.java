/*
 *
 *
 * Copyright (C) 2008 Pingtel Corp., certain elements licensed under a Contributor Agreement.
 * Contributors retain copyright to elements licensed under a Contributor Agreement.
 * Licensed to the User under the LGPL license.
 *
 * $
 */
package org.sipfoundry.sipxconfig.common;

import java.util.Date;
import java.util.List;

import org.sipfoundry.sipxconfig.admin.alarm.Alarm;
import org.sipfoundry.sipxconfig.admin.alarm.AlarmEvent;
import org.sipfoundry.sipxconfig.admin.alarm.AlarmServer;
import org.sipfoundry.sipxconfig.admin.commserver.Location;
import org.sipfoundry.sipxconfig.admin.commserver.SipxReplicationContext;

public interface AlarmContext {
    public static enum Command {
        RAISE_ALARM, GET_ALARM_COUNT, RELOAD_ALARMS;
    }

    /**
     * Send an alarm to the Alarm Server with a list of runtime parameters.
     * 
     * @param unique internal alarm id
     * @param list of runtime parameters
     */
    public void raiseAlarm(String alarmId, String... alarmParams);

    /**
     * Send request to Alarm Server to reload alarms (after config or language change).
     */
    public void reloadAlarms();

    public AlarmServer getAlarmServer();

    public List<Alarm> getAlarmTypes(String configPath, String stringPath);

    public void deployAlarmConfiguration(AlarmServer alarmServer, List<Alarm> alarms);

    public void replicateAlarmServer(SipxReplicationContext replicationContext, Location location);
    
    public String getSipxUser();

    public String getConfigDirectory();

    public String getAlarmsStringsDirectory();

    public List<AlarmEvent> getAlarmEvents(String host, Date startDate, Date endDate);

    public List<AlarmEvent> getAlarmEventsByPage(String host, Date startDate, Date endDate, int first, int pageSize);
}
