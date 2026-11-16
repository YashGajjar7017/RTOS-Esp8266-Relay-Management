#pragma once
#include <Arduino.h>

/**
 * Auto-generated embedded Web App asset.
 * Contains complete responsive Single Page Application with styling,
 * real-time SVG countdown, and client logic stored directly in ESP8266 Flash PROGMEM.
 */

static const char PAGE_INDEX[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <meta name="theme-color" content="#08090c">
  <title>CYBER-RELAY RTOS // Smart IoT &amp; Firebase Control</title>
  <style>
/* CYBER-RELAY RTOS // Apple Glassy Prism Obsidian Amber Luxury Design System */
:root {
  --bg-deep: #08090c;
  --bg-surface: #0f1117;
  --bg-card: rgba(18, 21, 29, 0.85);
  --bg-card-hover: rgba(26, 30, 42, 0.92);

  --border-subtle: rgba(255, 255, 255, 0.08);
  --border-glow: rgba(245, 158, 11, 0.28);

  --accent-cyan: #fbbf24;
  --accent-cyan-glow: rgba(245, 158, 11, 0.4);
  --accent-blue: #d97706;
  --accent-emerald: #fbbf24;
  --accent-emerald-glow: rgba(245, 158, 11, 0.35);
  --accent-amber: #f59e0b;
  --accent-amber-glow: rgba(245, 158, 11, 0.45);
  --accent-rose: #f43f5e;
  --accent-rose-glow: rgba(244, 63, 94, 0.35);
  --accent-gold: #fbbf24;
  --accent-purple: #d97706;

  --text-primary: #f8fafc;
  --text-secondary: #94a3b8;
  --text-muted: #64748b;

  --radius-sm: 8px;
  --radius-md: 14px;
  --radius-lg: 20px;
  --radius-full: 9999px;

  --font-main: -apple-system, BlinkMacSystemFont, 'SF Pro Display', 'Plus Jakarta Sans', system-ui, sans-serif;
  --font-mono: 'SF Mono', 'JetBrains Mono', Menlo, monospace;
}

* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}

/* Global Hidden Utility */
.hidden {
  display: none !important;
}

body {
  background-color: var(--bg-deep);
  color: var(--text-primary);
  font-family: var(--font-main);
  min-height: 100vh;
  position: relative;
  overflow-x: hidden;
  line-height: 1.5;
  -webkit-font-smoothing: antialiased;
}

/* Ambient glow orbs (Warm dark amber / golden refraction) */
.ambient-glow {
  position: fixed;
  border-radius: 50%;
  pointer-events: none;
  z-index: 0;
  opacity: 0.16;
}

.glow-1 {
  width: 360px;
  height: 360px;
  background: radial-gradient(circle, rgba(245, 158, 11, 0.4) 0%, rgba(245, 158, 11, 0) 70%);
  top: -80px;
  left: -80px;
}

.glow-2 {
  width: 420px;
  height: 420px;
  background: radial-gradient(circle, rgba(217, 119, 6, 0.3) 0%, rgba(217, 119, 6, 0) 70%);
  bottom: -80px;
  right: -80px;
}

.glow-3 {
  display: none;
}

/* Base Glass Card (Apple Glassy Prism aesthetic: frosted dark acrylic with specular rim highlight) */
.glass-card {
  background: rgba(16, 18, 25, 0.88);
  backdrop-filter: blur(12px);
  -webkit-backdrop-filter: blur(12px);
  border: 1px solid rgba(255, 255, 255, 0.08);
  border-radius: var(--radius-lg);
  box-shadow: 0 16px 36px -10px rgba(0, 0, 0, 0.75), inset 0 1px 1px 0 rgba(255, 255, 255, 0.12), inset 0 0 0 1px rgba(251, 191, 36, 0.08);
  transition: all 0.25s cubic-bezier(0.16, 1, 0.3, 1);
}

.glass-card:hover {
  border-color: rgba(251, 191, 36, 0.22);
}

/* App Layout */
.app-layout {
  position: relative;
  z-index: 1;
  max-width: 1280px;
  margin: 0 auto;
  padding: 24px 20px 40px;
  display: flex;
  flex-direction: column;
  gap: 20px;
}

/* Header */
.app-header {
  padding: 16px 24px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  flex-wrap: wrap;
}

.header-brand {
  display: flex;
  align-items: center;
  gap: 14px;
}

