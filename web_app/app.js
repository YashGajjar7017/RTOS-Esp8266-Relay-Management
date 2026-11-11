/**
 * CYBER-RELAY RTOS // Frontend Application Controller v3.0
 * Features 3-Tier User Security, 1-Time Session Grants with Admin Popups,
 * Monthly Switch Analytics, and Firebase Realtime Worldwide Cloud Sync.
 */

const App = (() => {
  // Predefined users matching requirements
  const PREDEFINED_USERS = [
    { id: 1, username: 'Yash_Gajjar', role: 'admin', label: 'Super Administrator', password: 'Iamyashacker@17' },
    { id: 2, username: 'Mahi_Gajjar', role: 'guarded_operator', label: 'Guarded Operator (1-Time Grant)', password: 'MahiSecure#2026' },
    { id: 3, username: 'Guest', role: 'guest_analytics', label: 'Guest Analytics (Read-Only)', password: 'Guest@123' }
  ];

  // State definition
  const state = {
    connected: false,
    connectionMode: 'local', // 'local' or 'cloud'
    pollInterval: null,
    uptimeTimer: null,
    simulated: false,
    
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

    // Current User Session (Defaulting to Yash_Gajjar for full preview)
    currentUser: PREDEFINED_USERS[0],
    users: PREDEFINED_USERS,

    // One-time session tracking for Mahi_Gajjar
    pendingRequest: {
      active: false,
      requestId: '',
      username: '',
      action: '',
      oneTimeToken: ''
    },
    waitingForApproval: false
  };

  const GAUGE_CIRCUMFERENCE = 691.15;

  function init() {
    setupEventListeners();
    updateUI();
    startPolling();
    startLocalUptime();

    // Check if running on local file or localhost
    if (window.location.protocol === 'file:' || window.location.hostname === 'localhost' || window.location.hostname === '127.0.0.1') {
      console.log('Running in Browser Preview / Simulation Mode');
      toast('Preview Mode: Standalone Simulation Active', 'info');
      state.connected = true;
      state.simulated = true;
      updateUI();
    }
  }

  function setupEventListeners() {
    document.addEventListener('keydown', (e) => {
      if (e.key === 'Escape') {
        closeAuthModal();
        dismissAdminApprovalModal();
      }
    });
  }

  // Polling Real-time Status from ESP8266 or Firebase
  function startPolling() {
    fetchStatus();
    if (state.pollInterval) clearInterval(state.pollInterval);
    state.pollInterval = setInterval(fetchStatus, 1000);
  }

  async function fetchStatus() {
    if (state.simulated) {
      tickSimulation();
      return;
    }

    try {
      const response = await fetch('/api/status', {
        headers: {
          'Authorization': state.currentUser ? `Bearer ${state.currentUser.token || 'auth'}` : ''
        }
      });

      if (!response.ok) throw new Error('ESP API Error');

      const data = await response.json();
      state.connected = true;
      state.system.mode = data.mode || 'IDLE';
      state.system.relayActive = data.relayActive || false;
      state.system.touchActive = data.touchActive !== false;
      state.system.lockoutActive = data.lockoutActive || false;
      state.system.onRemainingSec = data.onRemainingSec || 0;
      state.system.totalOnSec = data.totalOnSec || (data.config?.onHours || 8) * 3600;
      state.system.lockoutRemainingSec = data.lockoutRemainingSec || 0;
      state.system.totalLockoutSec = data.totalLockoutSec || (data.config?.lockoutDays || 6) * 86400;
      state.system.uptimeSeconds = data.uptimeSeconds || 0;

      if (data.config) {
        state.config.onHours = data.config.onHours;
        state.config.lockoutDays = data.config.lockoutDays;
      }

      if (data.analytics) {
        state.analytics.month = data.analytics.currentMonth || 'November 2026';
        state.analytics.totalOnHours = parseFloat(data.analytics.totalOnHours || 48.5);
        state.analytics.totalOffHours = parseFloat(data.analytics.totalOffHours || 240.2);
        state.analytics.totalCycles = data.analytics.totalCyclesCount || 6;
        const total = state.analytics.totalOnHours + state.analytics.totalOffHours;
        state.analytics.dutyPercent = total > 0 ? ((state.analytics.totalOnHours / total) * 100).toFixed(1) : 0;
      }

      // Check if there is an active pending request for Yash_Gajjar to approve
      if (data.pendingRequest && data.pendingRequest.active) {
        state.pendingRequest = data.pendingRequest;
        // If current user is Yash_Gajjar, popup the modal!
        if (state.currentUser && state.currentUser.role === 'admin') {
          showAdminApprovalModal(data.pendingRequest.username, data.pendingRequest.action);
        }
      } else {
        state.pendingRequest.active = false;
        // If Mahi was waiting and request got approved/cleared
        if (state.waitingForApproval && (!data.pendingRequest || !data.pendingRequest.active)) {
          state.waitingForApproval = false;
          closeWaitingModal();
        }
      }

      updateUI();
    } catch (err) {
      if (state.connected && !state.simulated) {
        state.connected = false;
        toast('Connection to ESP8266 lost', 'error');
        updateUI();
      }
    }
  }

  function tickSimulation() {
    state.system.uptimeSeconds++;
    
    // Increment simulated analytics
    if (state.system.relayActive) {
      state.analytics.totalOnHours = +(state.analytics.totalOnHours + 1/3600).toFixed(4);
    } else {
      state.analytics.totalOffHours = +(state.analytics.totalOffHours + 1/3600).toFixed(4);
    }
    const tot = state.analytics.totalOnHours + state.analytics.totalOffHours;
    state.analytics.dutyPercent = ((state.analytics.totalOnHours / tot) * 100).toFixed(1);

    if (state.system.mode === 'RELAY_ON') {
      if (state.system.onRemainingSec > 0) {
        state.system.onRemainingSec--;
      } else {
        state.system.mode = 'LOCKOUT';
        state.system.relayActive = false;
        state.system.touchActive = false;
        state.system.lockoutActive = true;
        state.system.lockoutRemainingSec = Math.round(state.config.lockoutDays * 86400);
        state.system.totalLockoutSec = state.system.lockoutRemainingSec;
        toast('Relay cycle complete. Lockout period initiated.', 'info');
      }
    } else if (state.system.mode === 'LOCKOUT') {
      if (state.system.lockoutRemainingSec > 0) {
        state.system.lockoutRemainingSec--;
      } else {
        state.system.mode = 'IDLE';
        state.system.relayActive = false;
        state.system.touchActive = true;
        state.system.lockoutActive = false;
        toast('Lockout timer expired! Touch sensor re-enabled.', 'success');
      }
    }
    updateUI();
  }

  function startLocalUptime() {
    if (state.uptimeTimer) clearInterval(state.uptimeTimer);
    state.uptimeTimer = setInterval(() => {
      document.getElementById('espUptime').textContent = formatSecondsToUptime(state.system.uptimeSeconds);
    }, 1000);
  }

  // Update UI Elements according to 3-tier user role
  function updateUI() {
    updateConnectionStatus();
    updateUserPill();
    updateRoleBanners();
    updateStateHero();
    updateHardwareTiles();
    updateActionButtons();
    updateAnalyticsCards();
    updateConfigSliders();
    renderUserTable();
  }

  function updateConnectionStatus() {
    const pill = document.getElementById('connStatusPill');
    const text = document.getElementById('connStatusText');
    if (state.connected) {
      pill.className = 'status-pill online';
      if (state.connectionMode === 'cloud') {
        text.textContent = 'FIREBASE CLOUD (WORLDWIDE)';
      } else {
        text.textContent = state.simulated ? 'SIMULATOR ACTIVE' : 'ESP8266 CONNECTED (AP)';
      }
    } else {
      pill.className = 'status-pill offline';
      text.textContent = 'DISCONNECTED';
    }
  }

  function updateUserPill() {
    const nameEl = document.getElementById('currentUserName');
    const roleEl = document.getElementById('currentUserRole');
    const initialEl = document.getElementById('userInitial');
    const ringEl = document.getElementById('userAvatarRing');

    if (state.currentUser) {
      nameEl.textContent = state.currentUser.username;
      initialEl.textContent = state.currentUser.username.charAt(0).toUpperCase();

      if (state.currentUser.role === 'admin') {
        roleEl.textContent = 'SUPER ADMIN';
        ringEl.className = 'avatar-ring role-admin';
      } else if (state.currentUser.role === 'guarded_operator') {
        roleEl.textContent = 'GUARDED OP (1-TIME)';
        ringEl.className = 'avatar-ring role-guarded';
      } else {
        roleEl.textContent = 'GUEST (READ-ONLY)';
        ringEl.className = 'avatar-ring role-guest';
      }
      document.getElementById('logoutBtn').style.display = 'flex';
    } else {
      nameEl.textContent = 'Guest';
      roleEl.textContent = 'SIGN IN';
      initialEl.textContent = '?';
      ringEl.className = 'avatar-ring role-guest';
      document.getElementById('logoutBtn').style.display = 'none';
    }
  }

  function updateRoleBanners() {
    const guestBanner = document.getElementById('guestNoticeBanner');
    const guardedBanner = document.getElementById('guardedNoticeBanner');
    const role = state.currentUser?.role;

    if (role === 'guest_analytics') {
      guestBanner.classList.remove('hidden');
      guardedBanner.classList.add('hidden');
    } else if (role === 'guarded_operator') {
      guardedBanner.classList.remove('hidden');
      guestBanner.classList.add('hidden');
    } else {
      guestBanner.classList.add('hidden');
      guardedBanner.classList.add('hidden');
    }
  }

  function updateStateHero() {
    const badge = document.getElementById('sysStateBadge');
    const badgeText = document.getElementById('sysStateText');
    const bulb = document.getElementById('relayBulb');
    const label = document.getElementById('countdownLabel');
    const subtext = document.getElementById('countdownSubtext');
    const radial = document.getElementById('radialProgress');

    let remaining = 0;
    let total = 1;
    let progressRatio = 0;

    if (state.system.mode === 'IDLE') {
      badge.className = 'state-badge badge-idle';
      badgeText.textContent = 'IDLE / READY';
      bulb.className = 'relay-bulb-indicator';
      label.textContent = 'READY FOR TOUCH';
      subtext.textContent = `Tap touch sensor or click button to run ${state.config.onHours}h duty cycle`;
      radial.style.stroke = 'url(#emeraldGlow)';
      radial.style.strokeDashoffset = '0';
      setDigitalClock(0, 0, 0, 0);
    } 
    else if (state.system.mode === 'RELAY_ON') {
      badge.className = 'state-badge badge-active';
      badgeText.textContent = 'RELAY ACTIVE (ON)';
      bulb.className = 'relay-bulb-indicator bulb-on';
      label.textContent = 'RELAY ON COUNTDOWN';
      subtext.textContent = 'Relay energized. Running custom duty cycle.';
      radial.style.stroke = 'url(#cyanGlow)';
      
      remaining = state.system.onRemainingSec;
      total = state.system.totalOnSec || (state.config.onHours * 3600);
      progressRatio = total > 0 ? (remaining / total) : 0;
      radial.style.strokeDashoffset = (GAUGE_CIRCUMFERENCE * (1 - progressRatio)).toString();
      
      const { d, h, m, s } = splitSeconds(remaining);
      setDigitalClock(d, h, m, s);
    } 
    else if (state.system.mode === 'LOCKOUT') {
      badge.className = 'state-badge badge-lockout';
      badgeText.textContent = 'LOCKOUT COOLDOWN';
      bulb.className = 'relay-bulb-indicator';
      label.textContent = 'LOCKED COUNTDOWN';
      subtext.textContent = 'Touch sensor locked. Responses blocked until timer finishes.';
      radial.style.stroke = 'url(#amberGlow)';
      
      remaining = state.system.lockoutRemainingSec;
      total = state.system.totalLockoutSec || (state.config.lockoutDays * 86400);
      progressRatio = total > 0 ? (remaining / total) : 0;
      radial.style.strokeDashoffset = (GAUGE_CIRCUMFERENCE * (1 - progressRatio)).toString();
      
      const { d, h, m, s } = splitSeconds(remaining);
      setDigitalClock(d, h, m, s);
    }
  }

  function setDigitalClock(d, h, m, s) {
    document.getElementById('clockDays').textContent = String(d).padStart(2, '0');
    document.getElementById('clockHours').textContent = String(h).padStart(2, '0');
    document.getElementById('clockMins').textContent = String(m).padStart(2, '0');
    document.getElementById('clockSecs').textContent = String(s).padStart(2, '0');
  }

  function updateHardwareTiles() {
    const relayDot = document.getElementById('relayStatusDot');
    const relayText = document.getElementById('relayStatusText');
    if (state.system.relayActive) {
      relayDot.className = 'status-dot dot-active';
      relayText.textContent = 'ENERGIZED (ON)';
      relayText.style.color = 'var(--accent-cyan)';
    } else {
      relayDot.className = 'status-dot dot-off';
      relayText.textContent = 'OFF';
      relayText.style.color = 'var(--text-muted)';
    }

    const touchDot = document.getElementById('touchStatusDot');
    const touchText = document.getElementById('touchStatusText');
    const touchSub = document.getElementById('touchPinInfo');
    if (state.system.mode === 'LOCKOUT') {
      touchDot.className = 'status-dot dot-disabled';
      touchText.textContent = 'BLOCKED / LOCKED';
      touchText.style.color = 'var(--accent-amber)';
      touchSub.textContent = 'Lockout filter active';
    } else {
      touchDot.className = 'status-dot dot-active';
      touchText.textContent = 'ACTIVE / READY';
      touchText.style.color = 'var(--accent-emerald)';
      touchSub.textContent = 'TTP223 Sensor Listening';
    }

    const lockDot = document.getElementById('lockoutStatusDot');
    const lockText = document.getElementById('lockoutStatusText');
    const lockDesc = document.getElementById('lockoutDesc');
    if (state.system.mode === 'LOCKOUT') {
      lockDot.className = 'status-dot dot-warning';
      lockText.textContent = 'ENGAGED';
      lockText.style.color = 'var(--accent-amber)';
      lockDesc.textContent = 'Ignoring all touch triggers';
    } else {
      lockDot.className = 'status-dot dot-disabled';
      lockText.textContent = 'UNLOCKED';
      lockText.style.color = 'var(--text-secondary)';
      lockDesc.textContent = 'Touch triggers allowed';
    }
  }

  function updateActionButtons() {
    const btnTrigger = document.getElementById('btnManualTrigger');
    const btnCancel = document.getElementById('btnCancelCycle');
    const btnResetLock = document.getElementById('btnResetLockout');
    const triggerText = document.getElementById('triggerBtnText');
    const role = state.currentUser?.role;

    // 3-Tier Policy Rules:
    if (role === 'guest_analytics') {
      // Guest: Cannot trigger anything
      btnTrigger.disabled = true;
      btnCancel.disabled = true;
      btnResetLock.disabled = true;
      triggerText.textContent = 'Disabled for Guest (Read-Only)';
      document.getElementById('calibrationCard').style.opacity = '0.5';
      document.getElementById('btnSaveConfig').disabled = true;
      return;
    } else {
      document.getElementById('calibrationCard').style.opacity = '1';
      document.getElementById('btnSaveConfig').disabled = false;
    }

    if (role === 'guarded_operator') {
      triggerText.textContent = 'Request 1-Time Trigger (Needs Yash Approval)';
      btnResetLock.disabled = true; // Only Yash can reset lockout
    } else {
      triggerText.textContent = 'Trigger Relay Cycle Now (Admin)';
    }

    if (state.system.mode === 'IDLE') {
      btnTrigger.disabled = false;
      btnCancel.disabled = true;
      btnResetLock.disabled = true;
    } else if (state.system.mode === 'RELAY_ON') {
      btnTrigger.disabled = true;
      btnCancel.disabled = (role === 'guest_analytics');
      btnResetLock.disabled = true;
    } else if (state.system.mode === 'LOCKOUT') {
      btnTrigger.disabled = true;
      btnCancel.disabled = true;
      btnResetLock.disabled = (role !== 'admin');
    }
  }

  // Monthly Analytics Visualizer (User 3 Requirement)
  function updateAnalyticsCards() {
    document.getElementById('anTotalOnHours').textContent = state.analytics.totalOnHours.toFixed(1);
    document.getElementById('anTotalOffHours').textContent = state.analytics.totalOffHours.toFixed(1);
    document.getElementById('anTotalCycles').textContent = state.analytics.totalCycles;

    const onPct = Math.min(100, Math.max(0, state.analytics.dutyPercent));
    const offPct = 100 - onPct;
    document.getElementById('dutyRatioPercent').textContent = `${onPct}% ON`;
    document.getElementById('ratioBarOn').style.width = `${onPct}%`;
    document.getElementById('ratioBarOff').style.width = `${offPct}%`;
  }

  function updateConfigSliders() {
    document.getElementById('onHoursDisplay').textContent = Number(state.config.onHours).toFixed(1);
    document.getElementById('onHoursSlider').value = state.config.onHours;

    document.getElementById('lockoutDaysDisplay').textContent = Number(state.config.lockoutDays).toFixed(1);
    document.getElementById('lockoutDaysSlider').value = state.config.lockoutDays;
  }

  function onTimerInput(type) {
    if (state.currentUser?.role === 'guest_analytics') {
      toast('Guest accounts cannot modify calibration', 'error');
      return;
    }

    if (type === 'on') {
      const val = parseFloat(document.getElementById('onHoursSlider').value);
      state.config.onHours = val;
      document.getElementById('onHoursDisplay').textContent = val.toFixed(1);
    } else if (type === 'lockout') {
      const val = parseFloat(document.getElementById('lockoutDaysSlider').value);
      state.config.lockoutDays = val;
      document.getElementById('lockoutDaysDisplay').textContent = val.toFixed(1);
    }
  }

  // Save updated timer values
  async function saveTimers() {
    if (state.currentUser?.role === 'guest_analytics') {
      toast('Guest accounts cannot modify calibration', 'error');
      return;
    }

    if (state.simulated) {
      toast(`Saved: Relay ON = ${state.config.onHours}h, Lockout = ${state.config.lockoutDays}d`, 'success');
      return;
    }

    try {
      const res = await fetch('/api/config', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          onHours: state.config.onHours,
          lockoutDays: state.config.lockoutDays,
          userRole: state.currentUser.role
        })
      });

      if (!res.ok) throw new Error();
      toast('Parameters successfully written to ESP8266 flash!', 'success');
      fetchStatus();
    } catch (e) {
      toast('Error saving configuration to ESP8266', 'error');
    }
  }

  // Handle Trigger Click based on 3-tier user role
  async function handleTriggerClick() {
    const role = state.currentUser?.role;

    if (role === 'guest_analytics') {
      toast('Action Denied: Guest accounts are read-only.', 'error');
      return;
    }

    // Level 2: Mahi_Gajjar requires 1-time grant popup on Yash's app!
    if (role === 'guarded_operator') {
      requestOneTimeAccess('TRIGGER_RELAY');
      return;
    }

    // Level 1: Yash_Gajjar (Super Admin) executes immediately
    executeTriggerDirectly();
  }

  // Level 2 (Mahi_Gajjar) sends 1-time request
  async function requestOneTimeAccess(action) {
    state.waitingForApproval = true;
    document.getElementById('userWaitingApprovalModal').classList.remove('hidden');

    if (state.simulated) {
      toast('1-time request dispatched! Simulating Admin popup for Yash_Gajjar...', 'info');
      setTimeout(() => {
        // If simulation, trigger the Admin popup
        showAdminApprovalModal('Mahi_Gajjar', 'Trigger Relay Cycle (8.0h)');
      }, 1000);
      return;
    }

    try {
      const res = await fetch('/api/auth/request_grant', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          username: state.currentUser.username,
          action: action
        })
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.message);
      toast('Request sent to Yash_Gajjar! Awaiting Admin confirmation popup...', 'info');
    } catch (err) {
      toast(err.message || 'Failed to dispatch request', 'error');
      closeWaitingModal();
    }
  }

  function closeWaitingModal() {
    state.waitingForApproval = false;
    document.getElementById('userWaitingApprovalModal').classList.add('hidden');
  }

  function cancelWaitingRequest() {
    closeWaitingModal();
    toast('Access request cancelled', 'info');
  }

  // Level 1 (Yash_Gajjar) Admin Confirmation Popup Modal
  function showAdminApprovalModal(user, action) {
    document.getElementById('popupReqUser').textContent = user;
    document.getElementById('popupReqAction').textContent = action;
    document.getElementById('popupAdminPassword').value = '';
    document.getElementById('adminOneTimeApprovalModal').classList.remove('hidden');
  }

  function dismissAdminApprovalModal() {
    document.getElementById('adminOneTimeApprovalModal').classList.add('hidden');
  }

  async function submitAdminApproval() {
    const enteredPass = document.getElementById('popupAdminPassword').value;

    if (state.simulated) {
      if (enteredPass !== 'Iamyashacker@17') {
        toast('Incorrect Master Admin Password!', 'error');
        return;
      }
      dismissAdminApprovalModal();
      closeWaitingModal();
      toast('1-Time Access Granted to Mahi_Gajjar! Session destroyed upon run.', 'success');
      
      // Execute the triggered cycle
      executeTriggerDirectly();
      return;
    }

    try {
      const res = await fetch('/api/auth/approve_grant', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ adminPassword: enteredPass })
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.message);

      dismissAdminApprovalModal();
      toast('Authorized! 1-time grant consumed & destroyed.', 'success');
      fetchStatus();
    } catch (err) {
      toast(err.message || 'Authorization failed', 'error');
    }
  }

  async function rejectAdminApproval() {
    dismissAdminApprovalModal();
    if (state.simulated) {
      closeWaitingModal();
      toast('Access request denied by Admin', 'info');
      return;
    }
    try {
      await fetch('/api/auth/reject_grant', { method: 'POST' });
      toast('Request rejected by Admin', 'info');
      fetchStatus();
    } catch (e) {}
  }

  // Execute direct trigger (Admin or after 1-time token verified)
  async function executeTriggerDirectly(oneTimeToken = '') {
    if (state.system.mode !== 'IDLE') {
      toast('Cannot trigger: system is currently ' + state.system.mode, 'error');
      return;
    }

    if (state.simulated) {
      state.system.mode = 'RELAY_ON';
      state.system.relayActive = true;
      state.system.touchActive = false;
      state.system.lockoutActive = false;
      state.system.totalOnSec = Math.round(state.config.onHours * 3600);
      state.system.onRemainingSec = state.system.totalOnSec;
      state.analytics.totalCycles++;
      toast(`Relay activated for ${state.config.onHours} hours!`, 'success');
      updateUI();
      return;
    }

    try {
      const res = await fetch('/api/control', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          action: 'trigger',
          userRole: state.currentUser.role,
          oneTimeToken: oneTimeToken
        })
      });
      if (!res.ok) throw new Error();
      toast('Relay triggered via RTOS command', 'success');
      fetchStatus();
    } catch (e) {
      toast('Failed to trigger relay', 'error');
    }
  }

  async function cancelCycle() {
    if (state.currentUser?.role === 'guest_analytics') {
      toast('Action Denied for Guest account', 'error');
      return;
    }

    if (state.simulated) {
      state.system.mode = 'IDLE';
      state.system.relayActive = false;
      state.system.touchActive = true;
      state.system.lockoutActive = false;
      state.system.onRemainingSec = 0;
      toast('Emergency Cutoff Executed', 'info');
      updateUI();
      return;
    }

    try {
      const res = await fetch('/api/control', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ action: 'cancel', userRole: state.currentUser.role })
      });
      if (!res.ok) throw new Error();
      toast('Cycle cancelled', 'info');
      fetchStatus();
    } catch (e) {
      toast('Cutoff command failed', 'error');
    }
  }

  async function resetLockout() {
    if (state.currentUser?.role !== 'admin') {
      toast('Only Super Administrator (Yash_Gajjar) can bypass lockout', 'error');
      return;
    }

    if (state.simulated) {
      state.system.mode = 'IDLE';
      state.system.lockoutRemainingSec = 0;
      state.system.touchActive = true;
      state.system.lockoutActive = false;
      toast('Lockout bypassed by Yash_Gajjar', 'success');
      updateUI();
      return;
    }

    try {
      const res = await fetch('/api/control', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ action: 'reset_lockout', userRole: 'admin' })
      });
      if (!res.ok) throw new Error();
      toast('Lockout bypassed by Admin', 'success');
      fetchStatus();
    } catch (e) {
      toast('Lockout reset failed', 'error');
    }
  }

  // Switch connection mode between Local AP and Firebase Cloud
  function setConnectionMode(mode) {
    state.connectionMode = mode;
    document.getElementById('btnModeLocal').className = mode === 'local' ? 'mode-btn active' : 'mode-btn';
    document.getElementById('btnModeCloud').className = mode === 'cloud' ? 'mode-btn active' : 'mode-btn';

    if (mode === 'cloud') {
      toast('Switched to Firebase Cloud Mode (Worldwide)', 'info');
      document.getElementById('ftCloudStatus').textContent = 'Worldwide Active';
    } else {
      toast('Switched to Local AP Mode (192.168.4.1)', 'info');
      document.getElementById('ftCloudStatus').textContent = 'Local Standby';
    }
    updateConnectionStatus();
  }

  // Render User Database Table
  function renderUserTable() {
    const tbody = document.getElementById('userTableBody');
    tbody.innerHTML = '';
    PREDEFINED_USERS.forEach(user => {
      const tr = document.createElement('tr');
      const isCurrent = state.currentUser && state.currentUser.username === user.username;

      let badgeClass = 'role-guest-badge';
      let tierLabel = 'Level 3: Guest Analytics';
      if (user.role === 'admin') {
        badgeClass = 'role-admin-badge';
        tierLabel = 'Level 1: Super Admin';
      } else if (user.role === 'guarded_operator') {
        badgeClass = 'role-guarded-badge';
        tierLabel = 'Level 2: Guarded (1-Time)';
      }

      tr.innerHTML = `
        <td>
          <strong>${user.username}</strong>
          ${isCurrent ? '<span style="color:var(--accent-cyan); font-size:0.7rem; margin-left:4px;">(Current)</span>' : ''}
        </td>
        <td>
          <span class="user-badge-role ${badgeClass}">${tierLabel}</span>
        </td>
        <td>
          <span style="color:var(--accent-emerald); font-size:0.75rem; font-weight:600;">● ACTIVE</span>
        </td>
      `;
      tbody.appendChild(tr);
    });
  }

  // Auth Modal
  function openAuthModal() {
    document.getElementById('loginUsername').value = '';
    document.getElementById('loginPassword').value = '';
    document.getElementById('authModal').classList.remove('hidden');
  }

  function closeAuthModal() {
    document.getElementById('authModal').classList.add('hidden');
  }

  function fillPreset(type) {
    if (type === 'yash') {
      document.getElementById('loginUsername').value = 'Yash_Gajjar';
      document.getElementById('loginPassword').value = 'Iamyashacker@17';
    } else if (type === 'mahi') {
      document.getElementById('loginUsername').value = 'Mahi_Gajjar';
      document.getElementById('loginPassword').value = 'MahiSecure#2026';
    } else if (type === 'guest') {
      document.getElementById('loginUsername').value = 'Guest';
      document.getElementById('loginPassword').value = 'Guest@123';
    }
  }

  async function submitLogin() {
    const user = document.getElementById('loginUsername').value.trim();
    const pass = document.getElementById('loginPassword').value;

    const matched = PREDEFINED_USERS.find(u => u.username.toLowerCase() === user.toLowerCase());
    if (matched && matched.password === pass) {
      state.currentUser = matched;
      closeAuthModal();
      toast(`Signed in as ${matched.username} (${matched.label})`, 'success');
      updateUI();
      return;
    }

    if (state.simulated) {
      toast('Invalid credentials for predefined users', 'error');
      return;
    }

    try {
      const res = await fetch('/api/auth/login', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ username: user, password: pass })
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.message || 'Login failed');

      state.currentUser = {
        username: data.username,
        role: data.role,
        token: data.token
      };
      closeAuthModal();
      toast(`Welcome back, ${data.username}!`, 'success');
      updateUI();
    } catch (e) {
      toast(e.message || 'Login failed', 'error');
    }
  }

  function logout() {
    // Revert to Guest
    state.currentUser = PREDEFINED_USERS[2]; // Guest
    toast('Switched to Guest mode', 'info');
    updateUI();
  }

  function splitSeconds(totalSec) {
    const sec = Math.max(0, Math.floor(totalSec));
    const d = Math.floor(sec / 86400);
    const h = Math.floor((sec % 86400) / 3600);
    const m = Math.floor((sec % 3600) / 60);
    const s = sec % 60;
    return { d, h, m, s };
  }

  function formatSecondsToUptime(totalSec) {
    const { d, h, m, s } = splitSeconds(totalSec);
    if (d > 0) return `${d}d ${String(h).padStart(2,'0')}:${String(m).padStart(2,'0')}:${String(s).padStart(2,'0')}`;
    return `${String(h).padStart(2,'0')}:${String(m).padStart(2,'0')}:${String(s).padStart(2,'0')}`;
  }

  function toast(message, type = 'info') {
    const container = document.getElementById('toastContainer');
    const toastEl = document.createElement('div');
    toastEl.className = `toast toast-${type}`;
    toastEl.textContent = message;
    container.appendChild(toastEl);
    setTimeout(() => {
      toastEl.style.opacity = '0';
      toastEl.style.transform = 'translateY(10px)';
      toastEl.style.transition = 'all 0.3s ease';
      setTimeout(() => toastEl.remove(), 300);
    }, 3500);
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }

  return {
    onTimerInput,
    saveTimers,
    handleTriggerClick,
    cancelCycle,
    resetLockout,
    setConnectionMode,
    openAuthModal,
    closeAuthModal,
    fillPreset,
    submitLogin,
    logout,
    dismissAdminApprovalModal,
    submitAdminApproval,
    rejectAdminApproval,
    cancelWaitingRequest
  };
})();
