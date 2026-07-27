import fs from 'fs';
import path from 'path';

const icoPath = path.resolve('src/app.ico');
const pngPath = path.resolve('res/app_icon.png');

if (!fs.existsSync(icoPath)) {
  console.error('src/app.ico not found!');
  process.exit(1);
}

const icoBuffer = fs.readFileSync(icoPath);
const pngBuffer = fs.existsSync(pngPath) ? fs.readFileSync(pngPath) : icoBuffer;

let headerContent = `// Auto-generated icon byte array for Dynamic Island
#pragma once
#include <windows.h>

const unsigned char g_appIcoBytes[] = {
`;

for (let i = 0; i < icoBuffer.length; i++) {
  headerContent += '0x' + icoBuffer[i].toString(16).padStart(2, '0') + ', ';
  if ((i + 1) % 16 === 0) headerContent += '\n';
}

headerContent += `
};
const size_t g_appIcoBytesSize = sizeof(g_appIcoBytes);

const unsigned char g_appPngBytes[] = {
`;

for (let i = 0; i < pngBuffer.length; i++) {
  headerContent += '0x' + pngBuffer[i].toString(16).padStart(2, '0') + ', ';
  if ((i + 1) % 16 === 0) headerContent += '\n';
}

headerContent += `
};
const size_t g_appPngBytesSize = sizeof(g_appPngBytes);

inline HICON GetDynamicIslandAppIcon(int cx = 32, int cy = 32) {
    HICON hIcon = CreateIconFromResourceEx(
        (PBYTE)g_appIcoBytes,
        (DWORD)g_appIcoBytesSize,
        TRUE,
        0x00030000,
        cx,
        cy,
        LR_DEFAULTCOLOR
    );
    if (!hIcon) {
        hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    }
    return hIcon;
}
`;

fs.writeFileSync(path.resolve('src/app_icon_data.h'), headerContent);
console.log('✅ Generated src/app_icon_data.h with embedded icon bytes!');