.brand-icon {
  width: 46px;
  height: 46px;
  border-radius: var(--radius-md);
  background: linear-gradient(135deg, #fbbf24, #d97706);
  display: flex;
  align-items: center;
  justify-content: center;
  color: #08090c;
  box-shadow: 0 0 24px rgba(245, 158, 11, 0.45);
}

.brand-icon svg {
  width: 24px;
  height: 24px;
}

.brand-info h1 {
  font-size: 1.35rem;
  font-weight: 800;
  letter-spacing: -0.02em;
  display: flex;
  align-items: center;
  gap: 10px;
}

.badge-rtos {
  font-family: var(--font-mono);
  font-size: 0.68rem;
  font-weight: 700;
  padding: 3px 8px;
  border-radius: var(--radius-full);
  background: rgba(245, 158, 11, 0.15);
  color: var(--accent-cyan);
  border: 1px solid rgba(245, 158, 11, 0.35);
  text-shadow: 0 0 8px var(--accent-cyan-glow);
}

.subtitle {
  font-size: 0.8rem;
  color: var(--text-secondary);
}

.header-meta {
  display: flex;
  align-items: center;
  gap: 12px;
  flex-wrap: wrap;
}

/* Connection Mode Switcher (Local AP vs Firebase Cloud) */
.conn-mode-switcher {
  display: flex;
  background: rgba(0, 0, 0, 0.4);
  padding: 3px;
  border-radius: var(--radius-full);
  border: 1px solid var(--border-subtle);
}

.mode-btn {
  background: none;
  border: none;
  color: var(--text-muted);
  font-family: var(--font-mono);
  font-size: 0.72rem;
  font-weight: 700;
  padding: 5px 12px;
  border-radius: var(--radius-full);
  cursor: pointer;
  transition: all 0.2s ease;
}

.mode-btn.active {
  background: linear-gradient(135deg, #fbbf24, #f59e0b);
  color: #08090c;
  font-weight: 800;
  box-shadow: 0 0 10px var(--accent-cyan-glow);
}

.status-pill {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 8px 14px;
  border-radius: var(--radius-full);
  font-size: 0.75rem;
  font-weight: 600;
  font-family: var(--font-mono);
  border: 1px solid var(--border-subtle);
  background: rgba(15, 23, 42, 0.6);
}

.status-pill.online {
  color: #fbbf24;
  border-color: rgba(251, 191, 36, 0.4);
  background: rgba(245, 158, 11, 0.12);
}

.status-pill.offline {
  color: var(--accent-rose);
  border-color: rgba(244, 63, 94, 0.3);
}

.pulsing-dot {
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background-color: currentColor;
  box-shadow: 0 0 10px currentColor;
  animation: pulseDot 1.8s infinite ease-in-out;
}

@keyframes pulseDot {

  0%,
  100% {
    opacity: 1;
    transform: scale(1);
  }

  50% {
    opacity: 0.4;
    transform: scale(0.85);
  }
}

.user-pill {
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 6px 12px 6px 6px;
  cursor: pointer;
}

.avatar-ring {
  width: 32px;
  height: 32px;
  border-radius: 50%;
  background: linear-gradient(135deg, #f59e0b, #d97706);
  display: flex;
  align-items: center;
  justify-content: center;
  font-weight: 700;
  font-size: 0.85rem;
  color: #08090c;
}

.avatar-ring.role-admin {
  background: linear-gradient(135deg, #fbbf24, #d97706);
}

.avatar-ring.role-guarded {
  background: linear-gradient(135deg, #fbbf24, #b45309);
  color: #08090c;
}

.avatar-ring.role-guest {
  background: linear-gradient(135deg, #64748b, #475569);
}

.user-info-text {
  display: flex;
  flex-direction: column;
  line-height: 1.1;
  text-align: left;
}

#currentUserName {
  font-size: 0.82rem;
  font-weight: 700;
}

.role-tag {
  font-size: 0.62rem;
  font-family: var(--font-mono);
  color: var(--accent-cyan);
  text-transform: uppercase;
}

/* Role Banner Notices */
.role-banner {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 12px 18px;
  border-radius: var(--radius-md);
  font-size: 0.82rem;
  border: 1px solid transparent;
}

.role-banner svg {
  width: 20px;
  height: 20px;
  flex-shrink: 0;
}

.role-banner.hidden {
  display: none !important;
}

.guest-banner {
  background: rgba(148, 163, 184, 0.08);
  border-color: rgba(148, 163, 184, 0.25);
  color: #cbd5e1;
}

.guarded-banner {
  background: rgba(245, 158, 11, 0.1);
  border-color: rgba(245, 158, 11, 0.3);
  color: #fbbf24;
}

/* Dashboard Grid */
.dashboard-grid {
  display: grid;
  grid-template-columns: 1.15fr 0.85fr;
  gap: 24px;
}

@media (max-width: 960px) {
  .dashboard-grid {
    grid-template-columns: 1fr;
  }
}

.grid-card {
  padding: 24px;
  display: flex;
  flex-direction: column;
  gap: 20px;
}

.card-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
}

.eyebrow {
  font-size: 0.68rem;
  font-family: var(--font-mono);
  font-weight: 700;
  letter-spacing: 0.08em;
  color: var(--accent-cyan);
  text-transform: uppercase;
  display: block;
  margin-bottom: 2px;
}

.card-title-group h2 {
  font-size: 1.22rem;
  font-weight: 700;
  letter-spacing: -0.01em;
}

/* State Badge */
.state-badge {
  padding: 6px 14px;
  border-radius: var(--radius-full);
  font-size: 0.78rem;
  font-family: var(--font-mono);
  font-weight: 700;
  display: flex;
  align-items: center;
  gap: 8px;
  border: 1px solid transparent;
  transition: all 0.3s ease;
}

.badge-idle {
  background: rgba(16, 185, 129, 0.12);
  color: #34d399;
  border-color: rgba(16, 185, 129, 0.3);
}

.badge-active {
  background: rgba(245, 158, 11, 0.18);
  color: #fbbf24;
  border-color: rgba(245, 158, 11, 0.45);
  box-shadow: 0 0 20px rgba(245, 158, 11, 0.3);
}

.badge-lockout {
  background: rgba(245, 158, 11, 0.15);
  color: #fbbf24;
  border-color: rgba(245, 158, 11, 0.4);
  box-shadow: 0 0 20px rgba(245, 158, 11, 0.25);
}

/* Radial Countdown Container */
.countdown-radial-container {
  position: relative;
  display: flex;
  align-items: center;
  justify-content: center;
  margin: 10px auto;
  width: 270px;
  height: 270px;
}

.radial-svg {
  width: 100%;
  height: 100%;
  transform: rotate(-90deg);
}

.radial-bg {
  fill: none;
  stroke: rgba(255, 255, 255, 0.05);
  stroke-width: 14;
}

.radial-progress {
  fill: none;
  stroke: url(#cyanGlow);
  stroke-width: 14;
  stroke-linecap: round;
  stroke-dasharray: 691;
  stroke-dashoffset: 0;
  transition: stroke-dashoffset 0.8s ease, stroke 0.5s ease;
  filter: drop-shadow(0 0 10px rgba(245, 158, 11, 0.45));
}

.radial-center-content {
  position: absolute;
  inset: 0;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  text-align: center;
  padding: 24px;
}

.relay-bulb-indicator {
  width: 48px;
  height: 48px;
  border-radius: 50%;
  background: rgba(255, 255, 255, 0.05);
  display: flex;
  align-items: center;
  justify-content: center;
  color: var(--text-muted);
  margin-bottom: 6px;
  transition: all 0.3s ease;
}

.relay-bulb-indicator.bulb-on {
  background: rgba(245, 158, 11, 0.2);
  color: var(--accent-cyan);
  box-shadow: 0 0 25px var(--accent-cyan-glow);
  transform: scale(1.05);
}

.relay-bulb-indicator svg {
  width: 26px;
  height: 26px;
}

.countdown-label {
  font-size: 0.68rem;
  font-family: var(--font-mono);
  font-weight: 700;
  letter-spacing: 0.06em;
  color: var(--text-secondary);
  text-transform: uppercase;
  margin-bottom: 3px;
}

.digital-clock {
  font-family: var(--font-mono);
  font-size: 1.12rem;
  font-weight: 700;
  letter-spacing: 0.02em;
  color: #fff;
  font-variant-numeric: tabular-nums;
  background: rgba(0, 0, 0, 0.45);
  border: 1px solid rgba(255, 255, 255, 0.08);
  border-radius: 8px;
  padding: 4px 10px;
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 2px;
  white-space: nowrap;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.3);
}

.digital-clock .unit {
  font-size: 0.68rem;
  color: var(--accent-cyan);
  margin-right: 3px;
  font-weight: 600;
}

.sub-status {
  font-size: 0.68rem;
  color: var(--text-muted);
  margin-top: 5px;
  max-width: 190px;
  line-height: 1.25;
}

/* Hardware Status Tiles */
.hardware-tiles-row {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 12px;
}

@media (max-width: 580px) {
  .hardware-tiles-row {
    grid-template-columns: 1fr;
  }
}

.hw-tile {
  background: rgba(255, 255, 255, 0.025);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-md);
  padding: 12px 14px;
  display: flex;
  flex-direction: column;
  gap: 4px;
}

.hw-label {
  font-size: 0.65rem;
  font-family: var(--font-mono);
  font-weight: 700;
  color: var(--text-muted);
}

.hw-val-group {
  display: flex;
  align-items: center;
  gap: 8px;
}

.hw-val-group strong {
  font-size: 1.05rem;
  font-weight: 700;
}

.hw-sub {
  font-size: 0.68rem;
  color: var(--text-secondary);
}

.status-dot {
  width: 9px;
  height: 9px;
  border-radius: 50%;
}

.dot-off {
  background: var(--text-muted);
}

.dot-active {
  background: var(--accent-cyan);
  box-shadow: 0 0 10px var(--accent-cyan);
}

.dot-disabled {
  background: var(--text-muted);
}

.dot-warning {
  background: var(--accent-amber);
  box-shadow: 0 0 10px var(--accent-amber);
}

/* Glass Buttons */
.glass-button {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  padding: 11px 18px;
  border-radius: var(--radius-md);
  font-family: var(--font-main);
  font-weight: 600;
  font-size: 0.88rem;
  cursor: pointer;
  border: 1px solid var(--border-subtle);
  background: rgba(255, 255, 255, 0.04);
  color: var(--text-primary);
  transition: all 0.2s cubic-bezier(0.16, 1, 0.3, 1);
  text-decoration: none;
}

.glass-button:hover:not(:disabled) {
  background: rgba(255, 255, 255, 0.09);
  border-color: rgba(255, 255, 255, 0.2);
  transform: translateY(-1px);
}

.glass-button:disabled {
  opacity: 0.35;
  cursor: not-allowed;
  pointer-events: none;
}

.glass-button svg {
  width: 18px;
  height: 18px;
}

.btn-accent {
  background: linear-gradient(135deg, #fbbf24, #d97706);
  border-color: rgba(251, 191, 36, 0.5);
  color: #08090c;
  font-weight: 800;
  box-shadow: 0 4px 18px rgba(245, 158, 11, 0.4);
}

.btn-accent:hover:not(:disabled) {
  background: linear-gradient(135deg, #f59e0b, #b45309);
  box-shadow: 0 6px 24px rgba(245, 158, 11, 0.6);
  color: #08090c;
}

.btn-primary {
  background: linear-gradient(135deg, #fbbf24, #f59e0b);
  border-color: rgba(251, 191, 36, 0.5);
  color: #08090c;
  font-weight: 800;
  box-shadow: 0 4px 18px rgba(245, 158, 11, 0.4);
}

.btn-danger {
  background: rgba(244, 63, 94, 0.12);
  border-color: rgba(244, 63, 94, 0.35);
  color: #fb7185;
}

.btn-warning {
  background: rgba(245, 158, 11, 0.12);
  border-color: rgba(245, 158, 11, 0.35);
  color: #fbbf24;
}

.btn-gold {
  background: linear-gradient(135deg, #d97706, #b45309);
  border-color: rgba(251, 191, 36, 0.5);
  color: #ffffff;
  box-shadow: 0 4px 18px rgba(217, 119, 6, 0.35);
}

.icon-btn-sm {
  background: none;
  border: none;
  color: var(--text-muted);
  cursor: pointer;
  padding: 4px;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: var(--radius-sm);
  transition: all 0.2s;
}

.icon-btn-sm:hover {
  color: var(--text-primary);
  background: rgba(255, 255, 255, 0.1);
}

.icon-btn-sm svg {
  width: 16px;
  height: 16px;
}

.action-buttons-group {
  display: flex;
  flex-direction: column;
  gap: 10px;
}

/* MONTHLY ANALYTICS SECTION (Guest & All Users) */
.analytics-metrics-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 12px;
}

@media (max-width: 580px) {
  .analytics-metrics-grid {
    grid-template-columns: 1fr;
  }
}

.metric-box {
  background: rgba(255, 255, 255, 0.025);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-md);
  padding: 16px 14px;
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.metric-box.box-on {
  border-color: rgba(245, 158, 11, 0.35);
  background: rgba(245, 158, 11, 0.06);
}

.metric-box.box-off {
  border-color: rgba(148, 163, 184, 0.25);
  background: rgba(148, 163, 184, 0.03);
}

.metric-box.box-cycles {
  border-color: rgba(245, 158, 11, 0.3);
  background: rgba(245, 158, 11, 0.04);
}

.metric-icon {
  font-size: 1.25rem;
}

.metric-data {
  display: flex;
  align-items: baseline;
  gap: 4px;
}

.metric-num {
  font-family: var(--font-mono);
  font-size: 1.45rem;
  font-weight: 800;
  color: var(--text-primary);
}

.metric-unit {
  font-size: 0.72rem;
  color: var(--text-secondary);
  font-family: var(--font-mono);
}

.metric-label {
  font-size: 0.7rem;
  color: var(--text-muted);
}

/* Monthly Duty Cycle Ratio Bar */
.analytics-bar-wrapper {
  background: rgba(255, 255, 255, 0.02);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-md);
  padding: 14px 16px;
  display: flex;
  flex-direction: column;
  gap: 10px;
}

.bar-header {
  display: flex;
  justify-content: space-between;
  font-size: 0.8rem;
}

.bar-header strong {
  font-family: var(--font-mono);
  color: var(--accent-cyan);
}

.ratio-bar-track {
  width: 100%;
  height: 10px;
  border-radius: var(--radius-full);
  background: rgba(255, 255, 255, 0.08);
  display: flex;
  overflow: hidden;
}

.ratio-bar-fill.on-fill {
  background: linear-gradient(90deg, #fbbf24, #d97706);
  box-shadow: 0 0 10px rgba(245, 158, 11, 0.5);
  transition: width 0.6s ease;
}

.ratio-bar-fill.off-fill {
  background: #334155;
  transition: width 0.6s ease;
}

.ratio-bar-legend {
  display: flex;
  align-items: center;
  font-size: 0.7rem;
  color: var(--text-muted);
}

.legend-dot {
  width: 7px;
  height: 7px;
  border-radius: 50%;
  margin-right: 6px;
}

.dot-cyan {
  background: var(--accent-cyan);
  box-shadow: 0 0 6px var(--accent-cyan);
}

.dot-slate {
  background: #64748b;
}

/* Right Stack Cards */
.right-stack {
  display: flex;
  flex-direction: column;
  gap: 24px;
}

.info-badge {
  font-size: 0.7rem;
  font-family: var(--font-mono);
  background: rgba(255, 255, 255, 0.06);
  padding: 4px 10px;
  border-radius: var(--radius-full);
  color: var(--text-secondary);
}

/* Config Controls */
.config-controls {
  display: flex;
  flex-direction: column;
  gap: 18px;
}

.control-box {
  background: rgba(255, 255, 255, 0.02);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-md);
  padding: 16px;
}

.control-header {
  display: flex;
  justify-content: space-between;
  align-items: flex-start;
  margin-bottom: 12px;
}

.control-info label {
  font-size: 0.92rem;
  font-weight: 700;
  display: block;
}

.control-desc {
  font-size: 0.74rem;
  color: var(--text-secondary);
}

.number-display {
  font-family: var(--font-mono);
  font-size: 1.25rem;
  font-weight: 700;
  color: var(--accent-cyan);
}

.number-display .unit {
  font-size: 0.75rem;
  color: var(--text-secondary);
}

.slider-wrapper input[type="range"] {
  width: 100%;
  height: 6px;
  border-radius: 3px;
  background: rgba(255, 255, 255, 0.1);
  outline: none;
  -webkit-appearance: none;
  cursor: pointer;
}

.slider-wrapper input[type="range"]::-webkit-slider-thumb {
  -webkit-appearance: none;
  width: 20px;
  height: 20px;
  border-radius: 50%;
  background: var(--accent-cyan);
  box-shadow: 0 0 12px var(--accent-cyan-glow);
  cursor: pointer;
  transition: transform 0.1s;
}

.slider-ticks {
  display: flex;
  justify-content: space-between;
  font-family: var(--font-mono);
  font-size: 0.65rem;
  color: var(--text-muted);
  margin-top: 8px;
}

.slider-ticks .highlight {
  color: var(--accent-cyan);
  font-weight: 700;
}

/* User Database Table */
.user-quota-badge {
  font-family: var(--font-mono);
  font-size: 0.75rem;
  background: rgba(245, 158, 11, 0.15);
  color: var(--accent-cyan);
  padding: 4px 10px;
  border-radius: var(--radius-full);
}

.card-subtext {
  font-size: 0.8rem;
  color: var(--text-secondary);
  margin-top: -10px;
}

.user-table-wrap {
  overflow-x: auto;
}

.user-table {
  width: 100%;
  border-collapse: collapse;
  text-align: left;
  font-size: 0.84rem;
}

.user-table th {
  padding: 8px 12px;
  font-family: var(--font-mono);
  font-size: 0.68rem;
  font-weight: 700;
  color: var(--text-muted);
  text-transform: uppercase;
  border-bottom: 1px solid var(--border-subtle);
}

.user-table td {
  padding: 10px 12px;
  border-bottom: 1px solid rgba(255, 255, 255, 0.04);
}

.user-badge-role {
  font-family: var(--font-mono);
  font-size: 0.68rem;
  padding: 2px 7px;
  border-radius: var(--radius-full);
  text-transform: uppercase;
  font-weight: 700;
}

.role-admin-badge {
  background: rgba(251, 191, 36, 0.15);
  color: var(--accent-gold);
  border: 1px solid rgba(251, 191, 36, 0.3);
}

.role-guarded-badge {
  background: rgba(56, 189, 248, 0.12);
  color: var(--accent-cyan);
  border: 1px solid rgba(56, 189, 248, 0.25);
}

.role-guest-badge {
  background: rgba(148, 163, 184, 0.15);
  color: #94a3b8;
  border: 1px solid rgba(148, 163, 184, 0.25);
}

/* Footer Telemetry */
.system-footer {
  padding: 14px 24px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  flex-wrap: wrap;
  gap: 16px;
}

.footer-item {
  font-family: var(--font-mono);
  font-size: 0.74rem;
}

.ft-label {
  color: var(--text-muted);
  margin-right: 6px;
}

.ft-val {
  color: var(--text-primary);
  font-weight: 600;
}

/* Modals */
.modal-backdrop {
  position: fixed !important;
  inset: 0 !important;
  background: rgba(6, 8, 12, 0.92) !important;
  backdrop-filter: blur(14px) !important;
  -webkit-backdrop-filter: blur(14px) !important;
  display: flex !important;
  align-items: center !important;
  justify-content: center !important;
  z-index: 999999 !important;
  isolation: isolate !important;
  transform: translateZ(0) !important;
  -webkit-transform: translateZ(0) !important;
  padding: 16px !important;
}

.modal-backdrop.hidden {
  display: none !important;
}

.modal-card {
  position: relative !important;
  z-index: 1000000 !important;
  transform: translateZ(0) !important;
  -webkit-transform: translateZ(0) !important;
  width: 100%;
  max-width: 490px;
  padding: 28px;
  display: flex;
  flex-direction: column;
  gap: 18px;
  animation: modalIn 0.25s ease-out;
  box-shadow: 0 20px 60px rgba(0, 0, 0, 0.85), 0 0 30px rgba(245, 158, 11, 0.15);
}

@keyframes modalIn {
  from {
    opacity: 0;
    transform: scale(0.94);
  }

  to {
    opacity: 1;
    transform: scale(1);
  }
}

.modal-highlight-gold {
  border-color: rgba(251, 191, 36, 0.5);
  box-shadow: 0 0 45px rgba(217, 119, 6, 0.35);
}

.modal-header {
  display: flex;
  align-items: flex-start;
  gap: 12px;
  justify-content: space-between;
}

.auth-icon-badge {
  width: 44px;
  height: 44px;
  border-radius: var(--radius-md);
  background: rgba(251, 191, 36, 0.15);
  color: var(--accent-gold);
  border: 1px solid rgba(251, 191, 36, 0.3);
  display: flex;
  align-items: center;
  justify-content: center;
  flex-shrink: 0;
}

.auth-icon-badge svg {
  width: 22px;
  height: 22px;
}

.modal-subtitle {
  font-size: 0.78rem;
  color: var(--text-secondary);
  margin-top: 2px;
}

.close-btn {
  background: none;
  border: none;
  color: var(--text-muted);
  font-size: 1.1rem;
  cursor: pointer;
  padding: 4px;
}

.close-btn:hover {
  color: var(--text-primary);
}

/* Req Details Box for Admin Popup */
.req-details-box {
  background: rgba(251, 191, 36, 0.05);
  border: 1px solid rgba(251, 191, 36, 0.25);
  border-radius: var(--radius-md);
  padding: 14px 16px;
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.req-row {
  display: flex;
  justify-content: space-between;
  font-size: 0.82rem;
}

.req-label {
  color: var(--text-muted);
}

.badge-single-use {
  font-family: var(--font-mono);
  font-size: 0.65rem;
  padding: 2px 7px;
  border-radius: var(--radius-full);
  background: rgba(244, 63, 94, 0.15);
  color: #fb7185;
  border: 1px solid rgba(244, 63, 94, 0.3);
}

/* Radar Pulsing Animation for Waiting Modal */
.radar-spinner {
  position: relative;
  width: 100px;
  height: 100px;
  margin: 10px auto;
  display: flex;
  align-items: center;
  justify-content: center;
}

.radar-circle {
  position: absolute;
  border-radius: 50%;
  border: 2px solid var(--accent-cyan);
  opacity: 0;
  animation: radarPulse 2.4s infinite cubic-bezier(0.2, 0.8, 0.2, 1);
}

.circle-1 {
  width: 100%;
  height: 100%;
  animation-delay: 0s;
}

.circle-2 {
  width: 100%;
  height: 100%;
  animation-delay: 0.8s;
}

.circle-3 {
  width: 100%;
  height: 100%;
  animation-delay: 1.6s;
}

.radar-core {
  font-size: 1.8rem;
  z-index: 2;
}

@keyframes radarPulse {
  0% {
    transform: scale(0.2);
    opacity: 0.9;
  }

  100% {
    transform: scale(1.4);
    opacity: 0;
  }
}

/* Form Styles */
.form-group {
  display: flex;
  flex-direction: column;
  gap: 6px;
  margin-bottom: 14px;
}

.form-group label {
  font-size: 0.8rem;
  font-weight: 600;
  color: var(--text-secondary);
}

.form-group input,
.form-group select {
  background: rgba(255, 255, 255, 0.04);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-sm);
  padding: 10px 14px;
  font-family: var(--font-main);
  font-size: 0.88rem;
  color: var(--text-primary);
  outline: none;
  transition: all 0.2s;
}

.form-group input:focus,
.form-group select:focus {
  border-color: var(--accent-cyan);
  background: rgba(255, 255, 255, 0.08);
  box-shadow: 0 0 12px var(--accent-cyan-glow);
}

.preset-users-chips {
  display: flex;
  align-items: center;
  gap: 6px;
  flex-wrap: wrap;
  margin-bottom: 14px;
}

.chip-label {
  font-size: 0.72rem;
  color: var(--text-muted);
}

.preset-chip {
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-full);
  padding: 3px 10px;
  font-size: 0.72rem;
  color: var(--text-secondary);
  cursor: pointer;
  transition: all 0.2s;
}

.preset-chip:hover {
  background: rgba(245, 158, 11, 0.18);
  border-color: var(--accent-cyan);
  color: var(--accent-cyan);
}

.text-gold {
  color: var(--accent-gold) !important;
}

.text-center {
  text-align: center;
}

.modal-actions {
  display: flex;
  justify-content: flex-end;
  gap: 10px;
  margin-top: 10px;
}

/* Toast System */
.toast-container {
  position: fixed;
  bottom: 24px;
  right: 24px;
  display: flex;
  flex-direction: column;
  gap: 10px;
  z-index: 1000001 !important;
}

.toast {
  padding: 12px 18px;
  border-radius: var(--radius-md);
  font-size: 0.85rem;
  font-weight: 600;
  display: flex;
  align-items: center;
  gap: 10px;
  background: var(--bg-surface);
  border: 1px solid var(--border-subtle);
  box-shadow: 0 10px 25px rgba(0, 0, 0, 0.6);
  animation: toastIn 0.3s ease;
}

@keyframes toastIn {
  from {
    opacity: 0;
    transform: translateY(12px);
  }

  to {
    opacity: 1;
    transform: translateY(0);
  }
}

.toast-success {
  border-color: rgba(16, 185, 129, 0.5);
  color: #34d399;
}

.toast-error {
  border-color: rgba(244, 63, 94, 0.5);
  color: #fb7185;
}

.toast-info {
  border-color: rgba(245, 158, 11, 0.5);
  color: #fbbf24;
}

.toast-gold {
  border-color: rgba(251, 191, 36, 0.6);
  color: #fbbf24;
}

.w-full {
  width: 100%;
}

/* ========================================================================= */
/* AUTHENTICATION PORTAL (FULL SCREEN LOGIN / SIGN-IN VIEW)                   */
/* ========================================================================= */
.auth-wrapper {
  position: relative;
  z-index: 10;
  min-height: 100vh;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 20px;
  animation: authFadeIn 0.4s ease-out;
}

@keyframes authFadeIn {
  from {
    opacity: 0;
    transform: scale(0.97);
  }

  to {
    opacity: 1;
    transform: scale(1);
  }
}

.auth-card {
  width: 100%;
  max-width: 720px;
  padding: 38px 36px;
  border-radius: var(--radius-lg);
  border: 1px solid rgba(245, 158, 11, 0.28);
  box-shadow: 0 24px 60px rgba(0, 0, 0, 0.8), 0 0 45px rgba(245, 158, 11, 0.16), inset 0 1px 1px 0 rgba(255, 255, 255, 0.15);
  position: relative;
  overflow: hidden;
}

.auth-card::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 3px;
  background: linear-gradient(90deg, var(--accent-cyan), var(--accent-blue), var(--accent-purple));
}

.auth-brand {
  text-align: center;
  margin-bottom: 20px;
}

.auth-logo-badge {
  width: 58px;
  height: 58px;
  margin: 0 auto 12px;
  background: linear-gradient(135deg, rgba(245, 158, 11, 0.25), rgba(217, 119, 6, 0.15));
  border: 1px solid rgba(245, 158, 11, 0.45);
  border-radius: 16px;
  display: flex;
  align-items: center;
  justify-content: center;
  color: var(--accent-cyan);
  box-shadow: 0 0 20px rgba(245, 158, 11, 0.35);
}

.auth-logo-badge svg {
  width: 30px;
  height: 30px;
}

.auth-title {
  font-size: 1.45rem;
  font-weight: 800;
  letter-spacing: -0.02em;
  color: #fff;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
}

.auth-subtitle {
  font-size: 0.8rem;
  color: var(--text-secondary);
  margin-top: 4px;
}

.auth-conn-bar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  background: rgba(255, 255, 255, 0.03);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-sm);
  padding: 8px 12px;
  margin-bottom: 18px;
}

.auth-bar-label {
  font-size: 0.75rem;
  font-weight: 600;
  color: var(--text-muted);
  text-transform: uppercase;
  letter-spacing: 0.04em;
}

.conn-mode-switcher.compact {
  padding: 2px;
  background: rgba(0, 0, 0, 0.3);
}

.conn-mode-switcher.compact .mode-btn {
  padding: 4px 10px;
  font-size: 0.72rem;
}

/* Tabs */
.auth-tabs {
  display: flex;
  gap: 6px;
  margin-bottom: 18px;
  background: rgba(0, 0, 0, 0.25);
  padding: 4px;
  border-radius: var(--radius-sm);
  border: 1px solid var(--border-subtle);
}

.auth-tab-btn {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 6px;
  padding: 9px 12px;
  background: transparent;
  border: none;
  border-radius: 6px;
  color: var(--text-muted);
  font-family: var(--font-main);
  font-size: 0.82rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.2s ease;
}

.auth-tab-btn svg {
  width: 15px;
  height: 15px;
}

.auth-tab-btn:hover {
  color: var(--text-primary);
}

.auth-tab-btn.active {
  background: rgba(245, 158, 11, 0.18);
  color: var(--accent-cyan);
  border: 1px solid rgba(245, 158, 11, 0.4);
  box-shadow: 0 0 10px rgba(245, 158, 11, 0.25);
}

.auth-tab-content {
  display: none;
}

.auth-tab-content.active {
  display: block;
  animation: authContentFade 0.25s ease-out;
}

@keyframes authContentFade {
  from {
    opacity: 0;
    transform: translateY(6px);
  }

  to {
    opacity: 1;
    transform: translateY(0);
  }
}

/* Input Icon Wrap */
.input-icon-wrap {
  position: relative;
  display: flex;
  align-items: center;
}

.input-icon-wrap svg {
  position: absolute;
  left: 12px;
  width: 16px;
  height: 16px;
  color: var(--text-muted);
  pointer-events: none;
}

.input-icon-wrap input,
.input-icon-wrap select {
  width: 100%;
  padding-left: 38px;
  padding-right: 38px;
}

.btn-toggle-eye {
  position: absolute;
  right: 10px;
  background: transparent;
  border: none;
  cursor: pointer;
  font-size: 0.95rem;
  opacity: 0.65;
  transition: opacity 0.2s;
  padding: 4px;
}

.btn-toggle-eye:hover {
  opacity: 1;
}

/* Role Login Quick Selectors (Admin Login / User Login / Guest Login) */
.role-select-section {
  margin-bottom: 16px;
}

.role-select-label {
  display: block;
  margin-bottom: 6px;
  font-size: 0.72rem;
  color: var(--text-muted);
  font-weight: 600;
  text-transform: uppercase;
  letter-spacing: 0.04em;
}

.role-select-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 8px;
}

.role-select-btn {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  padding: 10px 6px;
  border-radius: var(--radius-sm);
  background: rgba(255, 255, 255, 0.03);
  border: 1px solid var(--border-subtle);
  color: var(--text-secondary);
  cursor: pointer;
  transition: all 0.2s ease;
  font-family: var(--font-main);
  text-align: center;
}

.role-select-btn .btn-icon {
  font-size: 1.15rem;
  margin-bottom: 3px;
}

.role-select-btn .btn-title {
  font-size: 0.74rem;
  font-weight: 700;
  color: var(--text-primary);
  white-space: nowrap;
}

.role-select-btn .btn-user {
  font-size: 0.62rem;
  color: var(--text-muted);
  margin-top: 1px;
}

.role-select-btn:hover {
  transform: translateY(-2px);
}

.role-select-btn.btn-admin:hover,
.role-select-btn.btn-admin.selected {
  background: rgba(251, 191, 36, 0.12);
  border-color: var(--accent-gold);
  box-shadow: 0 0 12px rgba(251, 191, 36, 0.25);
}

.role-select-btn.btn-admin.selected .btn-title {
  color: var(--accent-gold);
}

.role-select-btn.btn-user:hover,
.role-select-btn.btn-user.selected {
  background: rgba(245, 158, 11, 0.12);
  border-color: var(--accent-cyan);
  box-shadow: 0 0 12px rgba(245, 158, 11, 0.25);
}

.role-select-btn.btn-user.selected .btn-title {
  color: var(--accent-cyan);
}

.role-select-btn.btn-guest:hover,
.role-select-btn.btn-guest.selected {
  background: rgba(148, 163, 184, 0.12);
  border-color: #94a3b8;
  box-shadow: 0 0 12px rgba(148, 163, 184, 0.25);
}

.role-select-btn.btn-guest.selected .btn-title {
  color: #fff;
}

/* Checkbox and Forgot Password Row */
.form-checkbox-row {
  margin: 14px 0 18px;
}

.form-extra-row {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin: 12px 0 16px;
  flex-wrap: wrap;
  gap: 8px;
}

.btn-link-forgot {
  background: transparent;
  border: none;
  color: var(--accent-amber);
  font-family: var(--font-main);
  font-size: 0.78rem;
  font-weight: 600;
  cursor: pointer;
  padding: 2px 4px;
  transition: all 0.2s ease;
  text-decoration: underline;
  text-underline-offset: 3px;
}

.btn-link-forgot:hover {
  color: #fde047;
  text-shadow: 0 0 8px rgba(245, 158, 11, 0.4);
}

.custom-checkbox-label {
  display: flex;
  align-items: flex-start;
  gap: 10px;
  cursor: pointer;
  user-select: none;
}

.custom-checkbox-label input[type="checkbox"] {
  position: absolute;
  opacity: 0;
  pointer-events: none;
}

.checkbox-custom {
  width: 18px;
  height: 18px;
  flex-shrink: 0;
  border-radius: 4px;
  border: 1px solid rgba(245, 158, 11, 0.4);
  background: rgba(245, 158, 11, 0.08);
  display: flex;
  align-items: center;
  justify-content: center;
  margin-top: 2px;
  transition: all 0.2s ease;
}

.custom-checkbox-label input[type="checkbox"]:checked+.checkbox-custom {
  background: var(--accent-cyan);
  border-color: var(--accent-cyan);
  box-shadow: 0 0 8px var(--accent-cyan-glow);
}

