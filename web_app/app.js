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
    activeView: 'control',   // 'control', 'timers', 'analytics', 'users', 'cloud', 'settings'
    pollInterval: null,
    uptimeTimer: null,
    simulated: false,
    consecutiveErrors: 0,
    pingMs: null,
    wifiRssi: -52,
    cloudLogs: [],

    // Firebase Cloud Credentials & Key details from /Firebase_Auth_Keys
    firebase: {
      projectId: 'cyber-relay-rtos-default-rtdb',
      clientEmail: 'firebase-adminsdk-fbsvc@cyber-relay-rtos-default-rtdb.iam.gserviceaccount.com',
      privateKeyId: 'b7a22fa5bc01d1962d398c5b0cc9a8b2b08aa603',
      keyLoaded: true
    },

    // Interactive Live Console State
    console: {
      logs: [],
      maxLogs: 200,
      filter: 'all', // 'all', 'cmd', 'tel', 'hw', 'err'
      autoScroll: true,
      streaming: true,
      history: [],
      historyIndex: -1
    },

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
      lockoutDays: 8.0
    },

    // Dynamic Multi-Relay Channels & Pin Bindings
    selectedRelayId: 1,
    relays: [
      {
        id: 1,
        name: 'Relay 1 (Main)',
        pin: 5,
        activeLevel: 0,
        onHours: 8.0,
        lockoutDays: 8.0,
        mode: 'IDLE',
        relayActive: false,
        onRemainingSec: 0,
        totalOnSec: 8 * 3600,
        lockoutRemainingSec: 0,
        totalLockoutSec: 8 * 86400,
        enabled: true
      }
    ],
    prevRelayStates: {},

    // Current User Session
    currentUser: null,
    users: [...PREDEFINED_USERS],
    oneTimeToken: '',

    // Handshake ACK & System Logs
    ack: {
      verified: false,
      bootReason: 'Power On',
      wifiConnected: false,
      softApSsid: 'CYBER-RELAY-HUB',
      softApIp: '192.168.4.1',
      staIp: '0.0.0.0',
      buzzerChimes: 3
    },
    systemLogs: [],

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
              const disabledList = JSON.parse(localStorage.getItem('cyberrelay_disabled_accounts') || '[]');
              if (disabledList.includes(matched.username) || matched.status === 'disabled') {
                console.warn('[SESSION] Account is disabled by admin:', matched.username);
                clearPermanentSession();
                return false;
              }
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
            const disabledList = JSON.parse(localStorage.getItem('cyberrelay_disabled_accounts') || '[]');
            if (disabledList.includes(matched.username) || matched.status === 'disabled') {
              console.warn('[SESSION] Account is disabled by admin:', matched.username);
              clearPermanentSession();
              return false;
            }
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
    } catch (ignored) { }

    // Save to Android Native SharedPreferences
    if (window.AndroidNative && typeof window.AndroidNative.saveSession === 'function') {
      try {
        window.AndroidNative.saveSession(user.username, user.role, sessionPayload.token);
      } catch (ignored) { }
    }
  }

  function clearPermanentSession() {
    state.currentUser = null;
    try {
      localStorage.removeItem('cyberrelay_session');
      localStorage.removeItem('cyberrelay_keep_logged_in');
    } catch (ignored) { }

    if (window.AndroidNative && typeof window.AndroidNative.clearSession === 'function') {
      try {
        window.AndroidNative.clearSession();
      } catch (ignored) { }
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
    } catch (ignored) { }

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
    } catch (ignored) { }

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
    } catch (ignored) { }
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
    if (loginSec) {
      loginSec.classList.remove('hidden');
      loginSec.style.display = 'flex';
    }
    if (dashSec) {
      dashSec.classList.add('hidden');
      dashSec.style.display = 'none';
    }
    updateConnectionModeButtons();
  }

  function showDashboardView() {
    const loginSec = document.getElementById('loginSection');
    const dashSec = document.getElementById('dashboardSection');
    if (loginSec) {
      loginSec.classList.add('hidden');
      loginSec.style.display = 'none';
    }
    if (dashSec) {
      dashSec.classList.remove('hidden');
      dashSec.style.display = 'flex';
    }

    switchView(state.activeView || 'control');
    updateUI();
    renderUserTable();
  }

  function switchView(viewKey) {
    state.activeView = viewKey;
    const views = {
      'control': { title: '⚡ Relay Control', subtitle: 'ESP8266 Channel 1 & Hardware Timers' },
      'timers': { title: '⏱️ Timer Calibrations', subtitle: 'Duty Cycle & Lockout Cooldown Tuning' },
      'analytics': { title: '📊 Monthly Analytics', subtitle: 'Comprehensive Usage & Duty Ratio' },
      'users': { title: '👥 User Accounts & Access', subtitle: 'Predefined Credentials & Admin Controls' },
      'cloud': { title: '☁️ Firebase Cloud & Logs', subtitle: 'Live Database Stream & Controller Traces' },
      'console': { title: '⚡ Interactive Live Console', subtitle: 'Bidirectional RTOS Terminal & Hardware Trace Stream' },
      'acknowledge': { title: '🔔 Connection ACK & Wireless Logs', subtitle: '3-Stage Pipeline Handshake, 3-Beep Chime & System Events' },
      'settings': { title: '⚙️ Device & Diagnostics', subtitle: 'Hardware Diagnostics & RF Telemetry' }
    };

    const targetInfo = views[viewKey] || views['control'];

    // Update Header Text
    const titleEl = document.getElementById('headerViewTitle');
    const subEl = document.getElementById('headerViewSubtitle');
    if (titleEl) titleEl.textContent = targetInfo.title;
    if (subEl) subEl.textContent = targetInfo.subtitle;

    // Switch view page sections
    document.querySelectorAll('.view-page').forEach(v => {
      v.classList.remove('active');
    });
    const targetSection = document.getElementById('view' + viewKey.charAt(0).toUpperCase() + viewKey.slice(1));
    if (targetSection) targetSection.classList.add('active');

    // Update sidebar navigation pills
    document.querySelectorAll('.sidebar-nav .nav-item').forEach(b => {
      b.classList.remove('active');
    });
    const navBtn = document.getElementById('nav' + viewKey.charAt(0).toUpperCase() + viewKey.slice(1));
    if (navBtn) navBtn.classList.add('active');

    if (viewKey === 'cloud') {
      renderCloudLogs();
    }

    closeSidebar();
  }

  function openSidebar() {
    const s = document.getElementById('appSidebar');
    const o = document.getElementById('sidebarOverlay');
    if (s) s.classList.add('open');
    if (o) o.classList.add('active');
  }

  function closeSidebar() {
    const s = document.getElementById('appSidebar');
    const o = document.getElementById('sidebarOverlay');
    if (s) s.classList.remove('open');
    if (o) o.classList.remove('active');
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

  function selectRoleLogin(role) {
    const userField = document.getElementById('inputUsername');
    const passField = document.getElementById('inputPassword');

    // Update selected button style
    ['btnSelectAdmin', 'btnSelectUser', 'btnSelectGuest'].forEach(id => {
      const el = document.getElementById(id);
      if (el) el.classList.remove('selected');
    });

    if (role === 'admin') {
      if (userField) userField.value = 'Yash_Gajjar';
      const b = document.getElementById('btnSelectAdmin');
      if (b) b.classList.add('selected');
    } else if (role === 'user') {
      if (userField) userField.value = 'Mahi_Gajjar';
      const b = document.getElementById('btnSelectUser');
      if (b) b.classList.add('selected');
    } else if (role === 'guest') {
      if (userField) userField.value = 'Guest';
      const b = document.getElementById('btnSelectGuest');
      if (b) b.classList.add('selected');
    }

    // DO NOT AUTO WRITE PASSWORD! Clear password and focus so user enters it manually
    if (passField) {
      passField.value = '';
      passField.focus();
    }
    vibrate(25);
  }

  function fillPreset(key) {
    // Legacy fallback mapped to selectRoleLogin
    if (key === 'yash') selectRoleLogin('admin');
    else if (key === 'mahi') selectRoleLogin('user');
    else if (key === 'guest') selectRoleLogin('guest');
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

    const disabledList = JSON.parse(localStorage.getItem('cyberrelay_disabled_accounts') || '[]');
    if (disabledList.includes(matched.username) || matched.status === 'disabled') {
      vibrate(250);
      toast(`⚠️ Access Denied: Account '${matched.username}' has been temporarily disabled by Administrator Yash_Gajjar.`, 'error');
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
    } catch (ignored) { }

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
    } catch (ignored) { }

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

  // ---------------------------------------------------------------------------
  // CONNECTION & REAL-TIME LOGGING ENGINE
  // ---------------------------------------------------------------------------
  function checkLocalWifiBeforeAction(actionName) {
    if (state.connectionMode === 'local' && !state.connected) {
      vibrate(180);
      showLocalWifiModal();
      return false;
    }
    return true;
  }

  function showLocalWifiModal() {
    const m = document.getElementById('localWifiPromptModal');
    if (m) m.classList.remove('hidden');
  }

  function closeLocalWifiModal() {
    const m = document.getElementById('localWifiPromptModal');
    if (m) m.classList.add('hidden');
  }

  async function retryLocalConnection() {
    closeLocalWifiModal();
    toast('Probing ESP8266 Local AP (192.168.4.1)...', 'info');
    await fetchLocalEspStatus();
    if (state.connected) {
      toast('Connected to ESP8266 Wi-Fi successfully!', 'success');
    } else {
      showLocalWifiModal();
    }
  }

  function logConsole(tag, message, details) {
    if (!state.console.streaming) return;
    const now = new Date();
    const timeStr = now.toTimeString().split(' ')[0] + '.' + String(now.getMilliseconds()).padStart(3, '0');
    const entry = { time: timeStr, tag: tag.toUpperCase(), message: String(message), details: details || '' };
    state.console.logs.push(entry);
    if (state.console.logs.length > state.console.maxLogs) state.console.logs.shift();

    // Mirror to cloudLogs for backward compatibility
    state.cloudLogs.push(entry);
    if (state.cloudLogs.length > 80) state.cloudLogs.shift();

    renderLiveConsole();
    renderCloudLogs();
  }

  function logCloudEvent(tag, message, details) {
    logConsole(tag, message, details);
  }

  function renderLiveConsole() {
    const liveTerm = document.getElementById('liveConsoleTerminal');
    if (!liveTerm) return;

    if (state.console.logs.length === 0) {
      liveTerm.innerHTML = '<div style="color:var(--text-muted); padding:10px;">⚡ Live RTOS & Cloud terminal active. System traces will appear here in real time...</div>';
      return;
    }

    const filter = state.console.filter;
    const filtered = state.console.logs.filter(l => {
      if (filter === 'all') return true;
      if (filter === 'cmd') return l.tag === 'CMD' || l.tag === 'IN' || l.tag === 'OUT';
      if (filter === 'tel') return l.tag === 'TEL' || l.tag === 'SYNC' || l.tag === 'TIMER';
      if (filter === 'hw') return l.tag === 'HW' || l.tag === 'ACK' || l.tag === 'BOOT' || l.tag === 'WIFI' || l.tag === 'RELAY' || l.tag === 'ALARM';
      if (filter === 'err') return l.tag === 'ERR' || l.tag.includes('ERR');
      return true;
    });

    liveTerm.innerHTML = filtered.map(l => {
      let tagClass = 'tag-poll';
      const t = l.tag;
      if (t === 'CMD') tagClass = 'tag-cmd';
      else if (t === 'TEL' || t === 'SYNC' || t === 'TIMER') tagClass = 'tag-tel';
      else if (t === 'HW' || t === 'BOOT' || t === 'WIFI') tagClass = 'tag-hw';
      else if (t === 'FIREBASE') tagClass = 'tag-cloud';
      else if (t === 'RELAY') tagClass = 'tag-poll';
      else if (t === 'ALARM') tagClass = 'tag-err';
      else if (t === 'ACK') tagClass = 'tag-ack';
      else if (t === 'IN') tagClass = 'tag-in';
      else if (t === 'OUT') tagClass = 'tag-out';
      else if (t === 'ERR' || t.includes('ERR')) tagClass = 'tag-err';

      return `
        <div class="terminal-line">
          <span class="term-time">[${l.time}]</span>
          <span class="term-tag ${tagClass}">${l.tag}</span>
          <span class="term-msg">${escapeHtml(l.message)}</span>
        </div>
      `;
    }).join('');

    if (state.console.autoScroll) {
      liveTerm.scrollTop = liveTerm.scrollHeight;
    }
  }

  function setConsoleFilter(filter) {
    state.console.filter = filter;
    document.querySelectorAll('.console-filter-btn').forEach(btn => {
      btn.classList.toggle('active', btn.getAttribute('data-filter') === filter);
    });
    renderLiveConsole();
  }

  function toggleConsoleStreaming() {
    state.console.streaming = !state.console.streaming;
    const btn = document.getElementById('btnToggleStreaming');
    const ind = document.getElementById('consoleLiveIndicator');
    if (btn) btn.textContent = state.console.streaming ? 'Pause Stream' : 'Resume Stream';
    if (ind) {
      ind.innerHTML = state.console.streaming
        ? '<span class="pulse-dot"></span> LIVE STREAMING'
        : '<span style="color:var(--text-muted);">⏸ PAUSED</span>';
    }
    toast(state.console.streaming ? 'Live stream resumed' : 'Live stream paused', 'info');
  }

  function clearConsoleLogs() {
    state.console.logs = [];
    state.cloudLogs = [];
    renderLiveConsole();
    renderCloudLogs();
    toast('Live console cleared', 'info');
  }

  function copyConsoleLogs() {
    const text = state.console.logs.map(l => `[${l.time}] [${l.tag}] ${l.message}`).join('\n');
    navigator.clipboard.writeText(text).then(() => {
      toast('Copied all console logs to clipboard', 'success');
    }).catch(() => {
      toast('Failed to copy to clipboard', 'error');
    });
  }

  function sendQuickCommand(cmd) {
    const input = document.getElementById('consoleCmdInput');
    if (input) input.value = cmd;
    executeConsoleCommand(cmd);
  }

  function handleConsoleInputSubmit() {
    const input = document.getElementById('consoleCmdInput');
    if (!input) return;
    const val = input.value.trim();
    if (!val) return;
    input.value = '';
    executeConsoleCommand(val);
  }

  async function executeConsoleCommand(cmdStr) {
    if (!cmdStr || !cmdStr.trim()) return;
    const raw = cmdStr.trim();
    state.console.history.push(raw);
    state.console.historyIndex = state.console.history.length;
    logConsole('IN', '> ' + raw);

    const parts = raw.split(/\s+/);
    const cmd = parts[0].toLowerCase();
    const arg1 = parts[1];

    switch(cmd) {
      case 'help':
        logConsole('OUT', '--- AVAILABLE RTOS COMMANDS ---');
        logConsole('OUT', '  status          : Display real-time RTOS telemetry snapshot');
        logConsole('OUT', '  relays          : List all configured multi-relay channels');
        logConsole('OUT', '  trigger [id]    : Trigger relay cycle (default: active channel)');
        logConsole('OUT', '  cancel [id]     : Emergency cutoff / turn relay OFF');
        logConsole('OUT', '  toggle [id]     : Toggle relay power ON/OFF');
        logConsole('OUT', '  extend <mins>   : Extend active countdown timer (Admin)');
        logConsole('OUT', '  reset_lockout   : Bypass lockout cooldown (Admin)');
        logConsole('OUT', '  ack             : Verify pipeline & sound 3-beep chime on D8');
        logConsole('OUT', '  buzzer          : Test hardware buzzer on NodeMCU D8');
        logConsole('OUT', '  heap            : View free RAM on ESP8266');
        logConsole('OUT', '  sync            : Force instant telemetry poll');
        logConsole('OUT', '  mode <local|cloud>: Switch connection mode');
        logConsole('OUT', '  clear           : Clear console buffer');
        break;

      case 'status':
        const r = getSelectedRelay();
        logConsole('OUT', `MODE=${r.mode} | ACTIVE=${r.relayActive} | REMAINING=${r.mode==='RELAY_ON'?r.onRemainingSec:r.lockoutRemainingSec}s | HEAP=${state.system.freeHeap||'--'}B | RSSI=${state.wifiRssi}dBm | CONN=${state.connectionMode.toUpperCase()}`);
        break;

      case 'relays':
        state.relays.forEach(ch => {
          logConsole('OUT', `Relay ${ch.id} [${ch.name}]: GPIO${ch.pin} | ON=${ch.onHours}h | OFF=${ch.lockoutDays}d | Mode=${ch.mode} | State=${ch.relayActive?'ON':'OFF'}`);
        });
        break;

      case 'trigger':
        const trgId = parseInt(arg1 || state.selectedRelayId || 1, 10);
        logConsole('OUT', `Triggering Relay ${trgId}...`);
        await executeControlCommand('trigger', { relayId: trgId });
        break;

      case 'cancel':
        const cnlId = parseInt(arg1 || state.selectedRelayId || 1, 10);
        logConsole('OUT', `Cancelling Relay ${cnlId}...`);
        await executeControlCommand('cancel', { relayId: cnlId });
        break;

      case 'toggle':
        const tglId = parseInt(arg1 || state.selectedRelayId || 1, 10);
        const targetRelay = state.relays.find(x => x.id === tglId) || getSelectedRelay();
        const action = (targetRelay.relayActive || targetRelay.mode === 'RELAY_ON') ? 'cancel' : 'trigger';
        logConsole('OUT', `Toggling Relay ${tglId} (${action})...`);
        await executeControlCommand(action, { relayId: tglId });
        break;

      case 'extend':
        const mins = parseInt(arg1 || 15, 10);
        logConsole('OUT', `Extending timer by +${mins}m...`);
        await extendActiveTimer(mins);
        break;

      case 'reset_lockout':
        const rstId = parseInt(arg1 || state.selectedRelayId || 1, 10);
        logConsole('OUT', `Bypassing lockout for Relay ${rstId}...`);
        await executeControlCommand('reset_lockout', { relayId: rstId });
        break;

      case 'ack':
      case 'check_ack':
        logConsole('OUT', 'Initiating 3-stage pipeline verification & 3-beep chime...');
        await triggerAckHandshake();
        break;

      case 'buzzer':
        logConsole('OUT', 'Sounding buzzer test on D8...');
        await testBuzzerSound();
        break;

      case 'heap':
        logConsole('OUT', `ESP8266 Free Heap Memory: ${state.system.freeHeap || 28416} bytes`);
        break;

      case 'sync':
        logConsole('OUT', 'Syncing telemetry now...');
        await fetchStatus();
        break;

      case 'mode':
        if (arg1 === 'cloud' || arg1 === 'local') {
          setConnectionMode(arg1);
          logConsole('OUT', `Connection mode switched to: ${arg1.toUpperCase()}`);
        } else {
          logConsole('ERR', 'Usage: mode local | mode cloud');
        }
        break;

      case 'clear':
        clearConsoleLogs();
        break;

      default:
        logConsole('ERR', `Unknown command: '${cmd}'. Type 'help' for available commands.`);
    }
  }

  function renderCloudLogs() {
    const terminal = document.getElementById('cloudLogTerminal');
    if (!terminal) return;

    if (state.cloudLogs.length === 0) {
      terminal.innerHTML = '<div style="color:var(--text-muted); padding:10px;">No cloud event logs yet. Sync operations will appear here in real-time...</div>';
      return;
    }

    terminal.innerHTML = state.cloudLogs.map(l => {
      let tagClass = 'tag-poll';
      if (l.tag === 'CMD') tagClass = 'tag-cmd';
      else if (l.tag === 'SYNC' || l.tag === 'TEL') tagClass = 'tag-sync';
      else if (l.tag === 'ERR') tagClass = 'tag-err';
      else if (l.tag === 'HW' || l.tag === 'ACK') tagClass = 'tag-ack';
      return `
        <div class="terminal-line">
          <span class="term-time">[${l.time}]</span>
          <span class="term-tag ${tagClass}">${l.tag}</span>
          <span class="term-msg">${escapeHtml(l.message)}</span>
        </div>
      `;
    }).join('');

    terminal.scrollTop = terminal.scrollHeight;
  }

  function clearCloudLogs() {
    clearConsoleLogs();
  }

  // 1. Fetch from Local ESP8266 (192.168.4.1)
  async function fetchLocalEspStatus() {
    const url = `${state.espHost.replace(/\/+$/, '')}/api/status`;
    const t0 = performance.now();
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

      const rtt = Math.round(performance.now() - t0);
      state.pingMs = rtt;

      if (!response.ok) throw new Error(`HTTP ${response.status}`);

      const data = await response.json();
      if (data.wifiRssi !== undefined && data.wifiRssi !== 0) {
        state.wifiRssi = data.wifiRssi;
      } else {
        state.wifiRssi = -46;
      }

      applyTelemetryData(data);
      state.connected = true;
      state.consecutiveErrors = 0;
      hideConnNotice();
      updateUI();
      logCloudEvent('POLL', `GET /api/status HTTP/200 (${rtt}ms) - Mode: ${state.system.mode}, Relay: ${state.system.relayActive ? 'ON' : 'OFF'}`);
    } catch (err) {
      const rtt = Math.round(performance.now() - t0);
      state.pingMs = rtt;
      handleConnectionFailure(err, 'Local AP');
      logCloudEvent('ERR', `Local AP status error: ${err.message} (${rtt}ms)`);
    }
  }

  // 2. Fetch from Firebase Realtime Cloud Database (Worldwide)
  async function fetchFirebaseTelemetry() {
    let host = state.firebaseHost.trim().replace(/^https?:\/\//, '').replace(/\/+$/, '');
    let url = `https://${host}/esp8266_relay/telemetry.json`;
    if (state.firebaseAuth && state.firebaseAuth.trim().length > 0) {
      url += `?auth=${encodeURIComponent(state.firebaseAuth.trim())}`;
    }

    const t0 = performance.now();
    try {
      const controller = new AbortController();
      const timeoutId = setTimeout(() => controller.abort(), 3500);

      const response = await fetch(url, {
        signal: controller.signal,
        headers: { 'Accept': 'application/json' }
      });
      clearTimeout(timeoutId);

      const rtt = Math.round(performance.now() - t0);
      state.pingMs = rtt;

      if (!response.ok) throw new Error(`Firebase HTTP ${response.status}`);

      const data = await response.json();
      if (!data) throw new Error('Empty Firebase telemetry document');

      if (data.wifiRssi !== undefined && data.wifiRssi !== 0) {
        state.wifiRssi = data.wifiRssi;
      }

      applyTelemetryData(data);
      state.connected = true;
      state.consecutiveErrors = 0;
      hideConnNotice();
      updateUI();
      logCloudEvent('SYNC', `GET /telemetry.json HTTP/200 (${rtt}ms) - Mode: ${state.system.mode}, Relay: ${state.system.relayActive ? 'ON' : 'OFF'}`);
    } catch (err) {
      const rtt = Math.round(performance.now() - t0);
      state.pingMs = rtt;
      handleConnectionFailure(err, 'Firebase Cloud');
      logCloudEvent('ERR', `Firebase Cloud sync error: ${err.message} (${rtt}ms)`);
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

    if (data.relays && Array.isArray(data.relays)) {
      state.relays = data.relays;
      checkRelayStateTransitions(data.relays);
    }

    if (data.ack) {
      state.ack = { ...state.ack, ...data.ack };
    }

    if (data.systemLogs && Array.isArray(data.systemLogs)) {
      state.systemLogs = data.systemLogs;
      data.systemLogs.forEach(sl => {
        const fullMsg = `[ESP8266] ${sl.tag}: ${sl.msg}`;
        if (!state.console.logs.some(cl => cl.message === fullMsg)) {
          let cat = 'HW';
          const t = (sl.tag || '').toUpperCase();
          if (t.includes('WIFI') || t.includes('AP')) cat = 'WIFI';
          else if (t.includes('FIREBASE') || t.includes('CLOUD')) cat = 'FIREBASE';
          else if (t.includes('ALARM') || t.includes('BUZZER')) cat = 'ALARM';
          else if (t.includes('RELAY')) cat = 'RELAY';
          else if (t.includes('TIMER')) cat = 'TIMER';
          logConsole(cat, fullMsg);
        }
      });
    }

    logConsole('TEL', `State: ${state.system.mode} | Rem: ${state.system.mode === 'RELAY_ON' ? state.system.onRemainingSec : state.system.lockoutRemainingSec}s | Heap: ${state.system.freeHeap || '--'}B | RSSI: ${state.wifiRssi}dBm`);

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
  // HELPER: GET SELECTED RELAY CHANNEL
  // ---------------------------------------------------------------------------
  function getSelectedRelay() {
    return state.relays.find(r => r.id === state.selectedRelayId) || state.relays[0] || {
      id: 1, name: 'Relay 1', pin: 5,
      mode: state.system.mode,
      relayActive: state.system.relayActive,
      onRemainingSec: state.system.onRemainingSec,
      totalOnSec: state.system.totalOnSec,
      lockoutRemainingSec: state.system.lockoutRemainingSec,
      totalLockoutSec: state.system.totalLockoutSec,
      onHours: state.config.onHours,
      lockoutDays: state.config.lockoutDays,
      enabled: true
    };
  }

  // ---------------------------------------------------------------------------
  // NOTIFICATION STORE & RELAY STATE TRANSITION DETECTION
  // ---------------------------------------------------------------------------
  const notifications = [];

  function formatSeconds(secs) {
    secs = Math.max(0, Math.floor(secs));
    const d = Math.floor(secs / 86400);
    const h = Math.floor((secs % 86400) / 3600);
    const m = Math.floor((secs % 3600) / 60);
    if (d > 0) return `${d}d ${h}h`;
    if (h > 0) return `${h}h ${m}m`;
    return `${m}m`;
  }

  function pushNotification(icon, title, desc) {
    const time = new Date().toLocaleTimeString();
    notifications.unshift({ icon, title, desc, time });
    if (notifications.length > 50) notifications.pop();
    updateNotifBadge();
  }

  function updateNotifBadge() {
    const badge = document.getElementById('topNotifBadge');
    const navBadge = document.getElementById('navNotifBadge');
    const count = notifications.length;
    if (badge) { badge.textContent = count; badge.style.display = count > 0 ? 'inline-block' : 'none'; }
    if (navBadge) navBadge.textContent = `${Math.min(count, 99)} New`;
  }

  function checkRelayStateTransitions(relays) {
    if (!relays || !Array.isArray(relays)) return;
    relays.forEach(r => {
      const prev = state.prevRelayStates[r.id];
      const isActive = r.relayActive || r.mode === 'RELAY_ON';
      const wasActive = prev ? (prev.relayActive || prev.mode === 'RELAY_ON') : null;
      if (wasActive !== null && isActive !== wasActive) {
        const title = isActive
          ? `🔔 ${r.name} Switched ON`
          : `🔕 ${r.name} Switched OFF`;
        const desc = isActive
          ? `Relay energized — buzzer double-beeped on D8. Countdown: ${formatSeconds(r.totalOnSec)}.`
          : `Relay de-energized — buzzer long-beeped on D8. Lockout: ${formatSeconds(r.totalLockoutSec)}.`;
        pushNotification(isActive ? '⚡' : '🛑', title, desc);
        toast(title, isActive ? 'success' : 'info');
        vibrate(isActive ? [30, 20, 30] : [80]);
      }
      state.prevRelayStates[r.id] = { relayActive: r.relayActive, mode: r.mode };
    });
  }

  // ---------------------------------------------------------------------------
  // NOTIFICATION MODAL
  // ---------------------------------------------------------------------------
  function openNotificationsModal() {
    renderNotificationsList();
    const m = document.getElementById('notificationsModal');
    if (m) m.classList.remove('hidden');
  }

  function closeNotificationsModal() {
    const m = document.getElementById('notificationsModal');
    if (m) m.classList.add('hidden');
  }

  function clearAllNotifications() {
    notifications.length = 0;
    updateNotifBadge();
    renderNotificationsList();
    toast('All notifications cleared', 'info');
  }

  function renderNotificationsList() {
    const list = document.getElementById('notificationsList');
    if (!list) return;
    if (notifications.length === 0) {
      list.innerHTML = '<div class="notif-empty">🔔 No relay events yet. Switch a relay ON or OFF to see alerts here.</div>';
      return;
    }
    list.innerHTML = notifications.map(n => `
      <div class="notif-item">
        <span class="notif-icon">${n.icon}</span>
        <div class="notif-content">
          <div class="notif-title">${escapeHtml(n.title)}</div>
          <div class="notif-desc">${escapeHtml(n.desc)}</div>
          <div class="notif-time">${n.time}</div>
        </div>
      </div>
    `).join('');
  }

  // ---------------------------------------------------------------------------
  // RELAY CHANNEL TAB BAR
  // ---------------------------------------------------------------------------
  function renderRelayTabs() {
    const list = document.getElementById('relayTabsList');
    if (!list) return;
    const isAdmin = state.currentUser && state.currentUser.role === 'admin';
    if (!state.relays.length) return;
    list.innerHTML = state.relays.map(r => `
      <button class="relay-tab-btn ${r.id === state.selectedRelayId ? 'active' : ''}"
        type="button" onclick="App.selectRelayTab(${r.id})">
        ${escapeHtml(r.name)}
        <span class="relay-tab-pin-tag">GPIO${r.pin || '?'}</span>
      </button>
    `).join('') + (isAdmin && state.relays.length < 4 ? `
      <button class="btn-add-relay-pill" type="button" onclick="App.openAddRelayModal()">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="14" height="14">
          <line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/>
        </svg>
        Add Relay
      </button>
    ` : '');
  }

  function selectRelayTab(id) {
    state.selectedRelayId = id;
    renderRelayTabs();
    updateRelayChannelHeader();
    updateRadialGauge();
    updateActionButtons();
  }

  function updateRelayChannelHeader() {
    const r = getSelectedRelay();
    const badge = document.getElementById('relayChannelBadge');
    const title = document.getElementById('relayCardTitle');
    if (badge) badge.textContent = `CHANNEL ${r.id} // GPIO${r.pin || '?'}`;
    if (title) title.textContent = `${r.name} Operational State`;
  }

  // ---------------------------------------------------------------------------
  // SETTINGS: RELAY CHANNEL CARDS
  // ---------------------------------------------------------------------------
  function renderSettingsRelayCards() {
    const list = document.getElementById('settingsRelaysList');
    if (!list) return;
    const isAdmin = state.currentUser && state.currentUser.role === 'admin';
    if (!state.relays.length) {
      list.innerHTML = '<p style="color:var(--text-muted); font-size:0.85rem;">No relay channels configured.</p>';
      return;
    }
    list.innerHTML = state.relays.map(r => `
      <div class="relay-card-item ${r.relayActive ? 'is-active' : ''}">
        <div class="relay-card-head">
          <span class="relay-card-name">
            <span class="status-dot ${r.relayActive ? 'dot-active' : 'dot-off'}"></span>
            ${escapeHtml(r.name)}
          </span>
          <span class="state-badge ${r.mode === 'RELAY_ON' ? 'badge-active' : (r.mode === 'LOCKOUT' ? 'badge-lockout' : 'badge-idle')}" style="font-size:0.68rem;">${r.mode}</span>
        </div>
        <div class="relay-card-grid">
          <div class="relay-card-row"><span class="relay-card-label">GPIO PIN</span><span class="relay-card-val">GPIO${r.pin}</span></div>
          <div class="relay-card-row"><span class="relay-card-label">ON TIMER</span><span class="relay-card-val">${r.onHours}h</span></div>
          <div class="relay-card-row"><span class="relay-card-label">LOCKOUT</span><span class="relay-card-val">${r.lockoutDays}d</span></div>
          <div class="relay-card-row"><span class="relay-card-label">REMAINING</span><span class="relay-card-val">${formatSeconds(r.mode === 'RELAY_ON' ? r.onRemainingSec : r.lockoutRemainingSec)}</span></div>
        </div>
        ${isAdmin ? `<div class="relay-card-actions">
          <button class="glass-button btn-secondary" style="font-size:0.72rem;padding:5px 10px;" type="button" onclick="App.openEditRelayModal(${r.id})">Edit</button>
          ${r.id > 1 ? `<button class="glass-button btn-danger" style="font-size:0.72rem;padding:5px 10px;" type="button" onclick="App.deleteRelay(${r.id})">Remove</button>` : ''}
        </div>` : ''}
      </div>
    `).join('');
  }

  // ---------------------------------------------------------------------------
  // ADD / EDIT RELAY MODAL
  // ---------------------------------------------------------------------------
  function openAddRelayModal() {
    document.getElementById('relayFormId').value = '0';
    document.getElementById('relayModalTitle').textContent = 'Add New Relay Channel';
    document.getElementById('relayFormName').value = '';
    document.getElementById('relayFormPin').value = '4';
    document.getElementById('relayFormOnHours').value = '8.0';
    document.getElementById('relayFormLockoutDays').value = '8.0';
    document.getElementById('relayFormActiveLevel').value = '0';
    const m = document.getElementById('addRelayModal');
    if (m) m.classList.remove('hidden');
  }

  function openEditRelayModal(id) {
    const r = state.relays.find(x => x.id === id);
    if (!r) return;
    document.getElementById('relayFormId').value = String(id);
    document.getElementById('relayModalTitle').textContent = `Edit ${r.name}`;
    document.getElementById('relayFormName').value = r.name;
    document.getElementById('relayFormPin').value = String(r.pin);
    document.getElementById('relayFormOnHours').value = String(r.onHours);
    document.getElementById('relayFormLockoutDays').value = String(r.lockoutDays);
    document.getElementById('relayFormActiveLevel').value = String(r.activeLevel || 0);
    const m = document.getElementById('addRelayModal');
    if (m) m.classList.remove('hidden');
  }

  function closeAddRelayModal() {
    const m = document.getElementById('addRelayModal');
    if (m) m.classList.add('hidden');
  }

  async function handleRelayFormSubmit() {
    const id = parseInt(document.getElementById('relayFormId').value, 10);
    const name = (document.getElementById('relayFormName').value || '').trim();
    const pin = parseInt(document.getElementById('relayFormPin').value, 10);
    const onHours = parseFloat(document.getElementById('relayFormOnHours').value);
    const lockoutDays = parseFloat(document.getElementById('relayFormLockoutDays').value);
    const activeLevel = parseInt(document.getElementById('relayFormActiveLevel').value, 10);

    if (!name) { toast('Please enter a relay name', 'error'); return; }
    if (isNaN(pin) || pin < 0) { toast('Invalid GPIO pin', 'error'); return; }

    const url = `${state.espHost.replace(/\/+$/, '')}/api/relay/${id === 0 ? 'add' : 'update'}`;
    try {
      const res = await fetch(url, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ id, name, pin, onHours, lockoutDays, activeLevel,
          userRole: state.currentUser ? state.currentUser.role : '' })
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.message || 'Failed');
      toast(data.message || 'Relay saved', 'success');
      closeAddRelayModal();
      fetchStatus();
    } catch (e) {
      toast('Error: ' + e.message, 'error');
    }
  }

  async function deleteRelay(id) {
    if (!confirm(`Remove Relay channel ${id} from hardware? The GPIO pin will be released.`)) return;
    const url = `${state.espHost.replace(/\/+$/, '')}/api/relay/delete`;
    try {
      const res = await fetch(url, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ id, userRole: state.currentUser ? state.currentUser.role : '' })
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.message || 'Failed');
      toast('Relay removed', 'success');
      if (state.selectedRelayId === id) state.selectedRelayId = 1;
      fetchStatus();
    } catch (e) {
      toast('Delete failed: ' + e.message, 'error');
    }
  }

  // ---------------------------------------------------------------------------
  // BUZZER TEST
  // ---------------------------------------------------------------------------
  async function testBuzzerSound() {
    toast('Sending buzzer test to ESP8266...', 'info');
    try {
      const res = await fetch(`${state.espHost.replace(/\/+$/, '')}/api/control`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          action: 'buzzer_test',
          userRole: state.currentUser ? state.currentUser.role : ''
        })
      });
      if (res.ok) toast('Buzzer test sent! Listen for D8 beep.', 'success');
      else toast('Buzzer test failed — check ESP connection.', 'error');
    } catch (e) {
      toast('Could not reach ESP. Check connection.', 'error');
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
    if (!checkLocalWifiBeforeAction('Relay Power Control')) return;
    if (state.currentUser.role === 'guest_analytics') {
      vibrate(200);
      toast('Access Denied: Guest accounts are read-only', 'error');
      return;
    }

    const r = getSelectedRelay();
    // If relay is currently ON (countdown running), clicking turns it OFF!
    if (r.relayActive || r.mode === 'RELAY_ON') {
      await cancelCycle();
      return;
    }

    if (state.currentUser.role === 'guarded_operator') {
      if (!state.oneTimeToken) {
        requestOneTimeAccess('TRIGGER_RELAY');
        return;
      }
    }
    await executeControlCommand('trigger', { relayId: state.selectedRelayId || 1 });
  }

  async function handleToggleClick() {
    if (!state.currentUser) {
      toast('Please sign in first', 'error');
      return;
    }
    if (!checkLocalWifiBeforeAction('Toggle Relay Power')) return;
    if (state.currentUser.role === 'guest_analytics') {
      vibrate(200);
      toast('Access Denied: Guest accounts are read-only', 'error');
      return;
    }
    const r = getSelectedRelay();
    const action = (r.relayActive || r.mode === 'RELAY_ON') ? 'cancel' : 'trigger';
    await executeControlCommand(action, { relayId: r.id });
  }

  async function executeControlCommand(action, extraParams = {}) {
    vibrate(40);
    const relayId = extraParams.relayId !== undefined ? extraParams.relayId : (state.selectedRelayId || 1);
    const params = { relayId, ...extraParams };

    if (state.simulated) {
      handleSimulatedAction(action);
      return;
    }

    if (state.connectionMode === 'cloud') {
      await sendFirebaseCommand(action.toUpperCase(), params);
    } else {
      await sendLocalEspCommand(action, params);
    }
  }

  async function sendLocalEspCommand(action, extraParams = {}) {
    const url = `${state.espHost.replace(/\/+$/, '')}/api/control`;
    try {
      const res = await fetch(url, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          action: action,
          userRole: state.currentUser ? state.currentUser.role : '',
          oneTimeToken: state.oneTimeToken || '',
          relayId: state.selectedRelayId || 1,
          ...extraParams
        })
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.message || 'ESP execution failed');
      toast(data.message || `Command ${action} executed`, 'success');
      logCloudEvent('CMD', `POST /api/control -> ${action} (Relay ${extraParams.relayId || 1}) (HTTP 200)`);
      state.oneTimeToken = '';
      fetchStatus();
    } catch (e) {
      toast(e.message || 'Command failed', 'error');
      logCloudEvent('ERR', `Command ${action} failed: ${e.message}`);
    }
  }

  async function sendFirebaseCommand(actionName, extraParams) {
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
      timestamp: Math.floor(Date.now() / 1000),
      ...(extraParams || {})
    };

    try {
      const res = await fetch(url, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(commandPayload)
      });
      if (!res.ok) throw new Error(`Firebase Error HTTP ${res.status}`);
      toast(`Cloud Command '${actionName}' dispatched worldwide`, 'success');
      logCloudEvent('CMD', `PUT /commands.json -> ${actionName} by ${state.currentUser ? state.currentUser.username : 'USER'}`);
      state.oneTimeToken = '';
      fetchStatus();
    } catch (e) {
      toast(`Cloud dispatch failed: ${e.message}`, 'error');
      logCloudEvent('ERR', `Cloud dispatch failed: ${e.message}`);
    }
  }

  async function cancelCycle() {
    if (!checkLocalWifiBeforeAction('Emergency Cutoff')) return;
    if (state.currentUser && state.currentUser.role === 'guest_analytics') {
      toast('Access Denied: Guest accounts are read-only', 'error');
      return;
    }
    const r = getSelectedRelay();
    if (confirm(`Emergency Cutoff: Turn off ${r.name} now?`)) {
      await executeControlCommand('cancel', { relayId: r.id });
    }
  }

  async function resetLockout() {
    if (!checkLocalWifiBeforeAction('Reset Lockout')) return;
    if (!state.currentUser || state.currentUser.role !== 'admin') {
      vibrate(200);
      toast('Strict Admin Protection: Only Yash_Gajjar can bypass lockout', 'error');
      return;
    }
    const r = getSelectedRelay();
    if (confirm(`Bypass lockout cooldown on ${r.name}?`)) {
      await executeControlCommand('reset_lockout', { relayId: r.id });
    }
  }

  // ---------------------------------------------------------------------------
  // EXTEND ACTIVE RELAY TIMER (FOR ADMIN)
  // ---------------------------------------------------------------------------
  async function extendActiveTimer(minutes) {
    if (!checkLocalWifiBeforeAction('Extend Timer')) return;
    if (!state.currentUser || state.currentUser.role !== 'admin') {
      toast('Admin Authority Required: Only Super Administrator can extend active timer', 'error');
      return;
    }
    if (state.system.mode !== 'RELAY_ON') {
      toast('Cannot extend: Relay is not actively energized', 'error');
      return;
    }
    vibrate(40);
    const extraSec = minutes * 60;

    if (state.simulated) {
      state.system.onRemainingSec += extraSec;
      state.system.totalOnSec += extraSec;
      toast(`Timer extended by +${minutes} minutes (Simulation)`, 'success');
      logCloudEvent('CMD', `EXTEND +${minutes}m applied in Simulation mode`);
      updateUI();
      return;
    }

    if (state.connectionMode === 'cloud') {
      await sendFirebaseCommand('EXTEND', { minutes: minutes, seconds: extraSec });
      toast(`Requested +${minutes} min extension via Firebase Cloud`, 'success');
      logCloudEvent('CMD', `PUT /commands.json -> EXTEND +${minutes}m by ${state.currentUser.username}`);
    } else {
      const url = `${state.espHost.replace(/\/+$/, '')}/api/control`;
      try {
        const res = await fetch(url, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ action: 'extend', minutes: minutes, userRole: state.currentUser.role })
        });
        const data = await res.json();
        if (!res.ok) throw new Error(data.message || 'Failed to extend timer');
        toast(`Timer extended by +${minutes} minutes on ESP8266`, 'success');
        logCloudEvent('CMD', `POST /api/control -> extend +${minutes}m (HTTP 200)`);
        fetchStatus();
      } catch (e) {
        toast(e.message || 'Failed to extend timer', 'error');
        logCloudEvent('ERR', `Failed to extend timer: ${e.message}`);
      }
    }
  }

  function extendActiveTimerCustom() {
    const input = document.getElementById('inputCustomExtendMinutes');
    if (!input) return;
    const val = parseInt(input.value, 10);
    if (!val || isNaN(val) || val <= 0) {
      toast('Please enter a valid number of minutes (1-1440)', 'error');
      return;
    }
    input.value = '';
    extendActiveTimer(val);
  }

  // ---------------------------------------------------------------------------
  // ADMIN TEMPORARY DISABLE / ENABLE USER ACCOUNTS
  // ---------------------------------------------------------------------------
  async function toggleUserStatus(targetUsername, newStatus) {
    if (!checkLocalWifiBeforeAction('User Management')) return;
    if (!state.currentUser || state.currentUser.role !== 'admin') {
      toast('Only Administrator can change account status', 'error');
      return;
    }
    if (targetUsername.toLowerCase() === 'yash_gajjar' && newStatus === 'disabled') {
      if (!confirm('Warning: Disabling Yash_Gajjar will lock out the primary Master Administrator. Are you sure?')) {
        return;
      }
    }
    vibrate(50);
    const u = state.users.find(x => x.username.toLowerCase() === targetUsername.toLowerCase());
    if (u) u.status = newStatus;

    const disabledList = JSON.parse(localStorage.getItem('cyberrelay_disabled_accounts') || '[]');
    if (newStatus === 'disabled') {
      if (!disabledList.includes(targetUsername)) disabledList.push(targetUsername);
    } else {
      const idx = disabledList.indexOf(targetUsername);
      if (idx >= 0) disabledList.splice(idx, 1);
    }
    localStorage.setItem('cyberrelay_disabled_accounts', JSON.stringify(disabledList));
    toast(`Account ${targetUsername} ${newStatus === 'disabled' ? 'disabled' : 'enabled'}`, 'success');
    updateUserProfile();
  }

  // ---------------------------------------------------------------------------
  // ONE-TIME TOKEN / ACCESS REQUEST (MAHI_GAJJAR GUARDED_OPERATOR FLOW)
  // ---------------------------------------------------------------------------
  function requestOneTimeAccess(action) {
    const requestId = 'REQ_' + Math.random().toString(36).substr(2, 8).toUpperCase();
    state.pendingRequest = { active: true, requestId, username: state.currentUser.username, action };
    state.waitingForApproval = true;

    sendFirebaseCommand('REQUEST_ACCESS', {
      requestId, action,
      requestedBy: state.currentUser.username
    });

    showWaitingModal(requestId);
    toast('Request sent to Admin for one-time approval', 'info');
  }

  function showWaitingModal(requestId) {
    const m = document.getElementById('waitingApprovalModal');
    const reqEl = document.getElementById('waitingRequestId');
    if (m) m.classList.remove('hidden');
    if (reqEl) reqEl.textContent = requestId;
  }

  function closeWaitingModal() {
    const m = document.getElementById('waitingApprovalModal');
    if (m) m.classList.add('hidden');
  }

  async function cancelWaitingRequest() {
    state.waitingForApproval = false;
    state.pendingRequest.active = false;
    state.oneTimeToken = '';
    closeWaitingModal();
    toast('Access request cancelled', 'info');
  }

  function showAdminApprovalModal(username, action) {
    const m = document.getElementById('adminApprovalModal');
    const unEl = document.getElementById('approvalRequestUser');
    const actEl = document.getElementById('approvalRequestAction');
    if (m) m.classList.remove('hidden');
    if (unEl) unEl.textContent = username;
    if (actEl) actEl.textContent = action;
  }

  function dismissAdminApprovalModal() {
    const m = document.getElementById('adminApprovalModal');
    if (m) m.classList.add('hidden');
  }

  async function submitAdminApproval() {
    if (!state.pendingRequest.active) return;
    const token = 'TOK_' + Math.random().toString(36).substr(2, 12).toUpperCase();
    await sendFirebaseCommand('GRANT_ACCESS', {
      requestId: state.pendingRequest.requestId,
      token,
      grantedTo: state.pendingRequest.username,
      grantedBy: state.currentUser.username
    });
    dismissAdminApprovalModal();
    toast(`One-time token granted to ${state.pendingRequest.username}`, 'success');
    state.pendingRequest.active = false;
  }

  async function rejectAdminApproval() {
    await sendFirebaseCommand('REJECT_ACCESS', {
      requestId: state.pendingRequest.requestId,
      rejectedBy: state.currentUser.username
    });
    dismissAdminApprovalModal();
    state.pendingRequest.active = false;
    toast('Access request rejected', 'info');
  }

  // ---------------------------------------------------------------------------
  // TIMER CONFIG / SETTINGS
  // ---------------------------------------------------------------------------
  function onTimerInput() {
    const hrs = parseFloat(document.getElementById('inputOnHours')?.value || 0);
    const days = parseFloat(document.getElementById('inputLockoutDays')?.value || 0);
    const preview = document.getElementById('timerPreviewText');
    if (preview) {
      preview.textContent = `ON for ${hrs}h → Lockout ${days}d (${(days * 24).toFixed(0)}h)`;
    }
  }

  async function saveTimers() {
    if (!checkLocalWifiBeforeAction('Save Timer Config')) return;
    if (!state.currentUser || state.currentUser.role !== 'admin') {
      toast('Admin access required to change timer settings', 'error');
      return;
    }
    const hrs = parseFloat(document.getElementById('inputOnHours')?.value || 8);
    const days = parseFloat(document.getElementById('inputLockoutDays')?.value || 8);
    if (isNaN(hrs) || hrs <= 0 || isNaN(days) || days <= 0) {
      toast('Invalid timer values', 'error');
      return;
    }
    state.config.onHours = hrs;
    state.config.lockoutDays = days;

    if (state.simulated) {
      state.system.totalOnSec = hrs * 3600;
      state.system.totalLockoutSec = days * 86400;
      toast('Timer config saved (Simulation)', 'success');
      updateUI();
      return;
    }

    if (state.connectionMode === 'cloud') {
      await sendFirebaseCommand('UPDATE_CONFIG', { onHours: hrs, lockoutDays: days });
      toast('Config dispatched via Firebase', 'success');
    } else {
      const url = `${state.espHost.replace(/\/+$/, '')}/api/settings`;
      try {
        const res = await fetch(url, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ onHours: hrs, lockoutDays: days, userRole: state.currentUser.role })
        });
        const data = await res.json();
        if (!res.ok) throw new Error(data.message || 'Save failed');
        toast('Timer config saved on ESP8266', 'success');
        fetchStatus();
      } catch (e) {
        toast('Failed to save config: ' + e.message, 'error');
      }
    }
  }

  // ---------------------------------------------------------------------------
  // CLOUD CONFIG MODAL
  // ---------------------------------------------------------------------------
  function openCloudConfigModal() {
    const m = document.getElementById('cloudConfigModal');
    const hInput = document.getElementById('cfgFirebaseHost');
    const aInput = document.getElementById('cfgFirebaseAuth');
    if (hInput) hInput.value = state.firebaseHost;
    if (aInput) aInput.value = state.firebaseAuth;
    if (m) m.classList.remove('hidden');
  }

  function closeCloudConfigModal() {
    const m = document.getElementById('cloudConfigModal');
    if (m) m.classList.add('hidden');
  }

  async function saveCloudSettings() {
    const host = (document.getElementById('cfgFirebaseHost')?.value || '').trim();
    const auth = (document.getElementById('cfgFirebaseAuth')?.value || '').trim();
    if (!host) { toast('Firebase host is required', 'error'); return; }
    state.firebaseHost = host;
    state.firebaseAuth = auth;
    localStorage.setItem('cyberrelay_firebase_host', host);
    localStorage.setItem('cyberrelay_firebase_auth', auth);
    closeCloudConfigModal();
    toast('Cloud config saved', 'success');
  }

  async function testCloudConnection() {
    let host = (document.getElementById('cfgFirebaseHost')?.value || state.firebaseHost || '').trim().replace(/^https?:\/\//, '').replace(/\/+$/, '');
    if (!host) host = 'cyber-relay-rtos-default-rtdb-default-rtdb.firebaseio.com';
    let auth = (document.getElementById('cfgFirebaseAuth')?.value || state.firebaseAuth || '').trim();
    let url = `https://${host}/esp8266_relay/status.json`;
    if (auth) url += `?auth=${encodeURIComponent(auth)}`;
    toast('Testing Firebase connection...', 'info');
    try {
      const res = await fetch(url, { signal: AbortSignal.timeout(6000) });
      if (res.ok) {
        toast('Firebase connection SUCCESSFUL! (HTTP 200 OK)', 'success');
        logConsole('FIREBASE', `Test Ping to https://${host} succeeded (HTTP 200)`);
      } else if (res.status === 404) {
        toast('HTTP 404: Database not created yet in Firebase Console!', 'error');
        logConsole('ERR', `Test Ping to https://${host} failed: 404 Not Found. Please click "Create Database" in Firebase Console!`);
      } else if (res.status === 401 || res.status === 403) {
        toast('HTTP 401/403: Permission Denied! Set Security Rules to Test Mode.', 'error');
        logConsole('ERR', `Test Ping to https://${host} failed: ${res.status} Permission Denied (Update rules to: .read:true, .write:true)`);
      } else {
        toast(`Firebase returned HTTP ${res.status}`, 'error');
        logConsole('ERR', `Test Ping returned HTTP ${res.status}`);
      }
    } catch (e) {
      toast('Firebase unreachable: ' + e.message, 'error');
      logConsole('ERR', 'Firebase network/fetch error: ' + e.message);
    }
  }

  async function testLocalConnection() {
    if (!state.espHost) { toast('No ESP host configured', 'error'); return; }
    toast('Testing local ESP connection...', 'info');
    try {
      const res = await fetch(`${state.espHost.replace(/\/+$/, '')}/api/status`, { signal: AbortSignal.timeout(5000) });
      if (res.ok) toast('ESP8266 local connection OK!', 'success');
      else toast(`ESP returned HTTP ${res.status}`, 'error');
    } catch (e) {
      toast('ESP unreachable: ' + e.message, 'error');
    }
  }

  // ---------------------------------------------------------------------------
  // FORGOT PASSWORD MODAL
  // ---------------------------------------------------------------------------
  function openForgotPasswordModal() {
    const m = document.getElementById('forgotPasswordModal');
    if (m) m.classList.remove('hidden');
  }

  function closeForgotPasswordModal() {
    const m = document.getElementById('forgotPasswordModal');
    if (m) m.classList.add('hidden');
  }

  function onResetUserChange() {
    const sel = document.getElementById('resetTargetUser');
    const keyRow = document.getElementById('resetKeyRow');
    if (!sel || !keyRow) return;
    keyRow.style.display = sel.value ? 'block' : 'none';
  }

  async function submitForgotPassword() {
    if (!checkLocalWifiBeforeAction('Reset Password')) return;
    const target = document.getElementById('resetTargetUser')?.value;
    const key = document.getElementById('resetRecoveryKey')?.value?.trim();
    const newPw = document.getElementById('resetNewPassword')?.value?.trim();
    if (!target || !key || !newPw) { toast('All fields required', 'error'); return; }
    if (newPw.length < 4) { toast('Password must be at least 4 characters', 'error'); return; }

    const url = `${state.espHost.replace(/\/+$/, '')}/api/reset-password`;
    try {
      const res = await fetch(url, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ target, authKey: key, newPassword: newPw })
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.message || 'Reset failed');
      toast('Password reset successful', 'success');
      closeForgotPasswordModal();
    } catch (e) {
      toast('Reset failed: ' + e.message, 'error');
    }
  }


  // ---------------------------------------------------------------------------
  // SIMULATED ACTION (FOR DEMO / OFFLINE MODE)
  // ---------------------------------------------------------------------------
  function handleSimulatedAction(action) {
    if (action === 'trigger') {
      if (state.system.mode !== 'IDLE') {
        toast('Cannot trigger: System is not IDLE', 'error'); return;
      }
      state.system.mode = 'RELAY_ON';
      state.system.relayActive = true;
      state.system.lockoutActive = false;
      state.system.onRemainingSec = state.system.totalOnSec || state.config.onHours * 3600;
      toast('Simulated: Relay ACTIVATED', 'success');
    } else if (action === 'cancel') {
      state.system.mode = 'LOCKOUT';
      state.system.relayActive = false;
      state.system.lockoutActive = true;
      state.system.lockoutRemainingSec = state.system.totalLockoutSec || state.config.lockoutDays * 86400;
      toast('Simulated: Relay cancelled → LOCKOUT', 'info');
    } else if (action === 'reset_lockout') {
      state.system.mode = 'IDLE';
      state.system.lockoutActive = false;
      state.system.lockoutRemainingSec = 0;
      toast('Simulated: Lockout RESET → IDLE', 'success');
    }
    updateUI();
  }

  // ---------------------------------------------------------------------------
  // CONNECTION ACKNOWLEDGE (3-BEEP CHIME) & WIRELESS LOGS
  // ---------------------------------------------------------------------------
  function renderAcknowledgeView() {
    const badge = document.getElementById('ackVerifiedBadge');
    const navBadge = document.getElementById('navAckBadge');
    const title = document.getElementById('ackBannerStatusTitle');
    const sub = document.getElementById('ackBannerStatusSub');

    // Stage 1: Boot & Restart
    const s1Desc = document.getElementById('ackStep1Desc');
    const s1Detail = document.getElementById('ackStep1Detail');
    if (s1Desc) s1Desc.textContent = `Boot Reason: ${state.ack.bootReason || 'Power On'}`;
    if (s1Detail) s1Detail.textContent = `Uptime: ${formatSeconds(state.system.uptimeSeconds || 0)} // RAM: ${state.system.freeHeap || 28000}B`;

    // Stage 2: SoftAP & Wi-Fi Network
    const s2Desc = document.getElementById('ackStep2Desc');
    const s2Detail = document.getElementById('ackStep2Detail');
    if (s2Desc) s2Desc.textContent = `SoftAP: ${state.ack.softApSsid || 'CYBER-RELAY-HUB'} (${state.ack.softApIp || '192.168.4.1'})`;
    if (s2Detail) {
      if (state.ack.wifiConnected && state.ack.staIp && state.ack.staIp !== '0.0.0.0') {
        s2Detail.textContent = `Station: Connected (${state.ack.staIp}) ${state.wifiRssi ? state.wifiRssi + 'dBm' : ''}`;
      } else {
        s2Detail.textContent = 'Station: Standalone AP Mode';
      }
    }

    // Stage 3: Firebase Cloud Handshake
    const s3Desc = document.getElementById('ackStep3Desc');
    const s3Detail = document.getElementById('ackStep3Detail');
    const isCloudSynced = state.connected && (state.connectionMode === 'cloud' || state.ack.verified);
    if (s3Desc) s3Desc.textContent = isCloudSynced ? 'Realtime Stream Active' : (state.connected ? 'Local AP Direct Link' : 'Offline / Awaiting Connection');
    if (s3Detail) s3Detail.textContent = state.ack.verified ? 'Handshake Verified (3 Beeps Sounded)' : 'Pending 3-Beep Chime Handshake';

    if (badge) {
      badge.textContent = state.ack.verified ? 'ACK VERIFIED (3-BEEP)' : (state.connected ? 'CONNECTED (READY)' : 'AWAITING LINK');
      badge.className = 'state-badge ' + (state.ack.verified ? 'badge-active' : (state.connected ? 'badge-lockout' : 'badge-idle'));
    }
    if (navBadge) {
      navBadge.textContent = state.ack.verified ? '3x ACK' : 'READY';
    }
    if (title && sub) {
      if (state.ack.verified) {
        title.textContent = 'Handshake Acknowledged & Verified (3 Beeps Sounded)';
        sub.textContent = 'Device connected to Wi-Fi and confirmed realtime synchronization with cloud.';
      } else {
        title.textContent = 'Connection Handshake Ready';
        sub.textContent = 'Press the button below to verify Wi-Fi + Firebase pipeline and sound the 3-beep buzzer.';
      }
    }

    // Render Wireless & Device System Logs
    renderSystemLogs();
  }

  function renderSystemLogs() {
    const list = document.getElementById('systemLogsList');
    if (!list) return;

    if (!state.systemLogs || state.systemLogs.length === 0) {
      list.innerHTML = '<div class="terminal-line" style="color:var(--text-muted); padding:8px;">No wireless hardware logs yet. Connect to device to pull system traces.</div>';
      return;
    }

    list.innerHTML = state.systemLogs.map(l => {
      let tagClass = '';
      const tag = (l.tag || '').toUpperCase();
      if (tag.includes('BOOT') || tag.includes('RESTART')) tagClass = 'tag-boot';
      else if (tag.includes('WIFI') || tag.includes('AP')) tagClass = 'tag-wifi';
      else if (tag.includes('ACK')) tagClass = 'tag-ack';
      else if (tag.includes('CLOUD')) tagClass = 'tag-cloud';

      return `
        <div class="syslog-row">
          <span class="syslog-uptime">+${formatSeconds(l.uptime || 0)}</span>
          <span class="syslog-tag ${tagClass}">${escapeHtml(l.tag || 'SYS')}</span>
          <span class="syslog-msg">${escapeHtml(l.msg || '')}</span>
        </div>
      `;
    }).join('');
    list.scrollTop = list.scrollHeight;
  }

  async function triggerAckHandshake() {
    toast('Verifying Wi-Fi + Firebase connection & triggering 3-beep buzzer...', 'info');
    vibrate([40, 60, 40, 60, 40]);

    if (state.connectionMode === 'cloud') {
      await sendFirebaseCommand('CHECK_ACK', {});
      toast('ACK command dispatched via Firebase Cloud! Listen for 3 beeps.', 'success');
      state.ack.verified = true;
      renderAcknowledgeView();
    } else {
      const url = `${state.espHost.replace(/\/+$/, '')}/api/control`;
      try {
        const res = await fetch(url, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            action: 'check_ack',
            userRole: state.currentUser ? state.currentUser.role : ''
          })
        });
        const data = await res.json();
        if (!res.ok) throw new Error(data.message || 'Failed');
        toast('3-Beep ACK Handshake Verified on ESP8266!', 'success');
        state.ack.verified = true;
        fetchStatus();
      } catch (e) {
        toast('Could not verify handshake: ' + e.message, 'error');
      }
    }
  }

  // ---------------------------------------------------------------------------
  // UI UPDATE ENGINE
  // ---------------------------------------------------------------------------
  function updateUI() {
    updateConnectionStatus();
    updateUserProfile();
    updateOperationalState();
    updateRadialGauge();
    updateHardwareTiles();
    updateActionButtons();
    updateAnalyticsDisplay();
    updateSidebarBadges();
    updateDiagnosticsDisplay();
    renderRelayTabs();
    renderSettingsRelayCards();
    updateRelayChannelHeader();
    renderAcknowledgeView();
    renderLiveConsole();
    updateConsoleBadges();
  }

  function updateConsoleBadges() {
    const connBadge = document.getElementById('consoleConnBadge');
    const projLabel = document.getElementById('lblConsoleProject');
    if (connBadge) {
      connBadge.textContent = state.connected ? `${state.connectionMode.toUpperCase()} ACTIVE` : 'OFFLINE';
      connBadge.className = 'info-badge ' + (state.connected ? 'badge-active' : 'badge-idle');
    }
    if (projLabel && state.firebase) {
      projLabel.textContent = state.firebase.projectId;
    }
  }

  function updateSidebarBadges() {
    const rBadge = document.getElementById('navRelayBadge');
    const tBadge = document.getElementById('navTimersBadge');
    const dBadge = document.getElementById('navDutyBadge');
    const uBadge = document.getElementById('navUsersBadge');
    const cBadge = document.getElementById('navCloudBadge');
    const pBadge = document.getElementById('navPingBadge');

    if (rBadge) {
      rBadge.textContent = state.system.mode === 'RELAY_ON' ? 'ACTIVE' : (state.system.mode === 'LOCKOUT' ? 'LOCKED' : 'READY');
    }
    if (tBadge) tBadge.textContent = `${state.config.onHours}h / ${state.config.lockoutDays}d`;
    if (dBadge) dBadge.textContent = `${state.analytics.dutyPercent}%`;
    if (uBadge) uBadge.textContent = `${state.users.length} Enrolled`;
    if (cBadge) cBadge.textContent = state.connected ? 'Online' : 'Offline';
    if (pBadge) pBadge.textContent = state.pingMs !== null ? `${state.pingMs} ms` : '-- ms';

    const extendPanel = document.getElementById('adminExtendPanel');
    if (extendPanel) {
      const showExtend = (state.system.mode === 'RELAY_ON') && state.currentUser && (state.currentUser.role === 'admin');
      extendPanel.style.display = showExtend ? 'block' : 'none';
    }
  }

  function updateDiagnosticsDisplay() {
    const rssiNum = document.getElementById('diagRssiNum');
    const sigQual = document.getElementById('diagSignalQuality');
    const pingNum = document.getElementById('diagPingNum');
    const heapNum = document.getElementById('diagHeapNum');

    if (rssiNum) rssiNum.textContent = state.wifiRssi || -52;
    if (sigQual) {
      const r = state.wifiRssi || -52;
      if (r >= -60) sigQual.textContent = 'Excellent Signal (95%)';
      else if (r >= -72) sigQual.textContent = 'Good Signal (80%)';
      else if (r >= -82) sigQual.textContent = 'Fair Signal (60%)';
      else sigQual.textContent = 'Weak Signal (35%)';
    }
    if (pingNum) pingNum.textContent = state.pingMs !== null ? state.pingMs : '--';
    if (heapNum) heapNum.textContent = state.system.freeHeap || '28,416';
  }

  function updateConnectionStatus() {
    const pill = document.getElementById('connStatusPill');
    const text = document.getElementById('connStatusText');
    const pingVal = document.getElementById('headerPingVal');
    const rssiVal = document.getElementById('headerRssiVal');
    const pingPill = document.getElementById('headerPingPill');

    if (pingVal) {
      pingVal.textContent = state.pingMs !== null ? `${state.pingMs} ms` : '-- ms';
      if (pingPill) {
        if (state.pingMs && state.pingMs < 45) pingPill.className = 'diag-pill ping-good';
        else if (state.pingMs && state.pingMs < 120) pingPill.className = 'diag-pill ping-mid';
        else pingPill.className = 'diag-pill ping-slow';
      }
    }
    if (rssiVal) rssiVal.textContent = state.wifiRssi !== null ? `${state.wifiRssi} dBm` : '-- dBm';

    if (pill && text) {
      if (state.connected) {
        pill.className = 'conn-status-pill pill-connected';
        text.textContent = state.connectionMode === 'cloud' ? 'Firebase Connected' : 'ESP8266 Connected';
      } else {
        pill.className = 'conn-status-pill pill-offline';
        text.textContent = 'Offline';
      }
    }
  }

  function updateOperationalState() {
    const badge = document.getElementById('modeBadge');
    const modeText = document.getElementById('modeText');
    if (badge) {
      badge.className = 'mode-badge ' + (state.system.mode === 'RELAY_ON' ? 'badge-active' : (state.system.mode === 'LOCKOUT' ? 'badge-lockout' : 'badge-idle'));
      badge.textContent = state.system.mode;
    }
    if (modeText) {
      if (state.system.mode === 'RELAY_ON') modeText.textContent = 'Relay Energized';
      else if (state.system.mode === 'LOCKOUT') modeText.textContent = 'Cooldown Lockout';
      else modeText.textContent = 'System Ready';
    }
  }

  function updateUserProfile() {
    const nameEl = document.getElementById('profileUsername');
    const roleEl = document.getElementById('profileRole');
    const avatarEl = document.getElementById('profileAvatar');
    if (!state.currentUser) return;
    if (nameEl) nameEl.textContent = state.currentUser.username;
    if (roleEl) roleEl.textContent = state.currentUser.role;
    if (avatarEl) avatarEl.textContent = (state.currentUser.username || 'U')[0].toUpperCase();
  }

  function updateRadialGauge() {
    const progressCircle = document.getElementById('radialProgress');
    const countLabel = document.getElementById('countdownLabel');
    const countSub = document.getElementById('countdownSubtext');
    const bulbEl = document.getElementById('relayBulb');

    const r = getSelectedRelay();
    const mode = r.mode || state.system.mode;
    let total = 1, rem = 0, strokeColor = 'url(#cyanGlow)';

    if (bulbEl) {
      bulbEl.classList.remove('bulb-charging-active', 'bulb-cooldown', 'bulb-ready', 'bulb-on');
    }

    if (mode === 'RELAY_ON') {
      total = r.totalOnSec || state.system.totalOnSec || (state.config.onHours * 3600) || 1;
      rem = r.onRemainingSec !== undefined ? r.onRemainingSec : state.system.onRemainingSec;
      strokeColor = 'url(#cyanGlow)';
      if (countLabel) countLabel.textContent = 'CHARGING ACTIVE (8.0h)';
      if (countSub) countSub.textContent = 'Relay energized. Charging duty countdown in progress.';
      if (bulbEl) bulbEl.classList.add('bulb-charging-active', 'bulb-on');
    } else if (mode === 'LOCKOUT') {
      total = r.totalLockoutSec || state.system.totalLockoutSec || (state.config.lockoutDays * 86400) || 1;
      rem = r.lockoutRemainingSec !== undefined ? r.lockoutRemainingSec : state.system.lockoutRemainingSec;
      strokeColor = 'url(#amberGlow)';
      if (countLabel) countLabel.textContent = 'COOLDOWN / RESTING (7.0d)';
      if (countSub) countSub.textContent = 'Charger resting. Charging button locked until timer reaches zero.';
      if (bulbEl) bulbEl.classList.add('bulb-cooldown');
    } else {
      total = (r.onHours || state.config.onHours || 8.0) * 3600;
      rem = 0;
      strokeColor = 'url(#emeraldGlow)';
      if (countLabel) countLabel.textContent = 'CHARGER READY';
      if (countSub) countSub.textContent = 'Cooldown complete! Tap button below to start 8.0h charging.';
      if (bulbEl) bulbEl.classList.add('bulb-ready');
    }

    if (progressCircle) {
      const GAUGE_CIRCUMFERENCE = 2 * Math.PI * 110; // Exactly 691.15 px for r=110
      const ratio = total > 0 ? Math.max(0, Math.min(1, rem / total)) : 0;
      const offset = (mode === 'IDLE') ? 0 : (GAUGE_CIRCUMFERENCE * (1 - ratio));
      progressCircle.style.strokeDashoffset = offset;
      progressCircle.style.stroke = strokeColor;
    }

    if (mode === 'IDLE') {
      // In ready mode, show the configured charge duration ready to start (e.g. 00d : 08h : 00m : 00s)
      const chargeSec = (r.onHours || state.config.onHours || 8.0) * 3600;
      updateClock(chargeSec);
    } else {
      updateClock(rem);
    }
  }

  function updateClock(totalSec) {
    totalSec = Math.max(0, Math.floor(totalSec));
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

    const tDot = document.getElementById('touchStatusDot');
    const tText = document.getElementById('touchStatusText');
    if (tDot && tText) {
      if (state.system.touchActive && !state.system.lockoutActive) {
        tDot.className = 'status-dot dot-active';
        tText.textContent = 'READY';
      } else {
        tDot.className = 'status-dot dot-disabled';
        tText.textContent = 'FILTERED (OFF)';
      }
    }

    const lDot = document.getElementById('lockoutStatusDot');
    const lText = document.getElementById('lockoutStatusText');
    if (lDot && lText) {
      if (state.system.lockoutActive) {
        lDot.className = 'status-dot dot-locked';
        lText.textContent = 'COOLDOWN';
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
    const btnToggle = document.getElementById('btnDirectToggle');
    const toggleText = document.getElementById('directToggleText');

    const role = state.currentUser ? state.currentUser.role : 'guest_analytics';
    const r = getSelectedRelay();
    const isRelayOn = r.relayActive || r.mode === 'RELAY_ON';
    const isLockout = r.mode === 'LOCKOUT';
    const isReady = (r.mode === 'IDLE');

    // Primary Charging Button
    if (btnTrigger) {
      if (role === 'guest_analytics') {
        btnTrigger.disabled = true;
        btnTrigger.className = 'glass-button btn-accent btn-locked';
        if (trgText) trgText.textContent = 'Controls Disabled (Guest Mode)';
      } else if (isRelayOn) {
        // Charging duty active: button allows emergency/immediate shutdown
        btnTrigger.disabled = false;
        btnTrigger.className = 'glass-button btn-danger';
        if (trgText) trgText.textContent = '🛑 Stop Charging / Turn Relay OFF Now';
      } else if (isLockout) {
        // 7-day cooldown running: button is strictly DISABLED
        btnTrigger.disabled = true;
        btnTrigger.className = 'glass-button btn-accent btn-locked';
        if (trgText) trgText.textContent = '🔒 Charging Locked (Cooldown in Progress)';
      } else if (role === 'guarded_operator') {
        btnTrigger.disabled = false;
        btnTrigger.className = 'glass-button btn-accent btn-ready-pulse';
        if (trgText) trgText.textContent = '⚡ Request 1-Time Charging (Mahi)';
      } else {
        // Cooldown finished (READY): button is ACTIVE & ENABLED!
        btnTrigger.disabled = false;
        btnTrigger.className = 'glass-button btn-accent btn-ready-pulse';
        if (trgText) trgText.textContent = '⚡ Start 8.0h Charging Cycle Now';
      }
    }

    if (btnCancel) {
      btnCancel.disabled = (role === 'guest_analytics') || !isRelayOn;
    }

    if (btnReset) {
      btnReset.disabled = (role !== 'admin') || !isLockout;
      btnReset.title = isLockout ? "Admin: Skip 7-Day Cooldown" : "Lockout not active";
    }

    if (btnToggle && toggleText) {
      if (isRelayOn) {
        btnToggle.className = 'glass-button btn-power-toggle is-active btn-danger';
        toggleText.textContent = 'Turn Relay OFF';
        btnToggle.disabled = (role === 'guest_analytics');
      } else if (isLockout) {
        btnToggle.className = 'glass-button btn-power-toggle btn-locked';
        toggleText.textContent = 'Charging Locked (Cooldown)';
        btnToggle.disabled = (role !== 'admin');
      } else {
        btnToggle.className = 'glass-button btn-power-toggle btn-ready-pulse';
        toggleText.textContent = 'Start Charging (Relay ON)';
        btnToggle.disabled = (role === 'guest_analytics');
      }
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

  function escapeHtml(str) {
    if (!str) return '';
    return String(str)
      .replace(/&/g, '&amp;')
      .replace(/</g, '&lt;')
      .replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;')
      .replace(/'/g, '&#039;');
  }

  function toast(message, type = 'info') {
    const container = document.getElementById('toastContainer');
    if (!container) return;
    const el = document.createElement('div');
    el.className = `toast toast-${type}`;
    el.textContent = message;
    container.appendChild(el);
    if (window.AndroidNative && typeof window.AndroidNative.showToast === 'function') {
      try { window.AndroidNative.showToast(message); } catch (ignored) { }
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
      try { window.AndroidNative.vibrateAlert(ms); } catch (ignored) { }
    } else if (navigator.vibrate) {
      navigator.vibrate(ms);
    }
  }

  // Public Interface
  return {
    init,
    setConnectionMode,
    switchView,
    openSidebar,
    closeSidebar,
    switchAuthTab,
    selectRoleLogin,
    fillPreset,
    togglePasswordVisibility,
    handleSignInSubmit,
    handleRegisterSubmit,
    logout,
    handleTriggerClick,
    handleToggleClick,
    cancelCycle,
    resetLockout,
    extendActiveTimer,
    extendActiveTimerCustom,
    toggleUserStatus,
    clearCloudLogs,
    showLocalWifiModal,
    closeLocalWifiModal,
    retryLocalConnection,
    fetchStatus,
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
    openForgotPasswordModal,
    closeForgotPasswordModal,
    onResetUserChange,
    submitForgotPassword,
    dismissConnNotice,
    // Multi-relay & notifications
    selectRelayTab,
    openAddRelayModal,
    openEditRelayModal,
    closeAddRelayModal,
    handleRelayFormSubmit,
    deleteRelay,
    openNotificationsModal,
    closeNotificationsModal,
    clearAllNotifications,
    testBuzzerSound,
    triggerAckHandshake,
    // Live Console
    setConsoleFilter,
    toggleConsoleStreaming,
    clearConsoleLogs,
    copyConsoleLogs,
    sendQuickCommand,
    handleConsoleInputSubmit,
    executeConsoleCommand
  };
})();

// Start App when DOM is loaded
window.addEventListener('DOMContentLoaded', () => {
  App.init();
});
