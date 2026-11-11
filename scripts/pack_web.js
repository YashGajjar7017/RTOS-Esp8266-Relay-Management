const fs = require('fs');
const path = require('path');

const webAppDir = path.join(__dirname, '..', 'web_app');
const targetHeader = path.join(__dirname, '..', 'firmware', 'include', 'web_assets.h');
const targetDataHtml = path.join(__dirname, '..', 'firmware', 'data', 'index.html');

console.log('Packaging Web App for ESP8266...');

let html = fs.readFileSync(path.join(webAppDir, 'index.html'), 'utf-8');
const css = fs.readFileSync(path.join(webAppDir, 'style.css'), 'utf-8');
const js = fs.readFileSync(path.join(webAppDir, 'app.js'), 'utf-8');

// Replace external CSS and JS links with inlined content
html = html.replace('<link rel="stylesheet" href="style.css">', `<style>\n${css}\n</style>`);
html = html.replace('<script src="app.js"></script>', `<script>\n${js}\n</script>`);

// Write standalone inlined HTML to data/index.html
fs.writeFileSync(targetDataHtml, html, 'utf-8');
console.log(`Saved inlined HTML to ${targetDataHtml} (${(html.length / 1024).toFixed(1)} KB)`);

// Escape rawliteral terminator if any occurs
const safeHtml = html.replace(/\)rawliteral/g, ')_rawliteral');

// Generate C++ PROGMEM header
const headerContent = `#pragma once
#include <Arduino.h>

/**
 * Auto-generated embedded Web App asset.
 * Contains complete responsive Single Page Application with styling,
 * real-time SVG countdown, and client logic stored directly in ESP8266 Flash PROGMEM.
 */

static const char PAGE_INDEX[] PROGMEM = R"rawliteral(${safeHtml})rawliteral";
`;

fs.writeFileSync(targetHeader, headerContent, 'utf-8');
console.log(`Saved embedded C++ header to ${targetHeader} (${(headerContent.length / 1024).toFixed(1)} KB)`);
console.log('Web asset generation complete!');