.custom-checkbox-label input[type="checkbox"]:checked+.checkbox-custom::after {
  content: '✓';
  color: #07090e;
  font-size: 0.75rem;
  font-weight: 900;
}

.checkbox-text {
  display: flex;
  flex-direction: column;
  font-size: 0.82rem;
  color: var(--text-primary);
}

.checkbox-subtext {
  font-size: 0.72rem;
  color: var(--text-muted);
  margin-top: 1px;
}

.auth-submit-btn {
  padding: 12px;
  font-size: 0.9rem;
  font-weight: 700;
  letter-spacing: 0.03em;
  margin-top: 6px;
  box-shadow: 0 4px 20px rgba(245, 158, 11, 0.35);
}

.auth-footer-meta {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-top: 22px;
  padding-top: 14px;
  border-top: 1px solid var(--border-subtle);
  font-size: 0.7rem;
  color: var(--text-muted);
}

.security-badge {
  color: var(--accent-emerald);
  font-weight: 700;
  font-size: 0.68rem;
  letter-spacing: 0.04em;
}

/* Styled Logout Button */
.logout-btn-styled {
  display: inline-flex;
  align-items: center;
  gap: 5px;
  background: rgba(244, 63, 94, 0.12);
  border: 1px solid rgba(244, 63, 94, 0.3);
  color: #fb7185;
  border-radius: var(--radius-full);
  padding: 5px 11px;
  font-family: var(--font-main);
  font-size: 0.74rem;
  font-weight: 700;
  cursor: pointer;
  transition: all 0.2s ease;
  margin-left: 6px;
}

.logout-btn-styled svg {
  width: 14px;
  height: 14px;
}

.logout-btn-styled:hover {
  background: rgba(244, 63, 94, 0.22);
  border-color: #f43f5e;
  color: #fff;
  box-shadow: 0 0 10px rgba(244, 63, 94, 0.3);
}

/* Non-Intrusive Connection Assistance Banner */
.conn-help-banner {
  display: flex;
  align-items: center;
  justify-content: space-between;
  background: rgba(245, 158, 11, 0.12);
  border: 1px solid rgba(245, 158, 11, 0.35);
  border-radius: var(--radius-sm);
  padding: 9px 14px;
  margin-bottom: 14px;
  font-size: 0.8rem;
  color: #fde68a;
  animation: bannerSlide 0.3s ease-out;
}

@keyframes bannerSlide {
  from {
    opacity: 0;
    transform: translateY(-8px);
  }

  to {
    opacity: 1;
    transform: translateY(0);
  }
}

.conn-banner-left {
  display: flex;
  align-items: center;
  gap: 8px;
}

.pulsing-warn-dot {
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background: var(--accent-amber);
  box-shadow: 0 0 8px var(--accent-amber);
  animation: warnPulse 1.4s infinite;
}

@keyframes warnPulse {

  0%,
  100% {
    opacity: 1;
    transform: scale(1);
  }

  50% {
    opacity: 0.4;
    transform: scale(0.85);
  }
}

.conn-banner-actions {
  display: flex;
  align-items: center;
  gap: 8px;
}

.btn-banner-action {
  background: rgba(245, 158, 11, 0.2);
  border: 1px solid rgba(245, 158, 11, 0.4);
  color: #fbbf24;
  border-radius: 6px;
  padding: 4px 9px;
  font-size: 0.72rem;
  font-weight: 700;
  cursor: pointer;
  transition: all 0.2s;
}

.btn-banner-action:hover {
  background: var(--accent-cyan);
  color: #07090e;
}

.btn-banner-dismiss {
  background: transparent;
  border: none;
  color: var(--text-muted);
  cursor: pointer;
  font-size: 0.9rem;
  padding: 2px 6px;
}

/* Settings Icon Button */
.icon-btn-settings {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 36px;
  height: 36px;
  padding: 0;
  border-radius: 50%;
  color: var(--text-secondary);
}

.icon-btn-settings svg {
  width: 18px;
  height: 18px;
}

.icon-btn-settings:hover {
  color: var(--accent-cyan);
  border-color: var(--accent-cyan);
  box-shadow: 0 0 10px var(--accent-cyan-glow);
}

.field-hint {
  font-size: 0.68rem;
  color: var(--text-muted);
  margin-top: 3px;
}

.test-conn-row {
  display: flex;
  gap: 8px;
  margin-top: 10px;
}

.test-conn-row button {
  flex: 1;
  font-size: 0.75rem;
  padding: 7px;
}

@media (max-width: 680px) {
  .auth-wrapper {
    padding: 8px 6px;
  }

  .auth-card {
    padding: 22px 14px;
    width: 100%;
    max-width: 100%;
    border-radius: var(--radius-md);
  }

  .preset-users-chips .chip-group {
    grid-template-columns: 1fr;
  }

  .conn-help-banner {
    flex-direction: column;
    gap: 8px;
    align-items: flex-start;
  }

  .conn-banner-actions {
    width: 100%;
    justify-content: space-between;
  }

  /* Compact Top Bar for normal mobile phones (Prevents AP Button from covering 3-line hamburger) */
  .main-top-bar {
    padding: 6px 10px;
    height: 52px;
    min-height: 52px;
    gap: 8px;
    display: flex;
    align-items: center;
    justify-content: space-between;
    flex-wrap: nowrap;
    position: sticky;
    top: 0;
    z-index: 80;
  }

  .top-bar-left {
    display: flex;
    align-items: center;
    gap: 8px;
    flex-shrink: 0 !important;
    min-width: auto !important;
    z-index: 10;
  }

  .btn-toggle-sidebar {
    display: inline-flex !important;
    padding: 7px;
    width: 38px;
    height: 38px;
    min-width: 38px;
    flex-shrink: 0 !important;
    cursor: pointer;
    background: rgba(255, 255, 255, 0.08);
    border: 1px solid var(--border-subtle);
    border-radius: var(--radius-sm);
    z-index: 20;
  }

  .view-heading-group {
    min-width: 0;
  }

  .view-heading-group h1 {
    font-size: 0.95rem;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
    line-height: 1.1;
  }

  .top-bar-right {
    display: flex;
    align-items: center;
    gap: 6px;
    flex-wrap: nowrap;
    flex-shrink: 1;
    min-width: 0;
    justify-content: flex-end;
  }

  .conn-mode-switcher {
    padding: 2px;
    flex-shrink: 0;
  }

  .mode-btn {
    padding: 4px 8px;
    font-size: 0.68rem;
  }

  .btn-quick-log {
    display: none !important;
  }

  .btn-log-text {
    display: none !important;
  }

  .hidden-mobile {
    display: none !important;
  }

  .status-pill {
    padding: 4px 8px;
    font-size: 0.65rem;
    gap: 4px;
    flex-shrink: 0;
  }

  .telemetry-beacon-group {
    display: none !important;
  }
}

/* ========================================================================= */
/* SIDEBAR NAVIGATION & MULTI-VIEW ARCHITECTURE (APPLE PRISM OBSIDIAN AMBER)  */
/* ========================================================================= */
.app-shell {
  display: flex;
  min-height: 100vh;
  position: relative;
  z-index: 1;
}

.sidebar-overlay {
  position: fixed;
  inset: 0;
  background: rgba(0, 0, 0, 0.65);
  backdrop-filter: blur(4px);
  -webkit-backdrop-filter: blur(4px);
  z-index: 90;
  display: none;
  opacity: 0;
  transition: opacity 0.25s ease;
}

.sidebar-overlay.active {
  display: block;
  opacity: 1;
}

.app-sidebar {
  width: 270px;
  flex-shrink: 0;
  background: rgba(14, 16, 23, 0.94);
  backdrop-filter: blur(16px);
  -webkit-backdrop-filter: blur(16px);
  border-right: 1px solid var(--border-subtle);
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  position: sticky;
  top: 0;
  height: 100vh;
  z-index: 95;
  transition: transform 0.28s cubic-bezier(0.16, 1, 0.3, 1);
}

.sidebar-header {
  padding: 20px 18px 16px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  border-bottom: 1px solid rgba(255, 255, 255, 0.05);
}

.sidebar-brand {
  display: flex;
  align-items: center;
  gap: 12px;
}

.sidebar-brand .brand-icon {
  width: 38px;
  height: 38px;
}

.sidebar-brand h2 {
  font-size: 1.05rem;
  font-weight: 800;
  letter-spacing: -0.01em;
  line-height: 1.1;
}

.sidebar-brand .brand-tag {
  font-size: 0.62rem;
  font-family: var(--font-mono);
  color: var(--accent-amber);
}

.btn-close-sidebar {
  display: none;
  background: transparent;
  border: none;
  color: var(--text-muted);
  font-size: 1.25rem;
  cursor: pointer;
  padding: 4px;
}

.sidebar-nav {
  padding: 16px 12px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  flex: 1;
  overflow-y: auto;
}

.nav-section-title {
  font-size: 0.65rem;
  font-family: var(--font-mono);
  font-weight: 700;
  letter-spacing: 0.06em;
  color: var(--text-muted);
  text-transform: uppercase;
  padding: 8px 10px 4px;
}

.nav-item {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 10px 14px;
  border-radius: var(--radius-md);
  color: var(--text-secondary);
  background: transparent;
  border: 1px solid transparent;
  cursor: pointer;
  font-family: var(--font-main);
  font-size: 0.84rem;
  font-weight: 600;
  text-align: left;
  transition: all 0.2s cubic-bezier(0.16, 1, 0.3, 1);
  width: 100%;
}

.nav-item-left {
  display: flex;
  align-items: center;
  gap: 11px;
}

.nav-item svg {
  width: 18px;
  height: 18px;
  color: var(--text-muted);
  transition: color 0.2s;
}

.nav-item:hover {
  color: var(--text-primary);
  background: rgba(255, 255, 255, 0.04);
  border-color: rgba(255, 255, 255, 0.06);
}

.nav-item:hover svg {
  color: var(--accent-cyan);
}

.nav-item.active {
  background: rgba(245, 158, 11, 0.14);
  border-color: rgba(251, 191, 36, 0.35);
  color: #fbbf24;
  box-shadow: inset 0 1px 1px 0 rgba(255, 255, 255, 0.1), 0 0 16px rgba(245, 158, 11, 0.15);
}

.nav-item.active svg {
  color: var(--accent-cyan);
}

.nav-badge {
  font-size: 0.65rem;
  font-family: var(--font-mono);
  font-weight: 700;
  padding: 2px 7px;
  border-radius: var(--radius-full);
  background: rgba(255, 255, 255, 0.06);
  color: var(--text-muted);
}

.nav-item.active .nav-badge {
  background: rgba(245, 158, 11, 0.25);
  color: #fbbf24;
}

.sidebar-footer {
  padding: 14px 16px;
  border-top: 1px solid rgba(255, 255, 255, 0.05);
  background: rgba(8, 9, 12, 0.4);
}

.sidebar-user-box {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 10px;
}

.sidebar-user-info {
  display: flex;
  align-items: center;
  gap: 10px;
}

/* Main Content Area */
.app-main-content {
  flex: 1;
  display: flex;
  flex-direction: column;
  min-width: 0;
  min-height: 100vh;
}

.main-top-bar {
  padding: 14px 24px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 14px;
  border-bottom: 1px solid rgba(255, 255, 255, 0.06);
  background: rgba(14, 16, 23, 0.85);
  backdrop-filter: blur(12px);
  -webkit-backdrop-filter: blur(12px);
  position: sticky;
  top: 0;
  z-index: 80;
}

.top-bar-left {
  display: flex;
  align-items: center;
  gap: 14px;
}

.btn-toggle-sidebar {
  display: none;
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-sm);
  color: var(--text-primary);
  padding: 8px;
  cursor: pointer;
  align-items: center;
  justify-content: center;
}

.btn-toggle-sidebar svg {
  width: 20px;
  height: 20px;
}

.view-heading-group h1 {
  font-size: 1.25rem;
  font-weight: 800;
  letter-spacing: -0.01em;
  display: flex;
  align-items: center;
  gap: 8px;
}

.view-subheading {
  font-size: 0.74rem;
  color: var(--text-muted);
}

.top-bar-right {
  display: flex;
  align-items: center;
  gap: 10px;
  flex-wrap: wrap;
}

/* Real Telemetry Badges (Real ESP Ping & RSSI) */
.telemetry-beacon-group {
  display: flex;
  align-items: center;
  gap: 6px;
}

.diag-pill {
  display: flex;
  align-items: center;
  gap: 5px;
  padding: 5px 10px;
  border-radius: var(--radius-full);
  font-family: var(--font-mono);
  font-size: 0.7rem;
  font-weight: 700;
  background: rgba(255, 255, 255, 0.04);
  border: 1px solid rgba(255, 255, 255, 0.08);
  color: var(--text-secondary);
}

.diag-pill.ping-good {
  color: #34d399;
  border-color: rgba(52, 211, 153, 0.3);
}

.diag-pill.ping-mid {
  color: #fbbf24;
  border-color: rgba(251, 191, 36, 0.3);
}

.diag-pill.ping-slow {
  color: #f87171;
  border-color: rgba(248, 113, 113, 0.3);
}

.diag-pill.rssi-good {
  color: #fbbf24;
  border-color: rgba(251, 191, 36, 0.3);
}

.btn-quick-log {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  background: rgba(245, 158, 11, 0.12);
  border: 1px solid rgba(251, 191, 36, 0.35);
  color: #fbbf24;
  border-radius: var(--radius-full);
  padding: 6px 13px;
  font-family: var(--font-mono);
  font-size: 0.74rem;
  font-weight: 700;
  cursor: pointer;
  transition: all 0.2s ease;
}

.btn-quick-log:hover {
  background: rgba(245, 158, 11, 0.22);
  box-shadow: 0 0 14px rgba(245, 158, 11, 0.3);
}

.btn-quick-log svg {
  width: 14px;
  height: 14px;
}

/* Page Views Container */
.views-container {
  padding: 24px 28px 40px;
  max-width: 1240px;
  width: 100%;
  margin: 0 auto;
}

.view-page {
  display: none;
  animation: pageViewFadeIn 0.24s cubic-bezier(0.16, 1, 0.3, 1);
}

.view-page.active {
  display: block;
}

@keyframes pageViewFadeIn {
  from {
    opacity: 0;
    transform: translateY(8px);
  }

  to {
    opacity: 1;
    transform: translateY(0);
  }
}

/* ========================================================================= */
/* EXTEND RUNNING RELAY TIMER PANEL (FOR ADMIN)                              */
/* ========================================================================= */
.extend-timer-panel {
  margin-top: 18px;
  background: rgba(245, 158, 11, 0.06);
  border: 1px solid rgba(251, 191, 36, 0.25);
  border-radius: var(--radius-md);
  padding: 16px 18px;
  box-shadow: inset 0 1px 1px rgba(255, 255, 255, 0.08);
}

.extend-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 12px;
}

.extend-title {
  font-size: 0.8rem;
  font-family: var(--font-mono);
  font-weight: 700;
  color: #fbbf24;
  display: flex;
  align-items: center;
  gap: 7px;
  text-transform: uppercase;
  letter-spacing: 0.04em;
}

.extend-chips-grid {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 8px;
  margin-bottom: 12px;
}

.btn-extend-chip {
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid rgba(251, 191, 36, 0.3);
  color: #f8fafc;
  padding: 8px 10px;
  border-radius: var(--radius-sm);
  font-family: var(--font-mono);
  font-size: 0.8rem;
  font-weight: 700;
  cursor: pointer;
  transition: all 0.2s ease;
  text-align: center;
}

.btn-extend-chip:hover {
  background: rgba(245, 158, 11, 0.25);
  color: #fbbf24;
  border-color: #fbbf24;
  box-shadow: 0 0 10px rgba(245, 158, 11, 0.35);
  transform: translateY(-1px);
}

.extend-custom-row {
  display: flex;
  align-items: center;
  gap: 8px;
}

.extend-custom-row input {
  flex: 1;
  padding: 8px 12px;
  font-family: var(--font-mono);
  font-size: 0.8rem;
}

.extend-custom-row button {
  padding: 8px 16px;
  font-size: 0.8rem;
  font-weight: 700;
  white-space: nowrap;
}

/* ========================================================================= */
/* FIREBASE CLOUD LOG CONSOLE & INSPECTOR                                    */
/* ========================================================================= */
.cloud-log-container {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.cloud-target-box {
  background: rgba(0, 0, 0, 0.35);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-md);
  padding: 14px 18px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  flex-wrap: wrap;
  gap: 12px;
}

.cloud-target-url {
  font-family: var(--font-mono);
  font-size: 0.78rem;
  color: var(--accent-cyan);
  word-break: break-all;
}

.cloud-log-terminal {
  background: #040507;
  border: 1px solid rgba(255, 255, 255, 0.08);
  border-radius: var(--radius-md);
  padding: 16px;
  height: 420px;
  overflow-y: auto;
  font-family: var(--font-mono);
  font-size: 0.74rem;
  line-height: 1.6;
  color: #cbd5e1;
  box-shadow: inset 0 2px 8px rgba(0, 0, 0, 0.8);
}

.terminal-line {
  display: flex;
  align-items: flex-start;
  gap: 8px;
  margin-bottom: 5px;
  border-bottom: 1px solid rgba(255, 255, 255, 0.02);
  padding-bottom: 4px;
}

.term-time {
  color: var(--text-muted);
  flex-shrink: 0;
}

.term-tag {
  font-weight: 800;
  padding: 1px 6px;
  border-radius: 3px;
  font-size: 0.65rem;
  flex-shrink: 0;
}

.tag-poll {
  background: rgba(59, 130, 246, 0.2);
  color: #60a5fa;
}

.tag-cmd {
  background: rgba(245, 158, 11, 0.25);
  color: #fbbf24;
}

.tag-sync {
  background: rgba(16, 185, 129, 0.2);
  color: #34d399;
}

.tag-err {
  background: rgba(244, 63, 94, 0.25);
  color: #fb7185;
}

.term-msg {
  flex: 1;
  word-break: break-all;
}

.cloud-log-actions {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 10px;
  flex-wrap: wrap;
}

/* ========================================================================= */
/* USER STATUS TOGGLES (ACTIVE / SUSPENDED BY ADMIN)                         */
/* ========================================================================= */
.user-status-pill {
  font-family: var(--font-mono);
  font-size: 0.7rem;
  font-weight: 700;
  padding: 3px 8px;
  border-radius: var(--radius-full);
  display: inline-flex;
  align-items: center;
  gap: 4px;
}

.user-status-pill.status-active {
  background: rgba(16, 185, 129, 0.15);
  color: #34d399;
  border: 1px solid rgba(16, 185, 129, 0.3);
}

.user-status-pill.status-disabled {
  background: rgba(244, 63, 94, 0.18);
  color: #fb7185;
  border: 1px solid rgba(244, 63, 94, 0.35);
}

.btn-status-action {
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid var(--border-subtle);
  border-radius: 6px;
  font-size: 0.72rem;
  font-weight: 700;
  padding: 4px 10px;
  cursor: pointer;
  transition: all 0.2s;
}

.btn-disable-action {
  color: #fb7185;
  border-color: rgba(244, 63, 94, 0.3);
}

.btn-disable-action:hover {
  background: rgba(244, 63, 94, 0.2);
}

