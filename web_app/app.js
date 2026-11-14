/**
 * CYBER-RELAY RTOS // Frontend Application Controller v3.0
 * Features:
 * 1. Dedicated Login & Permanent Session Management (Stored forever on phone).
 * 2. High-performance Local AP (192.168.4.1) & Firebase Cloud (Worldwide) connectivity.
 * 3. 3-Tier User Security (Admin, Guarded Operator 1-Time Session, Guest Analytics).
 * 4. Monthly Switch Usage Analytics & Custom Timing Calibration.
 * 5. Direct Firebase REST integration (100% reliable, zero external CDN scripts).
 */

const App = (() => {
  // Predefined users matching system requirements
  const PREDEFINED_USERS = [
    { id: 1, username: 'Yash_Gajjar', role: 'admin', label: 'Super Administrator', password: 'Iamyashacker@17' },
    { id: 2, username: 'Mahi_Gajjar', role: 'guarded_operator', label: 'Guarded Operator (1-Time Grant)', password: 'MahiSecure#2026' },
    { id: 3, username: 'Guest', role: 'guest_analytics', label: 'Guest Analytics (Read-Only)', password: 'Guest@123' }
  ];

  const GAUGE_CIRCUMFERENCE = 691.15; // 2 * PI * 110

  // Central Application State
  const state = {
    connected: false,
    connectionMode: 'local', // 'local' or 'cloud'
    pollInterval: null,
    uptimeTimer: null,
    simulated: false,
    consecutiveErrors: 0,

    // Network Endpoints
    espHost: 'http://192.168.4.1',
    firebaseHost: 'cyber-relay-rtos-default-rtdb.firebaseio.com',
    firebaseAuth: '',

    // Live Hardware & RTOS State
    system: {
      mode: 'IDLE', // 'IDLE', 'RELAY_ON', 'LOCKOUT'
      relayActive: false,
      touchActive: true,
      lockoutActive: false,
      onRemainingSec: 0,
      totalOnSec: 8 * 3600,
      lockoutRemainingSec: 0,
      totalLockoutSec: 6 * 86400,
      uptimeSeconds: 0
    },

    // Monthly Analytics (User 3 Requirement)
    analytics: {
      month: 'November 2026',
      totalOnHours: 48.5,
      totalOffHours: 240.2,
      totalCycles: 6,
      dutyPercent: 16.8
    },

    // Timer Calibration Config
    config: {
      onHours: 8.0,
      lockoutDays: 6.0
    },

    // Current User Session
    currentUser: null,
    users: [...PREDEFINED_USERS],
    oneTimeToken: '',

    // One-time session tracking for Mahi_Gajjar
    pendingRequest: {
      active: false,
      requestId: '',
      username: '',
      action: ''
    },
    waitingForApproval: false
  };

  // ---------------------------------------------------------------------------
  // INITIALIZATION & LIFECYCLE
  // ---------------------------------------------------------------------------
  function init() {
    loadSavedSettings();
    loadCustomUsers();
    setupEventListeners();

    // Check for permanent session on phone
    const hasActiveSession = checkAndRestoreSession();

    if (hasActiveSession) {
      showDashboardView();
      startPolling();
    } else {
      showLoginView();
    }

    startLocalUptime();
  }

  function setupEventListeners() {
    document.addEventListener('keydown', (e) => {
      if (e.key === 'Escape') {
        dismissAdminApprovalModal();
        closeCloudConfigModal();
      }
    });
  }

  // ---------------------------------------------------------------------------
  // SESSION PERSISTENCE (FOREVER ON PHONE)
  // ---------------------------------------------------------------------------
  function checkAndRestoreSession() {
    try {
      // 1. First attempt native Android SharedPreferences
      if (window.AndroidNative && typeof window.AndroidNative.getSession === 'function') {
        const rawNative = window.AndroidNative.getSession();
        if (rawNative && rawNative !== '{}') {
          const nativeData = JSON.parse(rawNative);
          if (nativeData && nativeData.isLoggedIn && nativeData.username) {
            const matched = findUser(nativeData.username);
            if (matched) {
              state.currentUser = { ...matched, role: nativeData.role || matched.role, token: nativeData.token };
              console.log('[SESSION] Restored from Android Native SharedPreferences:', state.currentUser.username);
              return true;
            }
          }
        }
      }

      // 2. Fallback to browser localStorage
      const localRaw = localStorage.getItem('cyberrelay_session');
      if (localRaw) {
        const localData = JSON.parse(localRaw);
        if (localData && localData.username) {
          const matched = findUser(localData.username);
          if (matched) {
            state.currentUser = { ...matched, role: localData.role || matched.role, token: localData.token };
            console.log('[SESSION] Restored from localStorage:', state.currentUser.username);
            return true;
          }
        }
      }
    } catch (e) {
      console.warn('[SESSION] Failed to restore saved session:', e);
    }
    return false;
  }

  function savePermanentSession(user, token) {
    state.currentUser = user;
    const sessionPayload = {
      isLoggedIn: true,
      username: user.username,
      role: user.role,
      token: token || `token-${Date.now()}`,
      timestamp: Date.now()
    };

    // Save to localStorage
    try {
      localStorage.setItem('cyberrelay_session', JSON.stringify(sessionPayload));
      localStorage.setItem('cyberrelay_keep_logged_in', 'true');
    } catch (ignored) {}

    // Save to Android Native SharedPreferences
    if (window.AndroidNative && typeof window.AndroidNative.saveSession === 'function') {
      try {
        window.AndroidNative.saveSession(user.username, user.role, sessionPayload.token);
      } catch (ignored) {}
    }
  }

  function clearPermanentSession() {
    state.currentUser = null;
    try {
      localStorage.removeItem('cyberrelay_session');
      localStorage.removeItem('cyberrelay_keep_logged_in');
    } catch (ignored) {}

    if (window.AndroidNative && typeof window.AndroidNative.clearSession === 'function') {
      try {
        window.AndroidNative.clearSession();
      } catch (ignored) {}
    }
  }

  function loadSavedSettings() {
    // Load ESP Host
    try {
      if (window.AndroidNative && typeof window.AndroidNative.getEspHost === 'function') {
        const h = window.AndroidNative.getEspHost();
        if (h) state.espHost = h;
      } else {
        const h = localStorage.getItem('cyberrelay_esp_host');
        if (h) state.espHost = h;
      }
    } catch (ignored) {}

    // Load Firebase Config
    try {
      if (window.AndroidNative && typeof window.AndroidNative.getFirebaseConfig === 'function') {
        const raw = window.AndroidNative.getFirebaseConfig();
        if (raw) {
          const cfg = JSON.parse(raw);
          if (cfg.host) state.firebaseHost = cfg.host;
          if (cfg.auth) state.firebaseAuth = cfg.auth;
        }
      } else {
        const fh = localStorage.getItem('cyberrelay_firebase_host');
        const fa = localStorage.getItem('cyberrelay_firebase_auth');
        if (fh) state.firebaseHost = fh;
        if (fa) state.firebaseAuth = fa;
      }
    } catch (ignored) {}

    // Load preferred connection mode
    const savedMode = localStorage.getItem('cyberrelay_conn_mode');
    if (savedMode === 'cloud' || savedMode === 'local') {
      state.connectionMode = savedMode;
    }
  }

  function loadCustomUsers() {
    try {
      let customUsers = [];
      if (window.AndroidNative && typeof window.AndroidNative.getUsersJson === 'function') {
        const raw = window.AndroidNative.getUsersJson();
        if (raw && raw !== '[]') customUsers = JSON.parse(raw);
      } else {
        const raw = localStorage.getItem('cyberrelay_custom_users');
        if (raw) customUsers = JSON.parse(raw);
      }

      if (Array.isArray(customUsers)) {
        customUsers.forEach(u => {
          if (!state.users.some(existing => existing.username.toLowerCase() === u.username.toLowerCase())) {
            state.users.push(u);
          }
        });
      }
    } catch (ignored) {}
  }

  function findUser(username) {
    if (!username) return null;
    return state.users.find(u => u.username.toLowerCase() === username.trim().toLowerCase());
  }

  // ---------------------------------------------------------------------------
  // AUTHENTICATION UI & ACTIONS
  // ---------------------------------------------------------------------------
  function showLoginView() {
    const loginSec = document.getElementById('loginSection');
    const dashSec = document.getElementById('dashboardSection');
    if (loginSec) loginSec.classList.remove('hidden');
    if (dashSec) dashSec.classList.add('hidden');
    updateConnectionModeButtons();
  }

  function showDashboardView() {
    const loginSec = document.getElementById('loginSection');
    const dashSec = document.getElementById('dashboardSection');
    if (loginSec) loginSec.classList.add('hidden');
    if (dashSec) dashSec.classList.remove('hidden');

    updateUI();
    renderUserTable();
  }

  function switchAuthTab(tab) {
    const tabSignIn = document.getElementById('tabBtnSignIn');
    const tabRegister = document.getElementById('tabBtnRegister');
    const contentSignIn = document.getElementById('tabContentSignIn');
    const contentRegister = document.getElementById('tabContentRegister');

    if (tab === 'signin') {
      tabSignIn.classList.add('active');
      tabRegister.classList.remove('active');
      contentSignIn.classList.add('active');
      contentRegister.classList.remove('active');
    } else {
      tabRegister.classList.add('active');
      tabSignIn.classList.remove('active');
      contentRegister.classList.add('active');
      contentSignIn.classList.remove('active');
    }
  }

  function fillPreset(key) {
    const userField = document.getElementById('inputUsername');
    const passField = document.getElementById('inputPassword');

    let preset = PREDEFINED_USERS[0];
    if (key === 'mahi') preset = PREDEFINED_USERS[1];
    if (key === 'guest') preset = PREDEFINED_USERS[2];

    if (userField && passField) {
      userField.value = preset.username;
      passField.value = preset.password;
      vibrate(30);
    }
  }

  function togglePasswordVisibility(fieldId) {
    const f = document.getElementById(fieldId);
    if (f) {
      f.type = f.type === 'password' ? 'text' : 'password';
    }
  }

  async function handleSignInSubmit() {
    const username = document.getElementById('inputUsername').value.trim();
    const password = document.getElementById('inputPassword').value;
    const remember = document.getElementById('checkRememberSession').checked;

    if (!username || !password) {
      toast('Please enter username and password', 'error');
      return;
    }

    const matched = findUser(username);
    if (!matched) {
      vibrate(200);
      toast('Unknown user account. Register below if new.', 'error');
      return;
    }

    if (matched.password !== password) {
      vibrate(200);
      toast('Invalid credentials. Please re-enter password.', 'error');
      return;
    }

    // Success!
    vibrate(60);
    toast(`Welcome, ${matched.username}! Access Granted.`, 'success');

    if (remember) {
      savePermanentSession(matched);
    } else {
      state.currentUser = matched;
    }

    showDashboardView();
    startPolling();
  }

  function handleRegisterSubmit() {
    const username = document.getElementById('regUsername').value.trim();
    const password = document.getElementById('regPassword').value;
    const role = document.getElementById('regRole').value;
    const remember = document.getElementById('checkRegRememberSession').checked;

    if (!username || !password) {
      toast('Please enter a username and password', 'error');
      return;
    }

    if (findUser(username)) {
      toast('Username already exists. Please sign in.', 'error');
      return;
    }

    const newUser = {
      id: Date.now(),
      username: username,
      role: role,
      label: role === 'admin' ? 'Super Administrator' : (role === 'guarded_operator' ? 'Guarded Operator' : 'Guest Analytics'),
      password: password
    };

    state.users.push(newUser);

    // Save custom users to phone storage
    try {
      const customOnly = state.users.filter(u => !PREDEFINED_USERS.some(p => p.username.toLowerCase() === u.username.toLowerCase()));
      localStorage.setItem('cyberrelay_custom_users', JSON.stringify(customOnly));
      if (window.AndroidNative && typeof window.AndroidNative.saveUsersJson === 'function') {
        window.AndroidNative.saveUsersJson(JSON.stringify(customOnly));
      }
    } catch (ignored) {}

    vibrate(80);
    toast(`Account '${username}' created successfully!`, 'success');

    if (remember) {
      savePermanentSession(newUser);
    } else {
      state.currentUser = newUser;
    }

    showDashboardView();
    startPolling();
  }

  function logout() {
    if (confirm('Log out from Cyber-Relay RTOS and return to Sign-in screen?')) {
      clearPermanentSession();
      if (state.pollInterval) clearInterval(state.pollInterval);
      toast('Logged out successfully', 'info');
      showLoginView();
    }
  }

  // ---------------------------------------------------------------------------
  // CONNECTION MODE & DATA POLLING
  // ---------------------------------------------------------------------------
  function setConnectionMode(mode) {
    state.connectionMode = mode;
    try {
      localStorage.setItem('cyberrelay_conn_mode', mode);
    } catch (ignored) {}

    updateConnectionModeButtons();

    if (mode === 'cloud') {
      toast('Switched to Worldwide Firebase Cloud Mode', 'info');
      const ft = document.getElementById('ftCloudStatus');
      if (ft) ft.textContent = 'Firebase Cloud Active';
    } else {
      toast(`Switched to Local AP Mode (${state.espHost})`, 'info');
      const ft = document.getElementById('ftCloudStatus');
      if (ft) ft.textContent = 'Local AP Active';
    }

    // Refresh instantly
    fetchStatus();
  }

  function updateConnectionModeButtons() {
    const isLocal = state.connectionMode === 'local';
    ['btnModeLocal', 'authBtnLocal'].forEach(id => {
      const el = document.getElementById(id);
      if (el) el.className = isLocal ? 'mode-btn active' : 'mode-btn';
    });
    ['btnModeCloud', 'authBtnCloud'].forEach(id => {
      const el = document.getElementById(id);
      if (el) el.className = !isLocal ? 'mode-btn active' : 'mode-btn';
    });
    updateConnectionStatus();
  }

  function startPolling() {
    fetchStatus();
    if (state.pollInterval) clearInterval(state.pollInterval);
    state.pollInterval = setInterval(fetchStatus, 1500);
  }

  async function fetchStatus() {
    if (state.simulated) {
      tickSimulation();
      return;
    }

    if (state.connectionMode === 'cloud') {
      await fetchFirebaseTelemetry();
    } else {
      await fetchLocalEspStatus();
    }
  }

  // 1. Fetch from Local ESP8266 (192.168.4.1)
  async function fetchLocalEspStatus() {
    const url = `${state.espHost.replace(/\/+$/, '')}/api/status`;
    try {
      const controller = new AbortController();
      const timeoutId = setTimeout(() => controller.abort(), 2500);

      const response = await fetch(url, {
        signal: controller.signal,
        headers: {
          'Authorization': state.currentUser ? `Bearer ${state.currentUser.token || 'auth'}` : ''
        }
      });
      clearTimeout(timeoutId);

      if (!response.ok) throw new Error(`HTTP ${response.status}`);

      const data = await response.json();
      applyTelemetryData(data);
      state.connected = true;
      state.consecutiveErrors = 0;
      hideConnNotice();
      updateUI();
    } catch (err) {
      handleConnectionFailure(err, 'Local AP');
    }
  }

  // 2. Fetch from Firebase Realtime Cloud Database (Worldwide)
  async function fetchFirebaseTelemetry() {
    let host = state.firebaseHost.trim().replace(/^https?:\/\//, '').replace(/\/+$/, '');
    let url = `https://${host}/esp8266_relay/telemetry.json`;
    if (state.firebaseAuth && state.firebaseAuth.trim().length > 0) {
      url += `?auth=${encodeURIComponent(state.firebaseAuth.trim())}`;
    }

    try {
      const controller = new AbortController();
      const timeoutId = setTimeout(() => controller.abort(), 3500);

      const response = await fetch(url, {
        signal: controller.signal,
        headers: { 'Accept': 'application/json' }
      });
      clearTimeout(timeoutId);

      if (!response.ok) throw new Error(`Firebase HTTP ${response.status}`);

      const data = await response.json();
      if (!data) throw new Error('Empty Firebase telemetry document');

      applyTelemetryData(data);
      state.connected = true;
      state.consecutiveErrors = 0;
      hideConnNotice();
      updateUI();
    } catch (err) {
      handleConnectionFailure(err, 'Firebase Cloud');
    }
  }

  function handleConnectionFailure(err, context) {
    state.consecutiveErrors++;
    if (state.connected) {
      state.connected = false;
      toast(`${context} connection lost`, 'error');
    }

    // After 2 consecutive failures, display non-intrusive connection assistance banner
    if (state.consecutiveErrors >= 2) {
      showConnNotice();
    }

    updateUI();
  }

  function showConnNotice() {
    const banner = document.getElementById('connNoticeBanner');
    const text = document.getElementById('connNoticeText');
    if (banner && text) {
      if (state.connectionMode === 'local') {
        text.textContent = `Connecting to ESP8266 (${state.espHost})... Not connected to Wi-Fi 'CYBER-RELAY-HUB'?`;
      } else {
        text.textContent = `Connecting to Firebase Cloud (${state.firebaseHost})... Check internet connection.`;
      }
      banner.classList.remove('hidden');
    }
  }

  function hideConnNotice() {
    const banner = document.getElementById('connNoticeBanner');
    if (banner) banner.classList.add('hidden');
  }

  function dismissConnNotice() {
    hideConnNotice();
  }

  // Apply parsed telemetry from ESP or Firebase
  function applyTelemetryData(data) {
    state.system.mode = data.mode || 'IDLE';
    state.system.relayActive = data.relayActive === true;
    state.system.touchActive = data.touchActive !== false;
    state.system.lockoutActive = data.lockoutActive === true;
    state.system.onRemainingSec = parseInt(data.onRemainingSec || 0, 10);
    state.system.totalOnSec = parseInt(data.totalOnSec || (data.config?.onHours || 8) * 3600, 10);
    state.system.lockoutRemainingSec = parseInt(data.lockoutRemainingSec || 0, 10);
    state.system.totalLockoutSec = parseInt(data.totalLockoutSec || (data.config?.lockoutDays || 6) * 86400, 10);
    state.system.uptimeSeconds = parseInt(data.uptimeSeconds || 0, 10);

    if (data.config) {
      state.config.onHours = parseFloat(data.config.onHours || 8.0);
      state.config.lockoutDays = parseFloat(data.config.lockoutDays || 6.0);
    }

    if (data.analytics) {
      state.analytics.month = data.analytics.month || data.analytics.currentMonth || 'November 2026';
      state.analytics.totalOnHours = parseFloat(data.analytics.totalOnHours || 48.5);
      state.analytics.totalOffHours = parseFloat(data.analytics.totalOffHours || 240.2);
      state.analytics.totalCycles = parseInt(data.analytics.totalCycles || data.analytics.totalCyclesCount || 6, 10);
      const total = state.analytics.totalOnHours + state.analytics.totalOffHours;
      state.analytics.dutyPercent = total > 0 ? ((state.analytics.totalOnHours / total) * 100).toFixed(1) : 0;
    }

    // Check for Level 2 Pending Request for Admin Yash_Gajjar to approve
    if (data.pendingRequest && data.pendingRequest.active) {
      state.pendingRequest = data.pendingRequest;
      if (state.currentUser && state.currentUser.role === 'admin') {
        showAdminApprovalModal(data.pendingRequest.username, data.pendingRequest.action);
      }
    } else {
      state.pendingRequest.active = false;
      if (state.waitingForApproval && (!data.pendingRequest || !data.pendingRequest.active)) {
        state.waitingForApproval = false;
        closeWaitingModal();
      }
    }
  }

  // ---------------------------------------------------------------------------
  // CONTROL COMMAND EXECUTION (LOCAL AP & FIREBASE WORLDWIDE)
  // ---------------------------------------------------------------------------
  async function handleTriggerClick() {
    if (!state.currentUser) {
      toast('Please sign in first', 'error');
      return;
    }

    // Tier 3: Guest is read-only
    if (state.currentUser.role === 'guest_analytics') {
      vibrate(200);
      toast('Access Denied: Guest accounts are read-only', 'error');
      return;
    }

    // Tier 2: Mahi_Gajjar requires 1-time admin grant
    if (state.currentUser.role === 'guarded_operator') {
      if (!state.oneTimeToken) {
        requestOneTimeAccess('TRIGGER_RELAY');
        return;
      }
    }

    // Send trigger command
    await executeControlCommand('trigger');
  }

  async function executeControlCommand(action) {
    vibrate(40);

    if (state.simulated) {
      handleSimulatedAction(action);
      return;
    }

    if (state.connectionMode === 'cloud') {
      await sendFirebaseCommand(action.toUpperCase());
    } else {
      await sendLocalEspCommand(action);
    }
  }

  async function sendLocalEspCommand(action) {
    const url = `${state.espHost.replace(/\/+$/, '')}/api/control`;
    try {
      const res = await fetch(url, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          action: action,
          userRole: state.currentUser ? state.currentUser.role : '',
          oneTimeToken: state.oneTimeToken || ''
        })
      });

      const data = await res.json();
      if (!res.ok) throw new Error(data.message || 'ESP execution failed');

      toast(data.message || `Command ${action} executed`, 'success');
      state.oneTimeToken = ''; // Destroy token after 1 use
      fetchStatus();
    } catch (e) {
      toast(e.message || 'Command failed', 'error');
    }
  }

  async function sendFirebaseCommand(actionName) {
    let host = state.firebaseHost.trim().replace(/^https?:\/\//, '').replace(/\/+$/, '');
    let url = `https://${host}/esp8266_relay/commands.json`;
    if (state.firebaseAuth && state.firebaseAuth.trim().length > 0) {
      url += `?auth=${encodeURIComponent(state.firebaseAuth.trim())}`;
    }

    const commandPayload = {
      action: actionName,
      requestedBy: state.currentUser ? state.currentUser.username : 'APP_USER',
      userRole: state.currentUser ? state.currentUser.role : '',
      oneTimeToken: state.oneTimeToken || '',
      timestamp: Math.floor(Date.now() / 1000)
    };

    try {
      const res = await fetch(url, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(commandPayload)
      });

      if (!res.ok) throw new Error(`Firebase Error HTTP ${res.status}`);

      toast(`Cloud Command '${actionName}' dispatched worldwide`, 'success');
      state.oneTimeToken = '';
      fetchStatus();
    } catch (e) {
      toast(`Cloud dispatch failed: ${e.message}`, 'error');
    }
  }

  async function cancelCycle() {
    if (state.currentUser && state.currentUser.role === 'guest_analytics') {
      toast('Access Denied: Guest accounts are read-only', 'error');
      return;
    }
    if (confirm('Cancel active relay cycle and enter lockout?')) {
      await executeControlCommand('cancel');
    }
  }

  async function resetLockout() {
    if (!state.currentUser || state.currentUser.role !== 'admin') {
      vibrate(200);
      toast('Strict Admin Protection: Only Yash_Gajjar can bypass lockout', 'error');
      return;
    }
    if (confirm('Bypass lockout cooldown and re-enable touch sensor?')) {
      await executeControlCommand('reset_lockout');
    }
  }

  // ---------------------------------------------------------------------------
  // 1-TIME GRANT FLOW FOR MAHI_GAJJAR & ADMIN CONFIRMATION POPUP
  // ---------------------------------------------------------------------------
  async function requestOneTimeAccess(action) {
    state.waitingForApproval = true;
    showWaitingModal();

    if (state.connectionMode === 'cloud') {
      let host = state.firebaseHost.trim().replace(/^https?:\/\//, '').replace(/\/+$/, '');
      let url = `https://${host}/esp8266_relay/telemetry/pendingRequest.json`;
      if (state.firebaseAuth && state.firebaseAuth.trim().length > 0) {
        url += `?auth=${encodeURIComponent(state.firebaseAuth.trim())}`;
      }
      try {
        await fetch(url, {
          method: 'PUT',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            active: true,
            requestId: `REQ-${Date.now()}`,
            username: state.currentUser.username,
            action: action,
            requestedAt: Math.floor(Date.now() / 1000)
          })
        });
        toast('1-Time Request sent to Admin Yash_Gajjar over Firebase Cloud', 'info');
      } catch (e) {
        toast('Failed to post request to Cloud', 'error');
      }
    } else {
      const url = `${state.espHost.replace(/\/+$/, '')}/api/auth/request_grant`;
      try {
        const res = await fetch(url, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            username: state.currentUser.username,
            action: action
          })
        });
        const d = await res.json();
        if (!res.ok) throw new Error(d.message);
        toast('1-Time Request sent. Waiting for Admin confirmation popup.', 'info');
      } catch (e) {
        toast(e.message || 'Failed to submit grant request', 'error');
      }
    }
  }

  function showAdminApprovalModal(username, action) {
    const modal = document.getElementById('adminOneTimeApprovalModal');
    const u = document.getElementById('popupReqUser');
    const a = document.getElementById('popupReqAction');
    const p = document.getElementById('popupAdminPassword');

    if (u) u.textContent = username || 'Mahi_Gajjar';
    if (a) a.textContent = action || 'Trigger Relay Active Cycle';
    if (p) p.value = '';
    if (modal) modal.classList.remove('hidden');

    vibrate(100);
  }

  function dismissAdminApprovalModal() {
    const modal = document.getElementById('adminOneTimeApprovalModal');
    if (modal) modal.classList.add('hidden');
  }

  async function submitAdminApproval() {
    const pwd = document.getElementById('popupAdminPassword').value;
    if (pwd !== 'Iamyashacker@17') {
      vibrate(200);
      toast('Incorrect Admin Password! Grant Denied.', 'error');
      return;
    }

    const token = `ONE-TIME-GRANT-${Date.now()}`;

    if (state.connectionMode === 'cloud') {
      let host = state.firebaseHost.trim().replace(/^https?:\/\//, '').replace(/\/+$/, '');
      let clearReqUrl = `https://${host}/esp8266_relay/telemetry/pendingRequest.json`;
      let cmdUrl = `https://${host}/esp8266_relay/commands.json`;
      if (state.firebaseAuth && state.firebaseAuth.trim().length > 0) {
        const authP = `?auth=${encodeURIComponent(state.firebaseAuth.trim())}`;
        clearReqUrl += authP;
        cmdUrl += authP;
      }

      await fetch(clearReqUrl, { method: 'PUT', body: JSON.stringify({ active: false }) });
      await fetch(cmdUrl, {
        method: 'PUT',
        body: JSON.stringify({
          action: 'TRIGGER',
          requestedBy: 'Mahi_Gajjar (Authorized by Yash)',
          userRole: 'guarded_operator',
          oneTimeToken: token,
          timestamp: Math.floor(Date.now() / 1000)
        })
      });

      toast('Admin Yash_Gajjar approved 1-time grant! Trigger executed.', 'success');
    } else {
      const url = `${state.espHost.replace(/\/+$/, '')}/api/auth/approve_grant`;
      try {
        const res = await fetch(url, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ adminPassword: pwd })
        });
        const d = await res.json();
        if (!res.ok) throw new Error(d.message);
        toast('Admin Authorized 1-Time Session Successfully', 'success');
      } catch (e) {
        toast('Approval failed', 'error');
      }
    }

    dismissAdminApprovalModal();
    fetchStatus();
  }

  async function rejectAdminApproval() {
    if (state.connectionMode === 'cloud') {
      let host = state.firebaseHost.trim().replace(/^https?:\/\//, '').replace(/\/+$/, '');
      let url = `https://${host}/esp8266_relay/telemetry/pendingRequest.json`;
      if (state.firebaseAuth && state.firebaseAuth.trim().length > 0) {
        url += `?auth=${encodeURIComponent(state.firebaseAuth.trim())}`;
      }
      await fetch(url, { method: 'PUT', body: JSON.stringify({ active: false }) });
    } else {
      await fetch(`${state.espHost.replace(/\/+$/, '')}/api/auth/reject_grant`, { method: 'POST' });
    }
    toast('Access Request Denied & Dismissed', 'info');
    dismissAdminApprovalModal();
  }

  function showWaitingModal() {
    const modal = document.getElementById('userWaitingApprovalModal');
    if (modal) modal.classList.remove('hidden');
  }

  function closeWaitingModal() {
    const modal = document.getElementById('userWaitingApprovalModal');
    if (modal) modal.classList.add('hidden');
  }

  function cancelWaitingRequest() {
    state.waitingForApproval = false;
    closeWaitingModal();
    rejectAdminApproval();
  }

  // ---------------------------------------------------------------------------
  // TIMING CALIBRATION
  // ---------------------------------------------------------------------------
  function onTimerInput(type) {
    if (type === 'on') {
      const val = parseFloat(document.getElementById('onHoursSlider').value);
      document.getElementById('onHoursDisplay').textContent = val.toFixed(1);
      state.config.onHours = val;
    } else {
      const val = parseFloat(document.getElementById('lockoutDaysSlider').value);
      document.getElementById('lockoutDaysDisplay').textContent = val.toFixed(1);
      state.config.lockoutDays = val;
    }
  }

  async function saveTimers() {
    if (!state.currentUser || state.currentUser.role !== 'admin') {
      toast('Access Denied: Only Yash_Gajjar can modify timing calibrations', 'error');
      return;
    }

    vibrate(40);

    if (state.connectionMode === 'cloud') {
      await sendFirebaseCommand('UPDATE_CONFIG');
      let host = state.firebaseHost.trim().replace(/^https?:\/\//, '').replace(/\/+$/, '');
      let url = `https://${host}/esp8266_relay/telemetry/config.json`;
      if (state.firebaseAuth && state.firebaseAuth.trim().length > 0) {
        url += `?auth=${encodeURIComponent(state.firebaseAuth.trim())}`;
      }
      await fetch(url, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          onHours: state.config.onHours,
          lockoutDays: state.config.lockoutDays
        })
      });
      toast('Timing calibrated and synchronized to Cloud', 'success');
    } else {
      const url = `${state.espHost.replace(/\/+$/, '')}/api/config`;
      try {
        const res = await fetch(url, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            onHours: state.config.onHours,
            lockoutDays: state.config.lockoutDays,
            userRole: state.currentUser.role
          })
        });
        const d = await res.json();
        if (!res.ok) throw new Error(d.message);
        toast('Timing saved to ESP8266 Flash memory', 'success');
      } catch (e) {
        toast('Failed to save config: ' + e.message, 'error');
      }
    }
  }

  // ---------------------------------------------------------------------------
  // CLOUD & NETWORK CONFIGURATION MODAL
  // ---------------------------------------------------------------------------
  function openCloudConfigModal() {
    document.getElementById('cfgFirebaseHost').value = state.firebaseHost;
    document.getElementById('cfgFirebaseAuth').value = state.firebaseAuth;
    document.getElementById('cfgEspHost').value = state.espHost;
    document.getElementById('cloudConfigModal').classList.remove('hidden');
  }

  function closeCloudConfigModal() {
    document.getElementById('cloudConfigModal').classList.add('hidden');
  }

  function saveCloudSettings() {
    const fHost = document.getElementById('cfgFirebaseHost').value.trim();
    const fAuth = document.getElementById('cfgFirebaseAuth').value.trim();
    const eHost = document.getElementById('cfgEspHost').value.trim();

    if (fHost) state.firebaseHost = fHost;
    state.firebaseAuth = fAuth;
    if (eHost) state.espHost = eHost;

    // Persist to localStorage
    try {
      localStorage.setItem('cyberrelay_firebase_host', state.firebaseHost);
      localStorage.setItem('cyberrelay_firebase_auth', state.firebaseAuth);
      localStorage.setItem('cyberrelay_esp_host', state.espHost);
    } catch (ignored) {}

    // Persist to Android Native
    if (window.AndroidNative) {
      if (typeof window.AndroidNative.saveFirebaseConfig === 'function') {
        window.AndroidNative.saveFirebaseConfig(state.firebaseHost, state.firebaseAuth);
      }
      if (typeof window.AndroidNative.saveEspHost === 'function') {
        window.AndroidNative.saveEspHost(state.espHost);
      }
    }

    toast('Network & Firebase configurations saved', 'success');
    closeCloudConfigModal();
    fetchStatus();
  }

  async function testCloudConnection() {
    const fHost = document.getElementById('cfgFirebaseHost').value.trim().replace(/^https?:\/\//, '').replace(/\/+$/, '');
    const fAuth = document.getElementById('cfgFirebaseAuth').value.trim();
    let url = `https://${fHost}/esp8266_relay/telemetry.json`;
    if (fAuth) url += `?auth=${encodeURIComponent(fAuth)}`;

    toast('Pinging Firebase Realtime Cloud...', 'info');
    const start = Date.now();
    try {
      const res = await fetch(url, { headers: { 'Accept': 'application/json' } });
      const latency = Date.now() - start;
      if (res.ok) {
        toast(`Firebase Ping OK (${latency} ms) - Connected!`, 'success');
      } else {
        toast(`Firebase returned HTTP ${res.status}`, 'error');
      }
    } catch (e) {
      toast(`Firebase unreachable: ${e.message}`, 'error');
    }
  }

  async function testLocalConnection() {
    const eHost = document.getElementById('cfgEspHost').value.trim().replace(/\/+$/, '');
    toast(`Pinging Local ESP8266 at ${eHost}...`, 'info');
    const start = Date.now();
    try {
      const res = await fetch(`${eHost}/api/status`, { signal: AbortSignal.timeout(3000) });
      const latency = Date.now() - start;
      if (res.ok) {
        toast(`ESP8266 Ping OK (${latency} ms) - Connected!`, 'success');
      } else {
        toast(`ESP8266 returned HTTP ${res.status}`, 'error');
      }
    } catch (e) {
      toast(`ESP8266 unreachable at ${eHost}`, 'error');
    }
  }

  // ---------------------------------------------------------------------------
  // UI RENDERING & DATA VISUALIZATION
  // ---------------------------------------------------------------------------
  function updateUI() {
    updateConnectionStatus();
    updateUserProfile();
    updateOperationalState();
    updateRadialGauge();
    updateHardwareTiles();
    updateActionButtons();
    updateAnalyticsDisplay();
  }

  function updateConnectionStatus() {
    const pill = document.getElementById('connStatusPill');
    const text = document.getElementById('connStatusText');
    if (!pill || !text) return;

    if (state.connected) {
      pill.className = 'status-pill online';
      if (state.connectionMode === 'cloud') {
        text.textContent = 'FIREBASE CLOUD (ONLINE)';
      } else {
        text.textContent = 'ESP8266 (192.168.4.1) ONLINE';
      }
    } else {
      pill.className = 'status-pill offline';
      text.textContent = state.connectionMode === 'cloud' ? 'FIREBASE CONNECTING...' : 'ESP8266 OFFLINE';
    }
  }

  function updateUserProfile() {
    const user = state.currentUser;
    if (!user) return;

    const nameEl = document.getElementById('currentUserName');
    const roleEl = document.getElementById('currentUserRole');
    const initEl = document.getElementById('userInitial');
    const ringEl = document.getElementById('userAvatarRing');

    if (nameEl) nameEl.textContent = user.username;
    if (initEl) initEl.textContent = user.username.charAt(0).toUpperCase();

    if (roleEl) {
      if (user.role === 'admin') {
        roleEl.textContent = 'SUPER ADMIN';
        roleEl.className = 'role-tag role-admin';
        if (ringEl) ringEl.className = 'avatar-ring ring-admin';
      } else if (user.role === 'guarded_operator') {
        roleEl.textContent = 'GUARDED (1-TIME)';
        roleEl.className = 'role-tag role-guarded';
        if (ringEl) ringEl.className = 'avatar-ring ring-guarded';
      } else {
        roleEl.textContent = 'GUEST (ANALYTICS)';
        roleEl.className = 'role-tag role-guest';
        if (ringEl) ringEl.className = 'avatar-ring ring-guest';
      }
    }

    // Role Notices
    const guestBanner = document.getElementById('guestNoticeBanner');
    const guardedBanner = document.getElementById('guardedNoticeBanner');
    if (guestBanner) guestBanner.classList.toggle('hidden', user.role !== 'guest_analytics');
    if (guardedBanner) guardedBanner.classList.toggle('hidden', user.role !== 'guarded_operator');
  }

  function updateOperationalState() {
    const badge = document.getElementById('sysStateBadge');
    const text = document.getElementById('sysStateText');
    const bulb = document.getElementById('relayBulb');
    if (!badge || !text) return;

    if (state.system.mode === 'RELAY_ON') {
      badge.className = 'state-badge badge-active';
      text.textContent = 'RELAY ENERGIZED (8H RUN)';
      if (bulb) bulb.className = 'relay-bulb-indicator bulb-on';
    } else if (state.system.mode === 'LOCKOUT') {
      badge.className = 'state-badge badge-lockout';
      text.textContent = 'LOCKOUT PERIOD ACTIVE';
      if (bulb) bulb.className = 'relay-bulb-indicator bulb-locked';
    } else {
      badge.className = 'state-badge badge-idle';
      text.textContent = 'IDLE / READY';
      if (bulb) bulb.className = 'relay-bulb-indicator bulb-idle';
    }
  }

  function updateRadialGauge() {
    const progressCircle = document.getElementById('radialProgress');
    const countLabel = document.getElementById('countdownLabel');
    const countSub = document.getElementById('countdownSubtext');

    let total = 1;
    let rem = 0;
    let strokeColor = 'url(#cyanGlow)';

    if (state.system.mode === 'RELAY_ON') {
      total = state.system.totalOnSec || (state.config.onHours * 3600);
      rem = state.system.onRemainingSec;
      strokeColor = 'url(#cyanGlow)';
      if (countLabel) countLabel.textContent = 'ACTIVE DUTY COUNTDOWN';
      if (countSub) countSub.textContent = 'Relay energized. Touch ignored.';
    } else if (state.system.mode === 'LOCKOUT') {
      total = state.system.totalLockoutSec || (state.config.lockoutDays * 86400);
      rem = state.system.lockoutRemainingSec;
      strokeColor = 'url(#amberGlow)';
      if (countLabel) countLabel.textContent = 'COOLDOWN LOCKOUT COUNTDOWN';
      if (countSub) countSub.textContent = 'Cooldown active. Touch filter engaged.';
    } else {
      total = 1;
      rem = 1;
      strokeColor = 'url(#emeraldGlow)';
      if (countLabel) countLabel.textContent = 'READY FOR TRIGGER';
      if (countSub) countSub.textContent = 'Tap capacitive touch sensor to trigger cycle';
    }

    if (progressCircle) {
      const ratio = Math.max(0, Math.min(1, rem / (total || 1)));
      const offset = GAUGE_CIRCUMFERENCE * (1 - ratio);
      progressCircle.style.strokeDashoffset = offset;
      progressCircle.style.stroke = strokeColor;
    }

    updateClock(rem);
  }

  function updateClock(totalSec) {
    const days = Math.floor(totalSec / 86400);
    const hrs = Math.floor((totalSec % 86400) / 3600);
    const mins = Math.floor((totalSec % 3600) / 60);
    const secs = totalSec % 60;

    const pad = (n) => String(n).padStart(2, '0');
    const cd = document.getElementById('clockDays');
    const ch = document.getElementById('clockHours');
    const cm = document.getElementById('clockMins');
    const cs = document.getElementById('clockSecs');

    if (cd) cd.textContent = pad(days);
    if (ch) ch.textContent = pad(hrs);
    if (cm) cm.textContent = pad(mins);
    if (cs) cs.textContent = pad(secs);
  }

  function updateHardwareTiles() {
    // Relay Dot & Text
    const rDot = document.getElementById('relayStatusDot');
    const rText = document.getElementById('relayStatusText');
    if (rDot && rText) {
      if (state.system.relayActive) {
        rDot.className = 'status-dot dot-active';
        rText.textContent = 'ACTIVE (ON)';
      } else {
        rDot.className = 'status-dot dot-off';
        rText.textContent = 'OFF';
      }
    }

    // Touch Dot & Text
    const tDot = document.getElementById('touchStatusDot');
    const tText = document.getElementById('touchStatusText');
    if (tDot && tText) {
      if (state.system.touchActive && !state.system.lockoutActive) {
        tDot.className = 'status-dot dot-active';
        tText.textContent = 'ENABLED';
      } else {
        tDot.className = 'status-dot dot-disabled';
        tText.textContent = 'FILTERED (OFF)';
      }
    }

    // Lockout Dot & Text
    const lDot = document.getElementById('lockoutStatusDot');
    const lText = document.getElementById('lockoutStatusText');
    if (lDot && lText) {
      if (state.system.lockoutActive) {
        lDot.className = 'status-dot dot-locked';
        lText.textContent = 'LOCKED';
      } else {
        lDot.className = 'status-dot dot-disabled';
        lText.textContent = 'UNLOCKED';
      }
    }
  }

  function updateActionButtons() {
    const btnTrigger = document.getElementById('btnManualTrigger');
    const btnCancel = document.getElementById('btnCancelCycle');
    const btnReset = document.getElementById('btnResetLockout');
    const trgText = document.getElementById('triggerBtnText');

    const role = state.currentUser ? state.currentUser.role : 'guest_analytics';

    if (btnTrigger) {
      if (role === 'guest_analytics') {
        btnTrigger.disabled = true;
        if (trgText) trgText.textContent = 'Controls Disabled (Guest Mode)';
      } else if (role === 'guarded_operator') {
        btnTrigger.disabled = state.system.mode !== 'IDLE';
        if (trgText) trgText.textContent = 'Request 1-Time Trigger (Mahi)';
      } else {
        btnTrigger.disabled = state.system.mode !== 'IDLE';
        if (trgText) trgText.textContent = 'Trigger Relay Cycle Now';
      }
    }

    if (btnCancel) {
      btnCancel.disabled = (role === 'guest_analytics') || (state.system.mode !== 'RELAY_ON');
    }

    if (btnReset) {
      // Strictly Yash_Gajjar admin only
      btnReset.disabled = (role !== 'admin') || (state.system.mode !== 'LOCKOUT');
    }
  }

  function updateAnalyticsDisplay() {
    const mBadge = document.getElementById('analyticsMonthBadge');
    const onH = document.getElementById('anTotalOnHours');
    const offH = document.getElementById('anTotalOffHours');
    const cyc = document.getElementById('anTotalCycles');
    const ratioText = document.getElementById('dutyRatioPercent');
    const barOn = document.getElementById('ratioBarOn');
    const barOff = document.getElementById('ratioBarOff');

    if (mBadge) mBadge.textContent = state.analytics.month;
    if (onH) onH.textContent = state.analytics.totalOnHours.toFixed(1);
    if (offH) offH.textContent = state.analytics.totalOffHours.toFixed(1);
    if (cyc) cyc.textContent = state.analytics.totalCycles;

    if (ratioText) ratioText.textContent = `${state.analytics.dutyPercent}% ON`;
    if (barOn) barOn.style.width = `${state.analytics.dutyPercent}%`;
    if (barOff) barOff.style.width = `${Math.max(0, 100 - state.analytics.dutyPercent)}%`;
  }

  function renderUserTable() {
    const tbody = document.getElementById('userTableBody');
    if (!tbody) return;

    tbody.innerHTML = '';
    state.users.forEach(u => {
      const tr = document.createElement('tr');
      const isCurrent = state.currentUser && state.currentUser.username.toLowerCase() === u.username.toLowerCase();

      let badgeClass = 'role-guest-badge';
      let tierLabel = 'Level 3: Guest Analytics';
      if (u.role === 'admin') {
        badgeClass = 'role-admin-badge';
        tierLabel = 'Level 1: Super Admin';
      } else if (u.role === 'guarded_operator') {
        badgeClass = 'role-guarded-badge';
        tierLabel = 'Level 2: Guarded (1-Time)';
      }

      tr.innerHTML = `
        <td>
          <strong>${u.username}</strong>
          ${isCurrent ? '<span style="color:var(--accent-cyan); font-size:0.7rem; margin-left:4px;">(Current)</span>' : ''}
        </td>
        <td>
          <span class="user-badge-role ${badgeClass}">${tierLabel}</span>
        </td>
        <td>
          <span class="status-dot dot-active"></span> Active
        </td>
      `;
      tbody.appendChild(tr);
    });

    const userCount = document.getElementById('userCountText');
    if (userCount) userCount.textContent = `${state.users.length} / 10`;
  }

  function startLocalUptime() {
    setInterval(() => {
      state.system.uptimeSeconds++;
      const el = document.getElementById('espUptime');
      if (el) {
        const u = state.system.uptimeSeconds;
        const h = Math.floor(u / 3600);
        const m = Math.floor((u % 3600) / 60);
        const s = u % 60;
        const pad = (n) => String(n).padStart(2, '0');
        el.textContent = `${pad(h)}:${pad(m)}:${pad(s)}`;
      }
    }, 1000);
  }

  // ---------------------------------------------------------------------------
  // TOAST & HAPTIC UTILITIES
  // ---------------------------------------------------------------------------
  function toast(message, type = 'info') {
    const container = document.getElementById('toastContainer');
    if (!container) return;

    const el = document.createElement('div');
    el.className = `toast toast-${type}`;
    el.textContent = message;
    container.appendChild(el);

    if (window.AndroidNative && typeof window.AndroidNative.showToast === 'function') {
      try {
        window.AndroidNative.showToast(message);
      } catch (ignored) {}
    }

    setTimeout(() => {
      el.style.opacity = '0';
      el.style.transform = 'translateY(-6px)';
      el.style.transition = 'all 0.3s ease';
      setTimeout(() => el.remove(), 300);
    }, 3200);
  }

  function vibrate(ms = 60) {
    if (window.AndroidNative && typeof window.AndroidNative.vibrateAlert === 'function') {
      try {
        window.AndroidNative.vibrateAlert(ms);
      } catch (ignored) {}
    } else if (navigator.vibrate) {
      navigator.vibrate(ms);
    }
  }

  // Public Interface
  return {
    init,
    setConnectionMode,
    switchAuthTab,
    fillPreset,
    togglePasswordVisibility,
    handleSignInSubmit,
    handleRegisterSubmit,
    logout,
    handleTriggerClick,
    cancelCycle,
    resetLockout,
    dismissAdminApprovalModal,
    submitAdminApproval,
    rejectAdminApproval,
    cancelWaitingRequest,
    onTimerInput,
    saveTimers,
    openCloudConfigModal,
    closeCloudConfigModal,
    saveCloudSettings,
    testCloudConnection,
    testLocalConnection,
    dismissConnNotice
  };
})();

// Start App when DOM is loaded
window.addEventListener('DOMContentLoaded', () => {
  App.init();
});
