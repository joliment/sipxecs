/*
 *
 *
 * Copyright (C) 2009 Pingtel Corp., certain elements licensed under a Contributor Agreement.
 * Contributors retain copyright to elements licensed under a Contributor Agreement.
 * Licensed to the User under the LGPL license.
 *
 *
 */
package org.sipfoundry.sipxconfig.admin.logging;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.MDC;
import org.sipfoundry.sipxconfig.admin.commserver.Location;

public class AuditLogContextImpl implements AuditLogContext {

    /**
     * 
     */
    private static final String EVENT_TYPE = "eventType";
    private static final String REPLICATION_EVENT = "Replication";
    private static final String CONFIG_CHANGE_EVENT = "ConfigChange";

    private static final Log AUDIT_LOG = LogFactory.getLog("org.sipfoundry.sipxconfig.auditlog");
    private String m_configType;

    @Override
    public void logReplication(String dataName, Location location) {
        MDC.put(EVENT_TYPE, REPLICATION_EVENT);
        log("Replicated " + dataName + " to " + location.getFqdn());
    }

    @Override
    public void logConfigChange(CONFIG_CHANGE_TYPE changeType, String configType, String configName) {
        MDC.put(EVENT_TYPE, CONFIG_CHANGE_EVENT);
        StringBuffer message = new StringBuffer();
        message.append(changeType);
        message.append(' ');
        message.append(configType);
        message.append(' ');
        message.append('\'');
        message.append(configName);
        message.append('\'');

        String escapedString = StringUtils.escape(message.toString());

        log(escapedString);
    }

    private void log(String message) {
        AUDIT_LOG.info(message);
    }
}
