/*
 * 
 * 
 * Copyright (C) 2007 Pingtel Corp., certain elements licensed under a Contributor Agreement.  
 * Contributors retain copyright to elements licensed under a Contributor Agreement.
 * Licensed to the User under the LGPL license.
 * 
 * $
 */
package org.sipfoundry.sipxconfig.gateway.audiocodes;

import java.util.Set;

import org.sipfoundry.sipxconfig.phone.PhoneModel;

public class AudioCodesFxsModel extends PhoneModel {
    private boolean m_fxo;
    private boolean m_fxs;
    private boolean m_digital;
    private String m_configDirectory;

    public void setFxo(boolean fxo) {
        m_fxo = fxo;
    }

    public void setFxs(boolean fxs) {
        m_fxs = fxs;
    }

    public void setDigital(boolean digital) {
        m_digital = digital;
    }

    public void setConfigDirectory(String configDirectory) {
        m_configDirectory = configDirectory;
    }

    public boolean isFxs() {
        return m_fxs;
    }

    public boolean isFxo() {
        return m_fxo;
    }

    public boolean isDigital() {
        return m_digital;
    }

    public Set getDefinitions() {
        Set definitions = super.getDefinitions();
        if (m_fxo) {
            definitions.add("fxo");
        }
        if (m_fxs) {
            definitions.add("fxs");
        }
        if (m_digital) {
            definitions.add("digital");
        }
        return definitions;
    }

    public String getConfigDirectory() {
        return m_configDirectory;
    }

}