.btn-enable-action {
  color: #34d399;
  border-color: rgba(16, 185, 129, 0.3);
}

.btn-enable-action:hover {
  background: rgba(16, 185, 129, 0.2);
}

/* ========================================================================= */
/* RESPONSIVE BREAKPOINTS FOR SIDEBAR                                        */
/* ========================================================================= */
@media (max-width: 900px) {
  .app-sidebar {
    position: fixed;
    top: 0;
    left: 0;
    bottom: 0;
    transform: translateX(-100%);
    box-shadow: 10px 0 30px rgba(0, 0, 0, 0.7);
  }

  .app-sidebar.open {
    transform: translateX(0);
  }

  .btn-close-sidebar {
    display: block;
  }

  .btn-toggle-sidebar {
    display: inline-flex;
  }

  .views-container {
    padding: 16px 14px 30px;
  }

  .telemetry-beacon-group {
    display: none;
  }
}

/* ========================================================================= */
/* GLOBAL TOUCH-HOLD & CONTEXT MENU PREVENTION                               */
/* Prevents browser text-selection & context popup from rendering over UI    */
/* ========================================================================= */
*,
*::before,
*::after {
  -webkit-touch-callout: none;
}

html,
body {
  -webkit-touch-callout: none !important;
  -webkit-user-select: none !important;
  user-select: none !important;
}

input,
textarea,
[contenteditable="true"] {
  -webkit-touch-callout: default !important;
  -webkit-user-select: text !important;
  user-select: text !important;
}

/* ========================================================================= */
/* DYNAMIC MULTI-RELAY CHANNEL TABS & INDEPENDENT COUNTDOWNS                 */
/* ========================================================================= */
.relay-channels-nav {
  margin-bottom: 16px;
  width: 100%;
}

.relay-tabs-scroll {
  display: flex;
  align-items: center;
  gap: 8px;
  overflow-x: auto;
  padding: 4px 2px 8px;
  scrollbar-width: thin;
}

.relay-tab-btn {
  display: inline-flex;
  align-items: center;
  gap: 8px;
  padding: 8px 16px;
  background: rgba(255, 255, 255, 0.04);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-full);
  color: var(--text-secondary);
  font-family: var(--font-mono);
  font-size: 0.8rem;
  font-weight: 700;
  cursor: pointer;
  white-space: nowrap;
  transition: all 0.2s ease;
}

.relay-tab-btn:hover {
  background: rgba(255, 255, 255, 0.09);
  color: var(--text-primary);
  border-color: rgba(255, 255, 255, 0.2);
}

.relay-tab-btn.active {
  background: linear-gradient(135deg, rgba(245, 158, 11, 0.25), rgba(217, 119, 6, 0.15));
  border-color: var(--accent-amber);
  color: var(--accent-amber);
  box-shadow: 0 0 15px rgba(245, 158, 11, 0.25);
}

.relay-tab-pin-tag {
  font-size: 0.65rem;
  padding: 2px 6px;
  border-radius: var(--radius-full);
  background: rgba(0, 0, 0, 0.4);
  color: var(--text-muted);
}

.relay-tab-btn.active .relay-tab-pin-tag {
  background: rgba(245, 158, 11, 0.3);
  color: #fff;
}

.btn-add-relay-pill {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  padding: 8px 14px;
  border: 1px dashed var(--border-subtle);
  border-radius: var(--radius-full);
  background: transparent;
  color: var(--accent-cyan);
  font-size: 0.76rem;
  font-family: var(--font-mono);
  cursor: pointer;
  white-space: nowrap;
  transition: all 0.2s ease;
}

.btn-add-relay-pill:hover {
  border-color: var(--accent-cyan);
  background: rgba(6, 182, 212, 0.1);
}

/* Direct Power Toggle Button */
.btn-power-toggle {
  background: linear-gradient(135deg, #10b981, #059669);
  border-color: rgba(16, 185, 129, 0.6);
  color: #fff !important;
  font-weight: 800;
  font-size: 0.95rem;
  box-shadow: 0 4px 18px rgba(16, 185, 129, 0.35);
  padding: 12px 20px;
}

.btn-power-toggle.is-active {
  background: linear-gradient(135deg, #ef4444, #dc2626);
  border-color: rgba(239, 68, 68, 0.6);
  box-shadow: 0 4px 18px rgba(239, 68, 68, 0.35);
}

/* Notification Bell & Drawer */
.btn-quick-log {
  position: relative;
}

.notif-badge-pill {
  position: absolute;
  top: -4px;
  right: -4px;
  background: #ef4444;
  color: #fff;
  font-size: 0.62rem;
  font-weight: 800;
  padding: 1px 5px;
  border-radius: var(--radius-full);
  border: 1px solid #07090e;
  line-height: 1.2;
}

.notifications-list {
  max-height: 360px;
  overflow-y: auto;
  display: flex;
  flex-direction: column;
  gap: 8px;
  padding-right: 4px;
}

.notif-item {
  display: flex;
  align-items: flex-start;
  gap: 12px;
  padding: 12px 14px;
  background: rgba(255, 255, 255, 0.03);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-md);
  transition: all 0.2s ease;
}

.notif-item:hover {
  background: rgba(255, 255, 255, 0.06);
}

.notif-icon {
  font-size: 1.25rem;
  line-height: 1;
}

.notif-content {
  flex: 1;
}

.notif-title {
  font-weight: 700;
  font-size: 0.85rem;
  color: var(--text-primary);
  margin-bottom: 2px;
}

.notif-desc {
  font-size: 0.75rem;
  color: var(--text-secondary);
}

.notif-time {
  font-size: 0.68rem;
  font-family: var(--font-mono);
  color: var(--text-muted);
  margin-top: 4px;
}

.notif-empty {
  text-align: center;
  padding: 30px 10px;
  color: var(--text-muted);
  font-size: 0.85rem;
}

/* Dynamic Relay Management Section in Settings */
.relay-mgmt-card {
  margin-top: 20px;
}

.relay-card-list {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
  gap: 12px;
  margin-top: 14px;
}

.relay-card-item {
  background: rgba(255, 255, 255, 0.025);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-md);
  padding: 14px 16px;
  display: flex;
  flex-direction: column;
  gap: 10px;
  transition: all 0.2s ease;
}

.relay-card-item.is-active {
  border-color: rgba(16, 185, 129, 0.5);
  background: rgba(16, 185, 129, 0.04);
}

.relay-card-head {
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.relay-card-name {
  font-weight: 700;
  font-size: 0.95rem;
  display: flex;
  align-items: center;
  gap: 6px;
}

.relay-card-grid {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 6px;
  font-size: 0.72rem;
  font-family: var(--font-mono);
}

.relay-card-row {
  display: flex;
  flex-direction: column;
}

.relay-card-label {
  color: var(--text-muted);
  font-size: 0.64rem;
}

.relay-card-val {
  color: var(--text-primary);
  font-weight: 600;
}

.relay-card-actions {
  display: flex;
  align-items: center;
  gap: 8px;
  margin-top: 4px;
}


/* -----------------------------------------------------------------------------
 * CONNECTION ACKNOWLEDGE & WIRELESS LOGS
 * -------------------------------------------------------------------------- */
.ack-pipeline-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
  gap: 14px;
  margin-top: 14px;
}

.ack-step-card {
  background: rgba(255, 255, 255, 0.03);
  border: 1px solid var(--border-glass);
  border-radius: var(--radius-sm);
  padding: 14px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  transition: all 0.2s ease;
}

.ack-step-card.is-active {
  border-color: rgba(6, 182, 212, 0.4);
  background: rgba(6, 182, 212, 0.05);
}

.ack-step-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.ack-step-num {
  font-family: var(--font-mono);
  font-size: 0.65rem;
  letter-spacing: 0.1em;
  color: var(--color-cyan);
  font-weight: 700;
}

.ack-step-title {
  font-weight: 700;
  font-size: 0.92rem;
  color: var(--text-primary);
}

.ack-step-desc {
  font-size: 0.78rem;
  color: var(--text-muted);
  font-family: var(--font-mono);
}

.ack-step-detail {
  font-size: 0.72rem;
  color: var(--color-emerald);
  font-weight: 600;
}

.ack-action-banner {
  background: rgba(6, 182, 212, 0.06);
  border: 1px solid rgba(6, 182, 212, 0.25);
  border-radius: var(--radius-sm);
  padding: 14px 18px;
  margin-top: 16px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 14px;
  flex-wrap: wrap;
}

.ack-banner-left {
  display: flex;
  align-items: center;
  gap: 12px;
}

.ack-banner-icon {
  font-size: 1.6rem;
}

.ack-banner-title {
  font-weight: 700;
  font-size: 0.92rem;
  color: var(--text-primary);
}

.ack-banner-subtitle {
  font-size: 0.78rem;
  color: var(--text-muted);
}

.system-logs-container {
  background: #060911;
  border: 1px solid var(--border-glass);
  border-radius: var(--radius-sm);
  padding: 12px 14px;
  max-height: 320px;
  overflow-y: auto;
  font-family: var(--font-mono);
  font-size: 0.75rem;
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.syslog-row {
  display: flex;
  align-items: baseline;
  gap: 10px;
  padding: 4px 0;
  border-bottom: 1px solid rgba(255, 255, 255, 0.04);
}

.syslog-uptime {
  color: var(--text-muted);
  font-size: 0.68rem;
  min-width: 55px;
}

.syslog-tag {
  display: inline-block;
  padding: 1px 6px;
  border-radius: 3px;
  font-size: 0.65rem;
  font-weight: 700;
  letter-spacing: 0.05em;
  background: rgba(255, 255, 255, 0.08);
  color: var(--text-secondary);
  min-width: 65px;
  text-align: center;
}

.syslog-tag.tag-boot {
  background: rgba(59, 130, 246, 0.2);
  color: #60a5fa;
}

.syslog-tag.tag-wifi {
  background: rgba(16, 185, 129, 0.2);
  color: #34d399;
}

.syslog-tag.tag-ack {
  background: rgba(245, 158, 11, 0.25);
  color: #fbbf24;
}

.syslog-tag.tag-cloud {
  background: rgba(6, 182, 212, 0.2);
  color: #38bdf8;
}

.syslog-msg {
  color: var(--text-primary);
  word-break: break-all;
  flex: 1;
}


/* -----------------------------------------------------------------------------
 * CONNECTION ACKNOWLEDGE & WIRELESS LOGS
 * -------------------------------------------------------------------------- */
.ack-pipeline-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
  gap: 14px;
  margin-top: 14px;
}

.ack-step-card {
  background: rgba(255, 255, 255, 0.03);
  border: 1px solid var(--border-glass);
  border-radius: var(--radius-sm);
  padding: 14px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  transition: all 0.2s ease;
}

.ack-step-card.is-active {
  border-color: rgba(6, 182, 212, 0.4);
  background: rgba(6, 182, 212, 0.05);
}

.ack-step-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.ack-step-num {
  font-family: var(--font-mono);
  font-size: 0.65rem;
  letter-spacing: 0.1em;
  color: var(--color-cyan);
  font-weight: 700;
}

.ack-step-title {
  font-weight: 700;
  font-size: 0.92rem;
  color: var(--text-primary);
}

.ack-step-desc {
  font-size: 0.78rem;
  color: var(--text-muted);
  font-family: var(--font-mono);
}

.ack-step-detail {
  font-size: 0.72rem;
  color: var(--color-emerald);
  font-weight: 600;
}

.ack-action-banner {
  background: rgba(6, 182, 212, 0.06);
  border: 1px solid rgba(6, 182, 212, 0.25);
  border-radius: var(--radius-sm);
  padding: 14px 18px;
  margin-top: 16px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 14px;
  flex-wrap: wrap;
}

.ack-banner-left {
  display: flex;
  align-items: center;
  gap: 12px;
}

.ack-banner-icon {
  font-size: 1.6rem;
}

.ack-banner-title {
  font-weight: 700;
  font-size: 0.92rem;
  color: var(--text-primary);
}

.ack-banner-subtitle {
  font-size: 0.78rem;
  color: var(--text-muted);
}

.system-logs-container {
  background: #060911;
  border: 1px solid var(--border-glass);
  border-radius: var(--radius-sm);
  padding: 12px 14px;
  max-height: 320px;
  overflow-y: auto;
  font-family: var(--font-mono);
  font-size: 0.75rem;
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.syslog-row {
  display: flex;
  align-items: baseline;
  gap: 10px;
  padding: 4px 0;
  border-bottom: 1px solid rgba(255, 255, 255, 0.04);
}

.syslog-uptime {
  color: var(--text-muted);
  font-size: 0.68rem;
  min-width: 55px;
}

.syslog-tag {
  display: inline-block;
  padding: 1px 6px;
  border-radius: 3px;
  font-size: 0.65rem;
  font-weight: 700;
  letter-spacing: 0.05em;
  background: rgba(255, 255, 255, 0.08);
  color: var(--text-secondary);
  min-width: 65px;
  text-align: center;
}

.syslog-tag.tag-boot {
  background: rgba(59, 130, 246, 0.2);
  color: #60a5fa;
}

.syslog-tag.tag-wifi {
  background: rgba(16, 185, 129, 0.2);
  color: #34d399;
}

.syslog-tag.tag-ack {
  background: rgba(245, 158, 11, 0.25);
  color: #fbbf24;
}

.syslog-tag.tag-cloud {
  background: rgba(6, 182, 212, 0.2);
  color: #38bdf8;
}

.syslog-msg {
  color: var(--text-primary);
  word-break: break-all;
  flex: 1;
}


/* -----------------------------------------------------------------------------
 * INTERACTIVE LIVE CONSOLE
 * -------------------------------------------------------------------------- */
.live-stream-indicator {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  font-family: var(--font-mono);
  font-size: 0.72rem;
  font-weight: 700;
  color: #34d399;
  background: rgba(16, 185, 129, 0.15);
  border: 1px solid rgba(16, 185, 129, 0.35);
  padding: 3px 10px;
  border-radius: var(--radius-full);
}

.pulse-dot {
  width: 7px;
  height: 7px;
  background-color: #34d399;
  border-radius: 50%;
  animation: pulseDotAnim 1.6s infinite ease-in-out;
}

@keyframes pulseDotAnim {
  0% { transform: scale(0.9); opacity: 0.5; box-shadow: 0 0 0 0 rgba(52, 211, 153, 0.7); }
  50% { transform: scale(1.1); opacity: 1; box-shadow: 0 0 0 5px rgba(52, 211, 153, 0); }
  100% { transform: scale(0.9); opacity: 0.5; box-shadow: 0 0 0 0 rgba(52, 211, 153, 0); }
}

.console-key-banner {
  background: rgba(6, 182, 212, 0.06);
  border: 1px solid rgba(6, 182, 212, 0.22);
  border-radius: var(--radius-sm);
  padding: 10px 14px;
  margin-top: 12px;
  display: flex;
  align-items: center;
  gap: 12px;
  flex-wrap: wrap;
}

.key-banner-icon { font-size: 1.4rem; }
.key-banner-content { flex: 1; font-size: 0.8rem; }
.key-banner-title { color: var(--text-primary); }
.key-banner-sub { color: var(--text-muted); font-size: 0.72rem; font-family: var(--font-mono); margin-top: 2px; }
.key-banner-sub code { color: var(--color-cyan); }

.console-toolbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 10px;
  margin-top: 14px;
  flex-wrap: wrap;
}

.console-filters {
  display: flex;
  align-items: center;
  gap: 4px;
  flex-wrap: wrap;
}

.console-filter-btn {
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid var(--border-glass);
  color: var(--text-muted);
  font-family: var(--font-mono);
  font-size: 0.7rem;
  font-weight: 700;
  padding: 4px 10px;
  border-radius: var(--radius-xs);
  cursor: pointer;
  transition: all 0.2s ease;
}

.console-filter-btn.active, .console-filter-btn:hover {
  background: rgba(6, 182, 212, 0.2);
  color: var(--color-cyan);
  border-color: rgba(6, 182, 212, 0.45);
}

.console-controls {
  display: flex;
  align-items: center;
  gap: 6px;
}

.console-quick-bar {
  display: flex;
  align-items: center;
  gap: 6px;
  margin-top: 10px;
  flex-wrap: wrap;
}

.quick-label {
  font-family: var(--font-mono);
  font-size: 0.65rem;
  color: var(--text-muted);
  font-weight: 700;
}

.quick-pill {
  background: rgba(255, 255, 255, 0.04);
  border: 1px solid rgba(255, 255, 255, 0.08);
  color: var(--text-secondary);
  font-family: var(--font-mono);
  font-size: 0.7rem;
  padding: 3px 8px;
  border-radius: 4px;
  cursor: pointer;
  transition: all 0.15s ease;
}

.quick-pill:hover {
  background: rgba(6, 182, 212, 0.15);
  color: var(--color-cyan);
  border-color: rgba(6, 182, 212, 0.35);
}

.live-terminal-window {
  background: #04060d;
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: var(--radius-sm);
  padding: 14px;
  height: 420px;
  overflow-y: auto;
  font-family: var(--font-mono);
  font-size: 0.76rem;
  line-height: 1.6;
  color: #e2e8f0;
  margin-top: 10px;
  box-shadow: inset 0 2px 10px rgba(0, 0, 0, 0.85);
}

.terminal-prompt-form {
  display: flex;
  align-items: center;
  gap: 8px;
  margin-top: 12px;
  background: #080c18;
  border: 1px solid var(--border-glass);
  border-radius: var(--radius-sm);
  padding: 6px 10px;
}

.prompt-sym {
  font-family: var(--font-mono);
  font-weight: 800;
  font-size: 0.8rem;
  color: var(--color-cyan);
  white-space: nowrap;
}

.prompt-input {
  flex: 1;
  background: transparent !important;
  border: none !important;
  outline: none !important;
  color: var(--text-primary);
  font-family: var(--font-mono);
  font-size: 0.82rem;
  padding: 4px 6px;
}

.prompt-input::placeholder {
  color: var(--text-muted);
}

