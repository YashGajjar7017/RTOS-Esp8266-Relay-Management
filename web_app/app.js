/**
 * CYBER-RELAY RTOS // Frontend Application Controller
 * Handles real-time telemetry, timer calibration, user DB, and Admin Authorization Modal
 */

const App = (() => {
  // State definition
  const state = {
    connected: false,
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

    // Timer Calibration Config
    config: {
      onHours: 8.0,
      lockoutDays: 6.0
    },

    // Inbuilt User Database (Max 5 users)
    currentUser: {
      username: 'admin',
      role: 'admin',
      token: 'demo-token'
    },
    users: [
      { id: 1, username: 'admin', role: 'admin', status: 'active', created: '2026-11-10' }
    ]
  };

  // SVG Gauge constant (Circumference of r=110: 2 * PI * 110 = 691.15)
  const GAUGE_CIRCUMFERENCE = 691.15;

  // Initialize
  function init() {
    setupEventListeners();
    updateUI();
    startPolling();
    startLocalUptime();

    // Check if running on ESP8266 directly or local development file
    if (window.location.protocol === 'file:' || window.location.hostname === 'localhost' || window.location.hostname === '127.0.0.1') {
      console.log('Running in Browser Preview / Simulation Mode');
      toast('Preview Mode: Standalone Simulation Active', 'info');
      state.connected = true;
      state.simulated = true;
      updateUI();
    }
  }

  function setupEventListeners() {
    // Keyboard shortcuts
    document.addEventListener('keydown', (e) => {
      if (e.key === 'Escape') {
        closeAuthModal();
        closeNewUserModal();
      }
    });
  }

  // Polling Real-time Status from ESP8266 REST API
  function startPolling() {
    fetchStatus();
    if (state.pollInterval) clearInterval(state.pollInterval);
    state.pollInterval = setInterval(fetchStatus, 1000);
  }

  async function fetchStatus() {
    if (state.simulated) {
      // Run local simulated countdown logic
      tickSimulation();
      return;
    }

    try {
      const response = await fetch('/api/status', {
        headers: {
          'Authorization': state.currentUser ? `Bearer ${state.currentUser.token}` : ''
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

      if (data.users) {
        state.users = data.users;
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

  // Simulation tick for testing UI when not flashed to hardware
  function tickSimulation() {
    state.system.uptimeSeconds++;
    
    if (state.system.mode === 'RELAY_ON') {
      if (state.system.onRemainingSec > 0) {
        state.system.onRemainingSec--;
      } else {
        // Transition to Lockout
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
        // Transition back to Idle
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

  // Update all UI elements
  function updateUI() {
    updateConnectionStatus();
    updateStateHero();
    updateHardwareTiles();
    updateActionButtons();
    updateUserPill();
    updateConfigSliders();
    renderUserTable();
  }

  function updateConnectionStatus() {
    const pill = document.getElementById('connStatusPill');
    const text = document.getElementById('connStatusText');
    if (state.connected) {
      pill.className = 'status-pill online';
      text.textContent = state.simulated ? 'SIMULATOR MODE (ONLINE)' : 'ESP8266 CONNECTED (AP: 192.168.4.1)';
    } else {
      pill.className = 'status-pill offline';
      text.textContent = 'DISCONNECTED (CHECK WIFI)';
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
    // Relay tile
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

    // Touch module tile
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

    // Lockout tile
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
    const isAdmin = state.currentUser && state.currentUser.role === 'admin';

    if (state.system.mode === 'IDLE') {
      btnTrigger.disabled = false;
      btnCancel.disabled = true;
      btnResetLock.disabled = true;
    } else if (state.system.mode === 'RELAY_ON') {
      btnTrigger.disabled = true;
      btnCancel.disabled = !state.currentUser; // Operator or Admin can cutoff
      btnResetLock.disabled = true;
    } else if (state.system.mode === 'LOCKOUT') {
      btnTrigger.disabled = true;
      btnCancel.disabled = true;
      btnResetLock.disabled = !isAdmin; // Admin only can bypass lockout
    }
  }

  function updateUserPill() {
    const nameEl = document.getElementById('currentUserName');
    const roleEl = document.getElementById('currentUserRole');
    const initialEl = document.getElementById('userInitial');

    if (state.currentUser) {
      nameEl.textContent = state.currentUser.username;
      roleEl.textContent = state.currentUser.role.toUpperCase();
      initialEl.textContent = state.currentUser.username.charAt(0).toUpperCase();
      document.getElementById('logoutBtn').style.display = 'flex';
    } else {
      nameEl.textContent = 'Guest';
      roleEl.textContent = 'SIGN IN';
      initialEl.textContent = '?';
      document.getElementById('logoutBtn').style.display = 'none';
    }
  }

  function updateConfigSliders() {
    document.getElementById('onHoursDisplay').textContent = Number(state.config.onHours).toFixed(1);
    document.getElementById('onHoursSlider').value = state.config.onHours;

    document.getElementById('lockoutDaysDisplay').textContent = Number(state.config.lockoutDays).toFixed(1);
    document.getElementById('lockoutDaysSlider').value = state.config.lockoutDays;
  }

  function onTimerInput(type) {
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

  // Save updated timer values to ESP Flash
  async function saveTimers() {
    if (!state.currentUser) {
      toast('Please sign in to modify settings', 'error');
      openAuthModal();
      return;
    }

    if (state.simulated) {
      toast(`Saved: Relay ON = ${state.config.onHours}h, Lockout = ${state.config.lockoutDays}d`, 'success');
      return;
    }

    try {
      const res = await fetch('/api/config', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${state.currentUser.token}`
        },
        body: JSON.stringify({
          onHours: state.config.onHours,
          lockoutDays: state.config.lockoutDays
        })
      });

      if (!res.ok) throw new Error('Failed to update config');
      toast('Parameters successfully written to ESP8266 flash!', 'success');
      fetchStatus();
    } catch (e) {
      toast('Error saving configuration to ESP8266', 'error');
    }
  }

  // Trigger cycle manually
  async function triggerRelayCycle() {
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
      toast(`Relay activated for ${state.config.onHours} hours!`, 'success');
      updateUI();
      return;
    }

    try {
      const res = await fetch('/api/control', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ action: 'trigger' })
      });
      if (!res.ok) throw new Error();
      toast('Relay triggered via RTOS command', 'success');
      fetchStatus();
    } catch (e) {
      toast('Failed to trigger relay', 'error');
    }
  }

  // Cancel cycle
  async function cancelCycle() {
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
        body: JSON.stringify({ action: 'cancel' })
      });
      if (!res.ok) throw new Error();
      toast('Cycle cancelled', 'info');
      fetchStatus();
    } catch (e) {
      toast('Cutoff command failed', 'error');
    }
  }

  // Reset Lockout (Admin bypass)
  async function resetLockout() {
    if (!state.currentUser || state.currentUser.role !== 'admin') {
      toast('Admin privileges required to bypass lockout', 'error');
      return;
    }

    if (state.simulated) {
      state.system.mode = 'IDLE';
      state.system.lockoutRemainingSec = 0;
      state.system.touchActive = true;
      state.system.lockoutActive = false;
      toast('Lockout bypassed by Admin', 'success');
      updateUI();
      return;
    }

    try {
      const res = await fetch('/api/control', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${state.currentUser.token}`
        },
        body: JSON.stringify({ action: 'reset_lockout' })
      });
      if (!res.ok) throw new Error();
      toast('Lockout bypassed by Admin', 'success');
      fetchStatus();
    } catch (e) {
      toast('Lockout reset failed', 'error');
    }
  }

  // Render User Database Table (3 to 5 users)
  function renderUserTable() {
    const tbody = document.getElementById('userTableBody');
    const countBadge = document.getElementById('userCountText');
    countBadge.textContent = `${state.users.length} / 5`;

    tbody.innerHTML = '';
    state.users.forEach(user => {
      const tr = document.createElement('tr');
      const isAdmin = user.role === 'admin';
      const isCurrent = state.currentUser && state.currentUser.username === user.username;

      tr.innerHTML = `
        <td>
          <strong>${escapeHtml(user.username)}</strong>
          ${isCurrent ? '<span style="color:var(--accent-cyan); font-size:0.7rem; margin-left:4px;">(You)</span>' : ''}
        </td>
        <td>
          <span class="user-badge-role ${isAdmin ? 'role-admin' : 'role-operator'}">${user.role}</span>
        </td>
        <td>
          <span class="${user.status === 'active' ? 'status-active-tag' : 'status-pending-tag'}">
            ● ${user.status.toUpperCase()}
          </span>
        </td>
        <td class="text-right">
          ${!isAdmin ? `
            <button class="icon-btn-sm" title="Remove User" onclick="App.deleteUser(${user.id})">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="3 6 5 6 21 6"/><path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg>
            </button>
          ` : '<span style="font-size:0.7rem; color:var(--text-muted);">Master</span>'}
        </td>
      `;
      tbody.appendChild(tr);
    });
  }

  // Admin approval modal for adding new user
  function openNewUserModal() {
    if (state.users.length >= 5) {
      toast('Maximum capacity reached (5 users max in ESP DB)', 'error');
      return;
    }
    document.getElementById('newUsername').value = '';
    document.getElementById('newUserPassword').value = '';
    document.getElementById('adminAuthPassword').value = '';
    document.getElementById('adminApprovalModal').classList.remove('hidden');
  }

  function closeNewUserModal() {
    document.getElementById('adminApprovalModal').classList.add('hidden');
  }

  // Submit New User Registration (With Admin Password Approval)
  async function submitNewUserRegistration() {
    const newUsername = document.getElementById('newUsername').value.trim();
    const newUserPassword = document.getElementById('newUserPassword').value;
    const newUserRole = document.getElementById('newUserRole').value;
    const adminPassword = document.getElementById('adminAuthPassword').value;

    if (!newUsername || !newUserPassword || !adminPassword) {
      toast('All fields including Master Admin Password are required', 'error');
      return;
    }

    if (state.users.some(u => u.username.toLowerCase() === newUsername.toLowerCase())) {
      toast('Username already exists in database', 'error');
      return;
    }

    if (state.simulated) {
      // In simulation mode, verify if admin password is 'admin' or 'admin123'
      if (adminPassword !== 'admin' && adminPassword !== 'admin123') {
        toast('Invalid Admin Password. Authorization Denied!', 'error');
        return;
      }

      state.users.push({
        id: Date.now(),
        username: newUsername,
        role: newUserRole,
        status: 'active',
        created: new Date().toISOString().split('T')[0]
      });

      closeNewUserModal();
      toast(`User ${newUsername} authorized and added by Admin!`, 'success');
      updateUI();
      return;
    }

    try {
      const res = await fetch('/api/auth/register', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          username: newUsername,
          password: newUserPassword,
          role: newUserRole,
          adminPassword: adminPassword
        })
      });

      const data = await res.json();
      if (!res.ok) {
        throw new Error(data.message || 'Authorization failed');
      }

      closeNewUserModal();
      toast(`User "${newUsername}" approved & registered!`, 'success');
      fetchStatus();
    } catch (err) {
      toast(err.message || 'Admin authentication failed', 'error');
    }
  }

  // Delete User from DB
  async function deleteUser(id) {
    if (!state.currentUser || state.currentUser.role !== 'admin') {
      toast('Admin privileges required to remove users', 'error');
      return;
    }

    if (!confirm('Are you sure you want to remove this user from ESP8266 database?')) {
      return;
    }

    if (state.simulated) {
      state.users = state.users.filter(u => u.id !== id);
      toast('User deleted from local DB', 'info');
      updateUI();
      return;
    }

    try {
      const res = await fetch(`/api/users/${id}`, {
        method: 'DELETE',
        headers: {
          'Authorization': `Bearer ${state.currentUser.token}`
        }
      });
      if (!res.ok) throw new Error();
      toast('User deleted from ESP database', 'info');
      fetchStatus();
    } catch (e) {
      toast('Failed to delete user', 'error');
    }
  }

  // Auth Modal (Sign in)
  function openAuthModal() {
    document.getElementById('loginUsername').value = '';
    document.getElementById('loginPassword').value = '';
    document.getElementById('authModal').classList.remove('hidden');
  }

  function closeAuthModal() {
    document.getElementById('authModal').classList.add('hidden');
  }

  async function submitLogin() {
    const user = document.getElementById('loginUsername').value.trim();
    const pass = document.getElementById('loginPassword').value;

    if (state.simulated) {
      const matched = state.users.find(u => u.username.toLowerCase() === user.toLowerCase());
      if (matched && (pass === 'admin' || pass === 'admin123' || pass.length >= 4)) {
        state.currentUser = {
          username: matched.username,
          role: matched.role,
          token: 'sim-token-' + Date.now()
        };
        closeAuthModal();
        toast(`Signed in as ${matched.username} (${matched.role})`, 'success');
        updateUI();
      } else {
        toast('Invalid credentials in simulation mode', 'error');
      }
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
    state.currentUser = null;
    toast('Logged out successfully', 'info');
    updateUI();
  }

  // Helper utilities
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

  function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
  }

  // Auto-init on DOM Ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }

  // Public API
  return {
    onTimerInput,
    saveTimers,
    triggerRelayCycle,
    cancelCycle,
    resetLockout,
    openAuthModal,
    closeAuthModal,
    submitLogin,
    logout,
    openNewUserModal,
    closeNewUserModal,
    submitNewUserRegistration,
    deleteUser
  };
})();