.term-tag.tag-tel { background: rgba(59, 130, 246, 0.25); color: #60a5fa; }
.term-tag.tag-hw { background: rgba(168, 85, 247, 0.25); color: #c084fc; }
.term-tag.tag-in { background: rgba(245, 158, 11, 0.25); color: #fbbf24; }
.term-tag.tag-out { background: rgba(16, 185, 129, 0.25); color: #34d399; }
.term-tag.tag-ack { background: rgba(234, 179, 8, 0.3); color: #fde047; }
.term-tag.tag-cloud { background: rgba(249, 115, 22, 0.25); color: #fb923c; }

/* ========================================================================= */
/* CENTER RADIAL COUNTDOWN GAUGE & BULB ANIMATIONS                          */
/* ========================================================================= */
.countdown-radial-container {
  position: relative;
  display: flex;
  align-items: center;
  justify-content: center;
  margin: 15px auto;
  width: 280px;
  max-width: 90vw;
  height: 280px;
}

.radial-svg {
  width: 100%;
  height: 100%;
  transform: rotate(-90deg);
  overflow: visible;
}

.radial-bg {
  fill: none;
  stroke: rgba(255, 255, 255, 0.05);
  stroke-width: 14;
}

.radial-progress {
  fill: none;
  stroke: url(#cyanGlow);
  stroke-width: 14;
  stroke-linecap: round;
  stroke-dasharray: 691.15;
  stroke-dashoffset: 0;
  transition: stroke-dashoffset 0.8s ease, stroke 0.5s ease;
  filter: drop-shadow(0 0 12px rgba(245, 158, 11, 0.5));
}

.radial-center-content {
  position: absolute;
  inset: 0;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  text-align: center;
  padding: 16px;
  pointer-events: none;
}

.digital-clock {
  font-family: var(--font-mono);
  font-size: 1.05rem;
  font-weight: 700;
  letter-spacing: 0.01em;
  color: #fff;
  font-variant-numeric: tabular-nums;
  background: rgba(4, 8, 18, 0.75);
  border: 1px solid rgba(255, 255, 255, 0.12);
  border-radius: 8px;
  padding: 4px 8px;
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 2px;
  white-space: nowrap;
  box-shadow: 0 4px 16px rgba(0, 0, 0, 0.45);
  margin-top: 4px;
}

.bulb-charging-active {
  background: rgba(245, 158, 11, 0.25) !important;
  color: #fbbf24 !important;
  box-shadow: 0 0 30px rgba(251, 191, 36, 0.6) !important;
  animation: chargingBulbPulse 1.8s infinite ease-in-out;
}

.bulb-cooldown {
  background: rgba(59, 130, 246, 0.15) !important;
  color: #60a5fa !important;
  box-shadow: 0 0 20px rgba(59, 130, 246, 0.3) !important;
  animation: cooldownBreathe 3s infinite ease-in-out;
}

.bulb-ready {
  background: rgba(16, 185, 129, 0.2) !important;
  color: #34d399 !important;
  box-shadow: 0 0 25px rgba(52, 211, 153, 0.5) !important;
  animation: readyPulse 2s infinite ease-in-out;
}

@keyframes chargingBulbPulse {
  0%, 100% { transform: scale(1); filter: drop-shadow(0 0 8px rgba(245, 158, 11, 0.4)); }
  50% { transform: scale(1.08); filter: drop-shadow(0 0 24px rgba(245, 158, 11, 0.9)); }
}

@keyframes cooldownBreathe {
  0%, 100% { transform: scale(1); opacity: 0.85; }
  50% { transform: scale(1.05); opacity: 1; }
}

@keyframes readyPulse {
  0%, 100% { transform: scale(1); box-shadow: 0 0 15px rgba(52, 211, 153, 0.4); }
  50% { transform: scale(1.06); box-shadow: 0 0 30px rgba(52, 211, 153, 0.85); }
}

.btn-ready-pulse {
  animation: btnGlowPulse 2.2s infinite ease-in-out;
  border-color: rgba(52, 211, 153, 0.6) !important;
  color: #34d399 !important;
}

@keyframes btnGlowPulse {
  0%, 100% { box-shadow: 0 0 8px rgba(52, 211, 153, 0.3); }
  50% { box-shadow: 0 0 22px rgba(52, 211, 153, 0.7); }
}

.btn-locked {
  opacity: 0.55 !important;
  cursor: not-allowed !important;
  filter: grayscale(0.6);
}



/* -----------------------------------------------------------------------------
 * INTERACTIVE LIVE CONSOLE
 * -------------------------------------------------------------------------- */
.live-stream-indicator {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  font-family: var(--font-mono);
  font-size: 0.72rem;
  font-weight: 700;
  color: #34d399;
  background: rgba(16, 185, 129, 0.15);
  border: 1px solid rgba(16, 185, 129, 0.35);
  padding: 3px 10px;
  border-radius: var(--radius-full);
}

.pulse-dot {
  width: 7px;
  height: 7px;
  background-color: #34d399;
  border-radius: 50%;
  animation: pulseDotAnim 1.6s infinite ease-in-out;
}

@keyframes pulseDotAnim {
  0% { transform: scale(0.9); opacity: 0.5; box-shadow: 0 0 0 0 rgba(52, 211, 153, 0.7); }
  50% { transform: scale(1.1); opacity: 1; box-shadow: 0 0 0 5px rgba(52, 211, 153, 0); }
  100% { transform: scale(0.9); opacity: 0.5; box-shadow: 0 0 0 0 rgba(52, 211, 153, 0); }
}

.console-key-banner {
  background: rgba(6, 182, 212, 0.06);
  border: 1px solid rgba(6, 182, 212, 0.22);
  border-radius: var(--radius-sm);
  padding: 10px 14px;
  margin-top: 12px;
  display: flex;
  align-items: center;
  gap: 12px;
  flex-wrap: wrap;
}

.key-banner-icon { font-size: 1.4rem; }
.key-banner-content { flex: 1; font-size: 0.8rem; }
.key-banner-title { color: var(--text-primary); }
.key-banner-sub { color: var(--text-muted); font-size: 0.72rem; font-family: var(--font-mono); margin-top: 2px; }
.key-banner-sub code { color: var(--color-cyan); }

.console-toolbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 10px;
  margin-top: 14px;
  flex-wrap: wrap;
}

.console-filters {
  display: flex;
  align-items: center;
  gap: 4px;
  flex-wrap: wrap;
}

.console-filter-btn {
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid var(--border-glass);
  color: var(--text-muted);
  font-family: var(--font-mono);
  font-size: 0.7rem;
  font-weight: 700;
  padding: 4px 10px;
  border-radius: var(--radius-xs);
  cursor: pointer;
  transition: all 0.2s ease;
}

.console-filter-btn.active, .console-filter-btn:hover {
  background: rgba(6, 182, 212, 0.2);
  color: var(--color-cyan);
  border-color: rgba(6, 182, 212, 0.45);
}

.console-controls {
  display: flex;
  align-items: center;
  gap: 6px;
}

.console-quick-bar {
  display: flex;
  align-items: center;
  gap: 6px;
  margin-top: 10px;
  flex-wrap: wrap;
}

.quick-label {
  font-family: var(--font-mono);
  font-size: 0.65rem;
  color: var(--text-muted);
  font-weight: 700;
}

.quick-pill {
  background: rgba(255, 255, 255, 0.04);
  border: 1px solid rgba(255, 255, 255, 0.08);
  color: var(--text-secondary);
  font-family: var(--font-mono);
  font-size: 0.7rem;
  padding: 3px 8px;
  border-radius: 4px;
  cursor: pointer;
  transition: all 0.15s ease;
}

.quick-pill:hover {
  background: rgba(6, 182, 212, 0.15);
  color: var(--color-cyan);
  border-color: rgba(6, 182, 212, 0.35);
}

.live-terminal-window {
  background: #04060d;
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: var(--radius-sm);
  padding: 14px;
  height: 420px;
  overflow-y: auto;
  font-family: var(--font-mono);
  font-size: 0.76rem;
  line-height: 1.6;
  color: #e2e8f0;
  margin-top: 10px;
  box-shadow: inset 0 2px 10px rgba(0, 0, 0, 0.85);
}

.terminal-prompt-form {
  display: flex;
  align-items: center;
  gap: 8px;
  margin-top: 12px;
  background: #080c18;
  border: 1px solid var(--border-glass);
  border-radius: var(--radius-sm);
  padding: 6px 10px;
}

.prompt-sym {
  font-family: var(--font-mono);
  font-weight: 800;
  font-size: 0.8rem;
  color: var(--color-cyan);
  white-space: nowrap;
}

.prompt-input {
  flex: 1;
  background: transparent !important;
  border: none !important;
  outline: none !important;
  color: var(--text-primary);
  font-family: var(--font-mono);
  font-size: 0.82rem;
  padding: 4px 6px;
}

.prompt-input::placeholder {
  color: var(--text-muted);
}

.term-tag.tag-tel { background: rgba(59, 130, 246, 0.25); color: #60a5fa; }
.term-tag.tag-hw { background: rgba(168, 85, 247, 0.25); color: #c084fc; }
.term-tag.tag-in { background: rgba(245, 158, 11, 0.25); color: #fbbf24; }
.term-tag.tag-out { background: rgba(16, 185, 129, 0.25); color: #34d399; }
.term-tag.tag-ack { background: rgba(234, 179, 8, 0.3); color: #fde047; }
.term-tag.tag-cloud { background: rgba(249, 115, 22, 0.25); color: #fb923c; }

/* ========================================================================= */
/* CENTER RADIAL COUNTDOWN GAUGE & BULB ANIMATIONS                          */
/* ========================================================================= */
.countdown-radial-container {
  position: relative;
  display: flex;
  align-items: center;
  justify-content: center;
  margin: 15px auto;
  width: 280px;
  max-width: 90vw;
  height: 280px;
}

.radial-svg {
  width: 100%;
  height: 100%;
  transform: rotate(-90deg);
  overflow: visible;
}

.radial-bg {
  fill: none;
  stroke: rgba(255, 255, 255, 0.05);
  stroke-width: 14;
}

.radial-progress {
  fill: none;
  stroke: url(#cyanGlow);
  stroke-width: 14;
  stroke-linecap: round;
  stroke-dasharray: 691.15;
  stroke-dashoffset: 0;
  transition: stroke-dashoffset 0.8s ease, stroke 0.5s ease;
  filter: drop-shadow(0 0 12px rgba(245, 158, 11, 0.5));
}

.radial-center-content {
  position: absolute;
  inset: 0;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  text-align: center;
  padding: 16px;
  pointer-events: none;
}

.digital-clock {
  font-family: var(--font-mono);
  font-size: 1.05rem;
  font-weight: 700;
  letter-spacing: 0.01em;
  color: #fff;
  font-variant-numeric: tabular-nums;
  background: rgba(4, 8, 18, 0.75);
  border: 1px solid rgba(255, 255, 255, 0.12);
  border-radius: 8px;
  padding: 4px 8px;
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 2px;
  white-space: nowrap;
  box-shadow: 0 4px 16px rgba(0, 0, 0, 0.45);
  margin-top: 4px;
}

.bulb-charging-active {
  background: rgba(245, 158, 11, 0.25) !important;
  color: #fbbf24 !important;
  box-shadow: 0 0 30px rgba(251, 191, 36, 0.6) !important;
  animation: chargingBulbPulse 1.8s infinite ease-in-out;
}

.bulb-cooldown {
  background: rgba(59, 130, 246, 0.15) !important;
  color: #60a5fa !important;
  box-shadow: 0 0 20px rgba(59, 130, 246, 0.3) !important;
  animation: cooldownBreathe 3s infinite ease-in-out;
}

.bulb-ready {
  background: rgba(16, 185, 129, 0.2) !important;
  color: #34d399 !important;
  box-shadow: 0 0 25px rgba(52, 211, 153, 0.5) !important;
  animation: readyPulse 2s infinite ease-in-out;
}

@keyframes chargingBulbPulse {
  0%, 100% { transform: scale(1); filter: drop-shadow(0 0 8px rgba(245, 158, 11, 0.4)); }
  50% { transform: scale(1.08); filter: drop-shadow(0 0 24px rgba(245, 158, 11, 0.9)); }
}

@keyframes cooldownBreathe {
  0%, 100% { transform: scale(1); opacity: 0.85; }
  50% { transform: scale(1.05); opacity: 1; }
}

@keyframes readyPulse {
  0%, 100% { transform: scale(1); box-shadow: 0 0 15px rgba(52, 211, 153, 0.4); }
  50% { transform: scale(1.06); box-shadow: 0 0 30px rgba(52, 211, 153, 0.85); }
}

.btn-ready-pulse {
  animation: btnGlowPulse 2.2s infinite ease-in-out;
  border-color: rgba(52, 211, 153, 0.6) !important;
  color: #34d399 !important;
}

@keyframes btnGlowPulse {
  0%, 100% { box-shadow: 0 0 8px rgba(52, 211, 153, 0.3); }
  50% { box-shadow: 0 0 22px rgba(52, 211, 153, 0.7); }
}

.btn-locked {
  opacity: 0.55 !important;
  cursor: not-allowed !important;
  filter: grayscale(0.6);
}


</style>
</head>

<body>
  <!-- Ambient background glow elements -->
  <div class="ambient-glow glow-1"></div>
  <div class="ambient-glow glow-2"></div>
  <div class="ambient-glow glow-3"></div>

  <!-- ========================================================================= -->
  <!-- 1. FULL-SCREEN LOGIN & SIGN-IN PORTAL (First Screen for Authentication)    -->
  <!-- ========================================================================= -->
  <div id="loginSection" class="auth-wrapper">
    <div class="auth-card glass-card">

      <!-- Brand & Header -->
      <div class="auth-brand">
        <div class="auth-logo-badge">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <path d="M13 2L3 14h9l-1 8 10-12h-9l1-8z" stroke-linecap="round" stroke-linejoin="round" />
          </svg>
        </div>
        <h1 class="auth-title">CYBER-RELAY <span class="badge-rtos">RTOS v3.0</span></h1>
        <p class="auth-subtitle">Secure IoT Relay Management &amp; Worldwide Cloud Portal</p>
      </div>

      <!-- Network Mode Quick Switcher -->
      <div class="auth-conn-bar">
        <span class="auth-bar-label">Target Network:</span>
        <div class="conn-mode-switcher compact">
          <button id="authBtnLocal" class="mode-btn active" type="button" onclick="App.setConnectionMode('local')">Local
            AP</button>
          <button id="authBtnCloud" class="mode-btn" type="button" onclick="App.setConnectionMode('cloud')">Firebase
            Cloud</button>
        </div>
      </div>

      <!-- Tabs: Sign In / Register -->
      <div class="auth-tabs">
        <button id="tabBtnSignIn" class="auth-tab-btn active" type="button" onclick="App.switchAuthTab('signin')">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <path d="M15 3h4a2 2 0 0 1 2 2v14a2 2 0 0 1-2 2h-4M10 17l5-5-5-5M15 12H3" />
          </svg>
          <span>Sign In</span>
        </button>
        <button id="tabBtnRegister" class="auth-tab-btn" type="button" onclick="App.switchAuthTab('register')">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <path d="M16 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2" />
            <circle cx="8.5" cy="7" r="4" />
            <line x1="20" y1="8" x2="20" y2="14" />
            <line x1="23" y1="11" x2="17" y2="11" />
          </svg>
          <span>Register New</span>
        </button>
      </div>

      <!-- TAB 1: SIGN IN FORM -->
      <div id="tabContentSignIn" class="auth-tab-content active">
        <!-- Role Login Selectors (No auto password filling) -->
        <div class="role-select-section">
          <span class="role-select-label">Choose Account to Sign In:</span>
          <div class="role-select-grid">
            <button type="button" id="btnSelectAdmin" class="role-select-btn btn-admin selected"
              onclick="App.selectRoleLogin('admin')">
              <span class="btn-icon">🛡️</span>
              <span class="btn-title">Admin Login</span>
              <span class="btn-user">Yash_Gajjar</span>
            </button>
            <button type="button" id="btnSelectUser" class="role-select-btn btn-user"
              onclick="App.selectRoleLogin('user')">
              <span class="btn-icon">⚡</span>
              <span class="btn-title">User Login</span>
              <span class="btn-user">Mahi_Gajjar</span>
            </button>
            <button type="button" id="btnSelectGuest" class="role-select-btn btn-guest"
              onclick="App.selectRoleLogin('guest')">
              <span class="btn-icon">👁️</span>
              <span class="btn-title">Guest Login</span>
              <span class="btn-user">Analytics Only</span>
            </button>
          </div>
        </div>

        <form id="signInForm" onsubmit="event.preventDefault(); App.handleSignInSubmit();">
          <div class="form-group">
            <label for="inputUsername">Username</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2" />
                <circle cx="12" cy="7" r="4" />
              </svg>
              <input type="text" id="inputUsername" value="Yash_Gajjar" placeholder="e.g. Yash_Gajjar" required
                autocomplete="username">
            </div>
          </div>

          <div class="form-group">
            <label for="inputPassword">Password</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <rect x="3" y="11" width="18" height="11" rx="2" ry="2" />
                <path d="M7 11V7a5 5 0 0 1 10 0v4" />
              </svg>
              <input type="password" id="inputPassword" placeholder="Enter password to sign in" required
                autocomplete="current-password">
              <button type="button" class="btn-toggle-eye" onclick="App.togglePasswordVisibility('inputPassword')"
                title="Show/Hide Password">👁️</button>
            </div>
          </div>

          <!-- Permanent Session Checkbox (Forever on Phone) & Forgot Password -->
          <div class="form-extra-row">
            <label class="custom-checkbox-label">
              <input type="checkbox" id="checkRememberSession" checked>
              <span class="checkbox-custom"></span>
              <span class="checkbox-text">
                <strong>Remember session forever</strong>
                <span class="checkbox-subtext">Auto-login on app launch</span>
              </span>
            </label>
            <button type="button" class="btn-link-forgot" onclick="App.openForgotPasswordModal()">Forgot
              Password?</button>
          </div>

          <button type="submit" id="btnSubmitSignIn" class="glass-button btn-accent w-full auth-submit-btn">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <polyline points="20 6 9 17 4 12" />
            </svg>
            <span>SIGN IN &amp; CONNECT</span>
          </button>
        </form>
      </div>

      <!-- TAB 2: REGISTER OPERATOR FORM -->
      <div id="tabContentRegister" class="auth-tab-content">
        <form id="registerForm" onsubmit="event.preventDefault(); App.handleRegisterSubmit();">
          <div class="form-group">
            <label for="regUsername">Desired Username</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2" />
                <circle cx="12" cy="7" r="4" />
              </svg>
              <input type="text" id="regUsername" placeholder="e.g. Operator_1" required autocomplete="off">
            </div>
          </div>

          <div class="form-group">
            <label for="regPassword">Create Password</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <rect x="3" y="11" width="18" height="11" rx="2" ry="2" />
                <path d="M7 11V7a5 5 0 0 1 10 0v4" />
              </svg>
              <input type="password" id="regPassword" placeholder="Min. 6 characters" required
                autocomplete="new-password">
              <button type="button" class="btn-toggle-eye"
                onclick="App.togglePasswordVisibility('regPassword')">👁️</button>
            </div>
          </div>

          <div class="form-group">
            <label for="regRole">Privilege Tier</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <polygon
                  points="12 2 15.09 8.26 22 9.27 17 14.14 18.18 21.02 12 17.77 5.82 21.02 7 14.14 2 9.27 8.91 8.26 12 2" />
              </svg>
              <select id="regRole" class="custom-select">
                <option value="admin">Level 1: Super Administrator (Full Control &amp; Bypasses)</option>
                <option value="guarded_operator" selected>Level 2: Guarded Operator (1-Time Session Grants)</option>
                <option value="guest_analytics">Level 3: Guest Analytics (Read-Only Live Analytics)</option>
              </select>
            </div>
          </div>

          <div class="form-checkbox-row">
            <label class="custom-checkbox-label">
              <input type="checkbox" id="checkRegRememberSession" checked>
              <span class="checkbox-custom"></span>
              <span class="checkbox-text">
                <strong>Remember session forever on this device</strong>
              </span>
            </label>
          </div>

          <button type="submit" id="btnSubmitRegister" class="glass-button btn-primary w-full auth-submit-btn">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M16 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2" />
              <circle cx="8.5" cy="7" r="4" />
              <polyline points="17 11 19 13 23 9" />
            </svg>
            <span>ENROLL &amp; ENTER SYSTEM</span>
          </button>
        </form>
      </div>

      <!-- App Info Footer inside Auth Card -->
      <div class="auth-footer-meta">
        <span>Protected by Hardware RTOS &amp; AES Session Engine</span>
        <span class="security-badge">● ENCRYPTED</span>
      </div>
    </div>
  </div>

  <!-- ========================================================================= -->
  <!-- 2. MAIN CYBER-RELAY RTOS DASHBOARD (Shown when Authenticated)             -->
  <!-- ========================================================================= -->
  <!-- ========================================================================= -->
  <!-- 2. MAIN CYBER-RELAY RTOS APP SHELL (SIDEBAR + MULTI-VIEW ARCHITECTURE)    -->
  <!-- ========================================================================= -->
  <div id="dashboardSection" class="app-shell hidden" style="display: none;">

    <!-- Mobile Sidebar Backdrop Overlay -->
    <div class="sidebar-overlay" id="sidebarOverlay" onclick="App.closeSidebar()"></div>

    <!-- Apple Glassy Prism Sidebar Navigation -->
    <aside class="app-sidebar" id="appSidebar">
      <div class="sidebar-header">
        <div class="sidebar-brand">
          <div class="brand-icon">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M13 2L3 14h9l-1 8 10-12h-9l1-8z" stroke-linecap="round" stroke-linejoin="round" />
            </svg>
          </div>
          <div>
            <h2>CYBER-RELAY</h2>
            <span class="brand-tag">RTOS v3.0 PRISM</span>
          </div>
        </div>
        <button class="btn-close-sidebar" type="button" onclick="App.closeSidebar()"
          aria-label="Close Sidebar">✕</button>
      </div>

      <!-- Navigation Links -->
      <nav class="sidebar-nav">
        <span class="nav-section-title">CONTROL &amp; TIMERS</span>

        <button type="button" class="nav-item active" id="navControl" onclick="App.switchView('control')">
          <div class="nav-item-left">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M13 2L3 14h9l-1 8 10-12h-9l1-8z" />
            </svg>
            <span>Relay Control</span>
          </div>
          <span class="nav-badge" id="navRelayBadge">READY</span>
        </button>

        <button type="button" class="nav-item" id="navTimers" onclick="App.switchView('timers')">
          <div class="nav-item-left">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <circle cx="12" cy="12" r="10" />
              <polyline points="12 6 12 12 16 14" />
            </svg>
            <span>Timer Calibrations</span>
          </div>
          <span class="nav-badge" id="navTimersBadge">8h / 6d</span>
        </button>

        <span class="nav-section-title">INSIGHTS &amp; SECURITY</span>

        <button type="button" class="nav-item" id="navAnalytics" onclick="App.switchView('analytics')">
          <div class="nav-item-left">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M18 20V10M12 20V4M6 20v-6" />
            </svg>
            <span>Monthly Analytics</span>
          </div>
          <span class="nav-badge" id="navDutyBadge">16.8%</span>
        </button>

        <button type="button" class="nav-item" id="navUsers" onclick="App.switchView('users')">
          <div class="nav-item-left">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2" />
              <circle cx="9" cy="7" r="4" />
              <path d="M23 21v-2a4 4 0 0 0-3-3.87" />
              <path d="M16 3.13a4 4 0 0 1 0 7.75" />
            </svg>
            <span>User Accounts</span>
          </div>
          <span class="nav-badge" id="navUsersBadge">3 Active</span>
        </button>

        <span class="nav-section-title">SYSTEM &amp; CLOUD</span>

        <button type="button" class="nav-item" id="navCloud" onclick="App.switchView('cloud')">
          <div class="nav-item-left">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M18 10h-1.26A8 8 0 1 0 9 20h9a5 5 0 0 0 0-10z" />
            </svg>
            <span>Firebase Cloud &amp; Logs</span>
          </div>
          <span class="nav-badge" id="navCloudBadge">Online</span>
        </button>

        <button type="button" class="nav-item" id="navConsole" onclick="App.switchView('console')">
          <div class="nav-item-left">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <polyline points="4 17 10 11 4 5" />
              <line x1="12" y1="19" x2="20" y2="19" />
            </svg>
            <span>Live Console</span>
          </div>
          <span class="nav-badge" id="navConsoleBadge" style="background: rgba(16, 185, 129, 0.2); color: #34d399;">LIVE</span>
        </button>

        <button type="button" class="nav-item" id="navAcknowledge" onclick="App.switchView('acknowledge')">
          <div class="nav-item-left">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M12 22s8-4 8-10V5l-8-3-8 3v7c0 6 8 10 8 10z"/>
              <path d="m9 12 2 2 4-4"/>
            </svg>
            <span>Connection ACK &amp; Logs</span>
          </div>
          <span class="nav-badge" id="navAckBadge">READY</span>
        </button>

        <button type="button" class="nav-item" id="navNotifications" onclick="App.openNotificationsModal()">
          <div class="nav-item-left">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M18 8A6 6 0 0 0 6 8c0 7-3 9-3 9h18s-3-2-3-9" />
              <path d="M13.73 21a2 2 0 0 1-3.46 0" />
            </svg>
            <span>Alerts &amp; Notifications</span>
          </div>
          <span class="nav-badge" id="navNotifBadge">0 New</span>
        </button>

        <button type="button" class="nav-item" id="navSettings" onclick="App.switchView('settings')">
          <div class="nav-item-left">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <circle cx="12" cy="12" r="3" />
              <path
                d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z" />
            </svg>
            <span>Device Diagnostics</span>
          </div>
          <span class="nav-badge" id="navPingBadge">-- ms</span>
        </button>
      </nav>

      <!-- Sidebar User Profile Footer -->
      <div class="sidebar-footer">
        <div class="sidebar-user-box">
          <div class="sidebar-user-info">
            <div class="avatar-ring" id="sidebarAvatarRing">
              <span id="sidebarUserInitial">Y</span>
            </div>
            <div class="user-info-text">
              <span id="sidebarUserName">Yash_Gajjar</span>
              <span id="sidebarUserRole" class="role-tag">SUPER ADMIN</span>
            </div>
          </div>
          <button id="sidebarLogoutBtn" class="logout-btn-styled" title="Log Out / Switch Account" type="button"
            onclick="App.logout()">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4M16 17l5-5-5-5M21 12H9" />
            </svg>
            <span class="logout-text">Exit</span>
          </button>
        </div>
      </div>
    </aside>

    <!-- Main Content Canvas -->
    <div class="app-main-content">

      <!-- Top Dynamic Header Bar with Real Diagnostics & Mode Switcher -->
      <header class="main-top-bar glass-card">
        <div class="top-bar-left">
          <button type="button" class="btn-toggle-sidebar" id="btnToggleSidebar" onclick="App.openSidebar()"
            aria-label="Toggle Navigation">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <line x1="3" y1="12" x2="21" y2="12" />
              <line x1="3" y1="6" x2="21" y2="6" />
              <line x1="3" y1="18" x2="21" y2="18" />
            </svg>
          </button>
          <div class="view-heading-group">
            <h1 id="headerViewTitle">Relay Control</h1>
            <span class="view-subheading hidden-mobile" id="headerViewSubtitle">Channel 1</span>
          </div>
        </div>

        <div class="top-bar-right">
          <!-- Connection Mode Switcher (Local AP vs Firebase Cloud) -->
          <div class="conn-mode-switcher" title="Toggle Connection Mode">
            <button id="btnModeLocal" class="mode-btn active" type="button"
              onclick="App.setConnectionMode('local')">AP</button>
            <button id="btnModeCloud" class="mode-btn" type="button"
              onclick="App.setConnectionMode('cloud')">Cloud</button>
          </div>

          <!-- Real Telemetry Diagnostic Group (Real Ping & RSSI) -->
          <div class="telemetry-beacon-group">
            <div class="diag-pill ping-good" id="headerPingPill" title="Actual Round-Trip Ping">
              <span>⚡</span>
              <span id="headerPingVal">-- ms</span>
            </div>
            <div class="diag-pill rssi-good" id="headerRssiPill" title="Wi-Fi RSSI Signal Strength"
              style="display: none;">
              <span>📶</span>
              <span id="headerRssiVal">-- dBm</span>
            </div>
          </div>

          <!-- Cloud Log Launcher Button -->
          <button id="btnOpenCloudLog" class="btn-quick-log" type="button" title="Firebase Logs"
            onclick="App.switchView('cloud')">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M18 10h-1.26A8 8 0 1 0 9 20h9a5 5 0 0 0 0-10z" />
            </svg>
            <span class="btn-log-text">Logs</span>
          </button>

          <!-- Notifications Bell Button -->
          <button id="btnTopNotifications" class="btn-quick-log" type="button" title="Switch Alerts &amp; Notifications"
            onclick="App.openNotificationsModal()">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M18 8A6 6 0 0 0 6 8c0 7-3 9-3 9h18s-3-2-3-9" />
              <path d="M13.73 21a2 2 0 0 1-3.46 0" />
            </svg>
            <span class="notif-badge-pill" id="topNotifBadge" style="display:none;">0</span>
          </button>

          <!-- Status Pill -->
          <div id="connStatusPill" class="status-pill online" title="ESP Status">
            <span class="pulsing-dot"></span>
            <span id="connStatusText">ONLINE</span>
          </div>
        </div>
      </header>

      <!-- Connection & Role Banners -->
      <div id="connNoticeBanner" class="conn-help-banner hidden">
        <div class="conn-banner-left">
          <span class="pulsing-warn-dot"></span>
          <span id="connNoticeText">Connecting to ESP8266 Local AP (192.168.4.1)...</span>
        </div>
        <div class="conn-banner-actions">
          <button type="button" class="btn-banner-action" onclick="App.setConnectionMode('cloud')">Switch to Firebase
            Cloud</button>
          <button type="button" class="btn-banner-dismiss" onclick="App.dismissConnNotice()">✕</button>
        </div>
      </div>

      <div id="guestNoticeBanner" class="role-banner guest-banner hidden">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <circle cx="12" cy="12" r="10" />
          <line x1="12" y1="16" x2="12" y2="12" />
          <line x1="12" y1="8" x2="12.01" y2="8" />
        </svg>
        <span><strong>Guest Mode Active:</strong> You have read-only access to view live countdowns and Monthly Switch
          Analytics. Hardware controls and calibrations are disabled.</span>
      </div>

      <div id="guardedNoticeBanner" class="role-banner guarded-banner hidden">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <polygon points="7.86 2 16.14 2 22 7.86 22 16.14 16.14 22 7.86 22 2 16.14 2 7.86 7.86 2" />
          <line x1="12" y1="8" x2="12" y2="12" />
          <line x1="12" y1="16" x2="12.01" y2="16" />
        </svg>
        <span><strong>Guarded Operator (Mahi_Gajjar):</strong> Any trigger or system write requires instant 1-time
          approval from Yash_Gajjar (Admin). Session destroys after 1 use.</span>
      </div>

      <!-- VIEWS CONTAINER -->
      <main class="views-container">

        <!-- =================================================================== -->
        <!-- VIEW 1: RELAY CONTROL & EXTEND TIMER                                 -->
        <!-- =================================================================== -->
        <section id="viewControl" class="view-page active">
          <!-- Multi-Relay Channel Selector Navigation Tabs -->
          <div class="relay-channels-nav" id="relayTabsContainer">
            <div class="relay-tabs-scroll" id="relayTabsList">
              <!-- Dynamically populated relay channels -->
            </div>
          </div>

          <div class="grid-card glass-card state-hero-card">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow" id="relayChannelBadge">CHANNEL 1 // PIN D1 (GPIO5)</span>
                <h2 id="relayCardTitle">Relay 1 Operational State</h2>
              </div>
              <div id="sysStateBadge" class="state-badge badge-idle">
                <span class="state-icon">●</span>
                <span id="sysStateText">IDLE / READY</span>
              </div>
            </div>

            <!-- Radial Countdown Visualizer -->
            <div class="countdown-radial-container">
              <svg class="radial-svg" viewBox="0 0 260 260">
                <defs>
                  <linearGradient id="cyanGlow" x1="0%" y1="0%" x2="100%" y2="100%">
                    <stop offset="0%" stop-color="#fbbf24" />
                    <stop offset="100%" stop-color="#f59e0b" />
                  </linearGradient>
                  <linearGradient id="amberGlow" x1="0%" y1="0%" x2="100%" y2="100%">
                    <stop offset="0%" stop-color="#f59e0b" />
                    <stop offset="100%" stop-color="#d97706" />
                  </linearGradient>
                  <linearGradient id="emeraldGlow" x1="0%" y1="0%" x2="100%" y2="100%">
                    <stop offset="0%" stop-color="#fbbf24" />
                    <stop offset="100%" stop-color="#d97706" />
                  </linearGradient>
                </defs>
                <circle class="radial-bg" cx="130" cy="130" r="110"></circle>
                <circle id="radialProgress" class="radial-progress" cx="130" cy="130" r="110"></circle>
              </svg>

              <!-- Inside Radial Gauge -->
              <div class="radial-center-content">
                <div class="relay-bulb-indicator" id="relayBulb">
                  <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <path
                      d="M12 2v4M12 18v4M4.93 4.93l2.83 2.83M16.24 16.24l2.83 2.83M2 12h4M18 12h4M4.93 19.07l2.83-2.83M16.24 7.76l2.83-2.83" />
                  </svg>
                </div>
                <div class="countdown-label" id="countdownLabel">READY FOR TOUCH</div>
                <div class="digital-clock" id="digitalClock">
                  <span id="clockDays">00</span><span class="unit">d</span> :
                  <span id="clockHours">00</span><span class="unit">h</span> :
                  <span id="clockMins">00</span><span class="unit">m</span> :
                  <span id="clockSecs">00</span><span class="unit">s</span>
                </div>
                <div class="sub-status" id="countdownSubtext">Tap touch sensor to engage 8.0h duty cycle</div>
              </div>
            </div>

            <!-- Quick Hardware Status Tiles -->
            <div class="hardware-tiles-row">
              <div class="hw-tile">
                <span class="hw-label">RELAY PIN</span>
                <div class="hw-val-group">
                  <span id="relayStatusDot" class="status-dot dot-off"></span>
                  <strong id="relayStatusText">OFF</strong>
                </div>
                <span class="hw-sub" id="relayPinInfo">GPIO5 (D1) / GPIO1 (TX)</span>
              </div>

              <div class="hw-tile">
                <span class="hw-label">TOUCH SENSOR</span>
                <div class="hw-val-group">
                  <span id="touchStatusDot" class="status-dot dot-active"></span>
                  <strong id="touchStatusText">READY</strong>
                </div>
                <span class="hw-sub" id="touchPinInfo">TTP223 Active</span>
              </div>

              <div class="hw-tile">
                <span class="hw-label">LOCKOUT FILTER</span>
                <div class="hw-val-group">
                  <span id="lockoutStatusDot" class="status-dot dot-disabled"></span>
                  <strong id="lockoutStatusText">UNLOCKED</strong>
                </div>
                <span class="hw-sub" id="lockoutDesc">Triggers allowed</span>
              </div>
            </div>

            <!-- Action Controls -->
            <div class="action-buttons-group" id="controlsGroup">
              <!-- Direct ON/OFF Master Toggle Button -->
              <button id="btnDirectToggle" class="glass-button btn-power-toggle" type="button"
                onclick="App.handleToggleClick()">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <path d="M18.36 6.64a9 9 0 1 1-12.73 0" />
                  <line x1="12" y1="2" x2="12" y2="12" />
                </svg>
                <span id="directToggleText">Switch Relay ON</span>
              </button>

              <button id="btnManualTrigger" class="glass-button btn-accent" type="button"
                onclick="App.handleTriggerClick()">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <polygon points="5 3 19 12 5 21 5 3" />
                </svg>
                <span id="triggerBtnText">Trigger Relay Cycle Now</span>
              </button>
              <button id="btnCancelCycle" class="glass-button btn-danger" type="button" onclick="App.cancelCycle()"
                disabled>
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <rect x="3" y="3" width="18" height="18" rx="2" ry="2" />
                </svg>
                <span>Emergency Cutoff</span>
              </button>
              <button id="btnResetLockout" class="glass-button btn-warning" type="button" onclick="App.resetLockout()"
                disabled title="Admin Only">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <path d="M3 12a9 9 0 1 0 9-9 9.75 9.75 0 0 0-6.74 2.74L3 8" />
                  <path d="M3 3v5h5" />
                </svg>
                <span>Bypass Lockout (Admin Only)</span>
              </button>
            </div>

            <!-- ADMIN FEATURE: EXTEND ACTIVE RUNNING RELAY TIMER -->
            <div id="adminExtendPanel" class="extend-timer-panel">
              <div class="extend-header">
                <span class="extend-title">
                  <svg viewBox="0 0 24 24" width="16" height="16" fill="none" stroke="currentColor" stroke-width="2">
                    <circle cx="12" cy="12" r="10" />
                    <polyline points="12 6 12 12 14 10" />
                  </svg>
                  <span>Admin Extend Active Relay Timer</span>
                </span>
                <span class="badge-single-use">Live Sync</span>
              </div>
              <p class="card-subtext" style="margin-bottom: 10px;">Add extra running time to the active relay without
                restarting the cycle.</p>
              <div class="extend-chips-grid">
                <button type="button" class="btn-extend-chip" onclick="App.extendActiveTimer(15)">+15 min</button>
                <button type="button" class="btn-extend-chip" onclick="App.extendActiveTimer(30)">+30 min</button>
                <button type="button" class="btn-extend-chip" onclick="App.extendActiveTimer(60)">+1 hour</button>
                <button type="button" class="btn-extend-chip" onclick="App.extendActiveTimer(120)">+2 hours</button>
              </div>
              <div class="extend-custom-row">
                <input type="number" id="inputCustomExtendMinutes" placeholder="Custom minutes (e.g. 45)" min="1"
                  max="1440">
                <button type="button" class="glass-button btn-primary" onclick="App.extendActiveTimerCustom()">+ Add
                  Time</button>
              </div>
            </div>

          </div>
        </section>

        <!-- =================================================================== -->
        <!-- VIEW 2: TIMER CALIBRATIONS                                          -->
        <!-- =================================================================== -->
        <section id="viewTimers" class="view-page">
          <div class="grid-card glass-card" id="calibrationCard">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow">CUSTOM PARAMETERS</span>
                <h2>Timing Calibration</h2>
              </div>
              <span class="info-badge">RTOS Synced</span>
            </div>

            <div class="config-controls">
              <!-- Relay ON Duration Setting -->
              <div class="control-box">
                <div class="control-header">
                  <div class="control-info">
                    <label for="onHoursSlider">Relay Active Duration</label>
                    <p class="control-desc">Time relay remains energized when triggered</p>
                  </div>
                  <div class="number-display">
                    <span id="onHoursDisplay">8.0</span> <span class="unit">hours</span>
                  </div>
                </div>
                <div class="slider-wrapper">
                  <input type="range" id="onHoursSlider" min="1" max="24" step="0.5" value="8"
                    oninput="App.onTimerInput('on')">
                  <div class="slider-ticks">
                    <span>1h</span>
                    <span>7h</span>
                    <span class="highlight">8h (Default)</span>
                    <span>16h</span>
                    <span>24h</span>
                  </div>
                </div>
              </div>

              <!-- Lockout Cooldown Duration Setting -->
              <div class="control-box">
                <div class="control-header">
                  <div class="control-info">
                    <label for="lockoutDaysSlider">Lockout Cooldown Duration</label>
                    <p class="control-desc">Touch sensor completely ignored until countdown expires</p>
                  </div>
                  <div class="number-display">
                    <span id="lockoutDaysDisplay">6.0</span> <span class="unit">days</span>
                  </div>
                </div>
                <div class="slider-wrapper">
                  <input type="range" id="lockoutDaysSlider" min="1" max="30" step="0.5" value="6"
                    oninput="App.onTimerInput('lockout')">
                  <div class="slider-ticks">
                    <span>1d</span>
                    <span>5d</span>
                    <span class="highlight">6d - 7d (Target)</span>
                    <span>14d</span>
                    <span>30d</span>
                  </div>
                </div>
              </div>

              <button id="btnSaveConfig" class="glass-button btn-primary w-full" type="button"
                onclick="App.saveTimers()">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z" />
                  <polyline points="17 21 17 13 7 13 7 21" />
                  <polyline points="7 3 7 8 15 8" />
                </svg>
                <span>Save &amp; Flash to ESP8266 Memory</span>
              </button>
            </div>
          </div>
        </section>

        <!-- =================================================================== -->
        <!-- VIEW 3: MONTHLY USAGE ANALYTICS                                     -->
        <!-- =================================================================== -->
        <section id="viewAnalytics" class="view-page">
          <div class="grid-card glass-card analytics-card">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow">MONTHLY USAGE ANALYTICS</span>
                <h2>Switch Runtime &amp; Analysis</h2>
              </div>
              <span class="info-badge" id="analyticsMonthBadge">November 2026</span>
            </div>

            <p class="card-subtext">Comprehensive historical tracking of switch on/off durations and monthly cycles.</p>

            <!-- Metrics summary tiles -->
            <div class="analytics-metrics-grid">
              <div class="metric-box box-on">
                <div class="metric-icon">⚡</div>
                <div class="metric-data">
                  <span class="metric-num" id="anTotalOnHours">48.5</span>
                  <span class="metric-unit">hours ON</span>
                </div>
                <span class="metric-label">Total Switch Energized</span>
              </div>

              <div class="metric-box box-off">
                <div class="metric-icon">💤</div>
                <div class="metric-data">
                  <span class="metric-num" id="anTotalOffHours">240.2</span>
                  <span class="metric-unit">hours OFF</span>
                </div>
                <span class="metric-label">Total Inactive / Lockout</span>
              </div>

              <div class="metric-box box-cycles">
                <div class="metric-icon">🔄</div>
                <div class="metric-data">
                  <span class="metric-num" id="anTotalCycles">6</span>
                  <span class="metric-unit">cycles</span>
                </div>
                <span class="metric-label">Completed Runs</span>
              </div>
            </div>

            <!-- Monthly Duty Cycle Comparison Bar -->
            <div class="analytics-bar-wrapper">
              <div class="bar-header">
                <span>Monthly Duty Cycle Ratio</span>
                <strong id="dutyRatioPercent">16.8% ON</strong>
              </div>
              <div class="ratio-bar-track">
                <div id="ratioBarOn" class="ratio-bar-fill on-fill" style="width: 16.8%;"></div>
                <div id="ratioBarOff" class="ratio-bar-fill off-fill" style="width: 83.2%;"></div>
              </div>
              <div class="ratio-bar-legend">
                <span class="legend-dot dot-cyan"></span> Active Hours
                <span class="legend-dot dot-slate" style="margin-left: 14px;"></span> Lockout / Standby
              </div>
            </div>
          </div>
        </section>

        <!-- =================================================================== -->
        <!-- VIEW 4: USER SECURITY & ACCESS (WITH ADMIN DISABLE POWER)           -->
        <!-- =================================================================== -->
        <section id="viewUsers" class="view-page">
          <div class="grid-card glass-card" id="userManagementCard">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow">3-TIER ACCESS MATRIX</span>
                <h2>Enrolled User Database</h2>
              </div>
              <div class="user-quota-badge">
                <span id="userCountText">3 Active</span> Accounts
              </div>
            </div>

            <p class="card-subtext">Predefined credentials with 3 discrete privilege levels. Administrators have
              authority to temporarily disable or enable user accounts.</p>

            <div class="user-table-wrap">
              <table class="user-table">
                <thead>
                  <tr>
                    <th>Username</th>
                    <th>Privilege Tier</th>
                    <th>Status</th>
                    <th>Admin Authority</th>
                  </tr>
                </thead>
                <tbody id="userTableBody">
                  <!-- Dynamically populated -->
                </tbody>
              </table>
            </div>
          </div>
        </section>

        <!-- =================================================================== -->
        <!-- VIEW 5: FIREBASE CLOUD & REAL-TIME LOG VIEWER                       -->
        <!-- =================================================================== -->
        <section id="viewCloud" class="view-page">
          <div class="grid-card glass-card">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow">FIREBASE WORLDWIDE CLOUD</span>
                <h2>Cloud Stream &amp; Controller Logs</h2>
              </div>
              <span class="info-badge" id="cloudSyncStatusBadge">Connected</span>
            </div>

            <div class="cloud-log-container">
              <!-- Target and Node details -->
              <div class="cloud-target-box">
                <div>
                  <span class="eyebrow">ACTIVE FIREBASE ENDPOINT</span>
                  <div class="cloud-target-url" id="lblFirebaseTarget">
                    https://cyber-relay-rtos-default-rtdb.firebaseio.com/esp8266_relay</div>
                </div>
                <div style="display:flex; gap:8px;">
                  <button type="button" class="glass-button btn-secondary" onclick="App.testCloudConnection()">Test
                    Ping</button>
                  <button type="button" class="glass-button btn-primary" onclick="App.openCloudConfigModal()">Edit
                    Config</button>
                </div>
              </div>

              <!-- Terminal Stream -->
              <div class="cloud-log-terminal" id="cloudLogTerminal">
                <!-- Log entries added dynamically -->
              </div>

              <!-- Terminal actions -->
              <div class="cloud-log-actions">
                <span class="card-subtext">Live feed of read/write sync operations, controller origins, and command
                  latency.</span>
                <div style="display:flex; gap:8px;">
                  <button type="button" class="glass-button btn-secondary" onclick="App.clearCloudLogs()">Clear
                    Console</button>
                  <button type="button" class="glass-button btn-primary" onclick="App.fetchStatus()">Force Sync
                    Now</button>
                </div>
              </div>
            </div>
          </div>
        </section>

        <!-- =================================================================== -->
        <!-- VIEW 6: DEVICE & DIAGNOSTICS                                        -->
        <!-- =================================================================== -->
        <section id="viewSettings" class="view-page">
          <div class="grid-card glass-card">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow">HARDWARE &amp; RF TELEMETRY</span>
                <h2>Device &amp; Diagnostics</h2>
              </div>
              <span class="info-badge">RTOS v3.0</span>
            </div>

            <div class="analytics-metrics-grid">
              <div class="metric-box">
                <span class="eyebrow">REAL RF SIGNAL</span>
                <div class="metric-data">
                  <span class="metric-num" id="diagRssiNum">--</span>
                  <span class="metric-unit">dBm</span>
                </div>
                <span class="metric-label" id="diagSignalQuality">Checking...</span>
              </div>

              <div class="metric-box">
                <span class="eyebrow">ROUND-TRIP PING</span>
                <div class="metric-data">
                  <span class="metric-num" id="diagPingNum">--</span>
                  <span class="metric-unit">ms</span>
                </div>
                <span class="metric-label">Latency</span>
              </div>

              <div class="metric-box">
                <span class="eyebrow">ESP8266 HEAP</span>
                <div class="metric-data">
                  <span class="metric-num" id="diagHeapNum">--</span>
                  <span class="metric-unit">bytes</span>
                </div>
                <span class="metric-label">RAM Free</span>
              </div>
            </div>

            <div class="config-controls" style="margin-top: 18px;">
              <div class="test-conn-row">
                <button type="button" class="glass-button btn-secondary" onclick="App.testLocalConnection()">Test ESP
                  Ping</button>
                <button type="button" class="glass-button btn-secondary" onclick="App.testCloudConnection()">Test
                  Firebase Ping</button>
                <button type="button" class="glass-button btn-warning" onclick="App.openCloudConfigModal()">Network
                  Settings</button>
              </div>
            </div>
          </div>

          <!-- Dynamic Multi-Relay Configuration Card -->
          <div class="grid-card glass-card relay-mgmt-card">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow">HARDWARE EXTENSION</span>
                <h2>Dynamic Multi-Relay &amp; Pin Bindings</h2>
                <p class="card-subtext">Add separate relays with dedicated ESP8266 GPIO pins, independent ON timers, and
                  custom lockout durations.</p>
              </div>
              <button type="button" class="glass-button btn-primary" onclick="App.openAddRelayModal()">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <line x1="12" y1="5" x2="12" y2="19" />
                  <line x1="5" y1="12" x2="19" y2="12" />
                </svg>
                <span>Add Relay</span>
              </button>
            </div>

            <div class="relay-card-list" id="settingsRelaysList">
              <!-- Dynamically populated relay cards -->
            </div>
          </div>

          <!-- Hardware Alarm Buzzer Feedback Card -->
          <div class="grid-card glass-card" style="margin-top: 20px;">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow">ACOUSTIC TELEMETRY</span>
                <h2>Hardware Alarm Buzzer (NodeMCU D8 / GPIO15)</h2>
                <p class="card-subtext">Audible feedback sounds on D8 whenever any relay switches ON (double beep) or
                  OFF (single long tone).</p>
              </div>
              <span class="state-badge badge-active">BUZZER ON D8</span>
            </div>

            <div style="display: flex; gap: 10px; align-items: center; margin-top: 12px;">
              <button type="button" class="glass-button btn-secondary" onclick="App.testBuzzerSound()">
                <span>🔊 Test Buzzer Sound</span>
              </button>
              <span style="font-size: 0.75rem; color: var(--text-muted);">Plays immediate acoustic chime for hardware
                verification.</span>
            </div>
          </div>
        </section>

        <!-- =================================================================== -->
        <!-- VIEW 7: CONNECTION ACKNOWLEDGE & WIRELESS LOGS                      -->
        <!-- =================================================================== -->
        <section id="viewAcknowledge" class="view-page">
          <div class="grid-card glass-card">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow">PIPELINE VERIFICATION &amp; HARDWARE BUZZER</span>
                <h2>Connection Acknowledge (3-Beep Chime)</h2>
              </div>
              <span class="state-badge badge-active" id="ackVerifiedBadge">ACK READY</span>
            </div>

            <!-- 3-Stage Handshake Pipeline Visualizer -->
            <div class="ack-pipeline-grid">
              <div class="ack-step-card is-active" id="ackStep1">
                <div class="ack-step-header">
                  <span class="ack-step-num">STAGE 1</span>
                  <span class="status-dot dot-active" id="ackStep1Dot"></span>
                </div>
                <div class="ack-step-title">🔌 Device Boot &amp; Restart</div>
                <div class="ack-step-desc" id="ackStep1Desc">Boot Reason: Power On</div>
                <div class="ack-step-detail" id="ackStep1Detail">System RTOS Initialized</div>
              </div>

              <div class="ack-step-card is-active" id="ackStep2">
                <div class="ack-step-header">
                  <span class="ack-step-num">STAGE 2</span>
                  <span class="status-dot dot-active" id="ackStep2Dot"></span>
                </div>
                <div class="ack-step-title">📶 SoftAP &amp; Wi-Fi Network</div>
                <div class="ack-step-desc" id="ackStep2Desc">AP: CYBER-RELAY-HUB</div>
                <div class="ack-step-detail" id="ackStep2Detail">IP: 192.168.4.1</div>
              </div>

              <div class="ack-step-card is-active" id="ackStep3">
                <div class="ack-step-header">
                  <span class="ack-step-num">STAGE 3</span>
                  <span class="status-dot dot-active" id="ackStep3Dot"></span>
                </div>
                <div class="ack-step-title">☁️ Firebase Cloud Sync</div>
                <div class="ack-step-desc" id="ackStep3Desc">Realtime Stream Active</div>
                <div class="ack-step-detail" id="ackStep3Detail">3-Beep Audio Handshake</div>
              </div>
            </div>

            <!-- Handshake Action Banner -->
            <div class="ack-action-banner">
              <div class="ack-banner-left">
                <div class="ack-banner-icon">🔔</div>
                <div>
                  <div class="ack-banner-title" id="ackBannerStatusTitle">Handshake Acknowledged &amp; Confirmed</div>
                  <div class="ack-banner-subtitle" id="ackBannerStatusSub">ESP8266 sounds D8 buzzer 3 times when device joins Wi-Fi and connects to Firebase.</div>
                </div>
              </div>
              <button class="glass-button btn-accent" id="btnTestAckHandshake" type="button" onclick="App.triggerAckHandshake()">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16">
                  <polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/>
                  <path d="M19.07 4.93a10 10 0 0 1 0 14.14M15.54 8.46a5 5 0 0 1 0 7.07"/>
                </svg>
                <span>Verify &amp; Sound 3-Beep ACK</span>
              </button>
            </div>
          </div>

          <!-- Wireless & Device System Logs Card -->
          <div class="grid-card glass-card" style="margin-top: 20px;">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow">HARDWARE EVENT STREAM</span>
                <h2>Wireless &amp; Device System Logs</h2>
              </div>
              <div style="display: flex; gap: 8px;">
                <button class="glass-button btn-secondary" style="font-size:0.75rem; padding: 6px 12px;" type="button" onclick="App.fetchStatus()">Refresh Logs</button>
              </div>
            </div>

            <div class="system-logs-container" id="systemLogsList">
              <div class="terminal-line" style="color:var(--text-muted);">Awaiting device system events...</div>
            </div>
          </div>
        </section>

        <!-- =================================================================== -->
        <!-- VIEW 8: INTERACTIVE LIVE CONSOLE                                   -->
        <!-- =================================================================== -->
        <section id="viewConsole" class="view-page">
          <div class="grid-card glass-card">
            <div class="card-header">
              <div class="card-title-group">
                <span class="eyebrow">BIDIRECTIONAL REAL-TIME STREAM</span>
                <h2>⚡ Interactive RTOS &amp; Cloud Live Console</h2>
              </div>
              <div style="display:flex; align-items:center; gap:8px;">
                <span class="live-stream-indicator" id="consoleLiveIndicator">
                  <span class="pulse-dot"></span> LIVE STREAMING
                </span>
                <span class="info-badge" id="consoleConnBadge">ESP8266 LINKED</span>
              </div>
            </div>

            <!-- Firebase Key & Connection Info Banner -->
            <div class="console-key-banner">
              <div class="key-banner-icon">🔑</div>
              <div class="key-banner-content">
                <div class="key-banner-title">Firebase Service Account Connected: <strong id="lblConsoleProject">cyber-relay-rtos-default-rtdb</strong></div>
                <div class="key-banner-sub">Key ID: <code>b7a22fa5...</code> | Client: <code>firebase-adminsdk-fbsvc@cyber-relay-rtos-default-rtdb.iam.gserviceaccount.com</code></div>
              </div>
              <button type="button" class="glass-button btn-secondary" style="font-size:0.75rem; padding:5px 10px;" onclick="App.openCloudConfigModal()">Cloud Config</button>
            </div>

            <!-- Filter and Control Bar -->
            <div class="console-toolbar">
              <div class="console-filters">
                <button type="button" class="console-filter-btn active" data-filter="all" onclick="App.setConsoleFilter('all')">ALL</button>
                <button type="button" class="console-filter-btn" data-filter="cmd" onclick="App.setConsoleFilter('cmd')">COMMANDS</button>
                <button type="button" class="console-filter-btn" data-filter="tel" onclick="App.setConsoleFilter('tel')">TELEMETRY</button>
                <button type="button" class="console-filter-btn" data-filter="hw" onclick="App.setConsoleFilter('hw')">HARDWARE</button>
                <button type="button" class="console-filter-btn" data-filter="err" onclick="App.setConsoleFilter('err')">ERRORS</button>
              </div>

              <div class="console-controls">
                <button type="button" class="glass-button btn-secondary" style="font-size:0.75rem; padding: 5px 10px;" id="btnToggleStreaming" onclick="App.toggleConsoleStreaming()">Pause Stream</button>
                <button type="button" class="glass-button btn-secondary" style="font-size:0.75rem; padding: 5px 10px;" onclick="App.clearConsoleLogs()">Clear</button>
                <button type="button" class="glass-button btn-secondary" style="font-size:0.75rem; padding: 5px 10px;" onclick="App.copyConsoleLogs()">Copy All</button>
              </div>
            </div>

            <!-- Quick Command Pills -->
            <div class="console-quick-bar">
              <span class="quick-label">QUICK COMMANDS:</span>
              <button type="button" class="quick-pill" onclick="App.sendQuickCommand('help')">help</button>
              <button type="button" class="quick-pill" onclick="App.sendQuickCommand('status')">status</button>
              <button type="button" class="quick-pill" onclick="App.sendQuickCommand('relays')">relays</button>
              <button type="button" class="quick-pill" onclick="App.sendQuickCommand('toggle 1')">toggle 1</button>
              <button type="button" class="quick-pill" onclick="App.sendQuickCommand('ack')">ack (3-beep)</button>
              <button type="button" class="quick-pill" onclick="App.sendQuickCommand('buzzer')">buzzer</button>
              <button type="button" class="quick-pill" onclick="App.sendQuickCommand('heap')">heap</button>
              <button type="button" class="quick-pill" onclick="App.sendQuickCommand('sync')">sync</button>
            </div>

            <!-- Terminal Output Window -->
            <div class="live-terminal-window" id="liveConsoleTerminal">
              <!-- Dynamically populated -->
            </div>

            <!-- Interactive Command Prompt -->
            <form class="terminal-prompt-form" onsubmit="event.preventDefault(); App.handleConsoleInputSubmit();">
              <span class="prompt-sym">CYBER-RELAY&gt;</span>
              <input type="text" id="consoleCmdInput" class="prompt-input" placeholder="Type command here (e.g. status, trigger 1, toggle 1, ack, buzzer, heap, help)..." autocomplete="off" spellcheck="false">
              <button type="submit" class="glass-button btn-accent" style="padding: 8px 18px;">SEND</button>
            </form>
          </div>
        </section>

      </main>

      <!-- System Footer -->
      <footer class="system-footer glass-card">
        <div class="footer-item">
          <span class="ft-label">RTOS KERNEL:</span>
          <span class="ft-val">FreeRTOS Tasks</span>
        </div>
        <div class="footer-item">
          <span class="ft-label">CLOUD SYNC:</span>
          <span id="ftCloudStatus" class="ft-val" style="color:var(--accent-cyan)">Firebase Active</span>
        </div>
        <div class="footer-item">
          <span class="ft-label">TOUCH LOCK:</span>
          <span class="ft-val">Hardware Filter</span>
        </div>
        <div class="footer-item">
          <span class="ft-label">UPTIME:</span>
          <span id="espUptime" class="ft-val">00:00:00</span>
        </div>
      </footer>
    </div>
  </div>

  <!-- ========================================================================= -->
  <!-- MODAL 1: Real-Time Admin Confirmation Popup for Mahi_Gajjar 1-Time Access -->
  <!-- ========================================================================= -->
  <div id="adminOneTimeApprovalModal" class="modal-backdrop hidden">
    <div class="modal-card glass-card modal-highlight-gold">
      <div class="modal-header">
        <div class="auth-icon-badge">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z" />
            <line x1="12" y1="9" x2="12" y2="13" />
            <line x1="12" y1="17" x2="12.01" y2="17" />
          </svg>
        </div>
        <div>
          <h3>User Authorization Request</h3>
          <p class="modal-subtitle">Level 2 user <strong>Mahi_Gajjar</strong> is requesting permission.</p>
        </div>
        <button class="close-btn" type="button" onclick="App.dismissAdminApprovalModal()">✕</button>
      </div>

      <div class="modal-body">
        <div class="req-details-box">
          <div class="req-row">
            <span class="req-label">Requesting User:</span>
            <strong class="text-gold" id="popupReqUser">Mahi_Gajjar</strong>
          </div>
          <div class="req-row">
            <span class="req-label">Requested Action:</span>
            <strong id="popupReqAction">Trigger Relay Active Cycle (8.0h)</strong>
          </div>
          <div class="req-row">
            <span class="req-label">Session Policy:</span>
            <span class="badge-single-use">1-Time Only (Destroy Session Immediately)</span>
          </div>
        </div>

        <form onsubmit="event.preventDefault(); App.submitAdminApproval();">
          <div class="form-group" style="margin-top: 14px;">
            <label class="text-gold">Enter Yash_Gajjar Admin Password to Grant Access:</label>
            <input type="password" id="popupAdminPassword" placeholder="Enter password to approve" required autofocus>
          </div>

          <div class="modal-actions">
            <button type="button" class="glass-button btn-secondary" onclick="App.rejectAdminApproval()">Deny
              Access</button>
            <button type="submit" class="glass-button btn-gold">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <polyline points="20 6 9 17 4 12" />
              </svg>
              <span>Grant 1-Time Access</span>
            </button>
          </div>
        </form>
      </div>
    </div>
  </div>

  <!-- ========================================================================= -->
  <!-- MODAL 2: Mahi_Gajjar Waiting for Admin Approval Radar Modal                -->
  <!-- ========================================================================= -->
  <div id="userWaitingApprovalModal" class="modal-backdrop hidden">
    <div class="modal-card glass-card text-center">
      <div class="radar-spinner">
        <div class="radar-circle circle-1"></div>
        <div class="radar-circle circle-2"></div>
        <div class="radar-circle circle-3"></div>
        <div class="radar-core">🔒</div>
      </div>
      <h3 style="margin-top: 16px;">Waiting for Admin Confirmation</h3>
      <p class="modal-subtitle" style="margin: 8px auto 16px; max-width: 320px;">
        A real-time authorization popup has been sent to <strong>Yash_Gajjar</strong>. Your 1-time session will execute
        immediately once approved.
      </p>
      <button class="glass-button btn-secondary" type="button" onclick="App.cancelWaitingRequest()">Cancel
        Request</button>
    </div>
  </div>

  <!-- ========================================================================= -->
  <!-- MODAL 3: Firebase Cloud &amp; Local Network Settings Modal                    -->
  <!-- ========================================================================= -->
  <div id="cloudConfigModal" class="modal-backdrop hidden">
    <div class="modal-card glass-card">
      <div class="modal-header">
        <div class="auth-icon-badge" style="background: rgba(6,182,212,0.15); color: var(--accent-cyan);">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <circle cx="12" cy="12" r="3" />
            <path
              d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z" />
          </svg>
        </div>
        <div>
          <h3>Network &amp; Firebase Cloud Config</h3>
          <p class="modal-subtitle">Configure worldwide Firebase sync and Local ESP address.</p>
        </div>
        <button class="close-btn" type="button" onclick="App.closeCloudConfigModal()">✕</button>
      </div>

      <div class="modal-body">
        <div class="console-key-banner" style="margin-bottom: 14px; margin-top: 0;">
          <div class="key-banner-icon">🔑</div>
          <div class="key-banner-content">
            <div class="key-banner-title">Service Account: <strong style="color:var(--color-cyan);">cyber-relay-rtos-default-rtdb</strong></div>
            <div class="key-banner-sub">Key loaded from <code>/Firebase_Auth_Keys</code> folder.</div>
          </div>
          <span class="state-badge badge-active" style="font-size:0.65rem;">KEY VALIDATED</span>
        </div>
        <form onsubmit="event.preventDefault(); App.saveCloudSettings();">
          <div class="form-group">
            <label for="cfgFirebaseHost">Firebase Realtime Database Host URL</label>
            <input type="text" id="cfgFirebaseHost" placeholder="e.g. cyber-relay-rtos-default-rtdb-default-rtdb.firebaseio.com"
              required>
            <div style="display: flex; gap: 6px; margin-top: 6px; flex-wrap: wrap;">
              <span class="quick-label" style="align-self: center; font-size: 0.65rem;">Presets:</span>
              <button type="button" class="quick-pill" onclick="document.getElementById('cfgFirebaseHost').value='cyber-relay-rtos-default-rtdb-default-rtdb.firebaseio.com'">-default-rtdb.firebaseio.com</button>
              <button type="button" class="quick-pill" onclick="document.getElementById('cfgFirebaseHost').value='cyber-relay-rtos-default-rtdb.firebaseio.com'">.firebaseio.com</button>
              <button type="button" class="quick-pill" onclick="document.getElementById('cfgFirebaseHost').value='cyber-relay-rtos-default-rtdb.asia-southeast1.firebasedatabase.app'">.asia-southeast1</button>
            </div>
            <span class="field-hint">Your Firebase database domain without https://</span>
          </div>

          <!-- Firebase Setup Instructions Box -->
          <div class="console-key-banner" style="background: rgba(245, 158, 11, 0.1); border-color: rgba(245, 158, 11, 0.3); margin-top: 8px; margin-bottom: 12px;">
            <div class="key-banner-icon" style="background: rgba(245, 158, 11, 0.2);">💡</div>
            <div class="key-banner-content">
              <div class="key-banner-title" style="color: #fbbf24;">Firebase RTDB Setup Checklist</div>
              <div class="key-banner-sub" style="color: #e2e8f0; font-size: 0.72rem; line-height: 1.4;">
                1. Visit: <a href="https://console.firebase.google.com/project/cyber-relay-rtos-default-rtdb/database" target="_blank" style="color:var(--accent-cyan); text-decoration: underline;">Firebase Console -> Realtime Database</a><br>
                2. Click <strong>"Create Database"</strong> (choose US or Asia location).<br>
                3. Set Security Rules to <strong>Test Mode</strong> (<code>{ ".read": true, ".write": true }</code>) so the ESP8266 and App can communicate without authorization errors.
              </div>
            </div>
          </div>

          <div class="form-group">
            <label for="cfgFirebaseAuth">Firebase Database Secret / Auth Key (Optional)</label>
            <input type="password" id="cfgFirebaseAuth"
              placeholder="Enter Database Secret or leave blank for open rules">
            <span class="field-hint">Required only if Firebase Database rules require authentication.</span>
          </div>

          <div class="form-group">
            <label for="cfgEspHost">Local ESP8266 Host URL</label>
            <input type="text" id="cfgEspHost" placeholder="http://192.168.4.1" required>
            <span class="field-hint">Default SoftAP IP is http://192.168.4.1</span>
          </div>

          <div class="test-conn-row">
            <button type="button" class="glass-button btn-secondary" onclick="App.testCloudConnection()">Test Firebase
              Ping</button>
            <button type="button" class="glass-button btn-secondary" onclick="App.testLocalConnection()">Test ESP
              Ping</button>
          </div>

          <div class="modal-actions" style="margin-top: 18px;">
            <button type="button" class="glass-button btn-secondary"
              onclick="App.closeCloudConfigModal()">Cancel</button>
            <button type="submit" class="glass-button btn-primary">Save Settings</button>
          </div>
        </form>
      </div>
    </div>
  </div>

  <!-- ========================================================================= -->
  <!-- MODAL 4: Local ESP Wi-Fi Disconnected Protection Popup Modal               -->
  <!-- ========================================================================= -->
  <div id="localWifiPromptModal" class="modal-backdrop hidden">
    <div class="modal-card glass-card text-center" style="max-width: 440px;">
      <div class="auth-icon-badge"
        style="background: rgba(245, 158, 11, 0.2); color: #fbbf24; margin: 0 auto 12px; width: 62px; height: 62px;">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="width: 32px; height: 32px;">
          <path d="M5 12.55a11 11 0 0 1 14.08 0" />
          <path d="M1.42 9a16 16 0 0 1 21.16 0" />
          <path d="M8.53 16.11a6 6 0 0 1 6.95 0" />
          <line x1="12" y1="20" x2="12.01" y2="20" />
        </svg>
      </div>
      <h3 style="font-size: 1.25rem;">Connect Device Wi-Fi First</h3>
      <p class="modal-subtitle" style="margin: 10px auto 16px; font-size: 0.85rem; line-height: 1.5;">
        Your device is not connected to the ESP8266 local Wi-Fi hotspot (<strong>CYBER-RELAY-HUB</strong>).
        Please connect to <strong>CYBER-RELAY-HUB</strong> first before doing changes, or switch to <strong>Firebase
          Cloud</strong> mode.
      </p>
      <div
        style="background: rgba(0,0,0,0.3); border: 1px solid var(--border-subtle); border-radius: var(--radius-sm); padding: 10px 14px; margin-bottom: 18px; text-align: left; font-size: 0.76rem; font-family: var(--font-mono);">
        <div>• Wi-Fi SSID: <strong style="color:var(--accent-amber)">CYBER-RELAY-HUB</strong></div>
        <div>• Password: <strong style="color:var(--accent-amber)">cyber8266</strong></div>
        <div>• Gateway IP: <strong style="color:var(--accent-cyan)">http://192.168.4.1</strong></div>
      </div>
      <div class="modal-actions" style="justify-content: center; gap: 8px;">
        <button type="button" class="glass-button btn-primary"
          onclick="App.setConnectionMode('cloud'); App.closeLocalWifiModal();">Switch to Cloud</button>
        <button type="button" class="glass-button btn-secondary" onclick="App.retryLocalConnection()">Retry
          Connection</button>
        <button type="button" class="glass-button" onclick="App.closeLocalWifiModal()">Dismiss</button>
      </div>
    </div>
  </div>

  <!-- ========================================================================= -->
  <!-- MODAL 5: ESP8266 LittleFS Database Password Reset Modal                   -->
  <!-- ========================================================================= -->
  <div id="forgotPasswordModal" class="modal-backdrop hidden">
    <div class="modal-card glass-card" style="max-width: 480px;">
      <div class="modal-header">
        <div class="auth-icon-badge" style="background: rgba(245, 158, 11, 0.2); color: #fbbf24;">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <rect x="3" y="11" width="18" height="11" rx="2" ry="2" />
            <path d="M7 11V7a5 5 0 0 1 10 0v4" />
          </svg>
        </div>
        <div>
          <h3>Reset Password</h3>
          <p class="modal-subtitle">Directly updates ESP8266 chip database</p>
        </div>
        <button class="close-btn" type="button" onclick="App.closeForgotPasswordModal()">✕</button>
      </div>

      <div class="modal-body">
        <form onsubmit="event.preventDefault(); App.submitForgotPassword();">
          <div class="form-group">
            <label for="resetTargetUser">Select Account to Recover</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2" />
                <circle cx="12" cy="7" r="4" />
              </svg>
              <select id="resetTargetUser" class="custom-select" onchange="App.onResetUserChange()">
                <option value="Yash_Gajjar">Yash_Gajjar (Super Admin)</option>
                <option value="Mahi_Gajjar">Mahi_Gajjar (Guarded Operator)</option>
                <option value="Guest">Guest (Guest Analytics)</option>
              </select>
            </div>
          </div>

          <div class="form-group">
            <label for="resetAuthKey" id="lblResetAuthKey">Admin Password or Master Recovery PIN</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <polygon
                  points="12 2 15.09 8.26 22 9.27 17 14.14 18.18 21.02 12 17.77 5.82 21.02 7 14.14 2 9.27 8.91 8.26 12 2" />
              </svg>
              <input type="password" id="resetAuthKey" placeholder="Admin password or PIN (CYBER-8266-ROOT)" required
                autocomplete="off">
              <button type="button" class="btn-toggle-eye"
                onclick="App.togglePasswordVisibility('resetAuthKey')">👁️</button>
            </div>
            <p class="input-helper-text" id="resetAuthKeyHelp"
              style="font-size:0.75rem; color:var(--text-muted); margin-top:4px;">Master PIN:
              <code>CYBER-8266-ROOT</code> or Admin password</p>
          </div>

          <div class="form-group">
            <label for="resetNewPassword">New Password</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <rect x="3" y="11" width="18" height="11" rx="2" ry="2" />
                <path d="M7 11V7a5 5 0 0 1 10 0v4" />
              </svg>
              <input type="password" id="resetNewPassword" placeholder="Minimum 6 characters" required
                autocomplete="new-password">
              <button type="button" class="btn-toggle-eye"
                onclick="App.togglePasswordVisibility('resetNewPassword')">👁️</button>
            </div>
          </div>

          <div class="form-group">
            <label for="resetConfirmPassword">Confirm New Password</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <polyline points="20 6 9 17 4 12" />
              </svg>
              <input type="password" id="resetConfirmPassword" placeholder="Re-enter new password" required
                autocomplete="new-password">
            </div>
          </div>

          <div class="modal-actions" style="margin-top: 18px;">
            <button type="button" class="glass-button btn-secondary"
              onclick="App.closeForgotPasswordModal()">Cancel</button>
            <button type="submit" class="glass-button btn-primary" id="btnSubmitPasswordReset">Save on ESP Chip</button>
          </div>
        </form>
      </div>
    </div>
  </div>

  <!-- ========================================================================= -->
  <!-- MODAL 6: Dynamic Multi-Relay Configuration & Hardware Pin Binding Modal   -->
  <!-- ========================================================================= -->
  <div id="addRelayModal" class="modal-backdrop hidden">
    <div class="modal-card glass-card" style="max-width: 520px;">
      <div class="modal-header">
        <div class="auth-icon-badge" style="background: rgba(16, 185, 129, 0.2); color: #34d399;">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <rect x="3" y="3" width="18" height="18" rx="2" ry="2" />
            <path d="M12 8v8M8 12h8" />
          </svg>
        </div>
        <div>
          <h3 id="relayModalTitle">Configure Relay Channel</h3>
          <p class="modal-subtitle">Bind ESP8266 GPIO pins with independent timer countdowns</p>
        </div>
        <button class="close-btn" type="button" onclick="App.closeAddRelayModal()">✕</button>
      </div>

      <div class="modal-body">
        <form onsubmit="event.preventDefault(); App.handleRelayFormSubmit();">
          <input type="hidden" id="relayFormId" value="0">

          <div class="form-group">
            <label for="relayFormName">Relay Label / Device Name</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <path d="M13 2L3 14h9l-1 8 10-12h-9l1-8z" />
              </svg>
              <input type="text" id="relayFormName" placeholder="e.g. Relay 2 (Water Pump)" required>
            </div>
          </div>

          <div class="form-group">
            <label for="relayFormPin">ESP8266 Hardware GPIO Pin Binding</label>
            <div class="input-icon-wrap">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <circle cx="12" cy="12" r="3" />
                <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0" />
              </svg>
              <select id="relayFormPin" class="custom-select" required>
                <option value="14">GPIO14 (NodeMCU D5) [Recommended Safe Output]</option>
                <option value="12">GPIO12 (NodeMCU D6) [Recommended Safe Output]</option>
                <option value="13">GPIO13 (NodeMCU D7) [Recommended Safe Output]</option>
                <option value="16">GPIO16 (NodeMCU D0) [Standard Output]</option>
                <option value="5">GPIO5 (NodeMCU D1) [Primary Relay default]</option>
                <option value="4">GPIO4 (NodeMCU D2) [Touch Sensor / GPIO]</option>
                <option value="0">GPIO0 (NodeMCU D3) [Boot Pull-up]</option>
                <option value="2">GPIO2 (NodeMCU D4) [LED / Pull-up]</option>
              </select>
            </div>
            <p style="font-size: 0.72rem; color: var(--text-muted); margin-top: 3px;">
              * Note: GPIO15 (D8) is reserved for the hardware acoustic buzzer.
            </p>
          </div>

          <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 10px;">
            <div class="form-group">
              <label for="relayFormOnHours">ON Duration (Hours)</label>
              <input type="number" id="relayFormOnHours" min="0.1" max="72" step="0.5" value="8.0" required>
            </div>
            <div class="form-group">
              <label for="relayFormLockoutDays">Lockout/OFF Duration (Days)</label>
              <input type="number" id="relayFormLockoutDays" min="0" max="365" step="0.5" value="8.0" required>
            </div>
          </div>

          <div class="form-group">
            <label for="relayFormActiveLevel">Relay Trigger Logic Level</label>
            <select id="relayFormActiveLevel" class="custom-select">
              <option value="0">Active LOW (Standard Relay Modules - Recommended)</option>
              <option value="1">Active HIGH (High-trigger SSR / Transistor)</option>
            </select>
          </div>

          <div class="modal-actions" style="margin-top: 18px;">
            <button type="button" class="glass-button btn-secondary" onclick="App.closeAddRelayModal()">Cancel</button>
            <button type="submit" class="glass-button btn-primary" id="btnSaveRelayConfig">Save Relay Hardware</button>
          </div>
        </form>
      </div>
    </div>
  </div>

  <!-- ========================================================================= -->
  <!-- MODAL 7: App Alerts & Notifications Center Drawer Modal                   -->
  <!-- ========================================================================= -->
  <div id="notificationsModal" class="modal-backdrop hidden">
    <div class="modal-card glass-card" style="max-width: 500px;">
      <div class="modal-header">
        <div class="auth-icon-badge" style="background: rgba(245, 158, 11, 0.2); color: #fbbf24;">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <path d="M18 8A6 6 0 0 0 6 8c0 7-3 9-3 9h18s-3-2-3-9" />
            <path d="M13.73 21a2 2 0 0 1-3.46 0" />
          </svg>
        </div>
        <div>
          <h3>Switch Alerts &amp; Notifications</h3>
          <p class="modal-subtitle">Real-time log of relay ON/OFF transitions &amp; buzzer triggers</p>
        </div>
        <button class="close-btn" type="button" onclick="App.closeNotificationsModal()">✕</button>
      </div>

      <div class="modal-body">
        <div class="notifications-list" id="notificationsList">
          <!-- Populated dynamically -->
        </div>

        <div class="modal-actions" style="margin-top: 14px; justify-content: space-between;">
          <button type="button" class="glass-button btn-secondary" onclick="App.clearAllNotifications()">Clear
            Log</button>
          <button type="button" class="glass-button btn-primary" onclick="App.closeNotificationsModal()">Done</button>
        </div>
      </div>
    </div>
  </div>

  <!-- Toast Notification Container -->
  <div id="toastContainer" class="toast-container"></div>

  <script>
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

</script>
</body>

</html>)rawliteral";
